// CAnimateThing::CCompleteAnimation::CCompleteAnimation() -- default constructor: the main CAnimSet (empty Anims /
// InternalAlphas vectors, ChangeAlphaBy 0, no animator, HadUpdate cleared) and the two empty STLport lists of CAnimSet
// (TransitionAnimSets, CombinationAnims; each a fresh self-linked sentinel node)
// Leaf: ??0CCompleteAnimation@CAnimateThing@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00711CF0, length 82 bytes.
// Identity (FINDINGS chunk 390: the landed "CAnimSet::CAnimSet" is the CCompleteAnimation ctor): struct_layouts
//   CCompleteAnimation {CAnimSet MainAnimSet +0, list<CAnimSet> TransitionAnimSets, list<CAnimSet> CombinationAnims,
//   bool MainAnimCanTransitionOut} -- retail CAnimSet is 0x54 bytes (the PDB 0x5C minus 8 for its two 12-byte vectors),
//   so the lists sit at +0x54 / +0x58 and their sentinel nodes are 8 + 0x54 = 0x5C bytes (the `push 5Ch` mallocs);
//   Ego_r.pdb names ??0CCompleteAnimation@CAnimateThing@@QAE@XZ; xrefs: the only caller is the CAnimateThing
//   constructor 0x0070E710 (CAnimateThing::CurrentAnim is the CCompleteAnimation at +0; it goes on to build
//   DelayedAnimQueue at +0x60).
// Callees: malloc 0x00BFEA0E (twice, the list sentinel nodes; the two cleanups are merged into one add esp,8).
// Layout: CAnimSet (PDB names) ChangeAlphaBy +0x38, PAnimator +0x3c, bit-field byte +0x51 (HadUpdate is bit 3:
//   `and al,0F7h`). MainAnimCanTransitionOut is not initialised by this constructor.
// Shape: the inline CAnimSet() and the STLport `_List_base(const allocator_type&) : _M_node(__a, (_Node*)0)` with the
//   local `__n` sentinel (`__n->_M_next = __n; __n->_M_prev = __n; _M_node._M_data = __n;`).
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
};

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _List_base(const _Alloc&) : _M_node(allocator<_Node>(), (_Node*)0)
    {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    explicit list(const _Alloc& __a = _Alloc()) : _List_base<_Tp, _Alloc>(__a) { }
};
}

enum EAnimUpdateMode { ANIM_UPDATE_MODE_0 };
class CTransitionBlendAlpha { public: float Current; float Last; };
class CAnimateThing;

class CAnimSet {                                         // Ego_r.pdb names; 0x54 bytes in retail
public:
    CAnimSet() : ChangeAlphaBy(0.0f), PAnimator(0), HadUpdate(0) { }   // ??0CAnimSet@@QAE@XZ (inline here)
    EAnimUpdateMode UpdateMode;                          // +0x00
    std::vector<short> Anims;                            // +0x04
    std::vector<CTransitionBlendAlpha> InternalAlphas;   // +0x10
    float CurrentPercentageComplete;                     // +0x1c
    float LastPercentageComplete;                        // +0x20
    float Speed;                                         // +0x24
    unsigned long FrameInternalAlphasUpdated;            // +0x28
    float CurrentAlpha;                                  // +0x2c
    float LastAlpha;                                     // +0x30
    float LastUpdateChange;                              // +0x34
    float ChangeAlphaBy;                                 // +0x38
    CAnimateThing* PAnimator;                            // +0x3c
    unsigned long Flags;                                 // +0x40
    unsigned long TypeFlags;                             // +0x44
    long AnimDelay;                                      // +0x48
    long Priority;                                       // +0x4c
    unsigned char Type;                                  // +0x50
    bool Looping : 1;                                    // +0x51
    bool Finished : 1;
    bool Forwards : 1;
    bool HadUpdate : 1;
    bool OwnedByMainAnim : 1;
};

class CAnimateThing {
public:
    class CCompleteAnimation {
    public:
        CCompleteAnimation();
        CAnimSet MainAnimSet;                            // +0x00
        std::list<CAnimSet> TransitionAnimSets;          // +0x54
        std::list<CAnimSet> CombinationAnims;            // +0x58
        bool MainAnimCanTransitionOut;                   // +0x5c
    };
};

CAnimateThing::CCompleteAnimation::CCompleteAnimation()
{
}
