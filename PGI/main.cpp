#include <windows.h>
#include <iostream>

using namespace std;

struct head {
    unsigned short id;
    unsigned long f_size;
    unsigned short rez1, rez2;
    unsigned long bm_offset;
    unsigned long h_size;
    long width;
    long height;
    unsigned short planes;
    unsigned short bitperpixel;
    unsigned long compression;
    unsigned long sizeimage;
    unsigned long hres;
    unsigned long vres;
    unsigned long clrused;
    unsigned long clrimp;
} head_file;

void DrawBMP(const char* filename) {
    // Открыть BMP файл
    FILE* file;
    if (fopen_s(&file, filename, "rb") != 0) {
        printf("Не открывается!\n");
        return;
    }

    fread(&head_file, sizeof(head), 1, file);

    // Проверка на формат BMP
    if (head_file.id != 0x4D42) {
        cout << "Not a BMP file" << endl;
        fclose(file);
        return;
    }

    // Определение размеров изображения
    int width = head_file.width;
    int height = head_file.height;

    // Определение размера отступа (по 4 байта)
    int padding = (4 - (width * head_file.bitperpixel / 8) % 4) % 4;

    // Выделение памяти для пикселей
    unsigned char* pixels = new unsigned char[width * height * head_file.bitperpixel / 8];
    fread(pixels, sizeof(unsigned char), width * height * head_file.bitperpixel / 8, file);
    fclose(file);

    // Создание окна
    HWND hwnd = GetConsoleWindow();
    HDC hdc = GetDC(hwnd);

    // Отображение пикселей на экране
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            unsigned long color = 0;
            for (int i = 0; i < head_file.bitperpixel / 8; ++i) {
                color |= pixels[(x + y * width) * (head_file.bitperpixel / 8) + i] << (8 * i);
            }
            std::cout << "pixel" << std::endl;
            SetPixel(hdc, x, height - y, color);
        }
    }

    // Освобождение памяти
    delete[] pixels;
    ReleaseDC(hwnd, hdc);
}

int main() {
    DrawBMP("../res/cat.bmp");
    return 0;
}