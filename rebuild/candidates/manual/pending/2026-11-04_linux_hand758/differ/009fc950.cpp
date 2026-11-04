// CGraphicFrameData::InitialiseAsSurface(const CCountedPointer<CTexture>& texture, long width, long height, bool alpha, bool booleanAlpha) -- make the frame a whole-texture surface: share the texture, set size / pitch and the alpha flags, initialised, origin 0,0, and flag a screen graphic when the size differs from the texture's level-0 dimensions; returns true
// Grade: DIFFER(217v230) -- rewritten on the landed same-TU sibling InitialiseAsSurfaceSection 0x009FCA40 model (engine/CCountedPointer.h, PDB long width/height leaf) + an early-return IsScreenGraphicFor(const CCountedPointer<CTexture>&) helper: one shared extents slot (frame 8); left: the source Data word kept in ebp (retail homes it in the dead texture slot), the zero register for push 0 / TLX / TLY, `cmp ecx,[eax]` folding (retail mov eax,[eax] ; cmp ecx,eax) and the last compare as setne (retail one shared mov al,1); the || form gives the shared exit but two extents slots (frame 0x10); 11 grades
// Leaf: ?InitialiseAsSurface@CGraphicFrameData@@QAE_NABV?$CCountedPointer@VCTexture@@@@JJ_N1@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FC950, 230 bytes, `ret 14h`.
// Callees: the inline CCountedPointer assignment (engine/CCountedPointer.h: the old count block's DeleteFunc, operator delete
//   0x00BFE9BC), CTexture::GetDimensions(unsigned long) const @ 0x009F9DB0 (C2DExtentsI by hidden pointer, twice).
// Layout: Ego_r.pdb CGraphicFrameData, 27 bytes 1-packed, as the landed same-TU sibling InitialiseAsSurfaceSection 0x009FCA40.
// Shape: the sibling's model; ScreenGraphic compares Width and Height both with the level-0 extents' X (retail reads the first
//   word twice).
#include "engine/CCountedPointer.h"
#include "engine/C2DExtentsI.h"

class CTexture {
public:
    C2DExtentsI GetDimensions(unsigned long level) const;     // 0x009F9DB0
};

#pragma pack(push, 1)
class CGraphicFrameData {
public:
    unsigned long             ReferenceCount;            // +0x00
    void*                     Data;                      // +0x04
    CCountedPointer<CTexture> PTexture;                  // +0x08
    unsigned short            TLX;                       // +0x10
    unsigned short            TLY;                       // +0x12
    unsigned short            Width;                     // +0x14
    unsigned short            Height;                    // +0x16
    unsigned short            Pitch;                     // +0x18
    unsigned char             Alpha : 1;                 // +0x1a bit 0
    unsigned char             BooleanAlpha : 1;          //       bit 1
    unsigned char             UsesSurface : 1;           //       bit 2
    unsigned char             SectionOfTPage : 1;        //       bit 3
    unsigned char             IsInitialised : 1;         //       bit 4
    unsigned char             ScreenGraphic : 1;         //       bit 5
    bool IsScreenGraphicFor(const CCountedPointer<CTexture>& texture) const
    {
        if (Width != texture->GetDimensions(0).X)
            return true;
        if (Height != texture->GetDimensions(0).X)
            return true;
        return false;
    }
    bool InitialiseAsSurface(const CCountedPointer<CTexture>& texture, long width, long height, bool alpha, bool booleanAlpha);
};
#pragma pack(pop)

bool CGraphicFrameData::InitialiseAsSurface(const CCountedPointer<CTexture>& texture, long width, long height, bool alpha, bool booleanAlpha)
{
    UsesSurface = true;
    PTexture = texture;
    Width = (unsigned short)width;
    Pitch = (unsigned short)width;
    Height = (unsigned short)height;
    Alpha = alpha;
    BooleanAlpha = booleanAlpha;
    SectionOfTPage = false;
    IsInitialised = true;
    TLX = 0;
    TLY = 0;
    ScreenGraphic = IsScreenGraphicFor(texture);
    return true;
}
