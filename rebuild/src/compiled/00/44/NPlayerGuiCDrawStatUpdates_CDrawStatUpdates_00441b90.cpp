// NPlayerGui::CDrawStatUpdates::~CDrawStatUpdates() -- the compiler-generated destructor: free PreviousStats, then the CDrawBase part (destroy every primitive handle through its virtual destructor, free the handle vector)
// Leaf: ??1CDrawStatUpdates@NPlayerGui@@QAE@XZ
// Retail: Fable.exe @ 0x00441B90, 67 bytes, __fastcall, plain `ret`; no vtable store (implicit destructor).
// Identity (proven): CPlayerGui's constructor (0x0043B570) creates `new CDrawStatUpdates` (constructor 0x0064E0A1 stores
// vtable 0x0125A5F4, RTTI .?AVCDrawStatUpdates@NPlayerGui@@) and hands it to CCountedPointer<..>::Reset 0x00444590 on
// PDrawStatUpdates (+0x1A8 = Ego_r +0x1B0 - 8); Reset's count block stores the deleter 0x004437C0
// (?DeleteData@?$CCountedPointer@VCDrawStatUpdates@NPlayerGui@@@@SIXPAX@Z: null test, `call 0x00441B90`, operator delete),
// and the unreferenced non-virtual ??_G 0x00442CC0 calls it too. PDB name sets: ??1CDrawStatUpdates@NPlayerGui@@QAE@XZ,
// ??1CDrawBase@NPlayerGui@@QAE@XZ (both non-virtual). The label "Catch@0047e8cc" is wrong.
// Layout: CDrawBase (Ego_r.pdb, retail 0x28): vptr, PlayerGui +4, PlayerManager +8, World +0xC, Component +0x10, Bank +0x14,
// Drawing +0x18, PrimitiveHandles vector<CEnginePrimitiveHandle> +0x1C (STLport 12 bytes); CDrawStatUpdates:
// LastRenownChange +0x28, LastMoralityChange +0x2C, Initialised +0x30, PreviousStats vector<long> +0x34.
// Callees: free @ 0x00BFEA14 (allocator deallocate), each handle's virtual destructor (vtable slot 0, `push 0`).
// Shape: neither class declares a destructor (a user-declared one would store the vtable); the implicit ~CDrawStatUpdates
// is emitted out of line because its members need cleanup; STLport's vector destructor = _Destroy(start, finish) (no-op
// for long via __type_traits) + _Vector_base's `if (_M_start) deallocate`. DeleteData is modelled from its real
// symbol to make the TU emit the destructor (not graded).
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                   // 0x00BFEA14
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <> struct __type_traits<long> { typedef __true_type has_trivial_destructor; };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _ForwardIterator, class _Tp>
inline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy(__first, __last, (_Tp*)0); }
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}

class CBaseClass {
public:
    virtual ~CBaseClass();
};
class CEnginePersistentPrimitiveBase;
template <class T> class CIVCountedPointer {
public:
    T* Data;
};
class CEnginePrimitiveHandle : public CBaseClass {          // vtable 0x0122F598, 8 bytes
public:
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data; // +0x04
    virtual ~CEnginePrimitiveHandle();                      // slot 0 (??_G 0x0041C500)
};

class CPlayerGui;
class CPlayerManager;
class CWorld;
class CMainGameComponent;
class CGraphicDataBank;

namespace NPlayerGui {
class CDrawBase {                                           // retail 0x28 bytes, no destructor declared (implicit)
public:
    virtual void ClearPrimitives();
    virtual void ClearPrimitivesIfNecessary();
    CPlayerGui*         PlayerGui;                          // +0x04
    CPlayerManager*     PlayerManager;                      // +0x08
    CWorld*             World;                              // +0x0C
    CMainGameComponent* Component;                          // +0x10
    CGraphicDataBank*   Bank;                               // +0x14
    bool                Drawing;                            // +0x18
    std::vector<CEnginePrimitiveHandle> PrimitiveHandles;   // +0x1C
};
}

namespace NPlayerGui {
class CDrawStatUpdates : public CDrawBase {
public:
    long LastRenownChange;                                  // +0x28
    long LastMoralityChange;                                // +0x2C
    bool Initialised;                                       // +0x30
    std::vector<long> PreviousStats;                        // +0x34
};
}

template <class T> class CCountedPointer {
public:
    static void __fastcall DeleteData(void* data) { delete (T*)data; }
};

template class CCountedPointer<NPlayerGui::CDrawStatUpdates>;
template void __fastcall CCountedPointer<NPlayerGui::CDrawStatUpdates>::DeleteData(void*);