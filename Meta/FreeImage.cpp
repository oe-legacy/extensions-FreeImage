#include <Meta/FreeImage.h>

#include <Core/Exceptions.h>

using OpenEngine::Core::Exception;

std::string ColorTypeToString(FREE_IMAGE_COLOR_TYPE ct) {
    if (ct == FIC_MINISBLACK)
        return "FIC_MINISBLACK";
    else if (ct == FIC_MINISWHITE)
        return "FIC_MINISWHITE";
    else if (ct == FIC_PALETTE)
        return "FIC_PALETTE";
    else if (ct == FIC_RGB)
        return "FIC_RGB";
    else if (ct == FIC_RGBALPHA)
        return "FIC_RGBALPHA";
    else if (ct == FIC_CMYK)
        return "FIC_CMYK";
    else
        return "unknown type";
}

std::string TypeToString(FREE_IMAGE_TYPE t) {
    if (t == FIT_BITMAP)
        return "FIT_BITMAP";
    else if (t == FIT_UINT16)
        return "FIT_UINT16";
    else if (t == FIT_INT16)
        return "FIT_INT16";
    else if (t == FIT_UINT32)
        return "FIT_UINT32";
    else if (t == FIT_INT32)
        return "FIT_INT32";
    else if (t == FIT_FLOAT)
        return "FIT_FLOAT";
    else if (t == FIT_DOUBLE)
        return "FIT_DOUBLE";
    else if (t == FIT_COMPLEX)
        return "FIT_COMPLEX";
    else if (t == FIT_RGB16)
        return "FIT_RGB16";
    else if (t == FIT_RGBA16)
        return "FIT_RGBA16";
    else if (t == FIT_RGBF)
        return "FIT_RGBF";
    else if (t == FIT_RGBAF)
        return "FIT_RGBAF";
    else if (t == FIT_UNKNOWN)
        return "FIT_UNKNOWN";
    else
        return "unknown type";
}


unsigned int accountForEndian(unsigned int c) {
    if (c == 2)
        return FI_RGBA_RED;
    else if (c == 1)
        return FI_RGBA_GREEN;
    else if (c == 0)
        return FI_RGBA_BLUE;
    else if (c == 3)
        return FI_RGBA_ALPHA;
     else
         throw Exception("unknown FI COLOR CHANNEL");
}

