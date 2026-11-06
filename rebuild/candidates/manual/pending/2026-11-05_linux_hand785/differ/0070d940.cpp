// CAnimateThing::CalcInterpolatedMeshAnim(const CCompleteAnimation& anim, const CDelayedAnimation& delayed, float alpha) const -- build the blend state of a complete animation at an interpolation alpha: the main set, cross-faded from its transition sets (a chain of transition states weighted by their interpolated alphas, skipped beyond the animation LOD distance), plus every loaded combination anim of the current and the delayed animation, each list stable-sorted by priority
// Grade: DIFFER(2572v2395) -- callee set, LOD test, transition chain and both combination passes are retail-shaped; residue is VC7.1's inline budget: retail calls GetProgressFromAlpha 0x0070BE40 and vector<CIVCountedPointer<C3DAnimationTransitionState> >::push_back 0x00711C50 before the transition loop but expands both inside it (overflow 0x00711A50), calls the sorted vector's _M_insert_overflow 0x00711080 in the current-anim pass but expands it in the delayed pass, and inlines every list::size; ours calls GetProgressFromAlpha / push_back at both sites and list::size once and expands both overflows; the transitions push_back passes the new object through an address cast (retail passes a slot holding it with no AddRef)
// Next idea: the pre-loop first-transition setup as its own inline helper (a nested expansion would explain the pre-loop calls) and the CIVCountedPointer(T*) adopting constructor (objects are born with IVRefCount 1); the static's scope id in the bound spelling ?1 is a guess of the 0x01229630 row (see NAME_FIXES.tsv)
// Leaf: ?CalcInterpolatedMeshAnim@CAnimateThing@@ABE?AVC3DAnimationBlendState@@ABVCCompleteAnimation@1@ABVCDelayedAnimation@1@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0070D940, 2395 bytes, thiscall `ret 10h` (hidden result + 3 arguments), frame 0x3C, no EH.
//   Ego_r.pdb ?CalcInterpolatedMeshAnim@CAnimateThing@@ABE?AVC3DAnimationBlendState@@ABVCCompleteAnimation@1@ABVCDelayedAnimation@1@M@Z
//   (private const; Ego_r neighbours CalcMeshAnims / GetProgressFromAlpha); the function-static sorted_combination_anims 0x013BAC20
//   (guard 0x013BAC2C, atexit thunk _$E2 0x01229630) is named in both PDBs.
// Callees: CThing::GetPos 0x004C73D0, CWorld::GetGameLODCentre (slot 8), GFWorld3DDistanceSmallerThan 0x006614F0, CBaseClass ctor
//   0x0099A2D0 / dtor 0x0099A2E0, operator new 0x00BFEA1A (3 inline C3DAnimationTransitionState constructions, IVRefCount 1),
//   CAnimSet::GetMeshAnim 0x0070C900 (x6), C3DAnimationTransitionState::SetStartState 0x00710560 / SetEndState 0x007105A0 (x2 each),
//   GetProgressFromAlpha 0x0070BE40, C3DAnimationBlendState::SetBaseAnimation 0x00A3C2D0 (x2) / AddCombinationBlend 0x00A3C310 (x2),
//   vector<..TransitionState..>::push_back 0x00711C50 / overflow 0x00711A50 / ~vector 0x00711760, vector<CAnimSetIt>::reserve
//   0x00710B00 / _M_insert_overflow 0x00711080, STLport __stable_sort_aux 0x00711B80 (__fastcall, comparator carrying alpha),
//   CMeshDataBank::PeekAnimation 0x00A242C0 (x4), C3DAnimationInfo::IsLoaded 0x00A23F50 / TouchAnimation 0x00A26A50 (x2 each),
//   CAnimSet::GetInterpolatedTime 0x0070C800 (x2), malloc / free / atexit.
// Globals: g_primitiveDrawDistanceScale 0x013B8774, the bools 0x01380BDA (animation LOD) and 0x013756F7 (combination anims; both
//   names descriptive), CMainGameComponent::PMainGameComponent 0x013B86A0, DELTA_NOTIONAL_ZERO 0x0129BA3C, 1.0f / 24.0f literals.
// Layout: the landed CAnimateThing ctor 0x0070BA90 model (CCompleteAnimation 0x60: MainAnimSet, TransitionAnimSets +0x54,
//   CombinationAnims +0x58; CDelayedAnimation = one list; ParentThing +0xA0); C3DAnimationStateAlpha 0x14 (vtable 0x01265128,
//   C3DAnimationState index / time + Alpha); C3DAnimationTransitionState 0x14 (vtable 0x01265144). Row-local models (no CAnimateThing
//   header; the engine C3DAnimation* headers are Ego_r-flattened).
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR
#include "stlport/fable_containers.h"

void* __cdecl operator new(unsigned int size);                       // 0x00BFEA1A

