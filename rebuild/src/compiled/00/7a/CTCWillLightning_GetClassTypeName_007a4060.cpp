// CTCWillLightning::GetClassTypeName() const -- the class-type name "CTCWillLightning" as a CCharString (the in-class StatGetClassTypeName, inlined)
// Leaf: ?GetClassTypeName@CTCWillLightning@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007A4060, 25 bytes: push esi; mov esi,[esp+8]; push -1; push 0x0123C430 "CTCWillLightning"; mov ecx,esi;
//   call CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax,esi; pop esi; ret 4 -- a const virtual __thiscall returning
//   CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one stack argument (kept in esi).
// Identity (NF417, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell
//   ?GetClassTypeName@CTCWillLightning@@UBE?AVCCharString@@XZ; retail vftable 0x0126C724 (RTTI .?AVCTCWillLightning@@) slot 22
//   (0x0126C77C -> 0x007A4060) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h); the literal 0x0123C430 = "CTCWillLightning"
//   = the class name (read from the exe).
// Form (NF417): FableWin.pdb spells the class's static ?StatGetClassTypeName@CTCWillLightning@@SI?AVCCharString@@XZ (Ego_r has none);
//   the 25 retail bytes are that static's 19-byte body (push -1; push lit; call; mov eax) expanded into the virtual, so the static is
//   an in-class inline (as the landed static row 0x004D4CD8 and the binder 0x004EE23F define it) and the virtual calls it; retail's
//   out-of-line copy is 0x004D4CD8 (bound by 004ee23f). VC7.1 also emits the static's pick-any COMDAT copy in this object (identical to
//   the landed copies; the linker keeps one).
// Respell (NF417): was the stand-in self ?GetClassTypeName@CTCWillLightning_Methods@@QAE?AVCCharString@@XZ (a non-virtual member of a
//   made-up `_Methods` class).
// Header: engine/CTCWillLightning.h does not compile to the polymorphic class in any prelude P0..P3 (no opt-in guards a polymorphic
//   definition of the class in it) (header debt: its default branch is not the polymorphic retail class); a local class over its RTTI
//   direct base engine/CTCBase.h (rtti_bases.tsv: .?AVCTCWillLightning@@ : CTCBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/CTCBase.h"

class CTCWillLightning : public CTCBase
{
public:
    static CCharString __fastcall StatGetClassTypeName() { return CCharString("CTCWillLightning", -1); }
    virtual CCharString GetClassTypeName() const;
};

CCharString CTCWillLightning::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
