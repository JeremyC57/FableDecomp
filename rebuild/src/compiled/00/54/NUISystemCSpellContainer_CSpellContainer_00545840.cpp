// NUISystem::CSpellContainer::~CSpellContainer() -- unregisters the container's observer from the front-end UI manager, destroys BaseSpells (the counted component pointers, then the block) and the CChangingStateComponent base
// Leaf: ??1CSpellContainer@NUISystem@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00545840, 92 bytes, plain `ret`, `push ecx` frame (the __false_type tag slot).
// Identity: the first stored vtable 0x0124786C is NUISystem::CSpellContainer's (RTTI; also 0x01247848 +4, 0x01247840
//   +0x18), so this is Ego_r.pdb ??1CSpellContainer@NUISystem@@UAE@XZ; the catalog label "~CZoomingComponent" is wrong.
//   Only caller: the CSpellContainer deleting destructor at 0x005460A3.
// Callees: NUISystem::CManager::GetInstance 0x0041E5F2 (static __fastcall), CObservable slot 5 (+0x14, RemoveObserver),
//   std::__destroy_aux<CCountedPointer<NUISystem::CComponent>*> 0x00535420 (__fastcall: first ecx, last edx, the tag by
//   address), free 0x00BFEA14, ~CChangingStateComponent 0x0052CCF0.
// Layout: CChangingStateComponent is 0x15C bytes in retail; Ego_r.pdb CSpellContainer::BaseSpells
//   (vector<CCountedPointer<CComponent> >, PDB +0x18C) sits at retail +0x15C (STLport 12-byte vector).
// Shape: the user destructor body (RemoveObserver), then the implicit member destruction: STLport ~vector
//   `_Destroy(_M_start, _M_finish)` through the out-of-line __destroy_aux, ~_Vector_base's null-tested free, then the
//   base destructor called (not jumped to: the tag frame must be popped). The empty __false_type tag needs its user
//   constructor (STLport 4.6 declares none; without it VC7.1 zero-stores the tag byte).
extern "C" void __cdecl free(void* block);      // 0x00BFEA14

namespace std {
struct __true_type { };
struct __false_type { __false_type() { } };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x00535420
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
template <class _Tp, class _Alloc> class _Vector_base {
public:
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
} // namespace std

template <class T> class CCountedPointer {      // 8 bytes: Data, count block
public:
    T*    Data;
    void* Info;
    ~CCountedPointer();
};

namespace NUISystem {

enum EEvent { EEVENT_NONE = 0 };

class CObserver;

class CObservable {                             // NUISystem::CManager's base (vtable 0x01230134)
public:
    virtual ~CObservable();                     // slot 0
    virtual void SetOwnedEvent(CObserver* observer, EEvent event);   // slot 1
    virtual void AddObserver(CObserver* observer);                   // slot 2
    virtual void _v3();
    virtual void _v4();
    virtual void RemoveObserver(CObserver* observer);                // slot 5 (+0x14)
};

class CManager : public CObservable {           // the front-end UI manager singleton (0xD0 bytes)
public:
    static CManager* __fastcall GetInstance();  // 0x0041E5F2
};

class CUIInterface {                            // +0x00
public:
    virtual ~CUIInterface();
};
class CObserver {                               // +0x04, 0x14 bytes in retail
public:
    virtual ~CObserver();
    unsigned char EventsToObserve[0xc];         // +0x04 std::set<EEvent>
    bool          PreventObservation;           // +0x10
};
class CActionParam {                            // +0x18
public:
    virtual ~CActionParam();
    long ActionParamType;                       // +0x1C
};

class CComponent : public CUIInterface, public CObserver, public CActionParam {
public:
    unsigned char _pad020[0x134 - 0x20];        // retail sizeof 0x134
};

class CChangingStateComponent : public CComponent {
public:
    virtual ~CChangingStateComponent();         // 0x0052CCF0
    unsigned char _pad134[0x15c - 0x134];       // StatesBeingDone .. PreviousState (retail sizeof 0x15C)
};

} // namespace NUISystem

namespace NUISystem {

class CSpellContainer : public CChangingStateComponent {
public:
    virtual ~CSpellContainer();
    std::vector<CCountedPointer<CComponent> > BaseSpells;   // +0x15C
};

CSpellContainer::~CSpellContainer()
{
    CManager::GetInstance()->RemoveObserver(this);
}

} // namespace NUISystem