extern const float DELTA_NOTIONAL_ZERO;                             // 0x0129BA3C
extern float g_primitiveDrawDistanceScale;                          // 0x013B8774
extern bool g_AnimationLOD;                                         // 0x01380BDA (name descriptive)
extern bool g_DrawCombinationAnims;                                 // 0x013756F7 (name descriptive)

class C3DVector { public: float X, Y, Z; };
bool __fastcall GFWorld3DDistanceSmallerThan(const C3DVector& a, const C3DVector& b, float distance);   // 0x006614F0

class CIVCountedPointeeBase { public:
    virtual ~CIVCountedPointeeBase() { }
    virtual void Delete() const;
    void AddRef() const { ++IVRefCount; }
    long Release() const { return --IVRefCount; }
    mutable unsigned long IVRefCount;
};
template <class T> class CIVCountedPointer { public:
    __forceinline CIVCountedPointer() : Object(0) { }
    __forceinline CIVCountedPointer(T* object) : Object(object) { if (Object) Object->AddRef(); }
    __forceinline CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    template <class U> __forceinline CIVCountedPointer(const CIVCountedPointer<U>& other) : Object(other.Peek()) { if (Object) Object->AddRef(); }
    __forceinline ~CIVCountedPointer() { Release(); }
    __forceinline CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            Release();
            Object = other.Object;
            if (Object) Object->AddRef();
        }
        return *this;
    }
    __forceinline void Release()
    {
        T* object = Object;
        if (object) {
            if (object->CIVCountedPointeeBase::Release() == 0) object->Delete();
            Object = 0;
        }
    }
    T* Peek() const { return Object; }
    T* operator->() const { return Object; }
    T* Object;
};

class C3DAnimationStateBase : public CIVCountedPointeeBase { public:
    C3DAnimationStateBase() { IVRefCount = 1; }
};
class C3DAnimationState : public C3DAnimationStateBase { public:
    C3DAnimationState(unsigned long index, float time) : AnimationIndex(index), AnimationTime(time) { }
    virtual void Delete() const;
    long AnimationIndex;                                            // +0x08
    float AnimationTime;                                            // +0x0c
};
class C3DAnimationStateAlpha : public C3DAnimationState { public:   // vtable 0x01265128
    C3DAnimationStateAlpha(unsigned long index, float time, float alpha) : C3DAnimationState(index, time), Alpha(alpha) { }
    virtual void Delete() const;
    float Alpha;                                                    // +0x10
};
class C3DAnimationTransitionState : public C3DAnimationStateBase { public:   // vtable 0x01265144, 0x14 bytes
    C3DAnimationTransitionState() { }
    virtual void Delete() const;
    void SetStartState(const CIVCountedPointer<C3DAnimationStateBase>& state);   // 0x00710560
    void SetEndState(const CIVCountedPointer<C3DAnimationStateBase>& state);     // 0x007105A0
    CIVCountedPointer<C3DAnimationStateBase> StartState;            // +0x08
    CIVCountedPointer<C3DAnimationStateBase> EndState;              // +0x0c
    float Progress;                                                 // +0x10
};

class CBaseClass { public:
    CBaseClass();                                                   // 0x0099A2D0
    virtual ~CBaseClass();                                          // 0x0099A2E0
};
class C3DAnimationBlendState : public CBaseClass { public:          // vtable 0x01239838
    class CSharedData;
    C3DAnimationBlendState() { }
    C3DAnimationBlendState(const C3DAnimationBlendState& other) : CBaseClass(other), Data(other.Data) { }
    void SetBaseAnimation(const CIVCountedPointer<C3DAnimationStateBase>& state);   // 0x00A3C2D0
    void AddCombinationBlend(const C3DAnimationStateAlpha& blend);                  // 0x00A3C310
    CIVCountedPointer<CIVCountedPointeeBase> Data;                  // +0x04
};

class C3DAnimationInfo { public:
    bool IsLoaded() const;                                          // 0x00A23F50
    void TouchAnimation() const;                                    // 0x00A26A50
};
class CMeshDataBank { public:
    const C3DAnimationInfo* PeekAnimation(unsigned long index) const;   // 0x00A242C0
};
class CWorld { public:
    virtual void _slot0(); virtual void _slot1(); virtual void _slot2(); virtual void _slot3();
    virtual void _slot4(); virtual void _slot5(); virtual void _slot6(); virtual void _slot7();
    virtual C3DVector GetGameLODCentre() const;                     // slot 8
    unsigned char _pad[0x38];
    CMeshDataBank* PMainMeshBank;                                   // +0x3c
};
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;                  // 0x013B86A0
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }
    unsigned char _pad[0x24];
    CWorld* PWorld;                                                 // +0x24
    CWorld& GetWorld() const { return *PWorld; }
};
class CThing { public:
    unsigned char _pad[0x20];
    unsigned long UsingComponents[9];                               // +0x20 bitset<274>
    bool HasTC(long type) const { return (UsingComponents[type >> 5] & (1UL << (type & 31))) != 0; }
    const C3DVector& GetPos() const;                                // 0x004C73D0
};

