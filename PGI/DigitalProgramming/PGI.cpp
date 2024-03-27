#include <iostream>
#include "Lab1.h"
#include "Lab2.h"
#include "Lab3.h"
#include "Lab4.h"
#include "Lab5.h"
#include "Lab6.h"
#include "Lab7.h"
#include "Lab8.h"

int main()
{
	setlocale(LC_ALL, "");

	ILab* lab;
	char n;
	std::cin >> n;
	
	switch (n)
	{
	case '1':
		lab = new BaWDeveloper();
		lab->Run();
		break;
	case '2':
		lab = new BorderDeveloper();
		lab->Run();
		break;
	case '3':
		lab = new RotatDeveloper();
		lab->Run();
		break;
	case '4':
		lab = new OutputBMP();
		lab->Run();
		break;
	case '5':
		lab = new Scaling();
		lab->Run();
		break;
	case '6':
		lab = new LogoDeveloper();
		lab->Run();
		break;
	case '7':
		lab = new Steganography();
		lab->Run();
		break;
	case '8':
		lab = new PCXDecoder();
		lab->Run();
		break;
	default:
		break;
	}

	return 0;
}

