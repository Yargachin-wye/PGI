#pragma pack (1)
#include <stdio.h>
#include <locale.h>
#include <wchar.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <vector>
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

void convertToBW(const char* inputFileName, const char* outputFileName) {
	FILE* inputFile;
	FILE* outputFile;

	if (fopen_s(&inputFile, inputFileName, "rb") != 0) {
		printf("�� ������� ������� ���� ��� ������.\n");
		return;
	}

	if (fopen_s(&outputFile, outputFileName, "wb") != 0) {
		printf("�� ������� ������� ���� ��� ������.\n");
		fclose(inputFile);
		return;
	}

	// ������ ���������
	struct head header;
	fread(&header, sizeof(header), 1, inputFile);

	// ������ ��������� � ����� ����
	fwrite(&header, sizeof(header), 1, outputFile);

	// ������� ��������� �� ������ ������ �����������
	fseek(inputFile, header.bm_offset, SEEK_SET);

	// ��������� ������� ������� � ������ ��� � ����� ���� � �����-����� �������
	unsigned char pixel[3];
	while (fread(pixel, sizeof(pixel), 1, inputFile) == 1) {
		// ������������ ������� ������� ��� ������� �������� �������� �������
		unsigned char brightness = (pixel[0] + pixel[1] + pixel[2]) / 3;

		// ���������� �����-����� ������� � ����� ����
		fwrite(&brightness, sizeof(brightness), 1, outputFile);
		fwrite(&brightness, sizeof(brightness), 1, outputFile);
		fwrite(&brightness, sizeof(brightness), 1, outputFile);
	}

	fclose(inputFile);
	fclose(outputFile);
}

