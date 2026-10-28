// std::vector::_M_insert_overflow(pointer position, const value_type& x, const __false_type&, size_type fill_len, bool atend) -- (vector<CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph> >) STLport regrowth of the per-body-part texture-morph maps: allocate max(2*size, size+fill_len) 16-byte maps, copy the head, fill_len copies of x and (unless at the end) the tail, clear the old block
// Leaf: ?_M_insert_overflow@?$vector@V?$CVectorMap@KVCTextureMorph@CRandomAppearanceMorph@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086E1C0, 197 bytes, thiscall, `ret 14h`, no EH frame.
// Identity: the manifest's "std::vector::Reallocate_with_fill_16byte" is descriptive. Ego_r.pdb
//   CRandomAppearanceMorph::BodyPartTextureMorphs is CArray<CVectorMap<unsigned long, CTextureMorph,
//   CKeyPairCompareLess<..> > > (12 such vector instantiations in the PDB name set); the element copy is the
//   CVectorMap's implicit one -- the out-of-line base vector<pair<unsigned long, CTextureMorph> > copy constructor
//   0x00673E10 (as in the landed-chunk push_back 0x0086E880), nothing for the empty comparator +0x0C, the Dirty byte
//   +0x0D. The only caller 0x0086E5AE is this vector's _M_fill_insert.
// Callees: malloc 0x00BFEA0E, __uninitialized_copy(first, last, result, const __false_type&) 0x0086D530,
//   the map's base copy constructor 0x00673E10, __uninitialized_fill_n(first, n, x, const __false_type&) 0x0086D090
//   (both `__fastcall`, the tag by reference), vector::_M_clear() 0x00675340 (out of line here: frees each map's
//   block, then the buffer).
// Shape: STLport 4.6 vector.c `_M_insert_overflow(.., const __false_type&, ..)` verbatim, as the siblings 0x005E29D0 /
//   0x0085E200 / 0x008657E0, with `_M_clear()` left out of line (declaration only) and `_M_set` inline. The
//   tag-taking callees get the address of the LIVE `atend` slot: same-TU noinline stand-ins that never touch the tag.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

namespace std {

struct __false_type { __false_type() { } };

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

// Same-TU stand-ins for 0x0086D530 / 0x0086D090 (their own rows), kept out of line.
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
#pragma auto_inline(on)

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }

    vector(const vector<_Tp, _Alloc>& __x);                 // out of line (0x00673E10 for the texture-morph map)
    protected:
    void _M_clear();                                        // out of line (0x00675340 for the vector of maps)
    public:
    protected:
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    public:
    protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
    public:
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                             size_type __fill_len, bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    // handle insertion
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        // copy remainder
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

} // namespace std

template <class K, class V> class CKeyPairCompareLess {
};

template <class K, class V, class C = CKeyPairCompareLess<K, V> >
class CVectorMap : public std::vector<std::pair<K, V> > {
public:
    C    Compare;                                           // +0x0C (empty)
    bool Dirty;                                             // +0x0D
};

class CRandomAppearanceMorph {
public:
    class CTextureMorph {
    public:
        unsigned long NewTexture;                           // +0x00
        unsigned long GroupID;                              // +0x04
    };
};

typedef CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph> CTextureMorphMap;
template void std::vector<CTextureMorphMap>::_M_insert_overflow(CTextureMorphMap*, const CTextureMorphMap&,
                                                                 const std::__false_type&, unsigned int, bool);