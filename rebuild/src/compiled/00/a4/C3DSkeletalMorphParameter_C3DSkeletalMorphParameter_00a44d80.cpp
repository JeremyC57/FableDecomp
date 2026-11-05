// C3DSkeletalMorphParameter::C3DSkeletalMorphParameter(const C3DSkeletalMorphParameter&) -- CBase(), vtable, an empty shared-data pointer assigned from the source's, the source's current value, no owning morph
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A44D80, length 104 bytes.
// Leaf: ??0C3DSkeletalMorphParameter
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Callees: 0x0099A2F0 CBaseClassNonCopyable::CBaseClassNonCopyable, the counted block's destroy thunk (`call [info+4]`) and 0x00BFE9BC
// operator delete (the assignment's release path, unreachable after the zeroing but still emitted).
// Vtable 0x0129D490 = C3DSkeletalMorphParameter (rtti-vtables.tsv).
// Layout (PDB, 20 bytes): CurrentValue +4, SkeletalMorph +8, SharedData +0xc (CCountedPointer<CShared>:
// data +0xc, count block +0x10).
// Shape: SharedData is default-constructed (both words nulled from ecx) and then ASSIGNED in the body through
// the inlined CCountedPointer::operator= (the source's block and object read before the block compare; VC7.1
// keeps the release test on the known-null block); CurrentValue and SkeletalMorph are body assignments after it.
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

#include "engine/CCountedPointer.h"

class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();   // 0x0099A300
    CBaseClassNonCopyable();                                             // 0x0099A2F0
};

class C3DSkeletalMorph;
class C3DSkeletalMorphParameter : public CBaseClassNonCopyable { public:
    class CShared;
    float                     CurrentValue;              // +0x04
    C3DSkeletalMorph*         SkeletalMorph;             // +0x08
    CCountedPointer<CShared>  SharedData;                // +0x0c   sizeof == 0x14
    C3DSkeletalMorphParameter(const C3DSkeletalMorphParameter& other);
};

C3DSkeletalMorphParameter::C3DSkeletalMorphParameter(const C3DSkeletalMorphParameter& other)
    : CBaseClassNonCopyable()
{
    SharedData = other.SharedData;
    CurrentValue = other.CurrentValue;
    SkeletalMorph = 0;
}