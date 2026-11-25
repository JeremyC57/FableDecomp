// CTCActionUseScriptedHook::GetClassTypeName() const -- the class-type name "CTCActionUseScriptedHook" as a CCharString (the in-class StatGetClassTypeName, inlined)
// Leaf: ?GetClassTypeName@CTCActionUseScriptedHook@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0072A3F0, 25 bytes: push esi; mov esi,[esp+8]; push -1; push 0x01239FE4 "CTCActionUseScriptedHook"; mov ecx,esi;
//   call CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax,esi; pop esi; ret 4 -- a const virtual __thiscall returning
//   CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one stack argument (kept in esi).
// Identity (NF417, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell
//   ?GetClassTypeName@CTCActionUseScriptedHook@@UBE?AVCCharString@@XZ; retail vftable 0x0126664C (RTTI .?AVCTCActionUseScriptedHook@@)
//   slot 22 (0x012666A4 -> 0x0072A3F0) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h); the literal 0x01239FE4 =
//   "CTCActionUseScriptedHook" = the class name (read from the exe).
// Form (NF417): FableWin.pdb spells the class's static ?StatGetClassTypeName@CTCActionUseScriptedHook@@SI?AVCCharString@@XZ (Ego_r has
//   none); the 25 retail bytes are that static's 19-byte body (push -1; push lit; call; mov eax) expanded into the virtual, so the
//   static is an in-class inline (as the binder 0x004EE23F defines it) and the virtual calls it; retail has no out-of-line copy bound
//   in reloc_bindings.tsv (every use expands it). VC7.1 also emits the static's pick-any COMDAT copy in this object (identical to the
//   landed copies; the linker keeps one).
// Respell (NF417): was the stand-in self ?GetClassTypeName@CTCActionUseScriptedHook_Methods@@QAE?AVCCharString@@XZ (a non-virtual
//   member of a made-up `_Methods` class).
// Header: engine/CTCActionUseScriptedHook.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#define CTCActionUseScriptedHook CTCActionUseScriptedHookLayout
#include "engine/CTCActionUseScriptedHook.h"
#undef CTCActionUseScriptedHook

class CTCActionUseScriptedHook : public CTCActionUseScriptedHookLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName() { return CCharString("CTCActionUseScriptedHook", -1); }
    virtual CCharString GetClassTypeName() const;
};

CCharString CTCActionUseScriptedHook::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
