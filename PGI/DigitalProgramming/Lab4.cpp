#define WINDOWS_IGNORE_PACKING_MISMATCH

#include "Lab4.h"
#include "stdafx.h"
#include "graphics.h"
#pragma comment(lib,"graphics.lib")

#define FILE_PATH_IN  "res/Lake.bmp"

void OutputBMP::Run()
{
    auto bmp = new BMP_File(FILE_PATH_IN);

    if (!bmp->Init())
        return;

    bmp->PrintInfo();

    initwindow(bmp->GetWidth(), bmp->GetHeight());
    for (size_t i = 0; i < bmp->GetHeight(); i++)
    {
        for (size_t j = 0; j < bmp->GetWidth(); j++)
        {
            auto color = bmp->GetPixelColor(j, i).values;
            putpixel(j, bmp->GetHeight() - i, RGB(color[2], color[1], color[0]));
        }
    }
    int a;
    std::cin >> a;
    closegraph();
}