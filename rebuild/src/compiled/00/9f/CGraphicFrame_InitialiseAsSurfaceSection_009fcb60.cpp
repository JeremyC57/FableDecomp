// CGraphicFrame::InitialiseAsSurfaceSection(const CCountedPointer<CTexture>&, float, float, long, long, bool, bool) -- give the frame a new shared frame data block (reference count 1, no texture, not initialised) and initialise that block as a section of a surface
// Leaf: ?InitialiseAsSurfaceSection@CGraphicFrame@@QAE_NABV?$CCountedPointer@VCTexture@@@@MMJJ_N1@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FCB60, 57 bytes (int3 padding to 0x009FCBA0), tail `jmp`; caller 0x009FE82D.
// Label correction: the catalog row carries its byte twin's name (CGraphicFrame::InitialiseAsSurface, 0x009FCB20).
// Name: Ego_r.pdb ?InitialiseAsSurfaceSection@CGraphicFrame@@QAE_NABV?$CCountedPointer@VCTexture@@@@MMJJ_N1@Z; the donor order
//   CGraphicFrameData::InitialiseAsSurface / InitialiseAsSurfaceSection / CGraphicFrame::InitialiseAsSurface / InitialiseAsSurfaceSection
//   (0x00B6C8F0 / 9D0 / CAB0 / CAF0) is retail's 0x009FC950 / 0x009FCA40 / 0x009FCB20 / 0x009FCB60, and the tail target 0x009FCA40 ends
//   `ret 1Ch` (seven arguments: texture, two float section coordinates (`fld` at +0x14 / +0x18), width, height, alpha, booleanAlpha;
//   the caller 0x009FE82D stores two fdiv results as those floats). The byte twin 0x009FCB20 is InitialiseAsSurface (tail 0x009FC950,
//   `ret 14h`). Two members, two symbols: no binding conflict.
// Callees: operator new 0x00BFEA1A (??2@YAPAXI@Z), CGraphicFrameData::InitialiseAsSurfaceSection 0x009FCA40 (Ego_r name).
// Header: engine/CGraphicFrame.h retail opt-in FABLE_RETAIL_CGRAPHICFRAME (hdr/ overlay, chunk 707): the generated header has no
//   member functions.
// Layout: CGraphicFrameData (no engine header) = Ego_r.pdb CGraphicFrameData, 27 bytes (the `push 1Bh` operator new size, so the
//   class is 1-packed as the PDB size says): ReferenceCount +0x00, Data +0x04, PTexture +0x08 (CCountedPointer<CTexture>, the shared
//   engine/CCountedPointer.h), TLX/TLY/Width/Height/Pitch +0x10..+0x18, the flag byte +0x1a (Alpha, BooleanAlpha, UsesSurface,
//   SectionOfTPage, IsInitialised, ScreenGraphic from bit 0); the landed ~CGraphicFrameData 0x0041BDB0 uses the same member list.
// Shape: `new CGraphicFrameData` expands the inline FableWin.pdb ??0CGraphicFrameData@@QAE@XZ (ReferenceCount = 1, the counted
//   texture's null Data / Info from one `xor ecx,ecx`, IsInitialised cleared with `and byte [eax+1Ah],0EFh`) behind the null test;
//   the null path stores 0 and still makes the call (`xor eax,eax`). The data member's initialiser takes the same five / seven
//   stack arguments, so VC7.1 turns the call into a `jmp` that reuses the caller's argument block.
#define FABLE_RETAIL_CGRAPHICFRAME
#include "engine/CGraphicFrame.h"

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
    CGraphicFrameData() : ReferenceCount(1) { IsInitialised = false; }   // FableWin.pdb ??0CGraphicFrameData@@QAE@XZ (inline)
    bool InitialiseAsSurfaceSection(const CCountedPointer<CTexture>& texture, float tlx, float tly, long width,
                                    long height, bool alpha, bool booleanAlpha);   // 0x009FCA40
};
#pragma pack(pop)

bool CGraphicFrame::InitialiseAsSurfaceSection(const CCountedPointer<CTexture>& texture, float tlx, float tly, long width,
                                               long height, bool alpha, bool booleanAlpha)
{
    FrameData = new CGraphicFrameData;
    return FrameData->InitialiseAsSurfaceSection(texture, tlx, tly, width, height, alpha, booleanAlpha);
}