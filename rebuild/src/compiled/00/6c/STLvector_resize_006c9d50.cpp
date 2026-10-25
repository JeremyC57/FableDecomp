// std::vector::resize(unsigned int new_size) -- STLport vector<pair<long, COpinionDeed*> >::resize(n): erase the surplus [begin + n, end) when shrinking, else fill-insert n - size() zeroed (handle, deed) pairs at the end
// Leaf: ?resize@?$vector@U?$pair@JPAVCOpinionDeed@@@std@@V?$allocator@U?$pair@JPAVCOpinionDeed@@@std@@@2@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C9D50, 85 bytes, thiscall on the vector, `ret 4`.
// Identity: FableWin.pdb ?resize@?$vector@U?$pair@JPAVCOpinionDeed@@@std@@...@@QAEXI@Z; sole caller
//   GFSerialiseVectorBinaryIn<pair<long, COpinionDeed*> > 0x006C9E40 (landed; the hero opinion-deed log's load).
// Callees: vector<pair<long, COpinionDeed*> >::erase(first, last) 0x006C7FE0 (landed as CRetailVector_pod8::erase),
//   vector<pair<long, COpinionDeed*> >::_M_fill_insert(pos, n, const T&) 0x006C8610 (the inline insert's body).
//   The leaf names the exact one-argument symbol (the inline two-argument resize is also emitted as a COMDAT).
// Shape: STLport 4.x `void resize(size_type n) { resize(n, _Tp()); }` with the two-argument
//   `resize(n, const _Tp& x) { if (n < size()) erase(begin() + n, end()); else insert(end(), n - size(), x); }` inline:
//   STLport pair's user default constructor `pair() : first(_T1()), second(_T2())` zeroes the temporary after the size
//   subtraction (the 0x00639810 shape); finish is re-read for the second size().
// Label correction: catalogued as `vector<std::pair<CCharString...>::resize` (propagated); the element is
//   pair<long, COpinionDeed*>.
class COpinionDeed;

namespace std {
template <class _T1, class _T2> struct pair {                       // STLport pair
    _T1 first;                                                      // +0x00
    _T2 second;                                                     // +0x04
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                                  // +0x00
    _Tp* _M_finish;                                                 // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;         // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
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

// 0x006C7FE0 erase, 0x006C8610 _M_fill_insert (out of line in retail)
template void std::vector<std::pair<long, COpinionDeed*> >::resize(unsigned int);