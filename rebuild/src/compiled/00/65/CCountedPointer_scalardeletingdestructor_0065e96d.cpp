#pragma optimize("s", on)
// CCountedPointer<CVillageTaskPickCrops>::`scalar deleting destructor'(unsigned int flags) -- run ~CCountedPointer (out of line), free the block when bit 0 of the flags is set, return this
// Leaf: ??_G?$CCountedPointer@VCVillageTaskPickCrops@@@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065E96D, 28 bytes, thiscall `ret 4`: push esi ; mov esi,ecx ; call 0x0065D533 ; test [esp+8],1 ;
//   je ; push esi ; call operator delete ; pop ecx ; mov eax,esi ; pop esi.
// Identity: VC7.1 emits a non-virtual scalar deleting destructor (??_G..@QAEPAXI@Z) for a class with a non-virtual destructor when an
//   object is deleted (GOTCHAS / the landed 0x00AD79C0); the only callee is ~CCountedPointer<CVillageTaskPickCrops> 0x0065D533 (landed self
//   row, FableWin.pdb ??1?$CCountedPointer@VCVillageTaskPickCrops@@@@QAE@XZ). FableWin.pdb holds this member for other T (e.g.
//   ??_G?$CCountedPointer@VCGuiVarTransferBase@@@@QAEPAXI@Z, ??_G?$CCountedPointer@VCComponent@NUISystem@@@@QAEPAXI@Z); the T comes from
//   the callee. The neighbours 0x0065E919 .. 0x0065E9A5 are the same member for the other CVillageTask* counted pointers.
// Naming (NF180, 2026-11-20, q4532 / q5232): was the free stand-in CLandscapeBackgroundPatch_ScalarDeletingDestructor (a propagated
//   label) calling AutoTinyScalarDestructor_0065d533; now the compiler-emitted member of the shared engine/CCountedPointer.h class
//   (FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY: the destructor called out of line), so the call binds the self row 0x0065D533's spelling.
#define FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY
#include "engine/CCountedPointer.h"
class CVillageTaskPickCrops;
namespace {
// Fidelity: scaffolding, not retail: a delete-expression of the header class makes VC7.1 emit its scalar deleting destructor (the
// row's leaf, as for 0x00AD79C0). Internal to this TU; never called.
void EmitDeletingDestructor(CCountedPointer<CVillageTaskPickCrops>* pointer)
{
    delete pointer;
}
}
