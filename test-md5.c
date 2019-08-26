//
// Created by eischeid on 8/26/19.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>
#include <math.h>

#include <stdbool.h>
#include <string.h>

#include "md5.h"

const char *md5sum(unsigned char* digestResult, const char *buffer, size_t len) {

    printf("in md5sum");

    struct md5_ctx ctx;
    unsigned char digest[16];

    printf("before md5 init");

    md5_init(&ctx);

    printf("after md5 init");

    ctx.size = len;
    strcpy(ctx.buf, buffer);



    // or:  strndup(ctx.buf, buffer, strlen(buffer))
    md5_update(&ctx);
    md5_final(digest, &ctx);

    // need to convert digest to a hex string.
    int i = 0;
    char tmp[32];
    // tmp = malloc(32 * sizeof(char));

    printf("concat string");

    for (i = 0; i < 16; i++) {
        strcat(tmp, sprintf("%02X", digest[i]) );
    }

    strcpy(digestResult, tmp);

    //  return digest;
}



int main(int argc, char *argv[]) {

    unsigned char* result;
    int i;
    int md5StrSize = 32;

    char strings[] = "val1|val2|val3";
    unsigned char tmp_strptr;

    int stringsCount = 3;

    // NOTE: assumes caret char is not in the incoming string; if any caret char is, then would cause unexpected behavior.
    // chose caret because it seems safe; if could do a non-printable char that would be better.
    const char delim[] = {'|'};

    result = malloc(stringsCount * md5StrSize * sizeof(char));

    char *strptr = strtok(strings, delim);
    i = -1;

    unsigned char* tmp = malloc(md5StrSize * sizeof(char));

    printf("before loop\n");
    while(strptr != NULL) {

        printf("string: %s\n", strptr);
      //  printf("tmp: %s\n", tmp);

        tmp_strptr = malloc(sizeof(char) * strlen(strptr));
        i++;

        strcpy(tmp_strptr, strptr);

        // printf("char: %s, length %lu\n", strptr, strlen(strptr));
        md5sum (tmp, tmp_strptr, strlen(tmp_strptr));
        //printf("result[i]: %i\n", result[i]);



        strcpy(result[i], tmp);

        printf("md5: %s\n", result[i]);

        free(&tmp_strptr);

        strptr = strtok(NULL, delim);
    }

    return result;




    return 0;
}