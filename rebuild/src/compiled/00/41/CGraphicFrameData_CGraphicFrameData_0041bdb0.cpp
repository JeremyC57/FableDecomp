// CGraphicFrameData::~CGraphicFrameData() -- uninitialise if still initialised, then release the texture counted pointer
// Leaf: ??1CGraphicFrameData@@QAE@XZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe @ 0x0041BDB0, 60 bytes, `ret`, no direct reference (reached from the ??_G / array helpers).
// Catalog label CGraphicFrameData::`scalar_deleting_destructor' names the wrong special member: nothing is freed and
// there is no flags argument; this is the plain destructor (the Ego_r.pdb ??_GCGraphicFrameData@@QAEPAXI@Z wraps it).
// Body: if (IsInitialised) (bit 4 of the bitfield byte +0x1a) Uninitialise() (0x009FCBA0, which itself clears that bit);
// then PTexture's ~CCountedPointer: if Info, --RefCount and at zero DeleteFunc(Data) (__fastcall) + operator delete(Info)
// (0x00BFE9BC); Data = Info = 0.
// Layout: Ego_r.pdb CGraphicFrameData (27 bytes): ReferenceCount +0, Data +4, PTexture +8 (CCountedPointer<CTexture>),
// TLX/TLY/Width/Height/Pitch +0x10..+0x18, bitfield byte +0x1a (Alpha, BooleanAlpha, UsesSurface, SectionOfTPage,
// IsInitialised, ScreenGraphic); no engine/ header.
class CTexture;
#include "engine/CCountedPointer.h"
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
    unsigned char             Alpha : 1;                 // +0x1a
    unsigned char             BooleanAlpha : 1;
    unsigned char             UsesSurface : 1;
    unsigned char             SectionOfTPage : 1;
    unsigned char             IsInitialised : 1;
    unsigned char             ScreenGraphic : 1;
    void Uninitialise();                                 // 0x009FCBA0
    ~CGraphicFrameData();
};
CGraphicFrameData::~CGraphicFrameData()
{
    if (IsInitialised)
        Uninitialise();
}