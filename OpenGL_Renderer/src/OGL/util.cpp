#include <GL/glew.h>

#include "util.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>   

/*
 * Boring, non-OpenGL-related utility functions
 */

static short util::le_short(unsigned char* bytes)
{
    return bytes[0] | ((char)bytes[1] << 8);
}

static std::string util::readFromFile(const char* pathToFile)
{
    std::string content;
    std::ifstream fileStream(pathToFile, std::ios::in);

    if (!fileStream.is_open()) {
        std::cerr << "Could not read file " << pathToFile << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    std::stringstream ss;
    while (!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
        ss << line << "\n";
    }

    fileStream.close();
    //std::cout << "'" << content << "'" << std::endl;

    return content;
}

void* util:: read_tga(const char* filename, int* width, int* height)
{
    struct tga_header {
        char  id_length;
        char  color_map_type;
        char  data_type_code;
        unsigned char  color_map_origin[2];
        unsigned char  color_map_length[2];
        char  color_map_depth;
        unsigned char  x_origin[2];
        unsigned char  y_origin[2];
        unsigned char  width[2];
        unsigned char  height[2];
        char  bits_per_pixel;
        char  image_descriptor;
    } header;

    int i, color_map_size, pixels_size;
    FILE* f;
    size_t read;
    void* pixels;

    f = fopen(filename, "r");

    if (!f) {
        fprintf(stderr, "Unable to open %s for reading\n", filename);
        return NULL;
    }

    read = fread(&header, 1, sizeof(header), f);

    if (read != sizeof(header)) {
        fprintf(stderr, "%s has incomplete tga header\n", filename);
        fclose(f);
        return NULL;
    }
    if (header.data_type_code != 2) {
        fprintf(stderr, "%s is not an uncompressed RGB tga file\n", filename);
        fclose(f);
        return NULL;
    }
    if (header.bits_per_pixel != 24) {
        fprintf(stderr, "%s is not a 24-bit uncompressed RGB tga file\n", filename);
        fclose(f);
        return NULL;
    }

    for (i = 0; i < header.id_length; ++i)
        if (getc(f) == EOF) {
            fprintf(stderr, "%s has incomplete id string\n", filename);
            fclose(f);
            return NULL;
        }

    color_map_size = le_short(header.color_map_length) * (header.color_map_depth / 8);
    for (i = 0; i < color_map_size; ++i)
        if (getc(f) == EOF) {
            fprintf(stderr, "%s has incomplete color map\n", filename);
            fclose(f);
            return NULL;
        }

    *width = le_short(header.width);
    *height = le_short(header.height);
    pixels_size = (*width) * (*height) * (header.bits_per_pixel / 8);
    pixels = malloc(pixels_size);

    read = fread(pixels, 1, pixels_size, f);
    fclose(f);

    if (sizeof((int)read) != sizeof(pixels_size)) {
        fprintf(stderr, "%s has incomplete image\n", filename);
        free(pixels);
        return NULL;
    }

    return pixels;
}