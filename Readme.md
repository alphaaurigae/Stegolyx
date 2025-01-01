# Stegolyx

- Image steganopgraphy - experimental (only LSB for now).
> Start to end marker apparently works more precise for text extraction, as end null byte appends characters on some images.

- Image to image only a placeholder yet.

- Testing, experimental....


## Deps
```
- libopencv-dev | https://opencv.org/ | https://github.com/opencv/opencv

```

## Build

- Build OS: "Ubuntu 24.04"

... cmake ...
(g++/clang - Cmakelists "option(USE_CLANG" ON/OFF - default clang)

- ```./build_cmake.sh``` - build
- ```./clean_cmake.sh``` - tidy build


## Todo

- multi line text
- image to image
- alternative embedding to LSB only
- other alternatives to startend marker / null marker?
- beautify code
- sec?
- alternative obfuscation tables?
- encryption?