import json
import sys
import numpy as np

def analyze_trace(trace, label):
    trace_min = np.min(trace)
    trace_max = np.max(trace)
    trace_median = np.median(trace)
    trace_mean = np.mean(trace)
    trace_variance = np.var(trace)
    third_moment = np.mean((trace - trace_mean) ** 3)

    result = (
        f"Trace {len(trace)} (Label: {label}):\n"
        f"  Min: {trace_min}\n"
        f"  Max: {trace_max}\n"
        f"  Median: {trace_median}\n"
        f"  Mean: {trace_mean}\n"
        f"  Variance: {trace_variance}\n"
        f"  third_moment: {third_moment}\n"
    )

    return result


def process_input(input_file):
    with open(input_file, 'r') as file:
        data = json.load(file)

    traces = data.get("traces", [])
    labels = data.get("labels", [])

    if len(traces) != len(labels):
        print("Error: The number of traces does not match the number of labels.")
        return

    # Prepare the output file name
    output_file = "good_analyzed.txt"

    with open(output_file, 'w') as out_file:
        for trace, label in zip(traces, labels):
            analysis_result = analyze_trace(trace, label)
            out_file.write(analysis_result)

    print(f"Analysis complete. Results saved to {output_file}")


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python script.py <input_file>")
    else:
        input_file = sys.argv[1]
        process_input(input_file)
