#define __STDC_FORMAT_MACROS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>
#include <math.h>

#define arrayCount(x)  ( sizeof(x) / sizeof((x)[0]) );


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





//the result of this function is intended to be used as indices of an array; this will choose [size] number
// of random items; like picking n cards from a deck. it will return the randomly chosen items as an int array
void getRandomArrayIdx(int srcSize, int itemCount, int* result) {
    
    int i;
    int j;
    int currItem = 0;
    int itemsRemainingToPick = itemCount;
    int* targItems = malloc(sizeof(int) * srcSize);
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
    
}


int main(int argc, char *argv[]) {

	char* seed1Str;
	char* seed2Str;

    uint64_t seed1;
    uint64_t seed2;
    
    // you can pass two seeds as arguments. if you don't, default random ones will be assigned here
    if (argv[1]) {
    	seed1Str =  argv[1];

    	//convert the char* arg to a uint64_t
	    int result = sscanf(seed1Str, "%" SCNu64, &seed1);

	    //sscanf returns the number of varables filled
		if (result >= 1)  {
	    //    printf("cast in seed: %"PRIu64"\n", seed1);
		} else {
	        // generate default seed; xor the unix time by a big int
			seed1 = (int)time(NULL)^23469655140;
		}
    }


    if (argv[2]) {

    	seed2Str =  argv[2];
    	int result2 = sscanf(seed2Str, "%" SCNu64, &seed2);

		if (result2 >= 1) {
	    //    printf("cast in seed2: %"PRIu64"\n", seed2);
		} else {
	        // generate default seed; xor the unix time by a big int
			seed2 = (int)time(NULL)^93450134347;
		}

    }

	//uint64_t seed[] = {seed1, seed2};
    gblSeed[0] = seed1;
    gblSeed[1] = seed2;
    
    
    int i;
    int j;
    uint64_t tmp;
    
    // first few values always seems to be garbage; so lets call here to throw those away.
    for(i = 0; i < 5; i++) {
       tmp = xoroshiro128plus();
    }
    
    int numbers[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int itemsToGet = 4;
    int srcCount = arrayCount(numbers);
    int* rndResult = malloc (sizeof (int) * sizeof(itemsToGet));
    
    for(i = 0; i < 400; i++) {
        
        //init to all -1
        for (j = 0; j < itemsToGet; j++) {
            rndResult[j] = -1;
        }
        
        getRandomArrayIdx(srcCount, itemsToGet, rndResult);
        
        printf("%i\t%i\t%i\t%i\n", rndResult[0], rndResult[1], rndResult[2], rndResult[3]);
    }
    

    
}

