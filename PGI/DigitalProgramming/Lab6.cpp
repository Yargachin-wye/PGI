#define WINDOWS_IGNORE_PACKING_MISMATCH

#include "Lab6.h"
#include "stdafx.h"
#include "graphics.h"
#pragma comment(lib,"graphics.lib")

#define FILE_PATH_IN  "res/LAKE256.BMP"
#define FILE_PATH_LOGO "res/logo.bmp"

const unsigned char BackgroundColor[] = { 255,255,255 };
const float koef = 0.5f;

void LogoDeveloper::Run()
{
    auto bmp_in = new BMP_File(FILE_PATH_IN);

    if (!bmp_in->Init())
        return;

    bmp_in->PrintInfo();

    auto bmp_logo = new BMP_File(FILE_PATH_LOGO);

    if (!bmp_logo->Init())
        return;

    bmp_logo->PrintInfo();

    initwindow(bmp_in->GetWidth(), bmp_in->GetHeight());
    for (size_t i = 0; i < bmp_in->GetHeight(); i++)
    {
        for (size_t j = 0; j < bmp_in->GetWidth(); j++)
        {
            auto color_in = bmp_in->GetPixelColor(j, i).values;
            unsigned char color[3] = { color_in[0], color_in[1], color_in[2] };

            if (i < bmp_logo->GetHeight() && j < bmp_logo->GetWidth())
            {
                auto temp_color = bmp_logo->GetPixelColor(j, i).values;

                for (int k = 0; k < 3; k++)
                    color[k] = temp_color[k] * koef + color_in[k] * (1 - koef);
            }

            putpixel(j, bmp_in->GetHeight() - i, RGB(color[2], color[1], color[0]));
        }
    }
    int a;
    std::cin >> a;
    closegraph();
}