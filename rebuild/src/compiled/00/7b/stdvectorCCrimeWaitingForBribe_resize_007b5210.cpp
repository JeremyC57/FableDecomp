// std::vector<CCrimeWaitingForBribe>::resize(size_type new_size) -- STLport resize(new_size, T()): erase the surplus [begin() + new_size, end()) or fill-insert new_size - size() copies of the value-initialised element at the end
// Leaf: ?resize@?$vector@VCCrimeWaitingForBribe@@V?$allocator@VCCrimeWaitingForBribe@@@std@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007b5210, 85 bytes, thiscall, `ret 4`.
// Identity (proven): bound (reloc_bindings) ?resize@?$vector@VCCrimeWaitingForBribe@@@std@@QAEXI@Z and Ego_r.pdb ?resize@?$vector@VCCrimeWaitingForBribe@@...@QAEXI@Z; caller the landed GFSerialiseVectorBinaryIn<CCrimeWaitingForBribe> 0x007B5370; the zeroing order of its T() temporary (both stores after the size computation) is that of an inline zeroing constructor, not of value-initialisation.
// Callees: vector::erase(iterator first, iterator last) 0x007B48E0 and vector::_M_fill_insert(iterator pos, size_type n,
//   const T& x) 0x007B4E00 (both thiscall, out of line).
// Shape (family L035): the zeroed 8-byte T() temporary lives in the frame (`sub esp,8`); size() via `sar 3`.
enum EOpinionDeedType { };
class CCrimeWaitingForBribe {                              // Ego_r.pdb, 8 bytes
public:
    long DeedHandle;                                      // +0x00
    EOpinionDeedType Type;                                // +0x04
    CCrimeWaitingForBribe() : DeedHandle(0), Type(EOpinionDeedType(0)) { }   // inline: T() stores both zeros in member order
};
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
    iterator erase(iterator __first, iterator __last);   // 0x007B48E0
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
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);   // 0x007B4E00
};
}
template void std::vector<CCrimeWaitingForBribe >::resize(unsigned int);