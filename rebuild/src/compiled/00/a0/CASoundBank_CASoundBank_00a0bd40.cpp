// CASoundBank::~CASoundBank() -- implicit destructor: release the counted symbol map (PSymbolMap, +0x04), then CBaseClassNonCopyable::~CBaseClassNonCopyable by tail jump
// Leaf: ??1CASoundBank@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0BD40, 60 bytes, thiscall, `jmp` tail; no call or address reference (an unreferenced
//   COMDAT kept by the linker, no /OPT:REF).
// Identity (proven): the CASoundBank TU run 0x00A0BD00 (ctor) / 0x00A0BD20, 0x00A0BD30 (vtable 0x0129C9AC slots 9, 10) /
//   0x00A0BD40 (this) / 0x00A0BD80 (vtable 0x0129C9AC slot 0 = ??_GCASoundBank, which inlines exactly this sequence:
//   release +0x04, Data = Info = 0, call 0x0099A300). RTTI .?AVCASoundBank@@ : CBaseClassNonCopyable, CBaseClass; Ego_r.pdb
//   CASoundBank = { CBaseClassNonCopyable, CCountedPointer<CCRCSymbolMap> PSymbolMap +0x04 } (12 bytes). The tail target
//   0x0099A300 is ??1CBaseClassNonCopyable@@MAE@XZ (reloc_bindings). No vtable store: the class declares no destructor
//   (VC7.1 drops the reset in a generated dtor). The manifest label (_Dest_val<..pair<CCharString const,
//   CCountedPointer<C3DSkeletalMorphTarget>>..>) is propagated over the shape and wrong.
// Callees: CCPPointerInfo::DeleteFunc through the block (Data in ecx), operator delete 0x00BFE9BC (??3@YAXPAX@Z, the block),
//   CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300 (tail).
// Layout: no engine/CASoundBank.h; the class is the Ego_r.pdb layout over engine/CBaseClassNonCopyable.h and
//   engine/CCountedPointer.h.
#include "engine/CCountedPointer.h"
#include "engine/CBaseClassNonCopyable.h"

class CCRCSymbolMap;

class CASoundBank : public CBaseClassNonCopyable {          // Ego_r.pdb, 12 bytes
public:
    CCountedPointer<CCRCSymbolMap> PSymbolMap;              // +0x04
};

void EmitCASoundBankDestructor(CASoundBank* bank)            // emits the implicit ??1 COMDAT; not a retail function
{
    bank->CASoundBank::~CASoundBank();
}