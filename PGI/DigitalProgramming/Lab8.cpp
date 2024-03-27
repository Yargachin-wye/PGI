#include "Lab8.h"
#include "stdafx.h"
#define WINDOWS_IGNORE_PACKING_MISMATCH
#include "graphics.h"
#pragma comment(lib, "graphics.lib")

#define FILE_PATH_IN  "res/200001.PCX"

#define BORDER_WIDTH 15

void PCXDecoder::Run()
{
    auto pcx = new PCX_File(FILE_PATH_IN);

    if (!pcx->Init())
        return;

    pcx->PrintInfo();

    initwindow(pcx->GetWidth() + BORDER_WIDTH, pcx->GetHeight() + BORDER_WIDTH); // Увеличили ширину и высоту окна на BORDER_WIDTH

    for (size_t i = 0; i < pcx->GetHeight(); i++)
    {
        for (size_t j = 0; j < pcx->GetWidth(); j++)
        {
            auto color = pcx->GetPixelColor(j, i).values;
            putpixel(j + BORDER_WIDTH / 2, i + BORDER_WIDTH / 2, RGB((int)color[0], (int)color[1], (int)color[2])); // Корректировка координат
        }
    }
    getch();
    closegraph();
}
