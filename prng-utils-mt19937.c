// #include <iostream>

#include <emscripten/emscripten.h>

#define __STDC_FORMAT_MACROS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>
#include <math.h>

#include <stdbool.h>
#include <string.h>

#include "mt64.h"
// #include "md5.h"

#define arrayCount(x)  ( sizeof(x) / sizeof((x)[0]) );


// using namespace std;

// Uses Mersenne twister; (2004/9/29 version). Takuji Nishimura and Makoto Matsumoto.

// get a random int in the specified integer range
int mt19937_range(int rangeLow, int rangeHigh) {
    
  double resultTmp = genrand64_real2() * (double)(rangeHigh - rangeLow);

  int result = rangeLow + (int)round(resultTmp);

  return result;

}

// get a random int in the specified integer range; range Low is assumed to be zero .
int mt19937_range2(int rangeHigh) {
    return mt19937_range(0, rangeHigh);
}





//the result of this function is intended to be used as indices of an array; this will choose [itemCount] number
// of random items; like picking n cards from a deck. it will return the randomly chosen items as an int array
// use the result as array element indices to pick out of your desired array.
void getRandomArrayIdxItems(int srcSize, int itemCount, int* result) {
    
    int i;
    int j;
    int currItem = 0;
    int itemsRemainingToPick = itemCount;
    int targItems[srcSize];
    int _srcSize = srcSize;
    int rndIdx;
    
    //populate the array we'll pick the items from.
    for (i = 0; i < srcSize; i++) {
        targItems[i] = i;
    }

    if (itemCount > 1) {
        for (i = 0; i < itemCount; i++) {

            rndIdx = mt19937_range2(_srcSize - 1); 
            result[currItem] = targItems[rndIdx];

            // shift aray elements to 'remove' taken item. we'll pick from a decreasing array.
            for (j = rndIdx; j < _srcSize - 1; j++) { targItems[j] = targItems[j + 1]; }
            
            _srcSize--;
            currItem++;
        }
    } else {
        rndIdx = mt19937_range2(_srcSize);
        result[0] = targItems[rndIdx];
    }

}




// picks a random item from a set adhering to the weighting array passed in.

//NOTE that this will return an *array index* for you to use in your actual array. Your items array is assumed to be the same length as the weights array, and each
// value in your weights array corresponds to the probability of the same-index item in your values array being chosen.
// by returning a simple array index, this gives you flexibility in the array that you use the random item for.
// the weights param represents proportions of the total of each item's chance of being selected; the weights should add up to 1.0

// 'count' is the array size of the weights parameter.
int getRandomItemWeighted(double* weights, int count) {

    double val = genrand64_real2();
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
// result must already be initialized.
void uuid(char* result, bool includeNull) {
    
    char lookup[] = {'0', '1', '2', '3', '4','5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    int resultSize = 37;  //UUID v4 is 36 chars plus null term.  the 'result' param needs to be alloocated as such, i.e., (char*)malloc(resultSize * sizeof(char)); 
   // if (includeNull) { resultSize = 37; }

   //NOTE: if includeNiull is false, should init result to be 36, and will be handled correctly here.
   //  otherwise init result to 37;
    
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
            result[i] = digit[mt19937_range2(3)];
            continue;
        }
        
        result[i] = lookup[mt19937_range2(15)];
    }
    
    if (includeNull) { result[resultSize - 1] = '\0'; }
}


// not as complex, or robust as a uuid; just gives a simple, 4 character, random id
// id will be all capital letters.
// result must already be initialized.
void getSimpleCharacterId(char* result, bool includeNull) {

    // NOTE: you have to init result outside of this fn. if including null, init to 5, else init to 4;

    int resultSize = 5; // string length of 4 plus null term.


    int i;

    for (i = 0; i < resultSize - 1; i++) {
        // 65 is 'A', 90 is 'Z'
        result[i] = (char)mt19937_range(65, 90);  // random letter between A and Z
    }

    if (includeNull) {
        result[resultSize - 1] = '\0';
    }


}




