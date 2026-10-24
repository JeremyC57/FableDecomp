// `anonymous namespace'::IsAnimStateBlendValid(const CMeshDataBank& bank, const C3DAnimationBlendState& blend) -- a blend state is valid when it has shared data whose base animation and every combination blend are valid animation states
// Leaf: ?IsAnimStateBlendValid@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070C770, length 131 bytes (+ int3 padding). `__fastcall`: bank in ecx, blend in
//         edx, plain `ret`. Ego_r.pdb: `?IsAnimStateBlendValid@?A0x22a65229@@YI_NABVCMeshDataBank@@ABVC3DAnimationBlendState@@@Z`
//         (a file-local helper of the CAnimateThing TU).
// Callees: `anonymous namespace'::IsAnimationStateBaseValid(const CMeshDataBank&, const C3DAnimationStateBase&) @ 0x0070BF50.
// Identity: the manifest label "AnimationBank_ValidateAnimationStates" is descriptive; four call sites in
//   CAnimateThing::UpdateCachedAnims (0x0070E520, landed), one per start/end/last-start/last-end blend state.
// Shape: two separate `return false` statements (no Data; base animation invalid) keep retail's inline early epilogue
//   ahead of the ebx/edi pushes (`if (a || b) return false;` moves it to the end), then an unsigned index loop to the
//   inline C3DAnimationBlendState::GetCombinationBlendCount() (`Data == 0 ? 0 : Data->CombinationBlends.size()`: the zero
//   arm laid out first, as retail's `test ; jne ; xor eax,eax ; jmp`; the
//   signed divide by 20, re-evaluated every pass with Data re-read through the blend kept in the `push ecx` word) over
//   `Data->CombinationBlends[i]` (STLport `*(begin() + n)`, the 20-byte stride strength-reduced into edi).
// Layout: local models -- retail C3DAnimationBlendState = vptr + CIVCountedPointer<CSharedData> Data (+4); CSharedData =
//   CIVCountedPointeeBase (vptr, RefCount) + BaseAnimation +0x08 + CombinationBlends +0x0C (STLport vector of 20-byte
//   C3DAnimationStateAlpha) + SkeletalMorph +0x18 (Ego_r.pdb, retail vectors are 12 bytes).
class CMeshDataBank;

class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase();                   // slot 0
    long RefCount;                                      // +0x04
};
class C3DAnimationStateBase : public CIVCountedPointeeBase {
public:
    virtual void Validate();                            // slot 1
    virtual long GetType() const;                       // slot 2
};
class C3DAnimationState : public C3DAnimationStateBase {
public:
    long  AnimationIndex;                               // +0x08
    float AnimationTime;                                // +0x0C
};
class C3DAnimationStateAlpha : public C3DAnimationState {
public:
    float AlphaWeight;                                  // +0x10
};

template <class T> class CIVCountedPointer {
public:
    T* Object;
    T& operator*() const { return *Object; }
    T* operator->() const { return Object; }
    operator T*() const { return Object; }
};

template <class T> class CArray {                      // CArray<T> = retail STLport vector (12 bytes)
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    const T& operator[](unsigned int n) const { return *(begin() + n); }
};

class C3DAnimationBlendState {
public:
    class CSharedData : public CIVCountedPointeeBase {
    public:
        CIVCountedPointer<C3DAnimationStateBase> BaseAnimation;   // +0x08
        CArray<C3DAnimationStateAlpha> CombinationBlends;         // +0x0C
        void* SkeletalMorph[2];                                   // +0x18 CCountedPointer<const C3DSkeletalMorph>
    };
    virtual ~C3DAnimationBlendState();
    CIVCountedPointer<CSharedData> Data;                // +0x04
    unsigned long GetCombinationBlendCount() const { return Data == 0 ? 0 : Data->CombinationBlends.size(); }
};

namespace {
bool __fastcall IsAnimationStateBaseValid(const CMeshDataBank& bank, const C3DAnimationStateBase& state);   // 0x0070BF50
bool __fastcall IsAnimStateBlendValid(const CMeshDataBank& bank, const C3DAnimationBlendState& blend);
}

bool __fastcall IsAnimStateBlendValid_Use(const CMeshDataBank& bank, const C3DAnimationBlendState& blend)
{
    return IsAnimStateBlendValid(bank, blend);
}

namespace {
bool __fastcall IsAnimStateBlendValid(const CMeshDataBank& bank, const C3DAnimationBlendState& blend)
{
    if (!blend.Data)
        return false;
    if (!IsAnimationStateBaseValid(bank, *blend.Data->BaseAnimation))
        return false;
    for (unsigned long i = 0; i != blend.GetCombinationBlendCount(); ++i)
        if (!IsAnimationStateBaseValid(bank, blend.Data->CombinationBlends[i]))
            return false;
    return true;
}
}