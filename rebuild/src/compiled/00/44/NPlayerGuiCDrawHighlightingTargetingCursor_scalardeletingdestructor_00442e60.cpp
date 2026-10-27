// NPlayerGui::CDrawHighlightingTargetingCursor::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated non-virtual deleting destructor: the implicit destructor (PTargetedThing, then the CDrawBase primitive handles) and operator delete when bit 0 is set; returns this
// Leaf: ??_GCDrawHighlightingTargetingCursor@NPlayerGui@@QAEPAXI@Z
// Retail: Fable.exe @ 0x00442E60, 78 bytes, __fastcall `ret 4`; unreferenced (like its siblings 0x00442CC0 / 0x00443080).
// Identity (probable): the PDB name sets hold the non-virtual ??_GCDrawHighlightingTargetingCursor@NPlayerGui@@QAEPAXI@Z;
// among the CDraw* classes (Ego_r.pdb) it is the one whose only destructible member after CDrawBase is a
// CIntelligentPointer<CThing> at the first derived offset (PTargetedThing Ego_r +0x2C = retail +0x28; CurrentColour,
// CurrentWidthScreenSpace/WorldSpace, CurrentFadeStep, Width, CursorType are trivial), matching the
// ~CBaseIntelligentPointer 0x00A01C10 call on +0x28 followed by the CDrawBase handle vector. The label "Catch@0047e8cc"
// is wrong.
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer() @ 0x00A01C10 (the inline implicit
// ~CIntelligentPointer<CThing> chains to it), free @ 0x00BFEA14, the handles' virtual destructors (slot 0), operator
// delete @ 0x00BFE9BC.
// Shape: `delete p` on a class without a virtual destructor emits the non-virtual ??_G COMDAT with the implicit
// destructor inlined (the landed CKeyCamera ??_G 0x0075F0B0 recipe); the helper below is not graded.
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
class CDrawHighlightingTargetingCursor : public CDrawBase {
public:
    CIntelligentPointer<CThing> PTargetedThing;             // +0x28
    unsigned long CurrentColour;                            // +0x30 CRGBColour
    float CurrentWidthScreenSpace;                          // +0x34
    float CurrentWidthWorldSpace;                           // +0x38
    long CurrentFadeStep;                                   // +0x3C
    long Width;                                             // +0x40
    long CursorType;                                        // +0x44 EHighlightingCursorType
};
}

void DeleteOne(NPlayerGui::CDrawHighlightingTargetingCursor* p) { delete p; }