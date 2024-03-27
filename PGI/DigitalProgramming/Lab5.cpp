#define WINDOWS_IGNORE_PACKING_MISMATCH

#include "Lab5.h"
#include "stdafx.h"
#include "graphics.h"
#pragma comment(lib,"graphics.lib")

#define FILE_PATH_IN  "res/LAKE256.BMP"
#define FILE_PATH_OUT "res/LakeScaled.BMP"

const float koef = 0.3;

void Scaling::Run()
{
    auto bmp = new BMP_File(FILE_PATH_IN);

    if (!bmp->Init())
        return;

    bmp->PrintInfo();

    initwindow(bmp->GetWidth() * koef, bmp->GetHeight() * koef);
    for (size_t i = 0; i < bmp->GetHeight() * koef; i++)
    {
        for (size_t j = 0; j < bmp->GetWidth() * koef; j++)
        {
            auto color = bmp->GetPixelColor(j / koef, i / koef).values;
            putpixel(j, bmp->GetHeight() * koef - i, RGB(color[2], color[1], color[0]));
        }
    }
    int a;
    std::cin >> a;
    closegraph();
}