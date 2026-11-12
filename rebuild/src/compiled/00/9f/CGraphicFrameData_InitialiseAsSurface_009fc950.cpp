// CGraphicFrameData::InitialiseAsSurface(const CCountedPointer<CTexture>& texture, long width, long height, bool alpha, bool booleanAlpha)
// -- make the frame a whole-texture surface: share the texture, set size / pitch and the alpha flags, not a texture-page section,
// initialised, origin 0,0, and flag a screen graphic when its size differs from the texture's level-0 width; returns true
// Leaf: ?InitialiseAsSurface@CGraphicFrameData@@QAE_NABV?$CCountedPointer@VCTexture@@@@JJ_N1@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FC950, 230 bytes, `ret 14h`; tail-called by CGraphicFrame::InitialiseAsSurface 0x009FCB20
//   (landed, which declares the same signature) -- the only two references (jmp 0x009FCB48 / 0x009FCB54).
// Name: Ego_r.pdb ?InitialiseAsSurface@CGraphicFrameData@@QAE_NABV?$CCountedPointer@VCTexture@@@@JJ_N1@Z.
// Callees: the inline CCountedPointer<CTexture>::operator= (engine/CCountedPointer.h ShareData: the old count block's DeleteFunc,
//   operator delete 0x00BFE9BC), CTexture::GetDimensions(unsigned long) const 0x009F9DB0 (C2DExtentsI by hidden pointer, twice,
//   through the inline CTexture::GetWidth(level)).
// Layout: engine/CGraphicFrame.h CGraphicFrameData (Ego_r.pdb, 27 bytes 1-packed; FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS), as the
//   landed same-TU sibling InitialiseAsSurfaceSection 0x009FCA40.
// Shape: UsesSurface first (its own read-modify-write), the texture shared, then Width, Height, Pitch in that order (that order
//   gives retail's registers in the merged flag-byte update); ScreenGraphic is a `||` of the inline GetWidth() / GetHeight() of
//   the frame against the texture's GetWidth(0) both times -- retail reads the first extents word for both compares
//   (0x009FC9F8 / 0x009FCA10 `mov eax,[eax]`), and the inline GetWidth(level) ends each C2DExtentsI temporary at its return, so
//   both calls share one 8-byte frame slot.
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CGRAPHICFRAMEDATA_INITIALISEASSURFACE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE_GETWIDTH
#include "engine/CCountedPointer.h"
#include "engine/CTexture.h"
#include "engine/CGraphicFrame.h"

bool CGraphicFrameData::InitialiseAsSurface(const CCountedPointer<CTexture>& texture, long width, long height, bool alpha, bool booleanAlpha)
{
    UsesSurface = true;
    PTexture = texture;
    Width = (unsigned short)width;
    Height = (unsigned short)height;
    Pitch = (unsigned short)width;
    Alpha = alpha;
    BooleanAlpha = booleanAlpha;
    SectionOfTPage = false;
    IsInitialised = true;
    TLX = 0;
    TLY = 0;
    ScreenGraphic = GetWidth() != texture->GetWidth(0) || GetHeight() != texture->GetWidth(0);
    return true;
}