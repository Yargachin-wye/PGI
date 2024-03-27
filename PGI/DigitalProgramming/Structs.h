#pragma once

struct Color
{
	unsigned char values[3];
};

union HiColor
{
	uint16_t val;
	unsigned char bytes[sizeof(uint16_t)];
};