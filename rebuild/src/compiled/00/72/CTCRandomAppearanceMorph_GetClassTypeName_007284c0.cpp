// CTCRandomAppearanceMorph::GetClassTypeName() const -- the class-type name "CTCRandomAppearanceMorph" as a CCharString (the in-class StatGetClassTypeName, inlined)
// Leaf: ?GetClassTypeName@CTCRandomAppearanceMorph@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007284C0, 25 bytes: push esi; mov esi,[esp+8]; push -1; push 0x01239F94 "CTCRandomAppearanceMorph"; mov ecx,esi;
//   call CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax,esi; pop esi; ret 4 -- a const virtual __thiscall returning
//   CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one stack argument (kept in esi).
// Identity (NF417, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell
//   ?GetClassTypeName@CTCRandomAppearanceMorph@@UBE?AVCCharString@@XZ; retail vftable 0x0126653C (RTTI .?AVCTCRandomAppearanceMorph@@)
//   slot 22 (0x01266594 -> 0x007284C0) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h); the literal 0x01239F94 =
//   "CTCRandomAppearanceMorph" = the class name (read from the exe).
// Form (NF417): FableWin.pdb spells the class's static ?StatGetClassTypeName@CTCRandomAppearanceMorph@@SI?AVCCharString@@XZ (Ego_r has
//   none); the 25 retail bytes are that static's 19-byte body (push -1; push lit; call; mov eax) expanded into the virtual, so the
//   static is an in-class inline (as the binder 0x004EE23F defines it) and the virtual calls it; retail has no out-of-line copy bound
//   in reloc_bindings.tsv (every use expands it). VC7.1 also emits the static's pick-any COMDAT copy in this object (identical to the
//   landed copies; the linker keeps one).
// Respell (NF417): was the stand-in self ?GetClassTypeName@CTCRandomAppearanceMorph_Methods@@QAE?AVCCharString@@XZ (a non-virtual
//   member of a made-up `_Methods` class).
// Header: engine header engine/CTCRandomAppearanceMorph.h does not compile to the polymorphic class; the RTTI ancestors' headers
//   (engine/CTCAppearanceMorphBase.h) do not reach the polymorphic CTCBase in any prelude (header debt), so a local class directly over
//   engine/CTCBase.h (rtti_bases.tsv: .?AVCTCRandomAppearanceMorph@@ : CTCAppearanceMorphBase ... : CTCBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/CTCBase.h"

class CTCRandomAppearanceMorph : public CTCBase
{
public:
    static CCharString __fastcall StatGetClassTypeName() { return CCharString("CTCRandomAppearanceMorph", -1); }
    virtual CCharString GetClassTypeName() const;
};

CCharString CTCRandomAppearanceMorph::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
