# Pseudo Random Number Generator (PRNG) WebAssembly Module.

Wraps various PRNG functions into a webassembly module (WASM).

Uses the Mersenne Twister Algorithm (see `mt19937-64.c`)

The webassembly module itself is in `prng-utils-mt19937.c`

This module is mainly used in the data synthesizer project.  


-----------------------------------------------------------
NOTE: after you recompile, you need to edit the prng-wasm.js file:
- remove: 'export default PrngEnscriptenModule;' at the end of the file.
- change the first line of the file from `var PrngEnscriptenModule = (function() {` to `exports.PrngEnscriptenModule = (function() {`
- in the second line, change `var _scriptDir = import.meta.url;` to `var _scriptDir;`. when in an angular app, import.meta.url is not recognized, and it is not needed.