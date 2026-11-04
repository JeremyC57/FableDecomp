// CGraphicFrameData::InitialiseAsSurfaceSection(const CCountedPointer<CTexture>&, float, float, long, long, bool, bool) -- make the frame a section of a texture page: share the texture, set size / pitch and the alpha flags, the top-left corner from the normalised tlx / tly (times 32767, rounded to nearest), mark it a surface section, initialised and not a screen graphic; returns true
// Leaf: ?InitialiseAsSurfaceSection@CGraphicFrameData@@QAE_NABV?$CCountedPointer@VCTexture@@@@MMJJ_N1@Z
// Flags: none
// Retail-asm: 0x009FCAD4 + 0x009FCAFD `fistp dword ptr [edx]` -- engine/GFFastFloatToLong.h (GFFastFloatToLong, the pointer-out form:
//   `lea edx,[esp+28h] ; mov [esp+24h],edx ; fld [esp+10h] ; mov edx,[esp+24h] ; fistp dword ptr [edx]`). VC7.1 C++ `(long)x`
//   calls __ftol2, `/QIfist` stores `fistp qword`, `_mm_cvtss_si32` emits cvtss2si (NOTES.md attempts table).
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FCA40, 218 bytes, `ret 1Ch`; tail-called by CGraphicFrame::InitialiseAsSurfaceSection 0x009FCB60.
// Name: Ego_r.pdb ?InitialiseAsSurfaceSection@CGraphicFrameData@@QAE_NABV?$CCountedPointer@VCTexture@@@@MMJJ_N1@Z (the landed
//   0x009FCB60 declares the same signature and parameter names).
// Callees: the inline CCountedPointer::ShareData (engine/CCountedPointer.h: the old count block's DeleteFunc, operator delete
//   0x00BFE9BC). Float pool 0x0129C820 = 32767.0f.
// Layout: Ego_r.pdb CGraphicFrameData, 27 bytes 1-packed (as the landed 0x009FCB60 models it).
#include "engine/CCountedPointer.h"
#include "engine/GFFastFloatToLong.h"

class CTexture;

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
    bool InitialiseAsSurfaceSection(const CCountedPointer<CTexture>& texture, float tlx, float tly, long width,
                                    long height, bool alpha, bool booleanAlpha);
};
#pragma pack(pop)

bool CGraphicFrameData::InitialiseAsSurfaceSection(const CCountedPointer<CTexture>& texture, float tlx, float tly, long width,
                                                   long height, bool alpha, bool booleanAlpha)
{
    UsesSurface = true;
    PTexture = texture;
    Width = (unsigned short)width;
    Pitch = (unsigned short)width;
    Height = (unsigned short)height;
    Alpha = alpha;
    BooleanAlpha = booleanAlpha;
    TLX = (unsigned short)GFFastFloatToLong(tlx * 32767.0f);
    TLY = (unsigned short)GFFastFloatToLong(tly * 32767.0f);
    ScreenGraphic = false;
    SectionOfTPage = true;
    IsInitialised = true;
    return true;
}