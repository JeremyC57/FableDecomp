// NPlayerGui::CDrawSpiritTargetingCursor::~CDrawSpiritTargetingCursor() -- the compiler-generated destructor: destroy every targeted-thing intelligent pointer and free PTargetedThings, then the CDrawBase part (primitive handles)
// Leaf: ??1CDrawSpiritTargetingCursor@NPlayerGui@@QAE@XZ
// Retail: Fable.exe @ 0x00442190, 99 bytes, __fastcall, plain `ret`; no vtable store (implicit destructor).
// Identity (proven): CPlayerGui's constructor (0x0043B570) creates `new CDrawSpiritTargetingCursor` (0x34 bytes, constructor
// 0x0064DD4D stores vtable 0x0125A5E8, RTTI .?AVCDrawSpiritTargetingCursor@NPlayerGui@@) and passes it to Reset 0x00444C20
// on PDrawSpiritTargetingCursor (+0x230 = Ego_r +0x238 - 8); that count block's deleter 0x00443B00
// (?DeleteData@?$CCountedPointer@VCDrawSpiritTargetingCursor@NPlayerGui@@@@SIXPAX@Z) calls 0x00442190 then operator delete;
// the unreferenced non-virtual ??_G 0x00443080 calls it too. PDB: ??1CDrawSpiritTargetingCursor@NPlayerGui@@QAE@XZ. The
// label "Catch@0047e8cc" is wrong.
// Layout: CDrawBase as in 0x00441B90 (PrimitiveHandles +0x1C); PTargetedThings vector<CIntelligentPointer<CThing> > +0x28
// (Ego_r +0x2C less the 4 bytes of the debug vector); retail size 0x34.
// Callees: free @ 0x00BFEA14, each element's virtual destructor (vtable slot 0, `push 0`).
// Shape: no destructor declared anywhere in the chain; the two vector destructors run in reverse member order, each
// `_Destroy(start, finish)` loop then `if (_M_start) free`. DeleteData is modelled to make the TU emit the destructor.
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
class CBaseObjectPointer;
class CBaseIntelligentPointer {                             // vtable 0x0129C95C, 8 bytes
public:
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10 (slot 0 = ??_G 0x00A01C50)
    CBaseObjectPointer* PData;                              // +0x04
};
class CThing;
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };   // implicit (inline) destructor

namespace NPlayerGui {
class CDrawSpiritTargetingCursor : public CDrawBase {
public:
    std::vector<CIntelligentPointer<CThing> > PTargetedThings;   // +0x28
};
}

template <class T> class CCountedPointer {
public:
    static void __fastcall DeleteData(void* data) { delete (T*)data; }
};

template class CCountedPointer<NPlayerGui::CDrawSpiritTargetingCursor>;
template void __fastcall CCountedPointer<NPlayerGui::CDrawSpiritTargetingCursor>::DeleteData(void*);