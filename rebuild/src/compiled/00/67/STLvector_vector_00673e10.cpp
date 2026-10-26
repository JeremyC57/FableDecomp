// std::vector::vector(const vector& x) -- STLport copy constructor of vector<pair<unsigned long, CRandomAppearanceMorph::CTextureMorph> > (the texture-morph entries): allocate x.size() elements, then placement-copy x's range
// Leaf: ??0?$vector@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00673E10, length 142 bytes, `ret 4`.
// Callers: 17 sites (0x006747F8 .. 0x00674FC5, 0x0086D0A7 .. 0x0086E8A8): copies of CRandomAppearanceMorph /
//   CBodyPartMesh texture-morph CVectorMaps.
// Callees: malloc @ 0x00BFEA0E (allocator<T>::allocate: `n != 0 ? malloc(n * 12) : 0`).
// Label: catalog "std::vector_CopyRange" is descriptive; the body is STLport's vector copy constructor.
// Element: pair<unsigned long, CRandomAppearanceMorph::CTextureMorph> (12 bytes; Ego_r.pdb CTextureMorph = NewTexture +0,
//   GroupID +4), the same model as the landed __uninitialized_copy 0x00673C60 whose loop this body inlines.
// Shape: STLport 4.6 `vector(const _Self& __x) : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator()) {
//   _M_finish = __uninitialized_copy(__x._M_start, __x._M_finish, _M_start, _IsPODType()); }` with `_Vector_base(size_t
//   __n, const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { _M_start =
//   _M_end_of_storage.allocate(__n); _M_finish = _M_start; _M_end_of_storage._M_data = _M_start + __n; }`: the three zero
//   stores come from one `mov eax,0` (flags of the size divide kept for the `n != 0` test of allocate), the pair copy is
//   the key by register and the 8-byte POD through the strength-reduced &first->second; __uninitialized_copy is
//   `__forceinline` (plain `inline` leaves it an out-of-line `__cdecl` call).
extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

class CRandomAppearanceMorph {
public:
    class CTextureMorph { public:                                // Ego_r.pdb, 8 bytes, POD
        unsigned long NewTexture;                         // +0x00
        unsigned long GroupID;                            // +0x04
    };
};

namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    typedef _Alloc allocator_type;
    _Tp* _M_start;                                        // +0x00
    _Tp* _M_finish;                                       // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    _Vector_base(unsigned int __n, const _Alloc& __a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef vector<_Tp, _Alloc> _Self;
    typedef const _Tp* const_pointer;
    typedef _Alloc allocator_type;
    unsigned int size() const { return unsigned int(this->_M_finish - this->_M_start); }
    allocator_type get_allocator() const { return this->_M_end_of_storage; }
    vector(const _Self& __x)
        : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const_pointer)__x._M_start, (const_pointer)__x._M_finish, this->_M_start, __false_type());
    }
};
}

typedef std::pair<unsigned long, CRandomAppearanceMorph::CTextureMorph> TTextureMorphEntry;
template std::vector<TTextureMorphEntry>::vector(const std::vector<TTextureMorphEntry>&);