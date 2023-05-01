#include "Bitmap.h"
#include <iostream>
#include <stdexcept>

#include <thirdParty/stb_image.h>

using namespace ogl;

inline unsigned GetPixelOffset(unsigned col, unsigned row, unsigned width, unsigned height, Bitmap::Format format) {
	return (row * width + col) * format;
}

/*
	Bitmap class
*/

Bitmap::Bitmap(unsigned width, unsigned height, Format format, void* pixels)
{
	_set(width, height, format, pixels);
}

Bitmap::~Bitmap()
{
	if (_pixels) free(_pixels);
}

Bitmap Bitmap::bitmapFromFile(std::string filePath)
{
	int width, height, channels = 3;
    void* pixels = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
		//util::read_tga(filePath.c_str(), &width, &height);
	if (!pixels) throw std::runtime_error(stbi_failure_reason());
	
	//std::cout << "pixel : " << pixels << std::endl;

	Bitmap bmp(width, height, (Format) channels, pixels);
	stbi_image_free(pixels);

	return bmp;
}

void Bitmap::flipVertically()
{
	unsigned long rowSize = _format * _width;
	unsigned char* rowBuffer = new unsigned char[rowSize];
	unsigned halfRows = _height / 2;

	for (unsigned rowIdx = 0; rowIdx < halfRows; rowIdx++) {
		void* row = (unsigned char*)_pixels + GetPixelOffset(0, rowIdx, _width, _height, _format);
		void* oppositeRow = (unsigned char*)_pixels + GetPixelOffset(0, _height - rowIdx - 1, _width, _height, _format);

		memcpy(rowBuffer, row, rowSize);
		memcpy(row, oppositeRow, rowSize);
		memcpy(oppositeRow, rowBuffer, rowSize);
	}

	delete rowBuffer;
}

unsigned int Bitmap::width() const{
	return _width;
}

unsigned int Bitmap::height() const{
	return _height;
}

Bitmap::Format Bitmap::format() const
{
	return _format;
}

void* Bitmap::pixelBuffer() const
{
	return _pixels;
}

void Bitmap::_set(unsigned width, unsigned height, Format format, void* pixels)
{
	if (width == 0) throw std::runtime_error("Zero width bitmap");
	if (height == 0) throw std::runtime_error("Zero height bitmap");
	if (format <= 0 || format > 4) throw std::runtime_error("Invalid bitmap format");

	_width = width;
	_height = height;
	_format = format;
	//_pixels = pixels;

	size_t newSize = _width * _height * _format;
	if (_pixels) {
		_pixels = realloc(_pixels, newSize);
	} else {
		_pixels = malloc(newSize);
	}

	if (pixels) {
		memcpy(_pixels, pixels, newSize);
		//std::cout << "size of pixel : " << newSize;
	}
}