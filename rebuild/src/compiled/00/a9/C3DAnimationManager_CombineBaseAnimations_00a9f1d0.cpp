// C3DAnimationManager::CombineBaseAnimations(const C3DAnimationStateBase& state, C3DKeyframe* keyframes, unsigned long boneCount, unsigned long depth, const C3DMesh2& mesh) -- evaluate a base animation state (or a transition between two, recursively) into the keyframe array
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A9F1D0, 277 bytes (`ret 14h`).
// Ego_r.pdb: ?CombineBaseAnimations@C3DAnimationManager@@AAEXABVC3DAnimationStateBase@@PAVC3DKeyframe@@KKABVC3DMesh2@@@Z.
// Callees: the state's GetType (vtable slot 2), CMeshDataBank::PeekAnimation(unsigned long) const 0x00A242C0 (manifest
// "CTCInventoryAbilities::GetAbilityLevel" is propagated), C3DAnimationInfo::GetAnimation(const C3DMesh2&) const 0x00A26C60
// (CCountedPointer<const C3DAnimation2> by hidden pointer), C3DAnimation2::CalcLocalTransformationsRelative 0x00A4D770 /
// CalcLocalTransformations 0x00A4CEC0, itself (recursion), C3DAnimationManager::BlendAnimations 0x00A9EB90,
// operator delete 0x00BFE9BC (count block).
// Shape: type 0 (C3DAnimationState) fetches the animation for the mesh and calls the relative or absolute evaluator by
// the animation's type bit 2 (whole-dword `shr edx,2 ; test dl,1`), then the counted pointer's long-form release; type 2
// (C3DAnimationTransitionState) evaluates the start state, and below depth 5 the end state into the next boneCount
// keyframes, then blends the two by Progress. `depth + 1` is a named local (it lands in the dead `state` slot).
// The animation index (unsigned long) and PeekAnimation's result are named locals: retail calls PeekAnimation before
// pushing GetAnimation's arguments, with the index in eax; the chained call pushes the arguments first (and an index
// read straight into the call rotates eax/ecx/edx by one through the rest of the block).
// Layout: Ego_r.pdb C3DAnimationState (AnimationIndex +8, AnimationTime +0xc), C3DAnimationTransitionState (StartState +8,
// EndState +0xc, Progress +0x10), C3DAnimationStats AnimType +8; C3DKeyframe 0x30 bytes; the bank at manager +4.
// Leaf: CombineBaseAnimations@C3DAnimationManager
struct SCountBlock {
    long m_references;                             // +0
    void (__fastcall* m_pfnDestroy)(void*);        // +4
    void* m_pObject;                               // +8
};
template <class T> class CCountedPointer { public:
    T* Object;                                     // +0
    SCountBlock* Info;                             // +4
    ~CCountedPointer()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                delete Info;
            }
        }
    }
    T* operator->() const { return Object; }
};

class C3DKeyframe { public:
    float Rotation[4];
    float Position[4];
    float Scaling[4];                              // 0x30 bytes
};
class C3DMesh2;

class C3DAnimation2 { public:
    void* __vftable;                               // +0x00 (C3DAnimationStats base)
    long _04;
    unsigned long AnimType;                        // +0x08 EAnimationType
    bool IsRelative() const { return (AnimType >> 2) & 1; }
    void CalcLocalTransformationsRelative(C3DKeyframe* keyframes, unsigned long count, float time, const C3DMesh2& mesh) const;   // 0x00A4D770
    void CalcLocalTransformations(C3DKeyframe* keyframes, unsigned long count, float time) const;   // 0x00A4CEC0
};

class C3DAnimationInfo { public:
    CCountedPointer<const C3DAnimation2> GetAnimation(const C3DMesh2& mesh) const;   // 0x00A26C60
};
class CMeshDataBank { public:
    const C3DAnimationInfo* PeekAnimation(unsigned long index) const;   // 0x00A242C0
};

class C3DAnimationStateBase { public:
    virtual void v00();
    virtual void v01();
    virtual long GetType() const;                  // slot 2 (+8)
    unsigned long IVRefCount;                      // +0x04
};
class C3DAnimationState : public C3DAnimationStateBase { public:
    long AnimationIndex;                           // +0x08
    float AnimationTime;                           // +0x0c
};
class C3DAnimationTransitionState : public C3DAnimationStateBase { public:
    C3DAnimationStateBase* StartState;             // +0x08 CIVCountedPointer<C3DAnimationStateBase>
    C3DAnimationStateBase* EndState;               // +0x0c CIVCountedPointer<C3DAnimationStateBase>
    float Progress;                                // +0x10
};

class C3DAnimationManager { public:
    virtual ~C3DAnimationManager();
    CMeshDataBank* MeshDataBank;                   // +0x04
    void BlendAnimations(C3DKeyframe* dest, const C3DKeyframe* src, unsigned long count, float alpha);   // 0x00A9EB90
    void CombineBaseAnimations(const C3DAnimationStateBase& state, C3DKeyframe* keyframes, unsigned long boneCount, unsigned long depth, const C3DMesh2& mesh);
};

void C3DAnimationManager::CombineBaseAnimations(const C3DAnimationStateBase& state, C3DKeyframe* keyframes, unsigned long boneCount, unsigned long depth, const C3DMesh2& mesh)
{
    long type = state.GetType();
    if (type == 0) {
        const C3DAnimationState& base = (const C3DAnimationState&)state;
        unsigned long index = base.AnimationIndex;
        const C3DAnimationInfo* info = MeshDataBank->PeekAnimation(index);
        CCountedPointer<const C3DAnimation2> animation = info->GetAnimation(mesh);
        if (animation->IsRelative())
            animation->CalcLocalTransformationsRelative(keyframes, boneCount, base.AnimationTime, mesh);
        else
            animation->CalcLocalTransformations(keyframes, boneCount, base.AnimationTime);
    } else if (type == 2) {
        const C3DAnimationTransitionState& transition = (const C3DAnimationTransitionState&)state;
        unsigned long nextDepth = depth + 1;
        CombineBaseAnimations(*transition.StartState, keyframes, boneCount, nextDepth, mesh);
        if (nextDepth < 5) {
            C3DKeyframe* second = keyframes + boneCount;
            CombineBaseAnimations(*transition.EndState, second, boneCount, nextDepth, mesh);
            BlendAnimations(keyframes, second, boneCount, transition.Progress);
        }
    }
}