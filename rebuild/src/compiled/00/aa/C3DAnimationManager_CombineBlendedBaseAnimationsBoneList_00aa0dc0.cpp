// C3DAnimationManager::CombineBlendedBaseAnimationsBoneList(const C3DAnimationStateBase& stateA, const C3DAnimationStateBase& stateB, float blend, C3DKeyframe* out, unsigned long count, unsigned long depth, const C3DMesh2& mesh, const CArray<unsigned long>& bones) -- evaluate two parallel animation state trees blended by a factor for a list of bones
// Leaf: CombineBlendedBaseAnimationsBoneList
// Retail @ 0x00AA0DC0, 413 bytes (`ret 20h`). Merge of the landed siblings CombineBlendedBaseAnimations 0x00A9F2F0 (state
//   pair + InterpolateFloat time/progress) and CombineBaseAnimationsBoneList 0x00AA0C50 (per-bone loops through
//   CalcLocalBoneTransformationRelative 0x00A4D920 / CalcLocalBoneTransformation 0x00A4D330); same callees otherwise
//   (GetAnimationInfo 0x00A242C0 -- manifest "CTCInventoryAbilities::GetAbilityLevel" is propagated -- GetAnimation
//   0x00A26C60, BlendAnimations 0x00A9EB90). Matched first try with the siblings' shapes verbatim.
// Layout: as the siblings (C3DAnimationState / C3DAnimationTransitionState modelled locally for the polymorphic base).
struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
};
template <class T> struct CCountedPointer {
    T* m_pObject;
    SCountBlock* m_pCount;
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;            // operator delete @ 0x00BFE9BC
            }
        }
    }
    T* operator->() const { return m_pObject; }
};
template <class T> struct CArray {
    T* m_pBegin;
    T* m_pEnd;
    T* m_pEndOfStorage;
    const T& operator[](unsigned long i) const { return m_pBegin[i]; }
};

struct C3DKeyframe { float Rotation[4]; float Position[4]; float Scaling[4]; };   // 0x30
struct C3DMesh2;
struct C3DAnimation2 {
    void* __vftable;
    long _04;
    unsigned long AnimType;                 // +0x08 (C3DAnimationStats)
    void CalcLocalBoneTransformationRelative(C3DKeyframe* out, unsigned long bone, float time, const C3DMesh2& mesh) const;   // 0x00A4D920
    void CalcLocalBoneTransformation(C3DKeyframe* out, unsigned long bone, float time) const;   // 0x00A4D330
    bool IsRelative() const { return (AnimType & 4) != 0; }
};
struct C3DAnimationInfo {
    CCountedPointer<const C3DAnimation2> GetAnimation(const C3DMesh2& mesh) const;   // 0x00A26C60
};
struct CMeshDataBank {
    C3DAnimationInfo* GetAnimationInfo(long index);   // 0x00A242C0
};

struct C3DAnimationStateBase {
    virtual void v0();
    virtual void v1();
    virtual long GetStateType() const;      // slot 2 (+8)
    long m_refCount;                        // +4
};
struct C3DAnimationState : C3DAnimationStateBase {
    long  AnimationIndex;                   // +0x08
    float AnimationTime;                    // +0x0c
};
struct C3DAnimationTransitionState : C3DAnimationStateBase {
    C3DAnimationStateBase* StartState;      // +0x08 CIVCountedPointer<C3DAnimationStateBase>
    C3DAnimationStateBase* EndState;        // +0x0c
    float Progress;                         // +0x10
};

inline float InterpolateFloat(float from, float to, float t) { return from + (to - from) * t; }

struct C3DAnimationManager {
    void* __vftable;
    CMeshDataBank* MeshBank;                // +0x04
    C3DAnimationInfo* GetAnimationInfo(long index) const { return MeshBank->GetAnimationInfo(index); }
    void BlendAnimations(C3DKeyframe* a, C3DKeyframe* b, unsigned long count, float weight);   // 0x00A9EB90
    void CombineBlendedBaseAnimationsBoneList(const C3DAnimationStateBase& stateA, const C3DAnimationStateBase& stateB, float blend,
                                              C3DKeyframe* out, unsigned long count, unsigned long depth, const C3DMesh2& mesh,
                                              const CArray<unsigned long>& bones);
};

void C3DAnimationManager::CombineBlendedBaseAnimationsBoneList(const C3DAnimationStateBase& stateA, const C3DAnimationStateBase& stateB, float blend,
                                                               C3DKeyframe* out, unsigned long count, unsigned long depth, const C3DMesh2& mesh,
                                                               const CArray<unsigned long>& bones)
{
    long type = stateA.GetStateType();
    if (type == 0) {
        const C3DAnimationState& singleA = static_cast<const C3DAnimationState&>(stateA);
        const C3DAnimationState& singleB = static_cast<const C3DAnimationState&>(stateB);
        long index = singleA.AnimationIndex;
        float time = InterpolateFloat(singleA.AnimationTime, singleB.AnimationTime, blend);
        C3DAnimationInfo* info = GetAnimationInfo(index);
        CCountedPointer<const C3DAnimation2> animation = info->GetAnimation(mesh);
        if (animation->IsRelative()) {
            for (unsigned long i = 0; i < count; ++i)
                animation->CalcLocalBoneTransformationRelative(&out[i], bones[i], time, mesh);
        }
        else {
            for (unsigned long i = 0; i < count; ++i)
                animation->CalcLocalBoneTransformation(&out[i], bones[i], time);
        }
    }
    else if (type == 2) {
        const C3DAnimationTransitionState& transitionA = static_cast<const C3DAnimationTransitionState&>(stateA);
        const C3DAnimationTransitionState& transitionB = static_cast<const C3DAnimationTransitionState&>(stateB);
        unsigned long childDepth = depth + 1;
        CombineBlendedBaseAnimationsBoneList(*transitionA.StartState, *transitionB.StartState, blend, out, count, childDepth, mesh, bones);
        if (childDepth < 5) {
            C3DKeyframe* scratch = out + count;
            CombineBlendedBaseAnimationsBoneList(*transitionA.EndState, *transitionB.EndState, blend, scratch, count, childDepth, mesh, bones);
            BlendAnimations(out, scratch, count, InterpolateFloat(transitionA.Progress, transitionB.Progress, blend));
        }
    }
}