# rppm
PPM Image Reader Include Only Library

## Usage

Include the header and at least one file, define ```RPPM_IMPLEMENTATION``` before inclusion.
Optionally, redefine the default allocation macros before including the library for the implementation.
```c
#define RPPM_MALLOC(m) malloc(m)
#define RPPM_FREE(m) free(m)
```
Define the ```struct rppm``` yourself before calling the API functions.

## API

### Load and free an image with the call:

```
int rppm_load(struct rppm *rppm, const char *filename);
void rppm_free(struct rppm *rppm);
```

Check error conditions when loading the image, ```rppm_load()``` will return non zero in case of error.

### Access pixels with macros:

```c
#define RPPM_RED(p32)   (uint8_t)(((uint32_t)p32)>>24)
#define RPPM_GREEN(p32) (uint8_t)((((uint32_t)p32)>>16)&0xff)
#define RPPM_BLUE(p32)  (uint8_t)((((uint32_t)p32)>>8)&0xff)

#define RPPM_PACKRGB(r,g,b) (uint32_t)(((uint8_t)r)<<24|((uint8_t)g)<<16|((uint8_t)b)<<8)
#define RPPM_GET_PIXEL(rppm,x,y) (uint32_t)((rppm)->pixels[(x)*(rppm)->width+(y)])
```

