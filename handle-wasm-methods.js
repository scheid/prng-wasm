
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
}

};

getUuids: function(count) {

    var result = Module.ccall(
        'getUuids',
        'number',
        ['number'],
        [count]
        );

    var returnData = [];

    // NOTE: uuid function from the wasm returns array of char pointers; a char is of type uint_8, which is accessed in the heap HEAPU8

    for (let i = 0; i < count; i++) {
        returnData.push( Module["HEAPU8"][(result / UInt8Array.BYTES_PER_ELEMENT) + i] );
    }

    return returnData;

}



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
}