void addBorder(const char* inputFileName, const char* outputFileName, int borderWidth) {
	FILE* original;
	FILE* modified;
	if (fopen_s(&original, inputFileName, "rb") != 0) {
		printf("�� ������� ������� ���� ��� ������.\n");
		return;
	}

	if (fopen_s(&modified, outputFileName, "wb") != 0) {
		printf("�� ������� ������� ���� ��� ������.\n");
		fclose(original);
		return;
	}

	struct head originalHeader, modifiedHeader;

	// ������ ��������� ������������� �����
	fread(&originalHeader, sizeof(originalHeader), 1, original);

	// ������������� ��������� ��� ����������������� �����
	modifiedHeader = originalHeader;

	// ��������� ����� ��������� ��� ����� �����
	modifiedHeader.width += 30; // ���������� ������ �� 15 �������� � ������ �������
	modifiedHeader.height += 30; // ���������� ������ �� 15 �������� � ������ �������
	modifiedHeader.sizeimage = modifiedHeader.width * modifiedHeader.height * (modifiedHeader.bitperpixel / 8);

	// ������ ��������� ����������������� �����
	fwrite(&modifiedHeader, sizeof(modifiedHeader), 1, modified);

	// ������ ����������� �������� ������� � ������ �����
	for (int i = 0; i < 30; i++) {
		unsigned char frameColor[4] = { rand() % 255, rand() % 255, rand() % 255, rand() % 255 }; // ��������, ������� ���� ��� �����
		fwrite(frameColor, sizeof(frameColor), 1, modified);
	}

	// ������ ���������� ����� ������� �� ������������� �����
	fseek(original, originalHeader.bm_offset - sizeof(originalHeader), SEEK_SET);
	unsigned char buffer[4];
	for (int i = 0; i < (originalHeader.bm_offset - sizeof(originalHeader)) / 4; i++) {
		fread(buffer, sizeof(buffer), 1, original);
		fwrite(buffer, sizeof(buffer), 1, modified);
	}

	// ����������� �������� �� ������������� ����� � ������ �����
	fseek(original, originalHeader.bm_offset, SEEK_SET);
	fseek(modified, modifiedHeader.bm_offset, SEEK_SET);

	int rowSize = modifiedHeader.width * (modifiedHeader.bitperpixel / 8);
	int padding = (4 - (rowSize % 4)) % 4; // ������������ �� 32-������� �������

	for (int i = 0; i < 15; i++) {
		// ���������� 15 �������� ������
		for (int j = 0; j < modifiedHeader.width; j++) {
			unsigned char framePixel[3] = { rand() % 255, rand() % 255, rand() % 255 }; // ��������, ������� ���� ��� �����
			fwrite(framePixel, sizeof(framePixel), 1, modified);
		}
		// ���������� ������������� ������
		for (int k = 0; k < padding; k++) {
			unsigned char paddingByte = 0;
			fwrite(&paddingByte, sizeof(paddingByte), 1, modified);
		}
	}

	for (int y = 0; y < originalHeader.height; y++) {
		// ���������� 15 �������� �����
		for (int i = 0; i < 15; i++) {
			unsigned char framePixel[3] = { rand() % 255, rand() % 255, rand() % 255 }; // ��������, ������� ���� ��� �����
			fwrite(framePixel, sizeof(framePixel), 1, modified);
		}

		// ����������� ������ �� ������������� �����
		unsigned char pixel[3];
		for (int x = 0; x < originalHeader.width; x++) {
			fread(pixel, sizeof(pixel), 1, original);
			fwrite(pixel, sizeof(pixel), 1, modified);
		}

		// ���������� 15 �������� ������
		for (int i = 0; i < 15; i++) {
			unsigned char framePixel[3] = { rand() % 255, rand() % 255, rand() % 255 }; // ��������, ������� ���� ��� �����
			fwrite(framePixel, sizeof(framePixel), 1, modified);
		}

		// ���������� ������������� ������
		for (int i = 0; i < padding; i++) {
			unsigned char paddingByte = 0;
			fwrite(&paddingByte, sizeof(paddingByte), 1, modified);
		}
	}

	for (int i = 0; i < 15; i++) {
		// ���������� 15 �������� �����
		for (int j = 0; j < modifiedHeader.width; j++) {
			unsigned char framePixel[3] = { rand() % 255, rand() % 255, rand() % 255 }; // ��������, ������� ���� ��� �����
			fwrite(framePixel, sizeof(framePixel), 1, modified);
		}
		// ���������� ������������� ������
		for (int k = 0; k < padding; k++) {
			unsigned char paddingByte = 0;
			fwrite(&paddingByte, sizeof(paddingByte), 1, modified);
		}
	}

	fclose(original);
	fclose(modified);
}
void rotateBMP(const char* inputFileName, const char* outputFileName) {
	std::ifstream inputFile(inputFileName, std::ios::binary);

	if (!inputFile.is_open()) {
		std::cerr << "������ �������� �����: " << inputFileName << std::endl;
		return;
	}

	// ��������� ���������
	head fileHeader;
	inputFile.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));

	// ��������� ��������� BMP �����
	if (fileHeader.id != 0x4D42) {
		std::cerr << "�������� ������ BMP �����" << std::endl;
		return;
	}

	// ��������� ����� ������ � ������ ����� �������� �� 90 ��������
	long newWidth = fileHeader.height;
	long newHeight = fileHeader.width;

	// ������� ��������� ����� ��� �������� ��������
	std::vector<char> pixelBuffer(fileHeader.sizeimage);
	inputFile.read(pixelBuffer.data(), fileHeader.sizeimage);

	// �������������� ������� �� 90 ��������
	for (long y = 0; y < fileHeader.height; ++y) {
		for (long x = 0; x < fileHeader.width; ++x) {
			long oldIndex = y * fileHeader.width * 3 + x * 3;
			long newIndex = (fileHeader.width - x - 1) * newWidth * 3 + y * 3;

			// �������� ������� �� ��������� � ����� �����
			for (int i = 0; i < 3; ++i) {
				pixelBuffer[newIndex + i] = pixelBuffer[oldIndex + i];
			}
		}
	}

	// ���������� ���������� ����
	std::ofstream outputFile(outputFileName, std::ios::binary);
	outputFile.write(reinterpret_cast<const char*>(&fileHeader), sizeof(fileHeader));
	outputFile.write(pixelBuffer.data(), fileHeader.sizeimage);

	std::cout << "���� ������� ���������� � �������� �: " << outputFileName << std::endl;
}
int main() {

	setlocale(LC_ALL, "");

	convertToBW("../res/Lake.bmp", "../res/Lake_BW.bmp");
	addBorder("../res/Lake.bmp", "../res/Lake_Bordered.bmp", 15);
	rotateBMP("../res/Lake.bmp", "../res/Lake_Rotated.bmp");
	printf("�������������� ���������.\n");



	return 0;
}
