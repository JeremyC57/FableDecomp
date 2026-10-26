// C3DSkeletalMorph::PeekCombinedMorph() -- build the combined morph target on first use (every parameter's positive or negative shared target, weighted by its current value) and return a counted reference to it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A45260, length 200 bytes (`ret 4`, CCountedPointer by hidden pointer).
// Class: C3DSkeletalMorph (animation/mesh layer; Ego_r.pdb names). No vtable store here; the class's own vptr is at +0.
// Callees: operator new @ 0x00BFEA1A (0x10 bytes = retail C3DSkeletalMorphTarget), CCharString::CCharString() @ 0x0099E4B0
// (the target's Name), CCountedPointer<C3DSkeletalMorphTarget>::Reset(T*) @ 0x006C3AC0 (manifest names the
// CIPhysicsFacesSegmentBase instantiation -- a byte twin), C3DSkeletalMorphTarget::CombineMorph(const
// C3DSkeletalMorphTarget&, float) @ 0x00A44F40. Constant 0.0f @ 0x0122DEDC.
// Retail offsets (PDB containers are 16 bytes, retail CArray/vector 12): MorphParameters +0x04 (8-byte
// CCountedPointer elements), CombinedMorph +0x10 {Object, count block}; parameter CurrentValue +0x04, SharedData +0x0c
// (CShared: PositiveTarget +0x00, NegativeTarget +0x10); target Name +0x00, BoneMorphs +0x04 (12-byte vector).
// Shape: the size is recomputed every iteration (`sar 3`, unsigned compare); STLport operator[] is `*(begin()+n)`; the
// weight is a float local (copied through edx); the returned copy bumps the count block when present.
class CCharString
{ public:
    void* PStringData;
    CCharString();                                  // 0x0099E4B0
};

class C3DSkeletalBoneMorph;

class C3DSkeletalMorphTarget                       // retail 0x10 bytes
{ public:
    CCharString Name;                               // +0x00
    C3DSkeletalBoneMorph* BoneMorphsFirst;          // +0x04 CArray<C3DSkeletalBoneMorph>
    C3DSkeletalBoneMorph* BoneMorphsLast;           // +0x08
    C3DSkeletalBoneMorph* BoneMorphsEnd;            // +0x0c
    C3DSkeletalMorphTarget() : BoneMorphsFirst(0), BoneMorphsLast(0), BoneMorphsEnd(0) { }
    void CombineMorph(const C3DSkeletalMorphTarget& target, float weight);   // 0x00A44F40
};

struct CCountBlock { long Count; };

template <class T>
class CCountedPointer
{ public:
    T* Object;                                      // +0x00
    CCountBlock* Block;                             // +0x04
    CCountedPointer(const CCountedPointer& other) : Object(other.Object), Block(other.Block) { if (Block) ++Block->Count; }
    void Reset(T* object);                          // 0x006C3AC0
    T* operator->() const { return Object; }
};

class C3DSkeletalMorphParameter
{ public:
    class CShared
    { public:
        C3DSkeletalMorphTarget PositiveTarget;      // +0x00
        C3DSkeletalMorphTarget NegativeTarget;      // +0x10
    };
    virtual void v00();
    float CurrentValue;                             // +0x04
    class C3DSkeletalMorph* SkeletalMorph;         // +0x08
    CCountedPointer<CShared> SharedData;            // +0x0c
};

struct CMorphParameterArray                         // CArray<CCountedPointer<C3DSkeletalMorphParameter> >
{
    CCountedPointer<C3DSkeletalMorphParameter>* first;
    CCountedPointer<C3DSkeletalMorphParameter>* last;
    CCountedPointer<C3DSkeletalMorphParameter>* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    CCountedPointer<C3DSkeletalMorphParameter>* begin() const { return first; }
    CCountedPointer<C3DSkeletalMorphParameter>& operator[](unsigned long n) const { return *(begin() + n); }
};

class C3DSkeletalMorph
{ public:
    virtual void v00();
    CMorphParameterArray MorphParameters;           // +0x04
    CCountedPointer<C3DSkeletalMorphTarget> CombinedMorph;   // +0x10
    CCountedPointer<C3DSkeletalMorphTarget> PeekCombinedMorph();
};

CCountedPointer<C3DSkeletalMorphTarget> C3DSkeletalMorph::PeekCombinedMorph()
{
    if (!CombinedMorph.Object)
    {
        CombinedMorph.Reset(new C3DSkeletalMorphTarget);
        for (unsigned long i = 0; i < MorphParameters.size(); ++i)
        {
            C3DSkeletalMorphParameter* parameter = MorphParameters[i].Object;
            float value = parameter->CurrentValue;
            if (value >= 0.0f)
                CombinedMorph->CombineMorph(parameter->SharedData->PositiveTarget, value);
            else
                CombinedMorph->CombineMorph(parameter->SharedData->NegativeTarget, -value);
        }
    }
    return CombinedMorph;
}