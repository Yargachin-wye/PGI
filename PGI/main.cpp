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

/*
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
} head_file;*/
/*
void convertToBW(const char* inputFileName, const char* outputFileName) {
	FILE* inputFile;
	FILE* outputFile;

	if (fopen_s(&inputFile, inputFileName, "rb") != 0) {
		printf("Не удалось открыть файл для чтения.\n");
		return;
	}

	if (fopen_s(&outputFile, outputFileName, "wb") != 0) {
		printf("Не удалось открыть файл для записи.\n");
		fclose(inputFile);
		return;
	}

	// Чтение заголовка
	struct head header;
	fread(&header, sizeof(header), 1, inputFile);

	// Запись заголовка в новый файл
	fwrite(&header, sizeof(header), 1, outputFile);

	// Перевод указателя на начало данных изображения
	fseek(inputFile, header.bm_offset, SEEK_SET);

	// Обработка каждого пикселя и запись его в новый файл в чёрно-белом формате
	unsigned char pixel[3];
	while (fread(pixel, sizeof(pixel), 1, inputFile) == 1) {
		// Рассчитываем яркость пикселя как среднее значение цветовых каналов
		unsigned char brightness = (pixel[0] + pixel[1] + pixel[2]) / 3;

		// Записываем чёрно-белый пиксель в новый файл
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
		printf("Не удалось открыть файл для чтения.\n");
		return;
	}

	if (fopen_s(&modified, outputFileName, "wb") != 0) {
		printf("Не удалось открыть файл для записи.\n");
		fclose(original);
		return;
	}

	struct head originalHeader, modifiedHeader;

	// Чтение заголовка оригинального файла
	fread(&originalHeader, sizeof(originalHeader), 1, original);

	// Инициализация заголовка для модифицированного файла
	modifiedHeader = originalHeader;

	// Изменение полей заголовка для учета рамки
	modifiedHeader.width += 30; // увеличение ширины на 15 пикселей с каждой стороны
	modifiedHeader.height += 30; // увеличение высоты на 15 пикселей с каждой стороны
	modifiedHeader.sizeimage = modifiedHeader.width * modifiedHeader.height * (modifiedHeader.bitperpixel / 8);

	// Запись заголовка модифицированного файла
	fwrite(&modifiedHeader, sizeof(modifiedHeader), 1, modified);

	// Запись расширенной цветовой палитры с учетом рамки
	for (int i = 0; i < 30; i++) {
		unsigned char frameColor[4] = { rand() % 255, rand() % 255, rand() % 255, rand() % 255 }; // Например, красный цвет для рамки
		fwrite(frameColor, sizeof(frameColor), 1, modified);
	}

	// Запись оставшейся части палитры из оригинального файла
	fseek(original, originalHeader.bm_offset - sizeof(originalHeader), SEEK_SET);
	unsigned char buffer[4];
	for (int i = 0; i < (originalHeader.bm_offset - sizeof(originalHeader)) / 4; i++) {
		fread(buffer, sizeof(buffer), 1, original);
		fwrite(buffer, sizeof(buffer), 1, modified);
	}

	// Копирование пикселей из оригинального файла с учетом рамки
	fseek(original, originalHeader.bm_offset, SEEK_SET);
	fseek(modified, modifiedHeader.bm_offset, SEEK_SET);

	int rowSize = modifiedHeader.width * (modifiedHeader.bitperpixel / 8);
	int padding = (4 - (rowSize % 4)) % 4; // Выравнивание по 32-битовой границе

	for (int i = 0; i < 15; i++) {
		// Добавление 15 пикселей сверху
		for (int j = 0; j < modifiedHeader.width; j++) {
			unsigned char framePixel[3] = { rand() % 255, rand() % 255, rand() % 255 }; // Например, красный цвет для рамки
			fwrite(framePixel, sizeof(framePixel), 1, modified);
		}
		// Добавление выравнивающих байтов
		for (int k = 0; k < padding; k++) {
			unsigned char paddingByte = 0;
			fwrite(&paddingByte, sizeof(paddingByte), 1, modified);
		}
	}

	for (int y = 0; y < originalHeader.height; y++) {
		// Добавление 15 пикселей слева
		for (int i = 0; i < 15; i++) {
			unsigned char framePixel[3] = { rand() % 255, rand() % 255, rand() % 255 }; // Например, красный цвет для рамки
			fwrite(framePixel, sizeof(framePixel), 1, modified);
		}

		// Копирование строки из оригинального файла
		unsigned char pixel[3];
		for (int x = 0; x < originalHeader.width; x++) {
			fread(pixel, sizeof(pixel), 1, original);
			fwrite(pixel, sizeof(pixel), 1, modified);
		}

		// Добавление 15 пикселей справа
		for (int i = 0; i < 15; i++) {
			unsigned char framePixel[3] = { rand() % 255, rand() % 255, rand() % 255 }; // Например, красный цвет для рамки
			fwrite(framePixel, sizeof(framePixel), 1, modified);
		}

		// Добавление выравнивающих байтов
		for (int i = 0; i < padding; i++) {
			unsigned char paddingByte = 0;
			fwrite(&paddingByte, sizeof(paddingByte), 1, modified);
		}
	}

	for (int i = 0; i < 15; i++) {
		// Добавление 15 пикселей снизу
		for (int j = 0; j < modifiedHeader.width; j++) {
			unsigned char framePixel[3] = { rand() % 255, rand() % 255, rand() % 255 }; // Например, красный цвет для рамки
			fwrite(framePixel, sizeof(framePixel), 1, modified);
		}
		// Добавление выравнивающих байтов
		for (int k = 0; k < padding; k++) {
			unsigned char paddingByte = 0;
			fwrite(&paddingByte, sizeof(paddingByte), 1, modified);
		}
	}

	fclose(original);
	fclose(modified);
}

void rotateBMP(const std::string& input_filename, const std::string& output_filename) {
	std::ifstream input_file(input_filename, std::ios::binary);
	if (!input_file.is_open()) {
		std::cerr << "Error opening input file: " << input_filename << std::endl;
		return;
	}

	head header;
	input_file.read(reinterpret_cast<char*>(&header), sizeof(head));

	if (header.bitperpixel != 24) {
		std::cerr << "This program supports only TrueColor BMP files (24 bits per pixel)." << std::endl;
		return;
	}

	int32_t new_width = header.height;
	int32_t new_height = header.width;

	// Update header with new dimensions
	header.width = new_width;
	header.height = new_height;
	header.sizeimage = (new_width * 3 + (4 - (new_width * 3) % 4) % 4) * new_height + header.bm_offset;

	std::ofstream output_file(output_filename, std::ios::binary);
	if (!output_file.is_open()) {
		std::cerr << "Error opening output file: " << output_filename << std::endl;
		return;
	}

	// Write updated header to the output file
	output_file.write(reinterpret_cast<const char*>(&header), sizeof(head));

	std::array<char, 3> pixel_data;

	// Rotate and transpose the image
	for (int x = 0; x < header.width; ++x) {
		for (int y = 0; y < header.height; ++y) {
			int original_index = (header.height - y - 1) * (header.width * 3 + (4 - (header.width * 3) % 4) % 4) + x * 3;
			int new_index = x * (new_width * 3 + (4 - (new_width * 3) % 4) % 4) + y * 3;
			input_file.seekg(header.bm_offset + original_index);
			input_file.read(pixel_data.data(), pixel_data.size());
			output_file.seekp(header.bm_offset + new_index);
			output_file.write(pixel_data.data(), pixel_data.size());
		}
	}

	// Add padding zeros to the end of each row
	for (int y = 0; y < new_height; ++y) {
		int start_index = y * (new_width * 3 + (4 - (new_width * 3) % 4) % 4);
		int end_index = (y + 1) * (new_width * 3 + (4 - (new_width * 3) % 4) % 4);
		for (int i = start_index; i < end_index; ++i) {
			output_file.put('\0');
		}
	}

	std::cout << "Transformation completed. Result saved to " << output_filename << "." << std::endl;
}
*/
/*
void save16ColorBMP() {

	char header[54] = {
		'B', 'M',            // Signature
		0, 0, 0, 0,           // File size
		0, 0, 0, 0,           // Reserved
		54, 0, 0, 0,          // Image data offset
		40, 0, 0, 0,          // Header size
		static_cast<char>(width), static_cast<char>(width >> 8), static_cast<char>(width >> 16), static_cast<char>(width >> 24),  // Width
		static_cast<char>(height), static_cast<char>(height >> 8), static_cast<char>(height >> 16), static_cast<char>(height >> 24),  // Height
		1, 0,                 // Color planes
		4, 0,                 // Bits per pixel
		0, 0, 0, 0,           // Compression
		0, 0, 0, 0,           // Image size
		0, 0, 0, 0,           // Horizontal resolution
		0, 0, 0, 0,           // Vertical resolution
		16, 0,                // Number of colors in the palette
		0, 0, 0, 0            // Number of important colors
	};
	*/

int main() {

	setlocale(LC_ALL, "");

	// convertToBW("../res/Lake.bmp", "../res/Lake_BW.bmp");
	// addBorder("../res/Lake.bmp", "../res/Lake_Bordered.bmp", 15);
	// rotateBMP("../res/Lake.bmp", "../res/Lake_Rotated.bmp");
	printf("Преобразование завершено.\n");
	/*
	initwindow(500, 500);
	getch();
	closegraph();
	*/
	return 0;
}
