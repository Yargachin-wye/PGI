#pragma once
#include <iostream>
#include <windows.h>
#include "head.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#pragma pack (1)
struct  head {
	unsigned short id;
	unsigned long  f_size;
	unsigned short rez1, rez2;
	unsigned long  bm_offset;
	unsigned long  h_size;
	long  width;
	long  height;
	unsigned short planes;
	unsigned short bitperpixel;
	unsigned long  compression;
	unsigned long  sizeimage;
	unsigned long  hres;
	unsigned long  vres;
	unsigned long  clrused;
	unsigned long  clrimp;
} head_file;

void drawBMP(const char* filename);