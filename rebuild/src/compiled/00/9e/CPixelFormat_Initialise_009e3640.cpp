// 009e3640 CPixelFormat::Initialise
// Leaf: ?Initialise@CPixelFormat@@QAEXABVCPixelFormatInit@@@Z
// Flags:
#define CPixelFormat CPixelFormatFields
#include "engine/CPixelFormat.h"
#undef CPixelFormat
// Layout: original public method restored on shared four-byte FormatIndex field.
class CPixelFormatInit;
class CPixelFormat:protected CPixelFormatFields{public:void Initialise(const CPixelFormatInit&);};
enum _D3DFORMAT{D3DFMT_UNKNOWN=0,D3DFMT_FORCE_DWORD=0x7fffffff};
enum EPixelFormatType{PF_TYPE_NULL=0,PF_TYPE_UNKNOWN=1,PF_TYPE_RGBA=2,PF_TYPE_DEPTH=3,PF_TYPE_COMPRESSED_ALPHA=4,PF_TYPE_COMPRESSED_BOOLEAN_ALPHA=5,PF_TYPE_BUMP=6,PF_TYPE_COMPRESSED_INTERPOLATED_ALPHA=7};
// Layout: original Ego TPI 0xabe8, 36 bytes; native table stride 0x24.
class CD3DFormatInfo{public:_D3DFORMAT D3DFormat;EPixelFormatType PFType;long BitDepth,AlphaBits,RBits,GBits,BBits,StencilBits;bool Linear;};
extern const CD3DFormatInfo GD3DFormats[];
// Layout: original Ego TPI 0x3142, six bytes; native byte loads at offsets 0..5.
class CPixelFormatInit{public:unsigned char Type:4;unsigned char Linear:1;unsigned char ColourDepth,RBits,GBits,BBits,ABits;};
FABLE_STATIC_ASSERT(sizeof(CPixelFormat)==4);
FABLE_STATIC_ASSERT(sizeof(CPixelFormatInit)==6);
FABLE_STATIC_ASSERT(sizeof(CD3DFormatInfo)==36);
void CPixelFormat::Initialise(const CPixelFormatInit& init)
{
    FormatIndex = -1;
    switch (init.Type) {
    case 1: {
        long index = 0;
        const CD3DFormatInfo* info = GD3DFormats;
        do {
            if (info->PFType == 2 && info->RBits == init.RBits && info->GBits == init.GBits && info->BBits == init.BBits &&
                info->AlphaBits == init.ABits && info->BitDepth == init.ColourDepth) {
                FormatIndex = index;
                return;
            }
            ++info;
            ++index;
        } while (info->BitDepth != -1);
        break;
    }
    case 4: {
        long index = 0;
        const CD3DFormatInfo* info = GD3DFormats;
        do {
            if (info->PFType == 6 && info->RBits == init.RBits && info->GBits == init.GBits && info->BBits == init.BBits &&
                info->AlphaBits == init.ABits && info->BitDepth == init.ColourDepth) {
                FormatIndex = index;
                break;
            }
            ++info;
            ++index;
        } while (info->BitDepth != -1);
        break;
    }
    case 3: {
        long index = 0;
        const CD3DFormatInfo* info = GD3DFormats;
        do {
            if (info->PFType == 5) {
                FormatIndex = index;
                return;
            }
            ++info;
            ++index;
        } while (info->BitDepth != -1);
        break;
    }
    case 2: {
        long index = 0;
        const CD3DFormatInfo* info = GD3DFormats;
        do {
            if (info->PFType == 4) {
                FormatIndex = index;
                return;
            }
            ++info;
            ++index;
        } while (info->BitDepth != -1);
        break;
    }
    case 5: {
        long index = 0;
        const CD3DFormatInfo* info = GD3DFormats;
        do {
            if (info->PFType == 7) {
                FormatIndex = index;
                return;
            }
            ++info;
            ++index;
        } while (info->BitDepth != -1);
        break;
    }
    }
}