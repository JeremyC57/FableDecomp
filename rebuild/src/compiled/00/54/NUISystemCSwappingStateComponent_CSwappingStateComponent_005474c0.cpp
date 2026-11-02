// NUISystem::CSwappingStateComponent::~CSwappingStateComponent() -- store the class's three vtables, free the SwappingStates vector storage, then run ~CChangingStateComponent
// Leaf: ??1CSwappingStateComponent@NUISystem@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005474C0, 50 bytes, thiscall, tail `jmp ~CChangingStateComponent`; no caller (the scalar
//   deleting destructor 0x00547AB0 inlines the same body; an unreferenced COMDAT the linker kept).
// Identity (proven): (1) it stores the three NUISystem::CSwappingStateComponent vtables 0x012485AC (+0x00), 0x01248584 (+0x04),
//   0x0124857C (+0x18) (rtti-vtables.tsv), and the class's ??_G 0x00547AB0 (slot 0 of 0x012485AC) expands exactly this body
//   before its flag test; (2) Ego_r.pdb / FableWin.pdb ??1CSwappingStateComponent@NUISystem@@UAE@XZ (Ego_r donor 0x005C1290, in
//   the class block Initialise / Update / ??1 / GetDataFromDef / ctor that retail keeps at Initialise 0x00547360 / Update 0x00547380 /
//   this row). Its only non-trivial member is SwappingStates (Ego_r vector<pair<unsigned long, float> > at +0x18C, retail +0x15C).
//   The catalog label "CParticleEffect::~CParticleEffect" is propagated.
// Callees: free 0x00BFEA14 (~_Vector_base's deallocate), NUISystem::CChangingStateComponent::~CChangingStateComponent 0x0052CCF0.
// Shape: a user-declared virtual destructor with an empty body (it stores all three vtables; an implicit one would store none),
//   then the STLport vector frees its block under the null test (trivial pair: no element loop), then the base destructor;
//   the same model as the landed ~CDynamicList 0x00541350.
// Layout: NUISystem::CComponent (CUIInterface, CObserver +4, CActionParam +0x18; retail 0x134 bytes) / CChangingStateComponent
//   (retail 0x15C) as the landed ~CChangingStateComponent 0x0052CCF0; CSwappingStateComponent members from Ego_r.pdb 0x30 lower.
//   No engine header exists for these NUISystem classes.
extern "C" void __cdecl free(void* block);                      // 0x00BFEA14
namespace std {
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base() : _M_start(0), _M_finish(0) { _M_end_of_storage._M_data = 0; }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
template <class _Tp> struct less { };
template <class _Tp, class _Alloc> class _List_base {
public:
    ~_List_base();                                          // 0x0052D5C0 (EChangingStateComponentState)
    void* _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> { };
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> > class set {
public:
    ~set();
    void* _M_header;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
};
}   // namespace std

namespace NUISystem {
enum EEvent { };
enum EActionParamType { };
class CUIInterface {                                        // vptr only
public:
    virtual ~CUIInterface();
};
class CObserver {                                           // +0x04 in CComponent, 0x14 bytes
public:
    virtual ~CObserver();
    std::set<EEvent> EventsToObserve;
    bool PreventObservation;
};
class CActionParam {                                        // +0x18 in CComponent
public:
    virtual ~CActionParam();
    EActionParamType ActionParamType;
};
class CComponent : public CUIInterface, public CObserver, public CActionParam {   // retail 0x134 bytes
public:
    virtual ~CComponent();                                  // 0x00532D90
    unsigned char _body[0x134 - 0x20];
};
class CChangingStateComponent : public CComponent {         // retail 0x15C bytes
public:
    enum EChangingStateComponentState { };
    virtual ~CChangingStateComponent();                     // 0x0052CCF0
    EChangingStateComponentState StatesBeingDone;           // +0x134
    EChangingStateComponentState StatesDone;                // +0x138
    std::list<EChangingStateComponentState> StatesToDo;     // +0x13c
    float ParentUpdateTime;                                 // +0x140
    unsigned long CurrentState;                             // +0x144
    unsigned long TargetState;                              // +0x148
    unsigned long RequestedState;                           // +0x14c
    float UpdateTime;                                       // +0x150
    bool PreviousUpdateChanged;                             // +0x154
    unsigned long PreviousState;                            // +0x158
};

class CSwappingStateComponent : public CChangingStateComponent {   // vtables 0x012485AC / 0x01248584 / 0x0124857C, retail 0x170 bytes
public:
    virtual ~CSwappingStateComponent();
    std::vector<std::pair<unsigned long, float> > SwappingStates;   // +0x15c
    float         LastSwapTime;                             // +0x168
    unsigned long Seed;                                     // +0x16c
};

CSwappingStateComponent::~CSwappingStateComponent()
{
}
} // namespace NUISystem