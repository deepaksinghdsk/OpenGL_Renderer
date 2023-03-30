#pragma once
#include <string>

namespace ogl {

	/* 
		A bitmap image (i.e. a grid of pixels).
	*/
	class Bitmap {
	public:
		enum Format {
			Format_Grayscale = 1, // one channel: grayscale
			Format_GrayscaleAlpha = 2, // two channels: grayscale and alpha
			Format_RGB = 3, // three channels: red, green, blue
			Format_RGBA = 4 // four channels: red, green, blue, alpha
		};

		Bitmap(unsigned width, unsigned height, Format format, void* pixels = NULL);
		~Bitmap();

		/*
			Tries to load the given file into a ogl::Bitmap.
		*/
		static Bitmap bitmapFromFile(std::string filePath);

		void flipVertically();

		unsigned width() const;

		unsigned height() const;

		Format format() const;

		void* pixelBuffer() const;

	private:
		Format _format;
		unsigned _width;
		unsigned _height;
		void* _pixels;

		void _set(unsigned width, unsigned height, Format format, void* pixels);
	};

}