//gets random variate from normal distribution; derived from pg 491, Law and Kelton
double normal(double mean, double std)  {
    double y  = 0.00;
    double u1 = 0.00;
    double u2 = 0.00;
    double v1 = 0.00;
    double v2 = 0.00;
    double x1 = 0.00;
    //var x2 = 0.00
    double w  = 2.00;  // need w > 1 so loop will fire at least once

    while (w > 1.0) {
        u1 = genrand64_real2();
        u2 = genrand64_real2();
        v1 = (2.0 * u1) - 1.0;
        v2 = (2.0 * u2) - 1.0;
        w = pow(v1, 2) + pow(v2, 2);
    }

    y = pow( ((-2.0 * log(w)) / w), 0.5);

    //x1 and x2 are normally dist random deviates
    //x1 and x2 are z-values
    x1 = v1 * y;
    //x2 = v2 * y

    return mean + (std * x1);
    //return (nmean + (nstd * x2));
}



//picks variates from an exponential distribution
//param lambda - the rate parameter of the distribution, = 1 / Mean
//higher lambda values (! 1.5 - 2.0) put more of the values toward the zero point, with a steep drop off;
//lower lambdas (~ 0.5) will show a more flat and spread out distribution
double exponential(double lambda)  {
  return (double)-1.0 * (log(genrand64_real2()) / lambda);
}





unsigned long elfHash (char *s, size_t strlen)
{
    unsigned long h = 0, high;
    int i;

    for (i = 0; i < strlen; i++) {

      //  printf("elfHash, %i: %lu\n", i, h);

        h = ( h << 4 ) + s[i]++;

        high = (h & 0xF0000000);

        if ( high ) {
            h ^= high >> 24;
        }
        
        h &= ~high;
    }

    return h;
}


/*
const char *md5sum(unsigned char* digestResult, const char *buffer, size_t len)
{
    struct md5_ctx ctx;
    unsigned char digest[16];
    md5_init(&ctx);
    ctx.size = len;
    strcpy(ctx.buf, buffer);
    // or:  strndup(ctx.buf, buffer, strlen(buffer))
    md5_update(&ctx);
    md5_final(digest, &ctx);

    // need to convert digest to a hex string.
    int i = 0;
    char tmp[32];
    // tmp = malloc(32 * sizeof(char));

    for (i = 0; i < 16; i++) {
        strcat(tmp, sprintf("%02X", digest[i]) );
    }

    strcpy(digestResult, tmp);

  //  return digest;
}
*/

// ***********************************************************************************************************************
// ***********************************************************************************************************************

int main(int argc, char *argv[]) {
	printf("PRNG Utils WASM module initialized.\n");
	return 0;
}



//sets the global seed
//IMPORTANT: this must be called from the javascript code before any other random number functions.
int EMSCRIPTEN_KEEPALIVE setSeed(unsigned long long seed) {
    
    init_genrand64(seed);

    return 0;
}


// valueCount is how many randomly generated items you want to return; more efficient on javascript side to get values in chunks than calling function for each value.
int32_t* EMSCRIPTEN_KEEPALIVE getRandomRange(int rangeLow, int rangeHigh, int valueCount) {

	//int32_t* result = new int32_t[valueCount];
    int32_t* result; //[valueCount];
	int i = 0;

    result = malloc(valueCount * sizeof(int32_t));


	for ( i = 0; i < valueCount; i++) {
		result[i] = mt19937_range(rangeLow, rangeHigh);
	}

    int32_t* res = &result[0];
	return res;
  //  return result;
}


// essentially like choosing cards from a deck; will choose 'itemsToChoose' number of items from the 'sourceArraySize'; see getRandomArrayIdxItems
// valueSetCount is basically how many sets of random items you want to pick; more efficient than having to call this function many times from teh js side
// NOTE: this will return a single 1d array. you must break it back apart into [valueSetCount] sets of [itemsToChoose] items on the js side
int32_t* EMSCRIPTEN_KEEPALIVE chooseRandomItems(int sourceArraySize, int itemsToChoose, int valueSetCount) {

    int32_t* result; //[itemsToChoose];
    int32_t* result_tmp;
    int i;
    int j;
    int cntr;

    result = malloc(itemsToChoose * valueSetCount * sizeof(int32_t));
    
    result_tmp = malloc(itemsToChoose * sizeof(int32_t));
    
    cntr = -1;

    for (i = 0; i < valueSetCount; i++) {
        
        for (j = 0; j < itemsToChoose; j++) { result_tmp[j] = -1; } // re-init
        
        getRandomArrayIdxItems(sourceArraySize, itemsToChoose, &result_tmp[0]);
        
        for (j = 0; j < itemsToChoose; j++) {
            //assign to global array
            cntr++;
            result[cntr] = result_tmp[j];
        }
        
    }
    

    //int32_t* res = &result[0];
	return result;

}




 // 'count' is the size of the weights array.
 // 'valueCount' is how many tiems you want to pick based on that weighted array.
