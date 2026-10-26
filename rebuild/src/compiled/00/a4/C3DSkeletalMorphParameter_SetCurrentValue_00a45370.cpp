// C3DSkeletalMorphParameter::SetCurrentValue(float value) -- stores the value clamped to [-1 or 0 (signed morphs go negative), 1] and invalidates the owning morph's combined target
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A45370, length 151 bytes.
// Leaf: SetCurrentValue
// Callees: the counted block's destroy thunk (`call [info+4]`) and 0x00BFE9BC operator delete when the
// combined morph's last reference goes. Float pool: 0x0122DEE0 = -1.0f, 0x0122DEDC = 0.0f, 0x0122DED8 = 1.0f.
// Layout (PDB): C3DSkeletalMorphParameter {CurrentValue +4, SkeletalMorph +8, SharedData +0xc}; CShared
// (retail: two 16-byte C3DSkeletalMorphTarget, then SignedFlag +0x20); C3DSkeletalMorph's CombinedMorph
// (CCountedPointer: data +0x10, count block +0x14).
// Shape: the value is stored first; the lower bound is an inline `SignedFlag ? -1.0f : 0.0f` evaluated twice
// (the flag byte read once and tested twice) for `if (value < min) CurrentValue = min;`; the upper clamp reloads
// the member and stores 1.0f as an immediate; the combined morph is cleared through an inline Clear() (release,
// then the block and the object nulled in that order).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CCPPointerInfo { public:                                  // PDB: RefCount +0, DeleteFunc +4, Data +8
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template<class T> class CCountedPointer { public:
    T*              Data;                                // +0
    CCPPointerInfo* Info;                                // +4
    void Clear()
    {
        if (Info != 0) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                operator delete(Info);
            }
        }
        Info = 0;
        Data = 0;
    }
};

class C3DSkeletalMorphTarget;
class C3DSkeletalMorph { public:
    virtual void v00();
    void*                                   MorphParameters[3];   // +0x04 retail CArray
    CCountedPointer<C3DSkeletalMorphTarget> CombinedMorph;        // +0x10
};

class C3DSkeletalMorphParameter { public:
    class CShared { public:
        unsigned char PositiveTarget[0x10];              // +0x00 C3DSkeletalMorphTarget (retail 16 bytes)
        unsigned char NegativeTarget[0x10];              // +0x10
        bool          SignedFlag;                        // +0x20
    };
    virtual void v00();
    float                     CurrentValue;              // +0x04
    C3DSkeletalMorph*         SkeletalMorph;             // +0x08
    CCountedPointer<CShared>  SharedData;                // +0x0c
    float GetMinimumValue() const { return SharedData.Data->SignedFlag ? -1.0f : 0.0f; }
    void SetCurrentValue(float value);
};

void C3DSkeletalMorphParameter::SetCurrentValue(float value)
{
    CurrentValue = value;
    if (value < GetMinimumValue())
        CurrentValue = GetMinimumValue();
    if (CurrentValue > 1.0f)
        CurrentValue = 1.0f;
    if (SkeletalMorph)
        SkeletalMorph->CombinedMorph.Clear();
}