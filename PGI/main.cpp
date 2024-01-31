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
void rotateBMP(const char* inputFileName, const char* outputFileName) {
	std::ifstream inputFile(inputFileName, std::ios::binary);

	if (!inputFile.is_open()) {
		std::cerr << "Ошибка открытия файла: " << inputFileName << std::endl;
		return;
	}

	// Считываем заголовок
	head fileHeader;
	inputFile.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));

	// Проверяем сигнатуру BMP файла
	if (fileHeader.id != 0x4D42) {
		std::cerr << "Неверный формат BMP файла" << std::endl;
		return;
	}

	// Вычисляем новую ширину и высоту после поворота на 90 градусов
	long newWidth = fileHeader.height;
	long newHeight = fileHeader.width;

	// Создаем временный буфер для хранения пикселей
	std::vector<char> pixelBuffer(fileHeader.sizeimage);
	inputFile.read(pixelBuffer.data(), fileHeader.sizeimage);

	// Переворачиваем пиксели на 90 градусов
	for (long y = 0; y < fileHeader.height; ++y) {
		for (long x = 0; x < fileHeader.width; ++x) {
			long oldIndex = y * fileHeader.width * 3 + x * 3;
			long newIndex = (fileHeader.width - x - 1) * newWidth * 3 + y * 3;

			// Копируем пиксель из оригинала в новое место
			for (int i = 0; i < 3; ++i) {
				pixelBuffer[newIndex + i] = pixelBuffer[oldIndex + i];
			}
		}
	}

	// Записываем измененный файл
	std::ofstream outputFile(outputFileName, std::ios::binary);
	outputFile.write(reinterpret_cast<const char*>(&fileHeader), sizeof(fileHeader));
	outputFile.write(pixelBuffer.data(), fileHeader.sizeimage);

	std::cout << "Файл успешно перевернут и сохранен в: " << outputFileName << std::endl;
}
int main() {

	setlocale(LC_ALL, "");

	convertToBW("../res/Lake.bmp", "../res/Lake_BW.bmp");
	addBorder("../res/Lake.bmp", "../res/Lake_Bordered.bmp", 15);
	rotateBMP("../res/Lake.bmp", "../res/Lake_Rotated.bmp");
	printf("Преобразование завершено.\n");



	return 0;
}
