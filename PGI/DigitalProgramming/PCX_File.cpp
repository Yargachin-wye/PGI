#include "PCX_File.h"
#include "stdafx.h"

using namespace std;

PCX_File::PCX_File(std::string filename) :
	m_info(nullptr),
	m_palitra(nullptr),
	m_pixelColors(nullptr)
{
	m_filename = filename;
}

bool PCX_File::Init()
{
	auto file = new FILE();

	try
	{
		fopen_s(&file, m_filename.c_str(), "rb");
	}
	catch (std::exception ex)
	{
		return false;
	}

	auto result = ReadFile(file);
	fclose(file);

	return result;
}

bool PCX_File::Reset()
{
	delete m_info;
	return Init();
}

void PCX_File::PrintInfo()
{
	printf("%s: %i\n", "ID", (int)m_info->ID);
	printf("%s: %i\n", "Version", (int)m_info->Version);
	printf("%s: %i\n", "Coding", (int)m_info->Coding);
	printf("%s: %i\n", "BitPerPixel", (int)m_info->BitPerPixel);
	printf("%s: %i\n", "XMin", (int)m_info->XMin);
	printf("%s: %i\n", "YMin", (int)m_info->YMin);
	printf("%s: %i\n", "XMax", (int)m_info->XMax);
	printf("%s: %i\n", "YMax", (int)m_info->YMax);
	printf("%s: %i\n", "HRes", (int)m_info->HRes);
	printf("%s: %i\n", "VRes", (int)m_info->VRes);
	printf("%s: %s\n", "Palette", m_info->Palette);
	printf("%s: %i\n", "Reserved", (int)m_info->Reserved);
	printf("%s: %i\n", "Planes", (int)m_info->Planes);
	printf("%s: %i\n", "BytePerLine", (int)m_info->BytePerLine);
	printf("%s: %i\n", "PaletteInfo", (int)m_info->PaletteInfo);
	printf("%s: %i\n", "HScreenSize", (int)m_info->HScreenSize);
	printf("%s: %i\n", "VScreenSize", (int)m_info->VScreenSize);
	printf("%s: %s\n", "Filler", m_info->Filler);
	printf("------------------------------------------\n");
}

const Color& PCX_File::GetPixelColor(unsigned int i, unsigned int j)
{
	unsigned long long ind = i * GetWidth() + j;
	if (ind >= m_pixelColors->size())
		return Color{ {0,0,0} };

	return (*m_pixelColors)[ind];
}

bool PCX_File::ReadFile(FILE* file)
{
	return ReadInfo(file) && ReadImageData(file);
}

bool PCX_File::ReadInfo(FILE* file)
{
	m_info = new Info();
	fseek(file, 0, SEEK_SET);
	fread(m_info, sizeof(*m_info), 1, file);
	if (ferror(file) != 0)
	{
		delete m_info;
		return false;
	}

	return true;
}

bool PCX_File::ReadImageData(FILE* file)
{
	fseek(file, -(1 << 8) * 3 - 1, SEEK_END);
	auto vga_offset = ftell(file);
	unsigned char vga_byte;
	fread(&vga_byte, 1, 1, file);

	bool vga_exist = m_info->Version == 5 && vga_byte == 0xC;

	m_palitra = new vector<Color>();

	if (vga_exist)
	{
		for (int i = 0; i < 1 << 8; i++)
		{
			Color buff{};
			fread(buff.values, 1, 3, file);
			m_palitra->push_back(buff);
		}
	}
	else
	{
		for (int i = 0; i < 1 << 4; i++)
		{
			Color buff{};
			for (int j = 0; j < 3; j++)
				buff.values[j] = m_info->Palette[i * 3 + j];
			m_palitra->push_back(buff);
		}
	}

	int n = 0;
	m_pixelColors = new vector<Color>();
	fseek(file, 128, SEEK_SET);
	do
	{
		unsigned char byte;
		n = fread(&byte, 1, 1, file);

		if (vga_exist && ftell(file) > vga_offset)
			break;

		if (byte >> 6 == 0b11)
		{
			int counter = byte & 0x3F;
			fread(&byte, 1, 1, file);
			for (int i = 0; i < counter; i++)
				m_pixelColors->push_back((*m_palitra)[byte]);
		}
		else
			m_pixelColors->push_back((*m_palitra)[byte]);

	} while (n == 1);

	return true;
}

unsigned long PCX_File::GetPaddind()
{
	return GetLineLength() * 8 / m_info->BitPerPixel - GetWidth();
}
