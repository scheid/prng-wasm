#include <iostream>

#include <emscripten/emscripten.h>

#define __STDC_FORMAT_MACROS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>
#include <math.h>
#include <iomanip>

#include <string>
#include <cstring>

#define arrayCount(x)  ( sizeof(x) / sizeof((x)[0]) );


using namespace std;

uint64_t max_val = 18446744073709551615U;
uint64_t gblSeed[2];

// got this from: https://nullprogram.com/blog/2017/09/21/
// all functions here ultimately call this function
/*
uint64_t xoroshiro128plus(uint64_t s[2]) {
    uint64_t s0 = s[0];
    uint64_t s1 = s[1];
    uint64_t result = s0 + s1;
    s1 ^= s0;
    s[0] = ((s0 << 55) | (s0 >> 9)) ^ s1 ^ (s1 << 14);
    s[1] = (s1 << 36) | (s1 >> 28);
    return result;
}
 */

// NOTE: you must get gblSeed prior to calling this function.
uint64_t xoroshiro128plus() {
    uint64_t s0 = gblSeed[0];
    uint64_t s1 = gblSeed[1];
    uint64_t result = s0 + s1;
    s1 ^= s0;
    gblSeed[0] = ((s0 << 55) | (s0 >> 9)) ^ s1 ^ (s1 << 14);
    gblSeed[1] = (s1 << 36) | (s1 >> 28);
    return result;
}




// get a random number as a double in the range 0-1
double xoroshiro128plus01() {
  
  // double and uint64_t are both 8 bytes
  return (double)xoroshiro128plus() / (double)max_val;
}


// get a random number in the specified integer range
int xoroshiro128plus_range(int rangeLow, int rangeHigh) {
  // double and uint64_t are both 8 bytes
  double resultTmp = (double)xoroshiro128plus01() * (double)(rangeHigh - rangeLow);

  int result = rangeLow + (int)round(resultTmp);

  return result;

}

// get a random number in the specified integer range; range Low is assumed to be zero .
int xoroshiro128plus_range2(int rangeHigh) {
    return xoroshiro128plus_range(0, rangeHigh);
}





//the result of this function is intended to be used as indices of an array; this will choose [itemCount] number
// of random items; like picking n cards from a deck. it will return the randomly chosen items as an int array
void getRandomArrayIdx(int srcSize, int itemCount, int* result) {
    
    int i;
    int j;
    int currItem = 0;
    int itemsRemainingToPick = itemCount;
    int* targItems = new int[srcSize]; // malloc(sizeof(int) * srcSize);
    int _srcSize = srcSize;
    int rndIdx;
    
    //populate the array we'll pick the items from.
    for (i = 0; i < srcSize; i++) {
        targItems[i] = i;
    }

    if (itemCount > 1) {
        for (i = 0; i < itemCount; i++) {
            rndIdx = xoroshiro128plus_range2(_srcSize);
            result[currItem] = targItems[rndIdx];

            // shift aray elements to 'remove' taken item.
            for (j = rndIdx; j < _srcSize - 1; j++) { targItems[j] = targItems[j + 1]; }
            
            _srcSize--;
            currItem++;
        }
    } else {
        rndIdx = xoroshiro128plus_range2(_srcSize);
        result[0] = targItems[rndIdx];
    }

    delete [] targItems;
    targItems = NULL;
    
}




// picks a random item from a set adhering to the weighting array passed in.

//NOTE that this will return an *array index* for you to use in your actual array. Your items array is assumed to be the same length as the weights array, and each
// value in your weights array corresponds to the probability of the same-index item in your values array.
// by returning a simple array index, this gives you max flexibility in the array that you use the random item for.
// the weights param represents proportions of the total of each item's chance of being selected; the weights should add up to 1.0

int getRandomItemWeighted(double* weights, int count) {

    double val = xoroshiro128plus01();
    double weightCumulative = 0.0;
    int result = -1;
    int i = 0;

    for (i = 0; i < count; i++) {
        if ((val > weightCumulative) && (val <= weightCumulative + weights[i])) {
            result = i;
            break;
        }
        weightCumulative += weights[i];
    }

    return result;
}




//uuid v4
//you can use this one if you need to  generate a predictable/consistent sequence of uuid's , because this will use the random generator that is seeded
// In other types of applications, this would be an undesired use case, but for some of our generation of hypothetical values (patient records),
// we may need to be able to reproduce the same value sequences, including 'random' id's
void uuid(char* result) {
    
    char lookup[] = {'0', '1', '2', '3', '4','5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    int resultSize = 37;
    char digit[] = {'8', '9', 'a', 'b'};
    int i;
    
    for (i = 0; i < resultSize - 1; i++) {
        
        if ((i == 8) || (i == 13) | (i == 18) || (i == 23)) {
            result[i] = '-';
            continue;
        }
        
        if (i == 14) {
            result[i] = '4';
            continue;
        }
        
        if (i == 19) {
            result[i] = digit[xoroshiro128plus_range2(3)];
            continue;
        }
        
        result[i] = lookup[xoroshiro128plus_range2(15)];
    }
    
    result[resultSize - 1] = '\0';
}

int main(int argc, char **argv) {
	cout << "\nTest wasm module initialized.\n";
	return 0;
}


// We are including the #ifdef blocks so that if you are trying to include this in C++ code, 
// the example will still work. Due to C versus C++ name mangling rules, this would otherwise break, 
// but here we are setting it so that it treats it as an external C function if you are using C++.
#ifdef __cplusplus
extern "C" {
#endif

//sets the global seed
//IMPORTANT: this must be called from the javascript code before any other random number functions.
int EMSCRIPTEN_KEEPALIVE setSeed(uint32_t seed1, uint32_t seed2) {
    gblSeed[0] = seed1;
    gblSeed[1] = seed2;

    return 0;
}


// valueCount is how many randomly generated items you want to return; more efficient on javascript side to get values in chunks than calling function for each value.
int32_t* EMSCRIPTEN_KEEPALIVE getRandomRange(int rangeLow, int rangeHigh, int valueCount) {

	//int32_t* result = new int32_t[valueCount];
    int32_t result[valueCount];
	int i = 0;

	for ( i = 0; i < valueCount; i++) {
		result[i] = xoroshiro128plus_range(rangeLow, rangeHigh);
	}

    int32_t* res = &result[0];
	return res;
  //  return result;
}


int32_t* EMSCRIPTEN_KEEPALIVE chooseRandomItems(int sourceArraySize, int itemsToChoose) {

    int result[itemsToChoose];
    int i;

    getRandomArrayIdx(sourceArraySize, itemsToChoose, &result[0]);

    int32_t* res = &result[0];
	return res;

}
    
    

char** EMSCRIPTEN_KEEPALIVE getUuid(int count) {
 
    int resultSize = 37;
    char** uuidStr;
    int i = 0;
    for (i = 0; i < resultSize; i++)  { uuidStr[i] = ' '; }

    int j;
    
    for (j = 0; j < count; j++) {
        
        uuidStr[0] = (char*)malloc(resultSize * sizeof(char));
        
        uuid(uuidStr[0]);
        //printf("%s\n", uuidStr);
    }
    
    return uuidStr
}



#ifdef __cplusplus
}
#endif


