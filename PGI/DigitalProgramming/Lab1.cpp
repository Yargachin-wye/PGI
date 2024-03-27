#include "Lab1.h"
#include "stdafx.h"

#define FILE_PATH_IN  "res/LAKE256.BMP"
#define FILE_PATH_OUT "res/LAKE_GRAY.bmp"

using namespace std;

void BaWDeveloper::Run()
{
    auto bmp = new BMP_File(FILE_PATH_IN);

    if (!bmp->Init())
        return;

    bmp->PrintInfo();

    for (int i = 0; i < bmp->GetColorTableSize(); i++)
    {
        auto color = bmp->GetPalitraColor(i);

        int sum = 0;
        for (int j = 0; j < 3; j++)
            sum += color.values[j];

        for (int j = 0; j < 3; j++)
            color.values[j] = sum / 3;

        bmp->SetPalitraColor(i, color);
    }
    bmp->PrintInfo();

    bmp->Write(FILE_PATH_OUT);
}