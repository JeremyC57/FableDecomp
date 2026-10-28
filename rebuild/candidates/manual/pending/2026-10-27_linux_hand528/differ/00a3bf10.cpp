// C3DAnimationBlendState::CreateInterpolate(const C3DAnimationStateBase& a, const C3DAnimationStateBase& b, float t) -- build a new animation state between a and b at t: a transition state recursively interpolates its start and end states and its progress; a plain state copies the animation index and lerps the time when both indices agree
// Grade: DIFFER(432v432) -- scheduling only: in the transition arm retail pops the kept progress (`fstp st(0)`) right after `mov ecx,[esi+4]`, VC7.1 here one instruction later (after `inc ecx`); every other byte lines up
// Leaf: ?CreateInterpolate@C3DAnimationBlendState@@CI?AV?$CIVCountedPointer@VC3DAnimationStateBase@@@@ABVC3DAnimationStateBase@@0M@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3BF10, 432 bytes, private static /Gr __fastcall (hidden return in ecx,
//   a in edx; b and t on the stack), `ret 8`.
// Identity (proven): Ego_r.pdb ?CreateInterpolate@C3DAnimationBlendState@@CI?AV?$CIVCountedPointer@VC3DAnimationStateBase@@@@
//   ABVC3DAnimationStateBase@@0M@Z (0x00B96750, next to SetBaseAnimation / AddCombinationBlend as retail 0x00A3C260..);
//   callers are itself and C3DAnimationBlendState::SetInterpolate 0x00A3C3B0; the two arms build C3DAnimationTransitionState
//   (vtable 0x01265144, 0x14 bytes: StartState +8, EndState +0xC, Progress +0x10) and C3DAnimationState (vtable 0x01249F50,
//   0x10 bytes: AnimationIndex +8 = -1, AnimationTime +0xC), the Ego_r layouts.
// Callees: operator new 0x00BFEA1A, the state's slot 2 (GetType; 2 = transition), itself, the pointee's slot 1 (Delete).
// Shape: Ego_r.pdb's CIVCountedPointer members (raw-pointer adopting ctor, template converting ctor from
//   CIVCountedPointer<U>, Release, operator=, operator*, operator->, CastToBase, InternalGetObject) and
//   CIVCountedPointeeBase AddRef / Release (Release deletes at zero and returns the count: the temporaries' `mov edi,[eax+4]
//   ; dec edi ; mov edx,edi ; test edx,edx` release); operator= keeps the old pointer in a register (`dec [ecx+4]`); the
//   lerp keeps its start on the x87 stack (`fld a ; fld b ; fsub st(1) ; fmul t ; fadd st(1)`). Tried: Release / AddRef /
//   converting-ctor forms, inline Lerp helper, named result, /G5 /G6 /G7, QFE 4035.
class CIVCountedPointeeBase {
public:
    CIVCountedPointeeBase() : IVRefCount(1) { }
    virtual ~CIVCountedPointeeBase() { }
    virtual void Delete() const;                                        // slot 1
    void AddRef() const { ++IVRefCount; }
    long Release() const
    {
        long count = --IVRefCount;
        if (count == 0)
            Delete();
        return count;
    }
    mutable unsigned long IVRefCount;                                   // +4
};

template <class T> class CIVCountedPointer {
public:
    CIVCountedPointer() : Object(0) { }
    CIVCountedPointer(T* p) : Object(CastToBase(p)) { }
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            Object->AddRef();
    }
    template <class U> CIVCountedPointer(const CIVCountedPointer<U>& other) : Object(other.InternalGetObject())
    {
        if (Object)
            Object->AddRef();
    }
    ~CIVCountedPointer() { Release(); }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        CIVCountedPointeeBase* old = Object;
        if (old != other.Object) {
            if (old) {
                if (--old->IVRefCount == 0)
                    old->Delete();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                Object->AddRef();
        }
        return *this;
    }
    void Release()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
    T& operator*() const { return *static_cast<T*>(Object); }
    T* operator->() const { return static_cast<T*>(Object); }
    CIVCountedPointeeBase* InternalGetObject() const { return Object; }
private:
    CIVCountedPointeeBase* CastToBase(T* p) { return p; }
    CIVCountedPointeeBase* Object;
};

class C3DAnimationStateBase : public CIVCountedPointeeBase {
public:
    enum EType { };
    virtual EType GetType() const = 0;                                  // slot 2
};

class C3DAnimationState : public C3DAnimationStateBase {               // vtable 0x01249F50, 16 bytes
public:
    C3DAnimationState() : AnimationIndex(-1) { }
    virtual EType GetType() const;
    long  AnimationIndex;                                               // +8
    float AnimationTime;                                                // +0xC
};

class C3DAnimationTransitionState : public C3DAnimationStateBase {     // vtable 0x01265144, 20 bytes
public:
    C3DAnimationTransitionState() { }
    virtual EType GetType() const;
    CIVCountedPointer<C3DAnimationStateBase> StartState;                // +8
    CIVCountedPointer<C3DAnimationStateBase> EndState;                  // +0xC
    float Progress;                                                     // +0x10
};

class C3DAnimationBlendState {
private:
    static CIVCountedPointer<C3DAnimationStateBase> __fastcall CreateInterpolate(const C3DAnimationStateBase& a,
                                                                                 const C3DAnimationStateBase& b, float t);
};

CIVCountedPointer<C3DAnimationStateBase> __fastcall C3DAnimationBlendState::CreateInterpolate(const C3DAnimationStateBase& a,
                                                                                              const C3DAnimationStateBase& b,
                                                                                              float t)
{
    if (a.GetType() == 2) {
        const C3DAnimationTransitionState& ta = static_cast<const C3DAnimationTransitionState&>(a);
        const C3DAnimationTransitionState& tb = static_cast<const C3DAnimationTransitionState&>(b);
        CIVCountedPointer<C3DAnimationTransitionState> trans(new C3DAnimationTransitionState);
        trans->StartState = CreateInterpolate(*ta.StartState, *tb.StartState, t);
        trans->EndState = CreateInterpolate(*ta.EndState, *tb.EndState, t);
        const float start = ta.Progress;
        trans->Progress = start + (tb.Progress - start) * t;
        return trans;
    }
    const C3DAnimationState& sa = static_cast<const C3DAnimationState&>(a);
    const C3DAnimationState& sb = static_cast<const C3DAnimationState&>(b);
    CIVCountedPointer<C3DAnimationState> state(new C3DAnimationState);
    if (sa.AnimationIndex == sb.AnimationIndex) {
        state->AnimationIndex = sa.AnimationIndex;
        const float start = sa.AnimationTime;
        state->AnimationTime = start + (sb.AnimationTime - start) * t;
    }
    return state;
}
