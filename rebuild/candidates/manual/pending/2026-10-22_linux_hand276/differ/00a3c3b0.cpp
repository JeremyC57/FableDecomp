// C3DAnimationBlendState::SetInterpolate(const C3DAnimationBlendState&, const C3DAnimationBlendState&, float) -- make this the interpolation of two blend states: own copy of the shared data, interpolated base animation, per-combination-blend index copy and time/weight lerp, the first state's skeletal morph
// Grade: DIFFER(777v777) -- scheduling only: in the 4x-unrolled blend loop retail pops the x87 copy (fstp st(0)) before reloading the next element pointer in two places where ours pops after, and the temporary's release schedules `mov ecx,eax` one slot later; all else relocation-equal
// Leaf: SetInterpolate
void operator delete(void* p);
class C3DAnimationStateBase {
public:
    virtual ~C3DAnimationStateBase();
    virtual void DeleteThis();      // slot 1
    long RefCount;                  // +0x04
    long DecRef() { return --RefCount; }
};
template <class T> class CIVCountedPointer {
public:
    T* Object;
    CIVCountedPointer() : Object(0) {}
    __forceinline ~CIVCountedPointer()
    {
        T* p = Object;
        if (p && p->DecRef() == 0)
            p->DeleteThis();
    }
    __forceinline CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                T* old = Object;
                if (--old->RefCount == 0)
                    old->DeleteThis();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->RefCount;
        }
        return *this;
    }
    T& operator*() const { return *Object; }
};
inline float Lerp(float a, float b, float t) { return a + (b - a) * t; }
class C3DAnimationStateAlpha {
public:
    C3DAnimationStateAlpha() : A(1), AnimationIndex(-1), AlphaWeight(1.0f) {}
    virtual void Slot0();
    long A;               // +0x04
    long AnimationIndex;  // +0x08
    float AnimationTime;  // +0x0c
    float AlphaWeight;    // +0x10
};
template <class T> class CVector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long i) { return _M_start[i]; }
    const T& operator[](unsigned long i) const { return _M_start[i]; }
};
class CAlphaVector : public CVector<C3DAnimationStateAlpha> {
public:
    void resize(unsigned long n, const C3DAnimationStateAlpha& value);   // 0x00A3D500
};
struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
};
class C3DSkeletalMorph;
class CMorphPointer {
public:
    const C3DSkeletalMorph* Object;
    SCountBlock* Info;
    __forceinline CMorphPointer& operator=(const CMorphPointer& other)
    {
        SCountBlock* c = other.Info;
        const C3DSkeletalMorph* p = other.Object;
        if (Info != c) {
            if (Info) {
                --Info->m_references;
                if (Info->m_references == 0) {
                    Info->m_pfnDestroy(Info->m_pObject);
                    delete Info;
                }
            }
            Object = p;
            Info = c;
            if (c)
                ++c->m_references;
        }
        return *this;
    }
};
class C3DAnimationBlendState {
public:
    class CSharedData {
    public:
        virtual ~CSharedData();
        long RefCount;                                          // +0x04
        CIVCountedPointer<C3DAnimationStateBase> BaseAnimation; // +0x08
        CAlphaVector CombinationBlends;                         // +0x0c
        CMorphPointer SkeletalMorph;                            // +0x18
    };
    void* _vptr;
    CSharedData* Data;                                          // +0x04
    void MakeOwnCopyOfData();                                   // 0x00A3C1A0
    static CIVCountedPointer<C3DAnimationStateBase> __fastcall CreateInterpolate(const C3DAnimationStateBase& a, const C3DAnimationStateBase& b, float t); // 0x00A3BF10
    void SetInterpolate(const C3DAnimationBlendState& a, const C3DAnimationBlendState& b, float t);
};

void C3DAnimationBlendState::SetInterpolate(const C3DAnimationBlendState& a, const C3DAnimationBlendState& b, float t)
{
    MakeOwnCopyOfData();
    CSharedData* data = Data;
    data->BaseAnimation = CreateInterpolate(*a.Data->BaseAnimation, *b.Data->BaseAnimation, t);
    unsigned long count = a.Data == 0 ? 0 : a.Data->CombinationBlends.size();
    Data->CombinationBlends.resize(count, C3DAnimationStateAlpha());
    for (unsigned long i = 0; i < count; ++i) {
        C3DAnimationStateAlpha& dst = Data->CombinationBlends[i];
        const C3DAnimationStateAlpha& from = a.Data->CombinationBlends[i];
        const C3DAnimationStateAlpha& to = b.Data->CombinationBlends[i];
        dst.AnimationIndex = from.AnimationIndex;
        dst.AnimationTime = Lerp(from.AnimationTime, to.AnimationTime, t);
        dst.AlphaWeight = Lerp(from.AlphaWeight, to.AlphaWeight, t);
    }
    if (a.Data->SkeletalMorph.Object)
        Data->SkeletalMorph = a.Data->SkeletalMorph;
}
