SET( EXTENSION_NAME "Extensions_FreeImage")

ADD_LIBRARY( ${EXTENSION_NAME}
  Meta/FreeImage.cpp
  Resources/FreeImage.cpp
)

TARGET_LINK_LIBRARIES( ${EXTENSION_NAME}
  OpenEngine_Core
  ${FREEIMAGE_LIBRARIES}
)
