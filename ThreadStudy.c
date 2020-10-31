/*
 ============================================================================
 Name        : ThreadStudy.c
 Author      : LiuMingLiang
 Version     :
 Copyright   : 
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "ABCPrint.h"
#include "GooglePrint.h"

//#define ABC_PRINT
#define GOOGLE_PRINT

int main(void) {

	char buf[512];
	char name[32];
	char* pbuf = buf;
	char* pname = name;

	const char* p0 = pbuf;
	p0 = pname;
	*p0 = '\0';

	char const* p1 = name;
	p1 = buf;

	char *const p2 = pname;
	*p2 = '\0';
	p2 = pbuf;


#ifdef ABC_PRINT
	ABCPrint();
#endif

#ifdef GOOGLE_PRINT
	GooglePrint();
#endif

	//puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */

	return EXIT_SUCCESS;
}