int32_t* EMSCRIPTEN_KEEPALIVE chooseRandomItemWeighted(double* weights, int count, int valueCount) {

    int32_t* result; //[valueCount];
    int i;

    result = malloc(valueCount * sizeof(int32_t));

    for (i = 0; i < valueCount; i++) {
        result[i] = getRandomItemWeighted(weights, count);
    }

    return result;
}



    
    
double* EMSCRIPTEN_KEEPALIVE getNormalDistributionVariates(double mean, double std, int valueCount) {
    
    double* result; //[valueCount];
    int i;

    result = malloc(valueCount * sizeof(double));
    
    for ( i = 0; i < valueCount; i++) {
        result[i] = normal(mean, std);
    }
    
    return result;
    
}


double* EMSCRIPTEN_KEEPALIVE getExponentialDistributionVariates(double lambda, int valueCount) {
    
    double* result; //[valueCount];
    int i;

    result = malloc(valueCount * sizeof(double));
    
    for ( i = 0; i < valueCount; i++) {
        result[i] = exponential(lambda);
    }
    
    return result;
    
}





// get the raw uniform distribute variates, range [0.0 to 1.0)
double* EMSCRIPTEN_KEEPALIVE getUniform01DistributionVariates(int valueCount) {

    double* result; //[valueCount];
    int i;

    result = malloc(valueCount * sizeof(double));

    for ( i = 0; i < valueCount; i++) {
        result[i] = genrand64_real2();
    }

    return result;

}



// when you just need a fairly simple numeric, randomly chosen id.
int32_t* EMSCRIPTEN_KEEPALIVE getSimpleNumericIds(int min, int max, int valueCount) {

    int32_t* result; // [valueCount];
    int i;


    result = malloc(valueCount * sizeof(int32_t));


    // choosing the ids this way guarantees that the id's returned are unique
    getRandomArrayIdxItems(max - min, valueCount, result);


    // id's return above start at zero, so to be in the specified range, we have to add min to every value.
    for (i = 0; i < valueCount; i++) {
   //     result[i] = mt19937_range(min, max);
        result[i] += min;
    }

    return result;
}
    

// get count number of uuidv4's
char* EMSCRIPTEN_KEEPALIVE getUuids(int count) {
 
    int resultSize = 36;  //size of the uuid string; 36 chars (no null)
    char* uuidStr;
    int i = 0;
    int j;
    uuidStr =  malloc(resultSize*count * sizeof(char));
    char*  tmp = malloc(resultSize * sizeof(char));
    int cntr = -1;


    // the approach here is that we would normally have a 2d char array (1d array of uuid strings)
    //  but for the wasm to work on the javascript side, we need to use (or it is just easier to user) a 1d char array with the uuid's (plus null term) just appended into one big char array,
    //  and then break it back apart again into 2d on the javascript side either by looking for null terms as the separator or by just 
    //  knowing the 2d array dimension sizes and looping through the 1d array.
    for (j = 0; j < count; j++) {

        for (i = 0; i < resultSize; i++){ tmp[i] = ' '; } // re-init
        uuid(tmp, false);

        for (i = 0; i < resultSize; i++) { 
            cntr++;
	        uuidStr[cntr] = tmp[i]; 
	    }
    }
    

    return uuidStr;
    
}

