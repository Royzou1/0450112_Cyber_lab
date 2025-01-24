
const runs = 10;
const N = 1000000;

function measureOneLine() {
  const LINE_SIZE = 16; // 64/sizeof(int)
  let result = [];
  // Fill with -1 to ensure allocation
  const M = new Array(runs * LINE_SIZE).fill(-1);

  for (let i = 0; i < runs; i++) {
    const start = performance.now();
    let val = M[i * LINE_SIZE];
    const end = performance.now();

    result.push(end - start);
  }

  return result;
}
/*
function measureNLines() {
  let result = [];
  const LINE_SIZE = 16; // 64/sizeof(int)
  let array_size = N * LINE_SIZE;
  const M = new Array(array_size).fill(-1);

  for (let j = 0; j < runs; j++) {
    const start = performance.now();
    for (let i = 0; i < N; i++) {
      let val = M[(j*(LINE_SIZE* N) + i * LINE_SIZE) % (array_size)];
    }
    let val = M[j * LINE_SIZE];

    const end = performance.now();
    result.push(end - start);
  }
  return result;
}
*/

//chat did
function measureNLines() {
  const LINE_SIZE = 16; // 64 / sizeof(int), assuming 64-byte cache lines
  const MAX_LINES = (1 << 23) / 64; // Adjust based on how many cache lines you want to measure
  let result = [];

  for (let n = 1; n <= MAX_LINES; n++) {
    let M = new Array(n * LINE_SIZE).fill(-1); // Allocate array with size for n lines
    let latencies = [];

    for (let i = 0; i < runs; i++) {
      const start = performance.now();
      for (let j = 0; j < n; j++) {
        let val = M[j * LINE_SIZE];
      }
      const end = performance.now();
      latencies.push(end - start);
    }

    result.push(latencies.reduce((a, b) => a + b) / runs); // Average latency
  }

  return result;
}

document.getElementById(
  "exercise1-values"
).innerText = `1 Cache Line: [${measureOneLine().join(", ")}]`;


document.getElementById(
  "exercise2-values"
).innerText = `N Cache Lines: [${measureNLines().join(", ")}]`;



