#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <cstdint>

#pragma pack(push, 1) // disable alignment
struct BMPHeader
{
    std::uint16_t magic = 0x4D42;
    std::uint32_t file_size = 0;
    std::uint16_t reserved = 0; // Always 0.
    std::uint16_t reserved_other = 0; // Always 0.
    std::uint32_t data_offset = 0;
};
#pragma pack(pop)

typedef struct                       /**** BMP file info structure ****/
{
    unsigned int   biSize;           /* Size of info header */
    int            biWidth;          /* Width of image */
    int            biHeight;         /* Height of image */
    unsigned short biPlanes;         /* Number of color planes */
    unsigned short biBitCount;       /* Number of bits per pixel */
    unsigned int   biCompression;    /* Type of compression to use */
    unsigned int   biSizeImage;      /* Size of image data */
    int            biXPelsPerMeter;  /* X pixels per meter */
    int            biYPelsPerMeter;  /* Y pixels per meter */
    unsigned int   biClrUsed;        /* Number of colors used */
    unsigned int   biClrImportant;   /* Number of important colors */
} BITMAPINFOHEADER;


#endif // MAIN_H_INCLUDEDs