class CTransitionBlendAlpha { public:
    float Current;
    float Last;
    float GetInterpolated(float alpha) const { return Last + (Current - Last) * alpha; }
};
class CAnimSet { public:                                            // retail 0x54 bytes
    long UpdateMode;                                                // +0x00
    std::vector<short> Anims;                                       // +0x04
    std::vector<CTransitionBlendAlpha> InternalAlphas;              // +0x10
    float CurrentPercentageComplete;                                // +0x1c
    float LastPercentageComplete;                                   // +0x20
    float Speed;                                                    // +0x24
    unsigned long FrameInternalAlphasUpdated;                       // +0x28
    float CurrentAlpha;                                             // +0x2c
    float LastAlpha;                                                // +0x30
    float LastUpdateChange;                                         // +0x34
    float ChangeAlphaBy;                                            // +0x38
    void* PAnimator;                                                // +0x3c
    unsigned long Flags;                                            // +0x40
    unsigned long TypeFlags;                                        // +0x44
    long AnimDelay;                                                 // +0x48
    long Priority;                                                  // +0x4c
    unsigned char Type;                                             // +0x50
    unsigned char Bits;                                             // +0x51
    CIVCountedPointer<C3DAnimationStateBase> GetMeshAnim(float alpha) const;   // 0x0070C900
    float GetInterpolatedTime(float alpha, long animIndex) const;  // 0x0070C800
    float GetInterpolatedAlpha(float alpha) const { return LastAlpha + (CurrentAlpha - LastAlpha) * alpha; }
    float GetInterpolatedInternalAlpha(float alpha, long index) const { return InternalAlphas[index].GetInterpolated(alpha); }
    C3DAnimationStateAlpha GetFirstMeshAnim(float alpha) const
    {
        float blend = GetInterpolatedInternalAlpha(alpha, 0);
        return C3DAnimationStateAlpha(Anims[0], GetInterpolatedTime(alpha, 0), blend);
    }
    const C3DAnimationInfo& PeekAnimInfo(short anim) const
    {
        return *CMainGameComponent::Get()->GetWorld().PMainMeshBank->PeekAnimation(anim);
    }
};
typedef std::list<CAnimSet>::const_iterator CAnimSetIt;
class CAnimSetItPriorityLessThan { public:
    float Alpha;
    CAnimSetItPriorityLessThan(float alpha) : Alpha(alpha) { }
};
namespace std {
template <class _RandomAccessIter, class _Tp, class _Distance, class _Compare>
void __fastcall __stable_sort_aux(_RandomAccessIter first, _RandomAccessIter last, _Tp*, _Distance*, _Compare comp);   // 0x00711B80
template <class _RandomAccessIter, class _Compare>
inline void stable_sort(_RandomAccessIter first, _RandomAccessIter last, _Compare comp)
{
    __stable_sort_aux(first, last, (CAnimSetIt*)0, (int*)0, comp);
}
}

class CAnimateThing { public:
    class CCompleteAnimation { public:                              // retail 0x60 bytes
        CAnimSet MainAnimSet;                                       // +0x00
        std::list<CAnimSet> TransitionAnimSets;                     // +0x54
        std::list<CAnimSet> CombinationAnims;                       // +0x58
        bool MainAnimCanTransitionOut;                              // +0x5c
    };
    class CDelayedAnimation { public:                               // retail 4 bytes
        std::list<CAnimSet> CombinationAnims;                       // +0x00
    };
    unsigned char _pad[0xa0];
    CThing* ParentThing;                                            // +0xa0
    float GetProgressFromAlpha(float alpha, float totalAlpha) const // 0x0070BE40
    {
        if (totalAlpha >= DELTA_NOTIONAL_ZERO && DELTA_NOTIONAL_ZERO + 1.0f >= totalAlpha)
            return alpha / totalAlpha;
        return 1.0f;
    }
private:
    C3DAnimationBlendState CalcInterpolatedMeshAnim(const CCompleteAnimation& anim, const CDelayedAnimation& delayed, float alpha) const;
};

__forceinline void AddCombinationBlends(C3DAnimationBlendState& state, const std::vector<CAnimSetIt>& sets, float alpha)
{
    for (const CAnimSetIt* it = sets.begin(); it != sets.end(); ++it) {
        const CAnimSet& set = **it;
        if (set.PeekAnimInfo(set.Anims[0]).IsLoaded()) {
            set.PeekAnimInfo(set.Anims[0]).TouchAnimation();
            state.AddCombinationBlend(set.GetFirstMeshAnim(alpha));
        }
    }
}

