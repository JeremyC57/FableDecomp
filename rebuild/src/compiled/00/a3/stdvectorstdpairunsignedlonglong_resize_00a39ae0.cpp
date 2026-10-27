// std::vector<std::pair<unsigned long, long>>::resize(size_type new_size) -- STLport resize(new_size, T()): erase the surplus [begin() + new_size, end()) or fill-insert new_size - size() copies of the value-initialised element at the end
// Leaf: ?resize@?$vector@U?$pair@KJ@std@@V?$allocator@U?$pair@KJ@std@@@2@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00a39ae0, 85 bytes, thiscall, `ret 4`.
// Identity (probable): its only caller CCRCSymbolMap::LoadBinary 0x00A38C20 sizes the symbol table, and Ego_r.pdb gives CCRCSymbolMap one 8-byte-element vector, SLONGMap (CVectorMap<unsigned long, long>, vector of pair<unsigned long, long> at +0).
// Callees: vector::erase(iterator first, iterator last) 0x00A39550 and vector::_M_fill_insert(iterator pos, size_type n,
//   const T& x) 0x00A397D0 (both thiscall, out of line).
// Shape (family L035): the zeroed 8-byte T() temporary lives in the frame (`sub esp,8`); size() via `sar 3`.
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                            // +0x00
    _T2 second;                                           // +0x04
    pair() : first(_T1()), second(_T2()) { }
};
}
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {                                             // STLport 4.6, 12 bytes
public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    _Tp* _M_start;                                        // +0x00
    _Tp* _M_finish;                                       // +0x04
    _Tp* _M_end_of_storage;                               // +0x08
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    iterator erase(iterator __first, iterator __last);   // 0x00A39550
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(size_type __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
protected:
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);   // 0x00A397D0
};
}
template void std::vector<std::pair<unsigned long, long> >::resize(unsigned int);