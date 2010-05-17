#ifndef _INCLUDE_FREE_IMAGE_
#define _INCLUDE_FREE_IMAGE_

#include <FreeImage.h>

#include <string>

std::string ColorTypeToString(FREE_IMAGE_COLOR_TYPE ct);
std::string TypeToString(FREE_IMAGE_TYPE t);
unsigned int accountForEndian(unsigned int c);

#endif // _INCLUDE_FREE_IMAGE_
