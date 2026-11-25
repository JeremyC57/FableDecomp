// CTCSoundPlayer::GetClassTypeName() const -- the class-type name "CTCSoundPlayer" as a CCharString (the in-class StatGetClassTypeName, inlined)
// Leaf: ?GetClassTypeName@CTCSoundPlayer@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066C2A0, 25 bytes: push esi; mov esi,[esp+8]; push -1; push 0x0123A504 "CTCSoundPlayer"; mov ecx,esi;
//   call CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax,esi; pop esi; ret 4 -- a const virtual __thiscall returning
//   CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one stack argument (kept in esi).
// Identity (NF417, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell
//   ?GetClassTypeName@CTCSoundPlayer@@UBE?AVCCharString@@XZ; retail vftable 0x0125B1C4 (RTTI .?AVCTCSoundPlayer@@) slot 22 (0x0125B21C
//   -> 0x0066C2A0) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h); the literal 0x0123A504 = "CTCSoundPlayer" = the class
//   name (read from the exe).
// Form (NF417): FableWin.pdb spells the class's static ?StatGetClassTypeName@CTCSoundPlayer@@SI?AVCCharString@@XZ (Ego_r has none); the
//   25 retail bytes are that static's 19-byte body (push -1; push lit; call; mov eax) expanded into the virtual, so the static is an
//   in-class inline (as the binder 0x004EE23F defines it) and the virtual calls it; retail has no out-of-line copy bound in
//   reloc_bindings.tsv (every use expands it). VC7.1 also emits the static's pick-any COMDAT copy in this object (identical to the
//   landed copies; the linker keeps one).
// Respell (NF417): was the stand-in self ?GetClassTypeName@CTCSoundPlayer_Methods@@QAE?AVCCharString@@XZ (a non-virtual member of a
//   made-up `_Methods` class).
// Header: engine/CTCSoundPlayer.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define CTCSoundPlayer CTCSoundPlayerLayout
#include "engine/CTCSoundPlayer.h"
#undef CTCSoundPlayer

class CTCSoundPlayer : public CTCSoundPlayerLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName() { return CCharString("CTCSoundPlayer", -1); }
    virtual CCharString GetClassTypeName() const;
};

CCharString CTCSoundPlayer::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
