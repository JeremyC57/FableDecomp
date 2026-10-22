// CGraphicDataBank::GetTextureType(const CGraphicHeader& header, bool param) -- pick the ETextureType for a graphic from its pixel format (signed / compressed) and its transparency type
// Leaf: GetTextureType
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FD2B0, 212 bytes incl. the 4-entry jump table, `ret 8`. Called from 0x009FD76D.
//   Real signature (FableWin.pdb): `CGraphicDataBank::ETextureType CGraphicDataBank::GetTextureType(const CGraphicHeader&, bool) const`.
// Callees: CPixelFormat::IsCompressed 0x009E3940, CPixelFormat::IsSigned 0x009E3970.
// Shape: the Ego_r build of this function (ghidra_out/decomp_localisation.c) keeps four result variables; retail has
//   five: a separate default (transparency 0 or > 4) result, 10/8/4/6/0, beside the case-4 one. All five stay in
//   registers (case 1 edx, 2 esi, 3 edi, 4 eax, default ecx) and the switch on the zero-extended TransparencyType
//   (`movzx ebp` -- a `long` local; an `unsigned char` local is spilled to the frame) returns the chosen one.
//   `!IsCompressed()` gives the `neg ; sbb ; inc` bool. Each branch assigns the default result FIRST in the source:
//   with the case-1 result first VC7.1 swaps their registers (ecx/edx) everywhere, although it still emits the
//   case-1 constant first. The ETextureType values are the retail constants (names not in the PDB dump).
#include "engine/CPixelFormat.h"

struct CPixelFormatMethods : CPixelFormat {
    bool IsCompressed() const;                     // 0x009E3940
    bool IsSigned() const;                         // 0x009E3970
};

class CGraphicDataBank {
public:
    enum ETextureType { TEXTURE_TYPE_0 = 0 };
    struct CGraphicHeader {                        // Ego_r.pdb, 0x1C bytes
        unsigned short      Width;                 // +0x00
        unsigned short      Height;                // +0x02
        unsigned short      Depth;                 // +0x04
        unsigned short      FrameWidth;            // +0x06
        unsigned short      FrameHeight;           // +0x08
        unsigned short      FrameCount;            // +0x0A
        CPixelFormatMethods PixelFormat;           // +0x0C
        unsigned char       TransparencyType;      // +0x10
        unsigned char       MipmapLevels;          // +0x11
        unsigned char       IsFlatAnimatedGraphic : 1;   // +0x12
        unsigned long       FrameDataSize;         // +0x14
        unsigned long       CompressedMipmapSizes[1];    // +0x18
    };
    ETextureType GetTextureType(const CGraphicHeader& header, bool param) const;
};

CGraphicDataBank::ETextureType CGraphicDataBank::GetTextureType(const CGraphicHeader& header, bool param) const
{
    long transparency = header.TransparencyType;
    bool uncompressed = !header.PixelFormat.IsCompressed();
    long type1, type2, type3, type4, type0;
    if (header.PixelFormat.IsSigned()) {
        type0 = 10; type1 = 10; type2 = 10; type4 = 10; type3 = 10;
    } else if (param) {
        if (uncompressed) {
            type0 = 8; type1 = 9; type2 = 9; type4 = 9; type3 = 9;
        } else {
            type0 = 4; type1 = 5; type2 = 5; type4 = 5; type3 = 5;
        }
    } else if (uncompressed) {
        type0 = 6; type1 = 7; type2 = 7; type4 = 7; type3 = 7;
    } else {
        type0 = 0; type1 = 1; type2 = 2; type4 = 1; type3 = 3;
    }
    switch (transparency) {
    case 1: return (ETextureType)type1;
    case 2: return (ETextureType)type2;
    case 3: return (ETextureType)type3;
    case 4: return (ETextureType)type4;
    default: return (ETextureType)type0;
    }
}