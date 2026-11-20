#pragma optimize("s", on)
// CCountedPointer<CVillageTaskSecuritySweep>::`scalar deleting destructor'(unsigned int) -- release the pointer (inline ~CCountedPointer = ReleaseData
//   0x0065D5CC) and free the object when bit 0 of the flags is set
// Leaf: ??_G?$CCountedPointer@VCVillageTaskSecuritySweep@@@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065E989: push esi ; mov esi,ecx ; call 0x0065D5CC ; test byte [esp+8],1 ; je ; push esi ; call operator delete
//   0x00BFE9BC ; pop ecx ; mov eax,esi ; pop esi ; ret 4 (size TU). No landed row references this address (Fable.exe scan).
// Name fix (NF202, 2026-11-20, q12231): ported as the landed sibling 0x0065E951 (??_G?$CCountedPointer@VCVillageTaskPatrol@@@@QAEPAXI@Z,
//   NF180), with the destructor expanded to its ReleaseData call (the release at 0x0065D5CC is ReleaseData<CVillageTaskSecuritySweep>: FableWin.pdb + the
//   jmp-thunk destructor(s) 0x0065E31E / 0x0065F939). Was the free stand-in ?CLandscapeBackgroundPatch_ScalarDeletingDestructor@@YIPAXPAXHI@Z calling
//   ?AutoTinyScalarDestructor_0065d5cc@@YIXPAX@Z (the CLandscapeBackgroundPatch label was propagated across byte twins).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
class CVillageTaskSecuritySweep;
namespace {
void EmitDeletingDestructor(CCountedPointer<CVillageTaskSecuritySweep>* pointer)
{
    delete pointer;
}
}
