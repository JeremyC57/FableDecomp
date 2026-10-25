// std::vector::vector(const vector& x) -- (vector<CHeroSoulsCreatureDef>) STLport copy constructor: allocate x.size() 0x34-byte CHeroSoulsCreatureDefs through the out-of-line _Vector_base constructor and copy-construct x's elements into them
// Leaf: ??0?$vector@VCHeroSoulsCreatureDef@@V?$allocator@VCHeroSoulsCreatureDef@@@std@@@std@@QAE@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F26FB0, 98 bytes (+ int3 padding), `ret 4` (returns this).
// Callees: std::_Vector_base<CHeroSoulsCreatureDef>::_Vector_base(unsigned int, const allocator&) @ 0x00F27020 (out of line), CHeroSoulsCreatureDef::CHeroSoulsCreatureDef(const CHeroSoulsCreatureDef&)
//   @ 0x00F27060 (the element copy, out of line, under the placement-new null test).
// Identity: element vtable 0x012FB500 (no RTTI; Transfer slot names CreatureType +0x28, Location +0x2c, NumCreatures
//   +0x30; Sizeof slot returns 0x34) = Ego_r.pdb CHeroSoulsCreatureDef (52 bytes). Catalog
//   "Container_CopyConstructPairs_0x34" is descriptive; the manifest's "pair<CCharString,CSymbolMap::CSymbolInfo>" label
//   on 0x00F27060 is propagated (it is the CHeroSoulsCreatureDef copy constructor).
// Shape: STLport 4.6 `vector(const vector& x) : _Vector_base(x.size(), x.get_allocator()) { _M_finish =
//   __uninitialized_copy(x._M_start, x._M_finish, _M_start, _IsPODType()); }` -- the allocator temporary is homed on x's
//   argument slot (`lea eax,[esp+14h] ; push eax`), size() divides the byte span by the element size (`imul` by the
//   reciprocal), the inline __false_type copy loop tests start == finish first and each placement new tests for null.
//   Local models: the generated engine/CDefClassBase.h is a 0x25-byte pack(1) flat layout with the vptr as data, unusable
//   as a polymorphic base of these 0x28-aligned defs (same local base as the landed CArenaWaveDef::operator= 0x00F25F60).
#include <new>

class CDefClassBase {                               // 0x28 bytes in retail (vptr + 0x24 body), polymorphic def root
public:
    virtual ~CDefClassBase();
    unsigned char _body[0x24];                      // +0x04..+0x27
};
class CCharString {
public:
    void* PStringData;
};
namespace std {

struct __false_type { __false_type() { } };

template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    _Vector_base(unsigned int __n, const _Alloc& __a);   // 0x00F27020
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;
    typedef const _Tp* const_pointer;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    allocator_type get_allocator() const { return allocator_type(); }
    vector(const vector<_Tp, _Alloc>& __x)
        : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const_pointer)__x._M_start, (const_pointer)__x._M_finish, this->_M_start, __false_type());
    }
};

}   // namespace std

class CHeroSoulsCreatureDef : public CDefClassBase {   // vtable 0x012FB500, 0x34 bytes
public:
    CCharString CreatureType;                       // +0x28
    CCharString Location;                           // +0x2c
    long        NumCreatures;                       // +0x30
    CHeroSoulsCreatureDef(const CHeroSoulsCreatureDef& other);   // 0x00F27060
};

template class std::vector<CHeroSoulsCreatureDef>;