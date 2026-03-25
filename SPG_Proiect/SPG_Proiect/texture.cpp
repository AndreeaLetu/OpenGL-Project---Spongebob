#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "texture.h"
#include <cstdio>

GLuint LoadTexture(const char* filename)
{
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);
    if (!data)
    {
        printf("Texture not loaded: %s\n", filename);
        return 0;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    GLenum format = GL_RGB;
    if (channels == 4)
        format = GL_RGBA;
    else if (channels == 3)
        format = GL_RGB;
    else if (channels == 1)
        format = GL_LUMINANCE;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    stbi_image_free(data);

    return texture;
}