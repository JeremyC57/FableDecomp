// C3DAnimationManager::CombineBaseAnimationsBoneList(const C3DAnimationStateBase&, C3DKeyframe*, unsigned long, unsigned long, const C3DMesh2&, const CArray<unsigned long>&) -- evaluate an animation state tree for a list of bones
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA0C50, length 351 bytes (`ret 18h`). Ego_r.pdb
//          ?CombineBaseAnimationsBoneList@C3DAnimationManager@@AAEXABVC3DAnimationStateBase@@PAVC3DKeyframe@@KKABVC3DMesh2@@ABV?$CArray@K@@@Z.
// Callees: state virtual slot 2 (+8) GetStateType, the mesh bank's GetAnimationInfo(long) @ 0x00A242C0 (manifest
//          "CTCInventoryAbilities::GetAbilityLevel" is propagated), C3DAnimationInfo::GetAnimation(const C3DMesh2&) const
//          @ 0x00A26C60 (CCountedPointer<const C3DAnimation2> by value), C3DAnimation2::CalcLocalBoneTransformationRelative
//          @ 0x00A4D920 and CalcLocalBoneTransformation @ 0x00A4D330, C3DAnimationManager::BlendAnimations @ 0x00A9EB90,
//          itself, operator delete @ 0x00BFE9BC.
// Shape: type 0 (C3DAnimationState) samples every listed bone from the instance -- relative animations (AnimType
//          bit 2, `shr ; test cl,1`) also take the mesh -- with the state time copied into a local (it homes in the
//          dead mesh slot); type 2 (C3DAnimationTransitionState) recurses into the start state, and below depth 5
//          into the end state in the scratch keyframes after `out` and blends the two by Progress. The child depth is
//          a named local (it homes in the dead state slot; `++depth` would reuse the depth slot). GetAnimationInfo
//          goes through an inline manager forwarder (MeshBank loads into ecx before the index push) and its result is
//          a named local, so the bank call precedes the GetAnimation argument pushes.
// Layout: C3DAnimationState AnimationIndex +8 / AnimationTime +0xC and C3DAnimationTransitionState StartState +8 /
//          EndState +0xC / Progress +0x10 agree with engine/C3DAnimationState.h and C3DAnimationTransitionState.h; they
//          are modelled locally because the call needs the polymorphic base (vptr +0, ref count +4). C3DAnimationManager
//          MeshBank +4 (PDB). C3DAnimation2 AnimType +8 (C3DAnimationStats).
struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
};
template <class T> class CCountedPointer { public:
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
template <class T> class CArray { public:
    T* m_pBegin;
    T* m_pEnd;
    T* m_pEndOfStorage;
    const T& operator[](unsigned long i) const { return m_pBegin[i]; }
};

class C3DKeyframe { public: float Rotation[4]; float Position[4]; float Scaling[4]; };   // 0x30
class C3DMesh2;
class C3DAnimation2 { public:
    void* __vftable;
    long _04;
    unsigned long AnimType;                 // +0x08 (C3DAnimationStats)
    void CalcLocalBoneTransformationRelative(C3DKeyframe* out, unsigned long bone, float time, const C3DMesh2& mesh) const;   // 0x00A4D920
    void CalcLocalBoneTransformation(C3DKeyframe* out, unsigned long bone, float time) const;   // 0x00A4D330
    bool IsRelative() const { return (AnimType & 4) != 0; }
};
class C3DAnimationInfo { public:
    CCountedPointer<const C3DAnimation2> GetAnimation(const C3DMesh2& mesh) const;   // 0x00A26C60
};
class CMeshDataBank { public:
    C3DAnimationInfo* GetAnimationInfo(long index);   // 0x00A242C0
};

class C3DAnimationStateBase { public:
    virtual void v0();
    virtual void v1();
    virtual long GetStateType() const;      // slot 2 (+8)
    long m_refCount;                        // +4
};
class C3DAnimationState : public C3DAnimationStateBase { public:
    long  AnimationIndex;                   // +0x08
    float AnimationTime;                    // +0x0c
};
class C3DAnimationTransitionState : public C3DAnimationStateBase { public:
    C3DAnimationStateBase* StartState;      // +0x08 CIVCountedPointer<C3DAnimationStateBase>
    C3DAnimationStateBase* EndState;        // +0x0c
    float Progress;                         // +0x10
};

class C3DAnimationManager { public:
    void* __vftable;
    CMeshDataBank* MeshBank;                // +0x04
    C3DAnimationInfo* GetAnimationInfo(long index) const { return MeshBank->GetAnimationInfo(index); }
    void BlendAnimations(C3DKeyframe* a, C3DKeyframe* b, unsigned long count, float weight);   // 0x00A9EB90
    void CombineBaseAnimationsBoneList(const C3DAnimationStateBase& state, C3DKeyframe* out, unsigned long count,
                                       unsigned long depth, const C3DMesh2& mesh, const CArray<unsigned long>& bones);
};

void C3DAnimationManager::CombineBaseAnimationsBoneList(const C3DAnimationStateBase& state, C3DKeyframe* out, unsigned long count,
                                                        unsigned long depth, const C3DMesh2& mesh, const CArray<unsigned long>& bones)
{
    long type = state.GetStateType();
    if (type == 0) {
        const C3DAnimationState& single = static_cast<const C3DAnimationState&>(state);
        C3DAnimationInfo* info = GetAnimationInfo(single.AnimationIndex);
        CCountedPointer<const C3DAnimation2> animation = info->GetAnimation(mesh);
        float time = single.AnimationTime;
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
        const C3DAnimationTransitionState& transition = static_cast<const C3DAnimationTransitionState&>(state);
        unsigned long childDepth = depth + 1;
        CombineBaseAnimationsBoneList(*transition.StartState, out, count, childDepth, mesh, bones);
        if (childDepth < 5) {
            C3DKeyframe* scratch = out + count;
            CombineBaseAnimationsBoneList(*transition.EndState, scratch, count, childDepth, mesh, bones);
            BlendAnimations(out, scratch, count, transition.Progress);
        }
    }
}