// get count number of uuidv4's; will include null term in each id
char* EMSCRIPTEN_KEEPALIVE getUuids2(int count) {
 
    int resultSize = 37;  //size of the uuid string; 36 chars plus null
    char* uuidStr;
    int i = 0;
    int j;
    uuidStr =  malloc(resultSize*count * sizeof(char));
    char*  tmp = malloc(resultSize * sizeof(char));
    int cntr = -1;


    // the approach here is that we would normally have a 2d char array (1d array of uuid strings)
    //  but for the wasm to work on the javascript side, we need to use (or it is just easier to user) a 1d char array with the uuid's (plus null term) just appended into one big char array,
    //  and then break it back apart again into 2d on the javascript side either by looking for null terms as the separator or by just 
    //  knowing the 2d array dimension sizes and looping through the 1d array.
    for (j = 0; j < count; j++) {

        for (i = 0; i < resultSize; i++){ tmp[i] = ' '; } // re-init
        uuid(tmp, true);

        for (i = 0; i < resultSize; i++) { 
            cntr++;
	        uuidStr[cntr] = tmp[i]; 
	    }
    }
    

    return uuidStr;
    
}




// get count number of simple char id's
char* EMSCRIPTEN_KEEPALIVE getCharacterIds(int count) {
 
    int resultSize = 4;
    char* idStr;
    int i = 0;
    int j;
    idStr =  malloc(resultSize*count * sizeof(char));
    char*  tmp = malloc(resultSize * sizeof(char));
    int cntr = -1;


    // the approach here is that we would normally have a 2d char array (1d array of uuid strings)
    //  but for the wasm to work on the javascript side, we need to use a 1d char array with the uuid's (plus null term) just appended into one big char array,
    //  and then break it back apart again into 2d on the javascript side either by looking for null terms or by just 
    //  knowing the 2d array dimension sizes and looping through the 1d array.
    for (j = 0; j < count; j++) {

        for (i = 0; i < resultSize; i++){ tmp[i] = ' '; } // re-init
        getSimpleCharacterId(tmp, false);

        for (i = 0; i < resultSize; i++) { 
            cntr++;
	        idStr[cntr] = tmp[i]; 
	    }
    }
    

    return idStr;
    
}


// creates id's for strings that are hashes of each string; you can use this when you wanto to be sure that the same 'id' is used for a given string always.
//  this will always return the same hash value for the same string, so you can use it as an id value in your generated data.
// the strings parameter should be a string that is delimited by a character, the code for which yo pass in as the delimCharCode
// param;  the string will then be split apart based on that delmiter.
// typically, on the javascript side you would have an array that you join together with this same delimiter.
// strings count would be the length of the initial array in javascript.
int32_t* EMSCRIPTEN_KEEPALIVE makeIdsForStrings(char* strings, int stringsCount, int delimCharCode) {

    int32_t* result;
    int i;

    // NOTE: assumes caret char is not in the incoming string; if any caret char is, then would cause unexpected behavior.
    // chose caret because it seems safe; if could do a non-printable char that would be better.
    const char delim[] = {delimCharCode};
    result = malloc(stringsCount * sizeof(int32_t));
    char *strptr = strtok(strings, delim);
    i = -1;

    while(strptr != NULL) {
        i++;
       // printf("char: %s, length %lu\n", strptr, strlen(strptr));
		result[i] = (int32_t) elfHash(strptr, strlen(strptr));
		//printf("result[i]: %i\n", result[i]);
		strptr = strtok(NULL, delim);
	}

    return result;

}

/*
// md5 hashes of input strings.
char* EMSCRIPTEN_KEEPALIVE makeIdsForStringsMD5(char* strings, int stringsCount, int delimCharCode) {

    unsigned char* result;
    int i;
    int md5StrSize = 32;

    // NOTE: assumes caret char is not in the incoming string; if any caret char is, then would cause unexpected behavior.
    // chose caret because it seems safe; if could do a non-printable char that would be better.
    const char delim[] = {delimCharCode};
    result = malloc(stringsCount * md5StrSize * sizeof(char));
    char *strptr = strtok(strings, delim);
    i = -1;

    while(strptr != NULL) {
        i++;
        // printf("char: %s, length %lu\n", strptr, strlen(strptr));
        md5sum (&result[i], strptr, strlen(strptr));
        //printf("result[i]: %i\n", result[i]);
        strptr = strtok(NULL, delim);
    }

    return result;

}

 */
