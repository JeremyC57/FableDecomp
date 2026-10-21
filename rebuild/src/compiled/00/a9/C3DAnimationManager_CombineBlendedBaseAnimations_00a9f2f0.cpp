// C3DAnimationManager::CombineBlendedBaseAnimations(const C3DAnimationStateBase&, const C3DAnimationStateBase&, float, C3DKeyframe*, unsigned long, unsigned long, const C3DMesh2&) -- evaluate two parallel animation state trees blended by a factor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A9F2F0, length 350 bytes (`ret 1Ch`). Ego_r.pdb
//          ?CombineBlendedBaseAnimations@C3DAnimationManager@@AAEXABVC3DAnimationStateBase@@0MPAVC3DKeyframe@@KKABVC3DMesh2@@@Z.
// Callees: state virtual slot 2 (+8) GetStateType, the mesh bank's GetAnimationInfo(long) @ 0x00A242C0 (manifest
//          "CTCInventoryAbilities::GetAbilityLevel" is propagated), C3DAnimationInfo::GetAnimation(const C3DMesh2&) const
//          @ 0x00A26C60, C3DAnimation2::CalcLocalBoneTransformationsRelative @ 0x00A4D770 and CalcLocalBoneTransformations
//          @ 0x00A4CEC0 (all bones at once), C3DAnimationManager::BlendAnimations @ 0x00A9EB90, itself, operator delete @ 0x00BFE9BC.
// Shape: the twin of CombineBaseAnimationsBoneList (0x00AA0C50): only the first state's type is tested. Type 0 samples
//          the first state's animation at the time interpolated between the two states (`a + (b - a) * blend`, the
//          result homed in the dead second-state slot); type 2 recurses pairwise into the start states and, below depth
//          5, into the end states in the scratch keyframes, then blends by the interpolated Progress. The interpolation
//          is an inline by-value helper (the `from` operand stays on the FPU stack: `fsub st(1) ; fadd st(1)`), the
//          animation index is read into a named local before it (keeps it out of edi). GetAnimationInfo
//          goes through the inline manager forwarder; the child depth is a named local (homes in the dead first-state slot).
// Layout: C3DAnimationState AnimationIndex +8 / AnimationTime +0xC and C3DAnimationTransitionState StartState +8 /
//          EndState +0xC / Progress +0x10 agree with engine/C3DAnimationState.h and C3DAnimationTransitionState.h; they
//          are modelled locally because the call needs the polymorphic base. C3DAnimationManager MeshBank +4 (PDB).
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

struct C3DKeyframe { float Rotation[4]; float Position[4]; float Scaling[4]; };   // 0x30
struct C3DMesh2;
struct C3DAnimation2 {
    void* __vftable;
    long _04;
    unsigned long AnimType;                 // +0x08 (C3DAnimationStats)
    void CalcLocalBoneTransformationsRelative(C3DKeyframe* out, unsigned long count, float time, const C3DMesh2& mesh) const;   // 0x00A4D770
    void CalcLocalBoneTransformations(C3DKeyframe* out, unsigned long count, float time) const;   // 0x00A4CEC0
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
    void CombineBlendedBaseAnimations(const C3DAnimationStateBase& stateA, const C3DAnimationStateBase& stateB, float blend,
                                      C3DKeyframe* out, unsigned long count, unsigned long depth, const C3DMesh2& mesh);
};

void C3DAnimationManager::CombineBlendedBaseAnimations(const C3DAnimationStateBase& stateA, const C3DAnimationStateBase& stateB, float blend,
                                                       C3DKeyframe* out, unsigned long count, unsigned long depth, const C3DMesh2& mesh)
{
    long type = stateA.GetStateType();
    if (type == 0) {
        const C3DAnimationState& singleA = static_cast<const C3DAnimationState&>(stateA);
        const C3DAnimationState& singleB = static_cast<const C3DAnimationState&>(stateB);
        long index = singleA.AnimationIndex;
        float time = InterpolateFloat(singleA.AnimationTime, singleB.AnimationTime, blend);
        C3DAnimationInfo* info = GetAnimationInfo(index);
        CCountedPointer<const C3DAnimation2> animation = info->GetAnimation(mesh);
        if (animation->IsRelative())
            animation->CalcLocalBoneTransformationsRelative(out, count, time, mesh);
        else
            animation->CalcLocalBoneTransformations(out, count, time);
    }
    else if (type == 2) {
        const C3DAnimationTransitionState& transitionA = static_cast<const C3DAnimationTransitionState&>(stateA);
        const C3DAnimationTransitionState& transitionB = static_cast<const C3DAnimationTransitionState&>(stateB);
        unsigned long childDepth = depth + 1;
        CombineBlendedBaseAnimations(*transitionA.StartState, *transitionB.StartState, blend, out, count, childDepth, mesh);
        if (childDepth < 5) {
            C3DKeyframe* scratch = out + count;
            CombineBlendedBaseAnimations(*transitionA.EndState, *transitionB.EndState, blend, scratch, count, childDepth, mesh);
            BlendAnimations(out, scratch, count, InterpolateFloat(transitionA.Progress, transitionB.Progress, blend));
        }
    }
}