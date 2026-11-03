#ifndef FABLE_ENGINE_CD3DFORMATINFO_H
#define FABLE_ENGINE_CD3DFORMATINFO_H
#include "engine/Direct3D9Types.h"
enum EPixelFormatType {PF_TYPE_NULL=0,PF_TYPE_UNKNOWN=1,PF_TYPE_RGBA=2,PF_TYPE_DEPTH=3,PF_TYPE_COMPRESSED_ALPHA=4,PF_TYPE_COMPRESSED_BOOLEAN_ALPHA=5,PF_TYPE_BUMP=6,PF_TYPE_COMPRESSED_INTERPOLATED_ALPHA=7};
// Layout: original classV/public fields; native GD3DFormats stride36.
class CD3DFormatInfo {public:_D3DFORMAT D3DFormat;EPixelFormatType PFType;long BitDepth,AlphaBits,RBits,GBits,BBits,StencilBits;bool Linear;};
extern const CD3DFormatInfo GD3DFormats[];
#endif
