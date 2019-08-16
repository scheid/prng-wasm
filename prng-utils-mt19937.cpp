#include <iostream>

#include <emscripten/emscripten.h>

#define __STDC_FORMAT_MACROS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>
#include <math.h>

#include "mt64.h"

#define arrayCount(x)  ( sizeof(x) / sizeof((x)[0]) );


using namespace std;

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
void getRandomArrayIdxItems(int srcSize, int itemCount, int* result) {
    
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
            rndIdx = mt19937_range2(_srcSize);
            result[currItem] = targItems[rndIdx];

            // shift aray elements to 'remove' taken item.
            for (j = rndIdx; j < _srcSize - 1; j++) { targItems[j] = targItems[j + 1]; }
            
            _srcSize--;
            currItem++;
        }
    } else {
        rndIdx = mt19937_range2(_srcSize);
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
void uuid(char* result) {
    
    char lookup[] = {'0', '1', '2', '3', '4','5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    int resultSize = 37;  //UUID v4 is 36 chars plus null term.  the 'result' param needs to be alloocated as such, i.e., (char*)malloc(resultSize * sizeof(char));
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
    
    result[resultSize - 1] = '\0';
}



//gets random variate from normal distribution; derived from pg 491, Law and Kelton
    double normal(double mean, double std)  {
        double y  = 0.00
        double u1 = 0.00
        double u2 = 0.00
        double v1 = 0.00
        double v2 = 0.00
        double x1 = 0.00
        //var x2 = 0.00
        double w  = 2.00  // need w > 1 so loop will fire at least once

        while (w > 1.0) {
            u1 = genrand64_real2()
            u2 = genrand64_real2()
            v1 = (2.0 * u1) - 1.0
            v2 = (2.0 * u2) - 1.0
            w = pow(v1, 2) + pow(v2, 2)
        }

        y = pow( ((-2.0 * log(w)) / w), 0.5)

        //x1 and x2 are normally dist random deviates
        //x1 and x2 are z-values
        x1 = v1 * y
        //x2 = v2 * y

        return mean + (std * x1)
        //return (nmean + (nstd * x2));
    }

    
    
    //picks variates from an exponential distribution
    //param lambda - the rate parameter of the distribution, = 1 / Mean
    //higher lambda values (! 1.5 - 2.0) put more of the values toward the zero point, with a steep drop off; 
    //lower lambdas (~ 0.5) will show a more flat and spread out distribution
    double exponential(double lambda)  {
      return (double)-1.0 * (log(genrand64_real2()) / lambda)
    }





// ***********************************************************************************************************************
// ***********************************************************************************************************************

int main(int argc, char **argv) {
	cout << "\nPRNG Utils WASM module initialized.\n";
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
int EMSCRIPTEN_KEEPALIVE setSeed(unsigned long long seed) {
    
    init_genrand64(seed);

    return 0;
}


// valueCount is how many randomly generated items you want to return; more efficient on javascript side to get values in chunks than calling function for each value.
int32_t* EMSCRIPTEN_KEEPALIVE getRandomRange(int rangeLow, int rangeHigh, int valueCount) {

	//int32_t* result = new int32_t[valueCount];
    int32_t result[valueCount];
	int i = 0;

	for ( i = 0; i < valueCount; i++) {
		result[i] = mt19937_range(rangeLow, rangeHigh);
	}

    int32_t* res = &result[0];
	return res;
  //  return result;
}


int32_t* EMSCRIPTEN_KEEPALIVE chooseRandomItems(int sourceArraySize, int itemsToChoose) {

    int result[itemsToChoose];
    int i;

    getRandomArrayIdxItems(sourceArraySize, itemsToChoose, &result[0]);

    int32_t* res = &result[0];
	return res;

}

    
    
double* EMSCRIPTEN_KEEPALIVE getNormalDistributionVariates(double mean, double std, int valueCount) {
    
    double result[valueCount];
    int i;
    
    for ( i = 0; i < valueCount; i++) {
        result[i] = normal(mean, std);
    }
    
    return result;
    
}


double* EMSCRIPTEN_KEEPALIVE getExponentialDistributionVariates(double lambda, int valueCount) {
    
    double result[valueCount];
    int i;
    
    for ( i = 0; i < valueCount; i++) {
        result[i] = exponential(lambda);
    }
    
    return result;
    
}

    
    
    
    
 // 'count' is the size of the weights array.
// TODO: make this return an array of weighted choices; same philosophy as the other functions.
int32_t* EMSCRIPTEN_KEEPALIVE chooseRandomItemWeighted(double* weights, int count) {

    return getRandomItemWeighted(weights, count);
}
    

// get count number of uuidv4's
char** EMSCRIPTEN_KEEPALIVE getUuids(int count) {
 
    int resultSize = 37;
    char** uuidStr;
    int i = 0;
    int j;
    uuidStr = (char**) calloc(count, sizeof(char*));

    for (j = 0; j < count; j++) {
        
        uuidStr[j] = (char*)malloc(resultSize * sizeof(char));
        uuid(uuidStr[j]);
    }
    
    return uuidStr;
    
}



#ifdef __cplusplus
}
#endif


