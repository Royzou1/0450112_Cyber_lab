const runs = 10;
const N = 100000;

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

function measureNLines() {
  let result = [];
  const LINE_SIZE = 16; // 64/sizeof(int)
  let array_size = 10000 * LINE_SIZE;
  const M = new Array(array_size).fill(-1);

  for (let j = 0; j < 10; j++) {
    const start = performance.now();
    for (let i = 0; i < N; i++) {
      let val = M[(j*(LINE_SIZE* N) + i * LINE_SIZE) % (array_size)];
    }
    const end = performance.now();
    result.push(end - start);
  }
  return result;
}

document.getElementById(
  "exercise1-values"
).innerText = `1 Cache Line: [${measureOneLine().join(", ")}]`;

document.getElementById(
  "exercise2-values"
).innerText = `N Cache Lines: [${measureNLines().join(", ")}]`;
