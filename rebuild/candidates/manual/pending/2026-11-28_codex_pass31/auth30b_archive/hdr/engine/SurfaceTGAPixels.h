#ifndef FABLE_SURFACE_TGA_PIXELS_H
#define FABLE_SURFACE_TGA_PIXELS_H
#ifdef FABLE_RETAIL_TGA_PIXEL_RECORDS_PASS29
// Both original type records agree on class keys, field names and byte offsets.
struct CATGAPixel { unsigned char B,G,R,A; };
struct CNATGAPixel { unsigned char B,G,R; };
class CRGBALong { public: unsigned char B,G,R,A; };
#endif
#endif
