/*
  
  NodeJs utility script to convert a WebAssembly wasm binary file into a ready to use javascript byte array that you can inline into your javascript source.
  This allows you to skip the step of fetching the wasm binary, and eliminate the need to deploy a wasm file at all.

  Todd Eischeid , 2019
*/
var fs = require('fs');

var cmdArgs = process.argv.slice(2);

console.log("  - processing file: " + cmdArgs[0]);

var inFile = cmdArgs[0]; // "prng-wasm.wasm";
var outFile = inFile.replace(".wasm", "") + "-byte-array.js";
var binary = fs.readFileSync(inFile);
var binaryBytesStr = binary.join(",");  // will be decimal bytes

fs.writeFile(outFile, "// Inline this byte array directly into your javascript source, and then compile the bytes as normal, a la:\n// WebAssembly.instantiate(wasmBytes, yourImportObject)).then(obj => { obj.instance.exports.exported_func();  });\n\nlet wasmBytes = [" + binaryBytesStr + "];", (err) => {
    if (err) { console.log("  - ERROR: file write error", err); } else { console.log("  - conversion complete: written to: " + outFile + "\n"); }
})

