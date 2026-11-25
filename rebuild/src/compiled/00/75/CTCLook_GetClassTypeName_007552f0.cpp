// CTCLook::GetClassTypeName() const -- the class-type name "CTCLook" as a CCharString (the in-class StatGetClassTypeName, inlined)
// Leaf: ?GetClassTypeName@CTCLook@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007552F0, 25 bytes: push esi; mov esi,[esp+8]; push -1; push 0x0123AE08 "CTCLook"; mov ecx,esi;
//   call CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax,esi; pop esi; ret 4 -- a const virtual __thiscall returning
//   CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one stack argument (kept in esi).
// Identity (NF417, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCLook@@UBE?AVCCharString@@XZ;
//   retail vftable 0x01269744 (RTTI .?AVCTCLook@@) slot 22 (0x0126979C -> 0x007552F0) is CTCBase's pure GetClassTypeName slot
//   (engine/CTCBase.h); the literal 0x0123AE08 = "CTCLook" = the class name (read from the exe).
// Form (NF417): FableWin.pdb spells the class's static ?StatGetClassTypeName@CTCLook@@SI?AVCCharString@@XZ (Ego_r has none); the 25
//   retail bytes are that static's 19-byte body (push -1; push lit; call; mov eax) expanded into the virtual, so the static is an
//   in-class inline (as the binder 0x004EE23F defines it) and the virtual calls it; retail has no out-of-line copy bound in
//   reloc_bindings.tsv (every use expands it). VC7.1 also emits the static's pick-any COMDAT copy in this object (identical to the
//   landed copies; the linker keeps one).
// Respell (NF417): was the stand-in self ?GetClassTypeName@CTCLook_Methods@@QAE?AVCCharString@@XZ (a non-virtual member of a made-up
//   `_Methods` class).
// Header: engine/CTCLook.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form); its polymorphic branch compiles with the
//   prelude FABLE_RETAIL_CLOOKDEF, FABLE_RETAIL_DEF_CLASS_BASE, FABLE_RETAIL_CTCPHYSICSBASE, FABLE_RETAIL_C3DVECTOR,
//   FABLE_RETAIL_C3DVECTOR_NOT_EQUAL (the smallest prelude, from the header's landed includers, minimised line by line, under which the
//   polymorphic check TU compiles: static_cast to CTCBase, &CTCBase::GetClassTypeName, sizeof(class) == sizeof(layout); NF421
//   tools/psearch.py). Port (NF421, T3 `nf417 headers debt`): was a local class over CTCBase; no header edit was needed.
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CLOOKDEF
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NOT_EQUAL
#define CTCLook CTCLookLayout
#include "engine/CTCLook.h"
#undef CTCLook

class CTCLook : public CTCLookLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName() { return CCharString("CTCLook", -1); }
    virtual CCharString GetClassTypeName() const;
};

CCharString CTCLook::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
