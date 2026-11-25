// CTCPhysicsLight::GetClassTypeName() const -- the class-type name "CTCPhysicsLight" as a CCharString (the in-class StatGetClassTypeName, inlined)
// Leaf: ?GetClassTypeName@CTCPhysicsLight@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00723420, 25 bytes: push esi; mov esi,[esp+8]; push -1; push 0x01239F28 "CTCPhysicsLight"; mov ecx,esi;
//   call CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax,esi; pop esi; ret 4 -- a const virtual __thiscall returning
//   CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one stack argument (kept in esi).
// Identity (NF417, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell
//   ?GetClassTypeName@CTCPhysicsLight@@UBE?AVCCharString@@XZ; retail vftable 0x01265DFC (RTTI .?AVCTCPhysicsLight@@) slot 22
//   (0x01265E54 -> 0x00723420) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h); the literal 0x01239F28 = "CTCPhysicsLight" =
//   the class name (read from the exe).
// Form (NF417): FableWin.pdb spells the class's static ?StatGetClassTypeName@CTCPhysicsLight@@SI?AVCCharString@@XZ (Ego_r has none);
//   the 25 retail bytes are that static's 19-byte body (push -1; push lit; call; mov eax) expanded into the virtual, so the static is
//   an in-class inline (as the binder 0x004EE23F defines it) and the virtual calls it; retail has no out-of-line copy bound in
//   reloc_bindings.tsv (every use expands it). VC7.1 also emits the static's pick-any COMDAT copy in this object (identical to the
//   landed copies; the linker keeps one).
// Respell (NF417): was the stand-in self ?GetClassTypeName@CTCPhysicsLight_Methods@@QAE?AVCCharString@@XZ (a non-virtual member of a
//   made-up `_Methods` class).
// Header: no engine header defines CTCPhysicsLight; the RTTI ancestors' headers (engine/CTCPhysicsBase.h) do not reach the polymorphic
//   CTCBase in any prelude (header debt), so a local class directly over engine/CTCBase.h (rtti_bases.tsv: .?AVCTCPhysicsLight@@ :
//   CTCPhysicsBase ... : CTCBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/CTCBase.h"

class CTCPhysicsLight : public CTCBase
{
public:
    static CCharString __fastcall StatGetClassTypeName() { return CCharString("CTCPhysicsLight", -1); }
    virtual CCharString GetClassTypeName() const;
};

CCharString CTCPhysicsLight::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
