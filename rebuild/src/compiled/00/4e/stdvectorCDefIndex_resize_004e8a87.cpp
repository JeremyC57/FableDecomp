// std::vector<CDefIndex>::resize(unsigned int new_size, const CDefIndex& x) -- STLport 4.6 resize: shrink by erase(begin() + n, end()), else
//   insert(end(), n - size(), x), which is the inline forwarder to _M_fill_insert
// Leaf: ?resize@?$vector@VCDefIndex@@V?$allocator@VCDefIndex@@@std@@@std@@QAEXIABVCDefIndex@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E8A87, 62 bytes, thiscall `ret 8`, size-optimised TU.
// Identity: the row was catalogued "CArray<std::pair<...>>::push_back" (invented). Its two callees are the landed
//   vector<CDefIndex>::erase(iterator, iterator) 0x004E8AC5 and vector<CDefIndex>::_M_fill_insert 0x004E8AF4; the element is the def member
//   CContainerRewardHeroDef::ObjectFamilies (CArray<CDefIndex>) (Ego_r layout), reached through CContainerRewardHeroDef::Transfer 0x004E8987 -> TransferBinary 0x004E8997 -> GFSerialiseVectorBinaryIn 0x004E89F7. Family pass 2 (agent951) G023/G064/G067 agrees. Retail has no ICF:
//   each address is its own instantiation.
// Shape: `const T&` is passed straight through (no copy), so the parameter is a reference, as in STLport.
#pragma optimize("s", on)

class CDefIndex {                                   // Ego_r: global class (VCDefIndex@@); only its retail size matters here
public:
    unsigned char _Data[0x4];
};

namespace std {

template <class _Tp> class allocator {                // STLport allocator: a class
public:
    allocator() { }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                     // +0x00
    _Tp* _M_finish;                                    // +0x04
    _Tp* _M_end_of_storage;                            // +0x08 (_STLP_alloc_proxy::_M_data)
};

template <class _Tp, class _Alloc> class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp          value_type;
    typedef _Tp*         iterator;
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    iterator erase(iterator __first, iterator __last);                 // 0x004E8AC5 (out of line in retail)
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);   // 0x004E8AF4
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(size_type __new_size, const _Tp& __x);
};

template <class _Tp, class _Alloc> void vector<_Tp, _Alloc>::resize(size_type __new_size, const _Tp& __x)
{
    if (__new_size < size())
        erase(begin() + __new_size, end());
    else
        insert(end(), __new_size - size(), __x);
}

template void vector<CDefIndex, allocator<CDefIndex> >::resize(unsigned int, const CDefIndex&);

}
