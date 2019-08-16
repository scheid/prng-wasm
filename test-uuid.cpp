//
//  test-uuid.cpp
//  
//
//  Created by Eischeid, Todd on 8/14/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mt64.h"


using namespace std;


// get a random number in the specified integer range
int mt19937_range(int rangeLow, int rangeHigh) {
    
    double resultTmp = genrand64_real2() * (double)(rangeHigh - rangeLow);
    
    int result = rangeLow + (int)round(resultTmp);
    
    return result;
    
}

// get a random number in the specified integer range; range Low is assumed to be zero .
int mt19937_range2(int rangeHigh) {
    return mt19937_range(0, rangeHigh);
}







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
            result[i] = digit[mt19937_range2(3)];
            continue;
        }
        
        result[i] = lookup[mt19937_range2(15)];
    }
    
    result[resultSize - 1] = '\0';
}


char** getUuids(int count) {
 
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



int main(int argc, char **argv) {
    
    init_genrand64(293856122398);
    

    int resultSize = 37;
    
    char* uuidStr = (char*)malloc(resultSize * sizeof(char));
    int i = 0;
    for (i = 0; i < resultSize; i++)  { uuidStr[i] = ' '; }

    
    int j;

int count = 100;

    char** result;
   //char* result[count]; 

   result = getUuids(count);

    
    for (j = 0; j < count; j++) {
        printf("%s\n", result[j]);
    }

  //  for (j = 0; j < 200; j++) {
  //      uuid(uuidStr);
  //      printf("%s\n", uuidStr);
  //  }
    
    
    
    return 0;
}




