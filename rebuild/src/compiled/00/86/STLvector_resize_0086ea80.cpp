// std::vector::resize(unsigned int new_size) -- STLport vector<CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph> >::resize(n): erase the surplus [begin + n, end) when shrinking, else fill-insert n - size() empty texture-morph maps at the end
// Leaf: ?resize@?$vector@V?$CVectorMap@KVCTextureMorph@CRandomAppearanceMorph@@V?$CKeyPairCompareLess@KVCTextureMorph@CRandomAppearanceMorph@@@@@@V?$allocator@V?$CVectorMap@KVCTextureMorph@CRandomAppearanceMorph@@V?$CKeyPairCompareLess@KVCTextureMorph@CRandomAppearanceMorph@@@@@@@std@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0086EA80, 95 bytes, thiscall on the vector, `ret 4`.
// Identity: FableWin.pdb ?resize@?$vector@V?$CVectorMap@KVCTextureMorph@CRandomAppearanceMorph@@...@@QAEXI@Z; sole
//   caller CRandomAppearanceMorph::CRandomAppearanceMorph 0x0086C4D0 (landed; `BodyPartTextureMorphs.resize(3)` on the
//   CArray<CVectorMap<unsigned long, CTextureMorph> > at +0xC).
// Callees: vector<CTextureMorphMap>::erase(first, last) 0x0086E0B0, vector<CTextureMorphMap>::_M_fill_insert(pos, n,
//   const T&) 0x0086E4B0 (landed; the inline insert's body).
// Layout: the 16-byte map = STLport vector of (key, CTextureMorph) pairs +0, empty CKeyPairCompareLess +0xC (user
//   constructor: not stored), bool Dirty +0xD (the landed 0x0086C4D0 / 0x00675310 model).
//   The leaf names the exact one-argument symbol (the inline two-argument resize is also emitted as a COMDAT).
// Shape: STLport 4.x `void resize(size_type n) { resize(n, _Tp()); }` with the two-argument
//   `resize(n, const _Tp& x) { if (n < size()) erase(begin() + n, end()); else insert(end(), n - size(), x); }` inline:
//   CVectorMap's default constructor zeroes the vector's three words and Dirty (`mov byte ptr [esp+15h],al`) from one
//   zero register. Retail destroys nothing afterwards although the temporary's vector has a freeing destructor: the
//   retail TU holds the bodies of erase / _M_fill_insert, so VC7.1 sees that the calls never write the temporary and
//   folds its `if (_M_start) free(_M_start)` away. Declaration-only callees make VC7.1 re-test the start word after
//   the call (+13 bytes); the same-TU noinline stand-ins below (never graded, they only read the value) restore it.
// Label correction: catalogued as `vector<std::pair<CCharString...>::resize` (propagated); the element is
//   CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph, CKeyPairCompareLess<...> >.
extern "C" void __cdecl free(void* block);                         // 0x00BFEA14 (_Vector_base destructor)
namespace std {
template <class _T1, class _T2> struct pair {                       // STLport pair
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
    ~_Vector_base() { if (_M_start) free(_M_start); }
    _Tp* _M_start;                                                  // +0x00
    _Tp* _M_finish;                                                 // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;         // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    iterator erase(iterator __first, iterator __last);
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(size_type __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};
}

template <class K, class V> class CKeyPairCompareLess {             // empty
public:
    CKeyPairCompareLess() { }
};

template <class K, class V, class C> class CVectorMap : public std::vector<std::pair<K, V> > {
public:
    C    Compare;                                                   // +0x0C
    bool Dirty;                                                     // +0x0D
    CVectorMap() : Dirty(false) { }
};

class CRandomAppearanceMorph {
public:
    class CTextureMorph {                                           // Ego_r.pdb, POD
    public:
        unsigned long NewTexture;
        unsigned long GroupID;
    };
};

typedef CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph,
                   CKeyPairCompareLess<unsigned long, CRandomAppearanceMorph::CTextureMorph> > CTextureMorphMap;

// 0x0086E0B0 erase, 0x0086E4B0 _M_fill_insert (out of line in retail): same-TU noinline stand-ins that, like the
// retail bodies, never write through the value reference (not graded; the landed 0x0086E4B0 is the real fill insert)
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) _Tp* std::vector<_Tp, _Alloc>::erase(_Tp* __first, _Tp* __last)
{
    this->_M_finish = __first + (this->_M_finish - __last);
    return __first;
}
template <class _Tp, class _Alloc>
__declspec(noinline) void std::vector<_Tp, _Alloc>::_M_fill_insert(_Tp* __pos, unsigned int __n, const _Tp& __x)
{
    for (; __n != 0; --__n, ++__pos)
        __pos->Dirty = __x.Dirty;
}
#pragma auto_inline(on)
template void std::vector<CTextureMorphMap>::resize(unsigned int);