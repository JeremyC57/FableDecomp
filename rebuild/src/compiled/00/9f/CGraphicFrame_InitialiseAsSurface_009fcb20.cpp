// CGraphicFrame::InitialiseAsSurface(const CCountedPointer<CTexture>&, long, long, bool, bool) -- give the frame a new shared frame data block (reference count 1, no texture, not initialised) and initialise that block as a surface
// Leaf: ?InitialiseAsSurface@CGraphicFrame@@QAE_NABV?$CCountedPointer@VCTexture@@@@JJ_N1@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FCB20, 57 bytes (int3 padding to 0x009FCB60), tail `jmp`; callers 0x009FF1B3, 0x00A6A1E2.
// Name: Ego_r.pdb ?InitialiseAsSurface@CGraphicFrame@@QAE_NABV?$CCountedPointer@VCTexture@@@@JJ_N1@Z; the donor order
//   CGraphicFrameData::InitialiseAsSurface / InitialiseAsSurfaceSection / CGraphicFrame::InitialiseAsSurface / InitialiseAsSurfaceSection
//   (0x00B6C8F0 / 9D0 / CAB0 / CAF0) is retail's 0x009FC950 / 0x009FCA40 / 0x009FCB20 / 0x009FCB60, and the tail target 0x009FC950 ends
//   `ret 14h` (five arguments: texture, width, height, alpha, booleanAlpha). Its byte twin 0x009FCB60 is InitialiseAsSurfaceSection
//   (tail 0x009FCA40, `ret 1Ch`, seven arguments; its caller 0x009FE82D pushes two float section coordinates).
// Callees: operator new 0x00BFEA1A (??2@YAPAXI@Z), CGraphicFrameData::InitialiseAsSurface 0x009FC950 (Ego_r name).
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
    bool InitialiseAsSurface(const CCountedPointer<CTexture>& texture, long width, long height, bool alpha,
                             bool booleanAlpha);         // 0x009FC950
};
#pragma pack(pop)

bool CGraphicFrame::InitialiseAsSurface(const CCountedPointer<CTexture>& texture, long width, long height, bool alpha,
                                        bool booleanAlpha)
{
    FrameData = new CGraphicFrameData;
    return FrameData->InitialiseAsSurface(texture, width, height, alpha, booleanAlpha);
}