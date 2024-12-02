// Duration of your trace, in milliseconds
let Cache_line_elem = 8;
let TRACE_LENGTH = 1048576; // 8MB/8B
let ARR_SIZE = 100;

// Array of length TRACE_LENGTH with your trace's values
let T;

// Value of performance.now() when you started recording your trace
let start;

function record() {
  // Create empty array for saving trace values
  let P = 500;
  let N = 131072;
  T = new Array(TRACE_LENGTH);
  let result = new Array(ARR_SIZE);

  // Fill array with -1 so we can be sure memory is allocated
  result.fill(0, 0, result.length);
  T.fill(-1, 0, T.length);


  // Save start timestamp
  let count = 0 ;
  let end;
  for (let j = 0; j < ARR_SIZE; ++j) {
    count = 0;
    start = performance.now();
    end = start;
    while ( (end - start) < P) {
      for (i=0; i < N; ++i) {
        let tmp = T[i*Cache_line_elem];
      }
      count++;
      end = performance.now();
    }
    result[j] = count;
  }
  console.log(result);

  // TODO (Exercise 2-2): Record data for TRACE_LENGTH seconds and save values to T.

  // Once done recording, send result to main thread
  postMessage(JSON.stringify(result));
}

// DO NOT MODIFY BELOW THIS LINE -- PROVIDED BY COURSE STAFF
self.onmessage = (e) => {
  if (e.data.type === "start") {
    TRACE_LENGTH = e.data.trace_length;
    setTimeout(record, 0);
  }
};
