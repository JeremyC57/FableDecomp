#pragma optimize("s", on)
// CCountedPointer<CVillageTaskTorch>::`scalar deleting destructor'(unsigned int) -- release the pointer (inline ~CCountedPointer = ReleaseData
//   0x0065D574) and free the object when bit 0 of the flags is set
// Leaf: ??_G?$CCountedPointer@VCVillageTaskTorch@@@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065E919: push esi ; mov esi,ecx ; call 0x0065D574 ; test byte [esp+8],1 ; je ; push esi ; call operator delete
//   0x00BFE9BC ; pop ecx ; mov eax,esi ; pop esi ; ret 4 (size TU). No landed row references this address (Fable.exe scan).
// Name fix (NF202, 2026-11-20, q12230): ported as the landed sibling 0x0065E951 (??_G?$CCountedPointer@VCVillageTaskPatrol@@@@QAEPAXI@Z,
//   NF180), with the destructor expanded to its ReleaseData call (the release at 0x0065D574 is ReleaseData<CVillageTaskTorch>: FableWin.pdb + the
//   jmp-thunk destructor(s) 0x0065E298 / 0x0065F925). Was the free stand-in ?CLandscapeBackgroundPatch_ScalarDeletingDestructor@@YIPAXPAXHI@Z calling
//   ?AutoTinyScalarDestructor_0065d574@@YIXPAX@Z (the CLandscapeBackgroundPatch label was propagated across byte twins).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
class CVillageTaskTorch;
namespace {
void EmitDeletingDestructor(CCountedPointer<CVillageTaskTorch>* pointer)
{
    delete pointer;
}
}
