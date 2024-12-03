// Duration of your trace, in milliseconds
let Cache_line_elem = 8;
let TRACE_LENGTH = 1048576; // 8MB/8B
let ARR_SIZE = 10;

// Array of length TRACE_LENGTH with your trace's values
let T;

// Value of performance.now() when you started recording your trace
let start;

function record() {
  // Create empty array for saving trace values
  let P = 5000;
  let N = 131072;
  T = new Array(TRACE_LENGTH).fill(-1);
  let result = new Array(ARR_SIZE);

  // Fill array with -1 so we can be sure memory is allocated
  result.fill(0, 0, result.length);

  // Save start timestamp
  let count = 0 ;
  let end;
  for (let j = 0; j < ARR_SIZE; ++j) {
    console.log(j)
    count = 0;
    start = performance.now();
    end = start;
    while ( (end - start) < P) {
      for (i=0; i < (TRACE_LENGTH/Cache_line_elem); i+=8) {
        let tmp = T[i];
      }
      count = count + 1;
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
