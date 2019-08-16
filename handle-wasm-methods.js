
let obj = {
    seedSet: false,

 objSetSeed: function(val1, val2) {
    if (!this.seedSet) {
        var result = Module.ccall(
            'setSeed',
            'number',
            ['number', 'number'],
            [val1, val2]
        );

        seedSet = true;
    }

},

 chooseRandomItems: function(sourceSize, itemsToPick) {
    // const itemsToPick = 8;

    var result2 = Module.ccall(
        'chooseRandomItems',	// name of C function
        'number',	// return type; 'number' is for arrays too
        ['number', 'number'],	// argument types
        [sourceSize, itemsToPick]	// arguments
    );


    const returnData2 = [];

    for (let v = 0; v < itemsToPick; v++) {

        returnData2.push(Module["HEAP32"][(result2 / Int32Array.BYTES_PER_ELEMENT) + v]);

    }

    //console.log(returnData2.join(","));
    return returnData2;
},

getUuids: function(count) {

start = new Date().getTime();

    var result = Module.ccall(
        'getUuids',
        'number',
        ['number'],
        [count]
        );

    var returnData = [];

end1 = new Date().getTime();
    // NOTE: uuid function from the wasm returns  char pointers; a char is of type uint_8, which is accessed in the heap HEAPU8

    console.log("uuids ", count);
  //  console.log(result);
    let cntr = -1;
    let tmp = [];
    for (let i = 0; i < (count*37); i++) {

       if (Module["HEAPU8"][(result / Uint8Array.BYTES_PER_ELEMENT) + i] == 0) {
         returnData.push(tmp.join(""));
         tmp = [];
       } else {
         tmp.push( String.fromCharCode(Module["HEAPU8"][(result / Uint8Array.BYTES_PER_ELEMENT) + i]) );
       }

    }


end = new Date().getTime();

console.log("wasm+js processing time ms ", end - start);
console.log("wasm processing time ms ", end1 - start);

/*
    for (let i = 0; i < count; i++) {
        tmp = [];
        for (let j = 0; j < 37; j++) {
            cntr++;
            tmp.push( String.fromCharCode(Module["HEAPU8"][(result / Uint8Array.BYTES_PER_ELEMENT) + cntr]) );
        }
        returnData.push(tmp.join(""));
    }
*/

    return returnData;

}
};



function customFunctionHandler() {
    
    
    console.log('in customFunctionHandler');




    console.log("calling random range method");

    const itemsCount = 200;

    if (!this.seedSet) {
        var result = Module.ccall(
            'setSeed',
            'number',
            ['number', 'number'],
           [1248643, 1058374]
        );

        seedSet = true;
    }
   
//
// [10684, 2019566]

    var result = Module.ccall(
        'getRandomRange',	// name of C function 
        'number',	// return type; 'number' is for arrays too
        ['number', 'number', 'number'],	// argument types
        [1,10, itemsCount]	// arguments
    );

    const returnData = [];

    for (let v = 0; v < itemsCount; v++) {
        
        returnData.push(Module["HEAP32"][result/Int32Array.BYTES_PER_ELEMENT + v]);

    }


   // console.log("result ", result);
    console.log(returnData.join(","));


    console.log("calling choosing items");

    const itemsToPick = 8;

    var result2 = Module.ccall(
        'chooseRandomItems',	// name of C function 
        'number',	// return type; 'number' is for arrays too
        ['number', 'number'],	// argument types
        [20, itemsToPick]	// arguments
    );


    const returnData2 = [];

    for (let v = 0; v < itemsToPick; v++) {
        
        returnData2.push(Module["HEAP32"][result2/Int32Array.BYTES_PER_ELEMENT + v]);

    }

    console.log(returnData2.join(","));




    let uuidItems = obj.getUuids(50);

    console.log("gettinng uuid's");
  //  console.log(uuidItems);
   document.getElementById("output").value = uuidItems.join("\n");
}
