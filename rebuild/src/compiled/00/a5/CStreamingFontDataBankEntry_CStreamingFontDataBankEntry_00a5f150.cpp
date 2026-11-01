// CStreamingFontDataBankEntry::~CStreamingFontDataBankEntry() -- font data-bank entry destructor: restore the CStreamingFontDataBankEntry vtable, Unload() (release the Font counted pointer, count block first), then the Font member's own destructor and the CBaseClassNonCopyable base
// Leaf: ??1CStreamingFontDataBankEntry@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A5F150, 100 bytes, thiscall, tail `jmp ??1CBaseClassNonCopyable@@MAE@XZ` (0x0099A300).
// Identity (proven): stores vtable 0x0129D5B4 first (RTTI .?AVCStreamingFontDataBankEntry@@ : CBaseClassNonCopyable : CBaseClass); sole caller is that
//   vtable's slot-0 scalar deleting destructor 0x00A5F800 (call at 0x00A5F803); Ego_r.pdb ??1CStreamingFontDataBankEntry@@UAE@XZ.
//   Catalog label CFontDataBankEntry::~CFontDataBankEntry is propagated from the byte-identical (masked) 0x00A63480; the vtable and its ??_G name this class.
// Callees: the CCPPointerInfo deleter (fastcall, Data in ecx), operator delete 0x00BFE9BC (count block, twice), ~CBaseClassNonCopyable 0x0099A300.
// Layout: Ego_r CStreamingFontDataBankEntry (engine/CStreamingFontDataBankEntry.h is the flattened donor layout with a `void* __vftable` word, so the class is re-declared
//   over the real CBaseClassNonCopyable base): Font +0x04 (CCountedPointer<CFontBank>), Bank +0x0C, Index +0x10, DataType +0x14.
// Shape: the user destructor calls Unload() (devirtualised in the destructor and expanded from its same-TU body, retail's
//   own Unload follows in the file, landed at 0x00A5F250): engine/CCountedPointer.h ReleaseData clears Info then Data (`mov [esi+8],edi ;
//   mov [esi+4],edi`); then the member's implicit ~CCountedPointer runs its release again on the now-zero Info -- VC7.1
//   propagates the zero into eax (`mov eax,edi ; cmp eax,edi ; je`) but keeps the dead block -- and clears Data then Info.
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CCountedPointer.h"
class CFontBank;
class CStreamingFontDataBank;
class CStreamingFontDataBankEntry : public CBaseClassNonCopyable {
public:
    CCountedPointer<CFontBank> Font;                        // +0x04
    CStreamingFontDataBank* Bank;  // +0x0c
    unsigned long Index;                                    // +0x10
    unsigned long DataType;                                 // +0x14
    virtual ~CStreamingFontDataBankEntry();  // slot 0 (??_G)
    virtual bool IsLoaded() const;                          // slot 1
    virtual bool CanUnload() const;                         // slot 2
    virtual void Unload();                                  // slot 3
};
CStreamingFontDataBankEntry::~CStreamingFontDataBankEntry()
{
    Unload();
}
void CStreamingFontDataBankEntry::Unload()  // 0x00A5F250 (graded separately)
{
    Font.ReleaseData();
}