C3DAnimationBlendState CAnimateThing::CalcInterpolatedMeshAnim(const CCompleteAnimation& anim, const CDelayedAnimation& delayed, float alpha) const
{
    bool beyond_lod = false;
    if (ParentThing->HasTC(2)) {
        float distance = (g_primitiveDrawDistanceScale + 1.0f) * 24.0f;
        beyond_lod = g_AnimationLOD && !GFWorld3DDistanceSmallerThan(CMainGameComponent::Get()->GetWorld().GetGameLODCentre(), ParentThing->GetPos(), distance);
    }
    C3DAnimationBlendState state;
    unsigned long anim_count = anim.MainAnimSet.Anims.size();
    if ((anim_count == 1 || (anim_count == 2 && anim.MainAnimSet.Anims[1] > 0)) && anim.MainAnimSet.Anims[0] > 0
        && anim_count == anim.MainAnimSet.InternalAlphas.size()) {
        if (!beyond_lod && !anim.TransitionAnimSets.empty()) {
            CIVCountedPointer<C3DAnimationStateBase> transition;
            if (anim.TransitionAnimSets.size() > 1) {
                C3DAnimationTransitionState* first = new C3DAnimationTransitionState;
                std::list<CAnimSet>::const_iterator it = anim.TransitionAnimSets.begin();
                CIVCountedPointer<C3DAnimationStateBase> start = it->GetMeshAnim(alpha);
                float total_alpha = it->GetInterpolatedAlpha(alpha);
                ++it;
                CIVCountedPointer<C3DAnimationStateBase> end = it->GetMeshAnim(alpha);
                total_alpha += it->GetInterpolatedAlpha(alpha);
                first->SetStartState(start);
                first->SetEndState(end);
                first->Progress = GetProgressFromAlpha(it->GetInterpolatedAlpha(alpha), total_alpha);
                std::vector<CIVCountedPointer<C3DAnimationTransitionState> > transitions;
                transitions.push_back(*(CIVCountedPointer<C3DAnimationTransitionState>*)&first);
                for (++it; it != anim.TransitionAnimSets.end(); ++it) {
                    C3DAnimationTransitionState* next = new C3DAnimationTransitionState;
                    end = it->GetMeshAnim(alpha);
                    next->StartState = transitions.back();
                    next->EndState = end;
                    total_alpha += it->GetInterpolatedAlpha(alpha);
                    next->Progress = GetProgressFromAlpha(it->GetInterpolatedAlpha(alpha), total_alpha);
                    transitions.push_back(*(CIVCountedPointer<C3DAnimationTransitionState>*)&next);
                    if (next->CIVCountedPointeeBase::Release() == 0) next->Delete();
                }
                transition = transitions.back();
                if (first->CIVCountedPointeeBase::Release() == 0) first->Delete();
            } else {
                transition = anim.TransitionAnimSets.begin()->GetMeshAnim(alpha);
            }
            C3DAnimationTransitionState* main = new C3DAnimationTransitionState;
            main->SetStartState(transition);
            main->SetEndState(anim.MainAnimSet.GetMeshAnim(alpha));
            main->Progress = anim.MainAnimSet.GetInterpolatedAlpha(alpha);
            state.SetBaseAnimation(CIVCountedPointer<C3DAnimationStateBase>(main));
            if (main->CIVCountedPointeeBase::Release() == 0) main->Delete();
        } else {
            state.SetBaseAnimation(anim.MainAnimSet.GetMeshAnim(alpha));
        }
    }
    if (g_DrawCombinationAnims) {
        static std::vector<CAnimSetIt> sorted_combination_anims;
        sorted_combination_anims.clear();
        sorted_combination_anims.reserve(anim.CombinationAnims.size() > delayed.CombinationAnims.size() ? anim.CombinationAnims.size() : delayed.CombinationAnims.size());
        for (CAnimSetIt it = anim.CombinationAnims.begin(); it != anim.CombinationAnims.end(); ++it)
            sorted_combination_anims.push_back(it);
        std::stable_sort(sorted_combination_anims.begin(), sorted_combination_anims.end(), CAnimSetItPriorityLessThan(alpha));
        AddCombinationBlends(state, sorted_combination_anims, alpha);
        sorted_combination_anims.clear();
        for (CAnimSetIt it2 = delayed.CombinationAnims.begin(); it2 != delayed.CombinationAnims.end(); ++it2)
            sorted_combination_anims.push_back(it2);
        std::stable_sort(sorted_combination_anims.begin(), sorted_combination_anims.end(), CAnimSetItPriorityLessThan(alpha));
        AddCombinationBlends(state, sorted_combination_anims, alpha);
    }
    return state;
}
