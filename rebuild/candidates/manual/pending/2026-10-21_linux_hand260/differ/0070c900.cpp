// Grade: DIFFER(457v455) -- inline C3DAnimationState base stores scheduled before the GetAnimTime fstp; &Anims[1] materialised with add esi,2 (retail movsx [esi+2])
// CAnimSet::GetMeshAnim(float) const -- build the counted animation state for the set interpolated by a frame fraction (one anim, or a transition of two)
// Layout: no engine/CAnimSet.h; retail STLport vector is 12 bytes so InternalAlphas sits at +0x10 and later members 8 below the PDB.
// Callees: C3DAnimationTransitionState ctor @ 0x00A3BE70 (landed); C3DAnimationState vtable 0x01249F50 (inline ctor).
struct CIVCountedPointeeBase {
    virtual void Destroy(int flags);       // slot 0
    virtual void Delete();                 // slot 1
    unsigned long IVRefCount;              // +4
    CIVCountedPointeeBase() : IVRefCount(1) { }
    void DecRef() { --IVRefCount; }
    unsigned long GetRefCount() const { return IVRefCount; }
    void Release()
    {
        DecRef();
        if (GetRefCount() == 0)
            Delete();
    }
};

struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    template<class T> CIVCountedPointer(const T& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
    ~CIVCountedPointer()
    {
        if (Object)
            Object->Release();
        Object = 0;
    }
};

template<class T> struct CIVTypedPointer {
    T* Object;
    CIVTypedPointer(T* object) : Object(object) { }
    template<class U> CIVTypedPointer(const CIVTypedPointer<U>& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
    ~CIVTypedPointer()
    {
        if (Object && --Object->IVRefCount == 0)
            Object->Delete();
    }
};

struct C3DAnimationStateBase : CIVCountedPointeeBase {
    C3DAnimationStateBase() { }
};

struct C3DAnimationState : C3DAnimationStateBase {     // vtable 0x01249F50
    long  AnimationIndex;                  // +8
    float AnimationTime;                   // +0xc
    C3DAnimationState(float time, const short& index) : AnimationIndex(index), AnimationTime(time) { }
    virtual void Destroy(int flags);
    virtual void Delete();
};

struct C3DAnimationTransitionState : C3DAnimationStateBase {
    CIVCountedPointer StartState;
    CIVCountedPointer EndState;
    float Progress;
    C3DAnimationTransitionState(const CIVCountedPointer& start, const CIVCountedPointer& end, float progress);   // 0x00A3BE70
};

struct CShortVector {
    short* _M_start; short* _M_finish; short* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
};
struct CTransitionBlendAlpha { float CurrentAlpha; float LastAlpha; };
struct CAlphaVector { CTransitionBlendAlpha* _M_start; CTransitionBlendAlpha* _M_finish; CTransitionBlendAlpha* _M_end_of_storage; };

inline float Lerp(float from, float to, float t) { return (to - from) * t + from; }

class CAnimSet
{
public:
    long UpdateMode;
    CShortVector Anims;
    CAlphaVector InternalAlphas;
    float CurrentPercentageComplete;
    float LastPercentageComplete;
    float GetAnimTime(float fraction, long index) const;   // 0x0070C800
    CIVTypedPointer<C3DAnimationStateBase> GetMeshAnim(float fraction) const;
};

CIVTypedPointer<C3DAnimationStateBase> CAnimSet::GetMeshAnim(float fraction) const
{
    CIVTypedPointer<C3DAnimationState> start(new C3DAnimationState(GetAnimTime(fraction, 0), Anims._M_start[0]));
    if (Anims.size() == 1)
        return start;
    CIVTypedPointer<C3DAnimationState> end(new C3DAnimationState(GetAnimTime(fraction, 1), Anims._M_start[1]));
    CIVTypedPointer<C3DAnimationTransitionState> transition(new C3DAnimationTransitionState(start, end, Lerp(InternalAlphas._M_start[1].LastAlpha, InternalAlphas._M_start[1].CurrentAlpha, fraction)));
    return transition;
}
