// CGraphicDataBank::CGraphicHeader::CGraphicHeader() -- an empty graphic header: zero sizes, one frame, no pixel format, no mipmaps, not a flat animated graphic
// Leaf: ??0CGraphicHeader@CGraphicDataBank@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FD6C0, 58 bytes. Name: Ego_r/FableWin PDB ??0CGraphicHeader@CGraphicDataBank@@QAE@XZ.
//   Evidence: the layout below is the PDB CGraphicDataBank::CGraphicHeader (0x1C bytes) store for store, and the sole caller
//   CEngineSurfaceCompositionManager::WriteTextureToBank (0x00B2BF1D) then fills Width/Height/FrameWidth/... words and
//   clears the IsFlatAnimatedGraphic bit of this local. The catalog label "C3DAnimationSequenceData::C3DAnimationSequenceData"
//   is propagated (that PDB class is 0x2C bytes with a char* first).
// Shape: a member-initialiser list in declaration order (body assignments put PixelFormat's ctor first); CPixelFormat's inline ctor (FormatIndex = -1); the one-bit field is read-modify-written (`and dl,0FEh`).
#define CPixelFormat CPixelFormatLayout
#include "engine/CPixelFormat.h"
#undef CPixelFormat
class CPixelFormat : public CPixelFormatLayout {
public:
    CPixelFormat() { FormatIndex = -1; }
};

class CGraphicDataBank {
public:
    class CGraphicHeader {
    public:
        unsigned short Width;                       // +0x00
        unsigned short Height;                      // +0x02
        unsigned short Depth;                       // +0x04
        unsigned short FrameWidth;                  // +0x06
        unsigned short FrameHeight;                 // +0x08
        unsigned short FrameCount;                  // +0x0a
        CPixelFormat   PixelFormat;                 // +0x0c
        unsigned char  TransparencyType;            // +0x10
        unsigned char  MipmapLevels;                // +0x11
        unsigned char  IsFlatAnimatedGraphic : 1;   // +0x12
        unsigned long  FrameDataSize;               // +0x14
        unsigned long  CompressedMipmapSizes[1];    // +0x18
        CGraphicHeader();
    };
};

CGraphicDataBank::CGraphicHeader::CGraphicHeader()
    : Width(0), Height(0), Depth(0), FrameWidth(0), FrameHeight(0), FrameCount(1), PixelFormat(),
      TransparencyType(0), MipmapLevels(0), IsFlatAnimatedGraphic(false), FrameDataSize(0)
{
    CompressedMipmapSizes[0] = 0;
}