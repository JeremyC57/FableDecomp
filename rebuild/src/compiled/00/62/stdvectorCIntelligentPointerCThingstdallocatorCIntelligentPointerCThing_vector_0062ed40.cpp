// std::vector<CIntelligentPointer<CThing> >::vector(const vector& x) -- STLport vector copy constructor: allocate x.size()
// elements through the base, copy-construct x's elements, set the finish
// Leaf: ??0?$vector@V?$CIntelligentPointer@VCThing@@@@V?$allocator@V?$CIntelligentPointer@VCThing@@@@@std@@@std@@QAE@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0062ED40, 79 bytes (+ padding), thiscall `ret 4`.
// Identity (FINDINGS chunk 386; was the address-named CRetailVector_CIntelligentPointer_0062ed40, manifest
//   CCombatWheel::ResetRings propagated): the landed callers already declare it as this copy constructor --
//   CCreatureActionBase::CCreatureActionBase 0x0062EBC0 for DependentThings and CTCFadeOutAndIn::UpdateFadingThings
//   0x006C3E80 for its local copy of FadingThings (both vector<CIntelligentPointer<CThing>> in struct_layouts_egor);
//   the 8-byte element is copy-built by 0x004AA030 (the CIntelligentPointer placement copy, vtable 0x01238C6C).
// Callees: _Vector_base(n, a) @ 0x0062ED90 (zero the three pointers, allocate n, set start/finish/end), _Construct @
//   0x004AA030 (`__fastcall`, destination in ecx, source in edx).
// Shape: STLport 4.x `vector(const vector& x) : _Vector_base(x.size(), x.get_allocator()) { _M_finish =
//   __uninitialized_copy(x.begin(), x.end(), _M_start, _IsPODType()); }` with the non-POD __uninitialized_copy inline (its by-value bounds keep x's finish in ebp); the empty
//   allocator temporary is homed in the dead `x` slot. Model as in the landed 0x006C3E80 (one symbol for both TUs).
class CBaseObject;
class CBaseObjectPointer;
class CThing;

class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    CBaseObjectPointer* PData;                          // +4
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
};

namespace std {

template <class _Tp> void __fastcall _Construct(_Tp* __p, const _Tp& __val);                  // 0x004AA030

struct __false_type { __false_type() { } };

template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(unsigned int __n, const _Alloc& __a);  // 0x0062ED90
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;
    allocator_type get_allocator() const { return this->_M_end_of_storage; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    vector(const vector& __x);
};

template <class _Tp, class _Alloc>
vector<_Tp, _Alloc>::vector(const vector& __x) : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
{
    this->_M_finish = __uninitialized_copy((const _Tp*)__x._M_start, (const _Tp*)__x._M_finish, this->_M_start, __false_type());
}

template vector<CIntelligentPointer<CThing> >::vector(const vector<CIntelligentPointer<CThing> >&);
}
