import argparse
import json
import logging
import os
import sys
import threading
import time
import socket

from flask import Flask, send_from_directory
from selenium import webdriver
from selenium.webdriver.chrome.options import Options
from werkzeug.serving import make_server

# Parse arguments
parser = argparse.ArgumentParser()
parser.add_argument("--browser", type=str, choices=["chrome", "firefox", "safari", "explorer"], default="chrome", help="Browser to run automation in.")
parser.add_argument("--domains", type=str, default="google.com,youtube.com,baidu.com,facebook.com", help="Comma-separated list of domain names to collect traces from.")
parser.add_argument("--enable_countermeasure", type=bool, default=False, help="Enable the countermeasure. Browser must be Chrome. Defaults to false.")
parser.add_argument("--num_traces_per_domain", type=int, default=40, help="Number of traces to collect per domain.")
parser.add_argument("--trace_length", type=int, default=1000, help="Length of each recorded trace, in milliseconds.")

required = parser.add_argument_group("required arguments")
required.add_argument("--out_filename", type=str, required=True, help="Name of the output file to save traces to.")
required.add_argument("--part", type=int, choices=[2, 3, 4], required=True, help="Set to the part of the lab you're working on.")

opts = parser.parse_args()

if opts.browser != "chrome" and opts.enable_countermeasure:
    print("Browser must be set to Chrome to enable the countermeasure.")
    sys.exit(1)

if os.path.exists(opts.out_filename):
    print(f"WARNING: Data already exists at {opts.out_filename}. What do you want to do?")
    res = input("[C]ancel [O]verwrite ").lower()
    if res == "o":
        os.remove(opts.out_filename)
    else:
        sys.exit(1)

# Flask setup
app = Flask(__name__)

# Disable Flask logs
os.environ["WERKZEUG_RUN_MAIN"] = "true"
log = logging.getLogger("werkzeug")
log.disabled = True

@app.route("/")
def root():
    return send_from_directory(f"part{opts.part}", "index.html")

@app.route("/<path:path>")
def static_dir(path):
    return send_from_directory(f"part{opts.part}", path)

# Flask server
def start_flask():
    """Start the Flask server using Werkzeug's make_server."""
    try:
        print("Starting Flask server...")
        server = make_server("127.0.0.1", 1234, app)
        print("Flask server is running on http://127.0.0.1:1234")
        server.serve_forever()
    except Exception as e:
        print(f"Flask server failed to start: {e}")

flask_thread = threading.Thread(target=start_flask)
flask_thread.daemon = True
flask_thread.start()

# Wait for Flask server
def wait_for_server(host, port, timeout=10):
    """Wait for the server to be ready."""
    start_time = time.time()
    while time.time() - start_time < timeout:
        try:
            with socket.create_connection((host, port), timeout=1):
                print("Flask server is ready!")
                return True
        except (ConnectionRefusedError, socket.timeout):
            print("Waiting for Flask server to be ready...")
            time.sleep(1)
    print("Flask server failed to start within the timeout period.")
    return False

if not wait_for_server("127.0.0.1", 1234):
    sys.exit(1)

# Browser setup
def get_browser(victim):
    if opts.browser == "chrome":
        chrome_opts = Options()
        chrome_opts.add_experimental_option("excludeSwitches", ["enable-automation"])
        if opts.enable_countermeasure and victim:
            chrome_opts.add_extension("part4/extension.crx")
        return webdriver.Chrome(options=chrome_opts)
    elif opts.browser == "firefox":
        return webdriver.Firefox()
    elif opts.browser == "safari":
        return webdriver.Safari()
    elif opts.browser == "explorer":
        return webdriver.Ie()
    else:
        raise ValueError("Unsupported browser selected.")

# Start the attacker browser
attacker = get_browser(victim=False)
attacker.get("http://localhost:1234")
attacker.execute_script(f"window.trace_length = {opts.trace_length}")
attacker.execute_script(f"window.using_automation_script = true")

# Function to collect traces
def collect_trace(url):
    victim = get_browser(victim=True)
    attacker.execute_script("collectTrace()")
    victim.get(url)
    time.sleep(float(opts.trace_length) / 1000)
    while attacker.execute_script("return recording"):
        time.sleep(0.1)
    victim.quit()
    return attacker.execute_script("return traces")[-1]

# Collect traces
urls = [f"https://www.{domain}" for domain in opts.domains.split(",")]
traces = []
labels = []

for url in urls:
    for i in range(opts.num_traces_per_domain):
        try:
            traces.append(collect_trace(url))
            labels.append(url)
        except Exception as e:
            print(f"Error collecting trace for {url}: {e}")

# Save traces to a file
with open(opts.out_filename, "w") as out:
    json.dump({"traces": traces, "labels": labels}, out, separators=(",", ":"))

# Clean up
attacker.quit()
