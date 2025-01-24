// Duration of your trace, in milliseconds
let TRACE_LENGTH = 1048576;

// Array of length TRACE_LENGTH with your trace's values
let T;

// Value of performance.now() when you started recording your trace
let start;

function record() {
  // Create empty array for saving trace values
  T = new Array(TRACE_LENGTH);

  // Fill array with -1 so we can be sure memory is allocated
  T.fill(-1, 0, T.length);

  // Save start timestamp
  start = performance.now();

  const LINE_SIZE = 16; // Assuming 64-byte cache lines
  const N = (1 << 23) / 256; // Number of memory addresses to traverse
  const M = new Array(N * LINE_SIZE).fill(-1); // Allocate memory for traversal

  let currentTime = start;
  let index = 0;

  while (index < TRACE_LENGTH) {
    let count = 0;
    const intervalStart = performance.now();

    // Traverse the memory repeatedly within the interval
    while (performance.now() - intervalStart < 2) {
      for (let i = 0; i < N; i++) {
        let val = M[i * LINE_SIZE];
      }
      count++;
    }

    // Record the number of traversals in this interval
    T[index] = count;

    // Move to the next interval
    index++;
    currentTime = performance.now();
  }

  // Once done recording, send result to main thread
  postMessage(JSON.stringify(T));
}





// DO NOT MODIFY BELOW THIS LINE -- PROVIDED BY COURSE STAFF
self.onmessage = (e) => {
  if (e.data.type === "start") {
    TRACE_LENGTH = e.data.trace_length;
    setTimeout(record, 0);
  }
};
