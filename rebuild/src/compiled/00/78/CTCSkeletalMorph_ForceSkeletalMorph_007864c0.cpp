// CTCSkeletalMorph::ForceSkeletalMorph(C3DSkeletalMorphTarget* target) -- reset the skeletal morph to a single parameter driving the given target at full weight
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007864C0, 182 bytes (`ret 4`).
// Callees: C3DSkeletalMorph members on PSkeletalMorph @ 0x00A45410 (clear the parameters), @ 0x00A45470 (add a parameter, by
// const CCountedPointer&) and @ 0x00A45330 (apply) -- unnamed in the manifest (propagated labels), named descriptively;
// operator new @ 0x00BFEA1A (the 0x14-byte parameter and the 12-byte count block), C3DSkeletalMorphParameter::
// C3DSkeletalMorphParameter(C3DSkeletalMorphTarget*) @ 0x00A455A0, C3DSkeletalMorphParameter::SetCurrentValue(float)
// @ 0x00A45370, operator delete @ 0x00BFE9BC; the count block holds the per-type deleter 0x00786A30.
// Layout (retail; Ego_r.pdb with the 12-byte CArray): Changed +0x0c, Morphs +0x10, PSkeletalMorph (CCountedPointer<
// C3DSkeletalMorph>) +0x1c.
// Shape: a CCountedPointer local built from `new` through the raw-pointer constructor (the block's object re-read from the
// pointer); its destructor decrements the block through the pointer and re-reads it before the destroy and the delete.
// NF56 (round 7, queue q10453): the count block and the counted pointer come from engine/CCountedPointer.h, so the deleter the block
//   stores binds ?DeleteData@?$CCountedPointer@VC3DSkeletalMorphParameter@@@@SIXPAX@Z (FableWin.pdb 0x02664170, retail 0x00786A30) as in the
//   sibling CalculateSkeletalMorph 0x00786700. Were local CCPPointerInfo / CCountedPointer<T> models and the stand-in deleter Destroy_00786a30.
//   The local C3DSkeletalMorphParameter / C3DSkeletalMorph / CTCSkeletalMorph models are pre-existing.
#include "engine/CCountedPointer.h"
class C3DSkeletalMorphTarget;
class C3DSkeletalMorphParameter { public:
    unsigned char _data[0x14];
    C3DSkeletalMorphParameter(const C3DSkeletalMorphTarget& target);   // 0x00A455A0 (NF374: Ego_r + FableWin ??0C3DSkeletalMorphParameter@@QAE@ABVC3DSkeletalMorphTarget@@@Z)
    void SetCurrentValue(float value);                  // 0x00A45370
};
class C3DSkeletalMorph { public:
    void Reset();                             // 0x00A45410
    void NotifyParamUpdate();                           // 0x00A45330 (Ego_r + FableWin; NF292, was ApplyParameters)
public:
    void AddMorphParameter(CCountedPointer<C3DSkeletalMorphParameter> const&);   // 0x00A45470
};
class CTCSkeletalMorph { public:
    virtual void v00();
    void* ParentThing;                                  // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    bool Changed;                                       // +0x0c
    unsigned char _pad0d[0x10 - 0x0d];
    unsigned char Morphs[0xc];                          // +0x10 CArray<CTCSkeletalMorph::CSkeletalMorph>
    CCountedPointer<C3DSkeletalMorph> PSkeletalMorph;   // +0x1c
    void ForceSkeletalMorph(C3DSkeletalMorphTarget* target);
};

void CTCSkeletalMorph::ForceSkeletalMorph(C3DSkeletalMorphTarget* target)
{
    Changed = false;
    PSkeletalMorph->Reset();
    CCountedPointer<C3DSkeletalMorphParameter> parameter(new C3DSkeletalMorphParameter(*target));
    PSkeletalMorph->AddMorphParameter(parameter);
    parameter->SetCurrentValue(1.0f);
    PSkeletalMorph->NotifyParamUpdate();
}