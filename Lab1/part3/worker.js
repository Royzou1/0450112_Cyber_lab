// Duration of your trace, in milliseconds
let TRACE_LENGTH;

// Array of length TRACE_LENGTH with your trace's values
let T;

// Value of performance.now() when you started recording your trace
let start;

function record() {
  T = new Array(TRACE_LENGTH);
  T.fill(-1, 0, T.length);
  start = performance.now();

  for (let i = 0; i < TRACE_LENGTH; i++) {
    let now = performance.now();
    T[i] = now - start; 
  }

  postMessage(JSON.stringify(T));
}


// DO NOT MODIFY BELOW THIS LINE -- PROVIDED BY COURSE STAFF
self.onmessage = (e) => {
  if (e.data.type === "start") {
    TRACE_LENGTH = e.data.trace_length;
    setTimeout(record, 0);
  }
};
