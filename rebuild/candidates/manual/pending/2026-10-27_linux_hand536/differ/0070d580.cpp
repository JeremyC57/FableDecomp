// CAnimateThing::SetMainAnim(const CAnimInit& init, long blend_frames, long anim_delay) -- finish the main-owned combination anims, start the new main anim (blending the old one into the transition list)
// Grade: DIFFER(504v504) -- register roles only: the vector end is loaded through eax then copied to ebp (retail loads ebp directly), and IsValid's `Anims[1]` reads through edx where retail uses ecx | tried: m1 plain model (534), STLport 4.6 list/iterator model + extern epsilon (531), `if (init.Anim != 0)` wrap + cached end (534->522), CAnimSet::SetAsFinished(long)/IsOwnedByMainAnim/IsValid inlines (504v504), flag store before ChangeAlphaBy (fixed the tail), three loop-head forms and a const-reference GetAnimIndices IsValid (no change) | next: the IsValid inline split (AlphasValid with a cached count) or a different CAnimSet* local scope at the loop head
// Leaf: ?SetMainAnim@CAnimateThing@@QAEXABVCAnimInit@@JJ@Z
// Layout: no shared engine header exists for CAnimateThing / CAnimSet / CAnimInit; retail sizes (CAnimSet 0x54, CCompleteAnimation 0x60, STLport 4-byte lists) from the callers' offsets.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {  // STLport, 12 bytes
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    typedef T* iterator;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector() { if (_M_start) free(_M_start); }
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) { return *(_M_start + n); }
    const T& operator[](unsigned int n) const { return *(_M_start + n); }
};
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class T> struct _List_node : _List_node_base { T _M_data; };
template <class T> struct _Nonconst_traits { typedef T value_type; typedef T* pointer; typedef T& reference; };
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* x) : _M_node(x) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator!=(const _List_iterator_base& y) const { return _M_node != y._M_node; }
};
template <class T, class Traits> struct _List_iterator : _List_iterator_base {
    typedef _List_iterator<T, _Nonconst_traits<T> > iterator;
    _List_iterator(_List_node<T>* x) : _List_iterator_base(x) { }
    _List_iterator(const iterator& x) : _List_iterator_base(x._M_node) { }
    T& operator*() const { return ((_List_node<T>*)_M_node)->_M_data; }
    T* operator->() const { return &((_List_node<T>*)_M_node)->_M_data; }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
};
template <class T> struct _STLP_alloc_proxy { T* _M_data; };
template <class T, class A> class _List_base {
public:
    _STLP_alloc_proxy<_List_node<T> > _M_node;
    void clear();                                           // 0x0070B8A0
};
template <class T, class A = allocator<T> > class list : public _List_base<T, A> {   // STLport 4.6: one header-node pointer
public:
    typedef _List_iterator<T, _Nonconst_traits<T> > iterator;
    iterator begin() { return iterator((_List_node<T>*)this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
    void push_back(const T& x);                             // 0x00711A10
};
template <class _InputIter, class _Function> _Function __fastcall for_each(_InputIter first, _InputIter last, _Function f);   // 0x007100A0
template <class T> inline const T& max(const T& a, const T& b) { return a < b ? b : a; }
}


class CAnimateThing;
class CAnimInit {
public:
    long          UpdateMode;                               // +0x00 EAnimUpdateMode
    unsigned char Type;                                     // +0x04
    bool          Looping;                                  // +0x05
    long          Anim;                                     // +0x08
    float         Speed;                                    // +0x0C
    float         Alpha;                                    // +0x10
    float         LastAlpha;                                // +0x14
    unsigned long Frame[2];                                 // +0x18 CAnimFrameInfo
    unsigned long Flags;                                    // +0x20
    unsigned long TypeFlags;                                // +0x24
    long          Priority;                                 // +0x28
};
struct CTransitionBlendAlpha { float CurrentAlpha; float LastAlpha; };
class CAnimSet {
    friend class CAnimateThing;
public:
    long                                  UpdateMode;           // +0x00
    std::vector<short>                    Anims;                // +0x04
    std::vector<CTransitionBlendAlpha>    InternalAlphas;       // +0x10
    float                                 CurrentPercentageComplete;   // +0x1C
    float                                 LastPercentageComplete;      // +0x20
    float                                 Speed;                // +0x24
    unsigned long                         FrameInternalAlphasUpdated;  // +0x28
    float                                 CurrentAlpha;         // +0x2C
    float                                 LastAlpha;            // +0x30
    float                                 LastUpdateChange;     // +0x34
    float                                 ChangeAlphaBy;        // +0x38
    CAnimateThing*                        PAnimator;            // +0x3C
    unsigned long                         Flags;                // +0x40
    unsigned long                         TypeFlags;            // +0x44
    long                                  AnimDelay;            // +0x48
    long                                  Priority;             // +0x4C
    unsigned char                         Type;                 // +0x50
    bool                                  Looping : 1;          // +0x51
    bool                                  Finished : 1;
    bool                                  Forwards : 1;
    bool                                  HadUpdate : 1;
    bool                                  OwnedByMainAnim : 1;
    bool IsOwnedByMainAnim() const { return OwnedByMainAnim; }
    void SetAsFinished(long blend_frames)
    {
        if (!Finished) {
            Finished = true;
            if (blend_frames < 1)
                blend_frames = 1;
            ChangeAlphaBy = CurrentAlpha / (float)blend_frames;
        }
    }
    bool IsValid() const
    {
        unsigned int count = Anims.size();
        return (count == 1 || (count == 2 && Anims[1] > 0)) && Anims[0] > 0 && count == InternalAlphas.size();
    }
private:
    void Init(CAnimateThing& animator, const CAnimInit& init, float alpha);   // 0x0070D1A0
};
namespace {
class CMultiplyAnimSetAlphaBy {
public:
    float Factor;
    CMultiplyAnimSetAlphaBy(float factor) : Factor(factor) { }
    void operator()(CAnimSet& set) const;
};
}
extern const float g_epsilon_0129ba3c;                      // 0x0129BA3C (1e-4f; descriptive, read as a variable)
class CAnimateThing {
public:
    class CCompleteAnimation {
    public:
        CAnimSet              MainAnimSet;                  // +0x00
        std::list<CAnimSet>   TransitionAnimSets;           // +0x54
        std::list<CAnimSet>   CombinationAnims;             // +0x58
        bool                  MainAnimCanTransitionOut;     // +0x5C
    };
    class CDelayedAnimation {
    public:
        std::list<CAnimSet>   CombinationAnims;             // +0x00
    };
    CCompleteAnimation    CurrentAnim;                      // +0x00
    std::list<CDelayedAnimation> DelayedAnimQueue;          // +0x60
    CDelayedAnimation     DelayedAnim;                      // +0x64
    unsigned long         BlendStates[8];                   // +0x68 Start/End/LastStart/LastEnd C3DAnimationBlendState
    float                 Scale;                            // +0x88
    float                 ChangeAlphaBy;                    // +0x8C
    long                  AnimDelay;                        // +0x90
    bool                  Interruptable : 1;                // +0x94
    unsigned long         CurrentAnimRefreshStamp;          // +0x98
    unsigned long         LastAnimRefreshStamp;             // +0x9C
    void*                 ParentThing;                      // +0xA0
    bool                  AnimFromFileInfoActive;           // +0xA4 CAnimFromFileInfo::Active
    void GetAllCombinationAnims(std::vector<CAnimSet*>& anims);   // 0x0070D100
    void SetMainAnim(const CAnimInit& init, long blend_frames, long anim_delay);
};

void CAnimateThing::SetMainAnim(const CAnimInit& init, long blend_frames, long anim_delay)
{
    LastAnimRefreshStamp = 0;
    AnimFromFileInfoActive = false;
    std::vector<CAnimSet*> combination_anims;
    GetAllCombinationAnims(combination_anims);
    std::vector<CAnimSet*>::iterator end = combination_anims.end();
    for (std::vector<CAnimSet*>::iterator it = combination_anims.begin(); it != end; ++it) {
        CAnimSet* anim_set = *it;
        if (anim_set->IsOwnedByMainAnim())
            anim_set->SetAsFinished(blend_frames);
    }
    if (init.Anim != 0) {
        if (init.Flags & 1) {
            CurrentAnim.CombinationAnims.clear();
            DelayedAnim.CombinationAnims.clear();
        }
        AnimDelay = anim_delay;
        float alpha = 0.0f;
        CAnimSet& main = CurrentAnim.MainAnimSet;
        if (blend_frames > 0 && CurrentAnim.MainAnimCanTransitionOut && main.IsValid()) {
            ChangeAlphaBy = 1.0f / blend_frames;
            CurrentAnim.TransitionAnimSets.push_back(main);
            for (std::list<CAnimSet>::iterator it = CurrentAnim.TransitionAnimSets.begin(); it != CurrentAnim.TransitionAnimSets.end(); ++it)
                it->ChangeAlphaBy = it->ChangeAlphaBy + ChangeAlphaBy;
            float total_alpha = 0.0f;
            for (std::list<CAnimSet>::iterator it2 = CurrentAnim.TransitionAnimSets.begin(); it2 != CurrentAnim.TransitionAnimSets.end(); ++it2)
                total_alpha += it2->CurrentAlpha;
            if (total_alpha > g_epsilon_0129ba3c && total_alpha < 1.0f - g_epsilon_0129ba3c)
                std::for_each(CurrentAnim.TransitionAnimSets.begin(), CurrentAnim.TransitionAnimSets.end(), CMultiplyAnimSetAlphaBy(1.0f / total_alpha));
        } else {
            CurrentAnim.TransitionAnimSets.clear();
            ChangeAlphaBy = 0.0f;
            alpha = 1.0f;
        }
        main.Init(*this, init, alpha);
        CurrentAnim.MainAnimCanTransitionOut = !(init.Flags & 2);
        main.ChangeAlphaBy = -ChangeAlphaBy;
    }
}
