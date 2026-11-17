// CCountedPointer<C3DSkeletalMorphParameter>::DeleteData(void* data) -- the per-type deleter stored in the count block: delete the morph
//   parameter through its virtual destructor (`test ecx,ecx ; je ; mov eax,[ecx] ; push 1 ; call [eax]`).
// Leaf: ?DeleteData@?$CCountedPointer@VC3DSkeletalMorphParameter@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00786A30, 11 bytes.
// Identity (NF56, round 7, queue q10453): FableWin.pdb 0x02664170 ?DeleteData@?$CCountedPointer@VC3DSkeletalMorphParameter@@@@SIXPAX@Z (the per-T
//   deleter of engine/CCountedPointer.h). Its absolute references are the count-block stores of CTCSkeletalMorph::ForceSkeletalMorph
//   0x007864C0 and CalculateSkeletalMorph 0x00786700 (which binds this spelling), each after `new C3DSkeletalMorphParameter(target)`.
//   C3DSkeletalMorphParameter is polymorphic: RTTI .?AVC3DSkeletalMorphParameter@@ : CBaseClassNonCopyable : CBaseClass (rtti_bases.tsv),
//   slot 0 the scalar deleting destructor 0x00A46420. No shared header for it yet: the row declares only the base it needs. Was the stand-in
//   free DeleteData over a local AutoTinyVirtualObject (catalog _global::DeleteViaVtable0_00786a30).
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CCountedPointer.h"

class C3DSkeletalMorphParameter : public CBaseClassNonCopyable { };   // RTTI base only; members not needed here

template void FABLE_FASTCALL CCountedPointer<C3DSkeletalMorphParameter>::DeleteData(void*);
