#include "lab4.h"

unsigned char pal[256][4];

void drawBMP(const char* filename) {
	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;

	FILE* file;
	if (fopen_s(&file, filename, "rb") != 0) {
		printf("Не открывает\n");
		return;
	}
	// Считать заголовок файла BMP
	fread(&head_file, sizeof(head), 1, file);

	// Проверка на формат BMP
	if (head_file.id != 0x4D42) {
		std::cout << "Not a BMP file" << std::endl;
		fclose(file);
		return;
	}

	// Проверка на 24-bit BMP (другие форматы не поддерживаются)
	if (head_file.bitperpixel != 24) {
		std::cout << "Only 24-bit BMP supported" << std::endl;
		fclose(file);
		return;
	}

	// Определение размеров изображения
	int width = head_file.width;
	int height = head_file.height;

	// Определение размера отступа (по 4 байта)
	int padding = (4 - (width * 3) % 4) % 4;

	// Выделение памяти для пикселей
	unsigned char* pixels = new unsigned char[width * height * 3];
	fread(pixels, sizeof(unsigned char), width * height * 3, file);
	fclose(file);

	// Создание окна
	HWND hwnd = GetConsoleWindow();
	HDC hdc = GetDC(hwnd);

	// Отображение пикселей на экране
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			unsigned char blue = pixels[(x + y * width) * 3];
			unsigned char green = pixels[(x + y * width) * 3 + 1];
			unsigned char red = pixels[(x + y * width) * 3 + 2];
			SetPixel(hdc, x, height - y, RGB(red, green, blue));
		}
	}

	// Освобождение памяти
	delete[] pixels;
	ReleaseDC(hwnd, hdc);
}