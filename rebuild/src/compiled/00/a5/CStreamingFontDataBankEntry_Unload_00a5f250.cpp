// CStreamingFontDataBankEntry::Unload() -- release the Font counted pointer (+0x4): drop the shared count, on zero run the deleter on the data and free the count block, then clear the pointer
// Leaf: ?Unload@CStreamingFontDataBankEntry@@UAEXXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00a5f250, 49 bytes, thiscall, plain `ret`.
// Identity (proven): vtable 0x0129D5B4 (RTTI CStreamingFontDataBankEntry) slot 3; Ego_r.pdb ?Unload@CStreamingFontDataBankEntry@@UAEXXZ and CStreamingFontDataBankEntry::Font (CCountedPointer<CFontBank>, +4).
// Callees: the CCPPointerInfo deleter (fastcall, Data in ecx), operator delete 0x00BFE9BC.
// Shape (family L108): CCountedPointer's release with `--Info->RefCount` (Info reloaded after the decrement and for the
//   delete), then Info and Data cleared in that order (ReleaseData) or Data then Info (the destructor).
void __cdecl operator delete(void* p);                     // 0x00BFE9BC
#include "engine/CCountedPointer.h"
class CFontBank;
class CBaseClassNonCopyable {                              // Fable.exe RTTI base
public:
    virtual ~CBaseClassNonCopyable();
};
class CStreamingFontDataBankEntry : public CBaseClassNonCopyable {
public:
    CCountedPointer<CFontBank> Font;                        // +0x4
    virtual void Unload();
};
void CStreamingFontDataBankEntry::Unload()
{
    Font.ReleaseData();
}