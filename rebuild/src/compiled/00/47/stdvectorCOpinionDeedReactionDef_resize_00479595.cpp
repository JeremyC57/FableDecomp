// std::vector<COpinionDeedReactionDef>::resize(unsigned int new_size, const COpinionDeedReactionDef& x) -- STLport 4.6 resize: shrink by erase(begin() + n, end()), else
//   insert(end(), n - size(), x), which is the inline forwarder to _M_fill_insert
// Leaf: ?resize@?$vector@VCOpinionDeedReactionDef@@V?$allocator@VCOpinionDeedReactionDef@@@std@@@std@@QAEXIABVCOpinionDeedReactionDef@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00479595, 75 bytes, thiscall `ret 8`, size-optimised TU.
// Identity: the row was catalogued "CArray<std::pair<...>>::push_back" (invented). Its two callees are the landed
//   vector<COpinionDeedReactionDef>::erase(iterator, iterator) 0x004795E0 and vector<COpinionDeedReactionDef>::_M_fill_insert 0x00479630 (not landed); the element is the def member
//   COpinionDeedEffectsDef::Reaction (Ego_r layout), reached through COpinionDeedEffectsDef::Transfer 0x00478EE2 -> TransferVectorOfSubComponents 0x00479466. Family pass 2 (agent951) G023/G064/G067 agrees. Retail has no ICF:
//   each address is its own instantiation.
// Shape: `const T&` is passed straight through (no copy), so the parameter is a reference, as in STLport.
#pragma optimize("s", on)

class COpinionDeedReactionDef {                                   // Ego_r: global class (VCOpinionDeedReactionDef@@); only its retail size matters here
public:
    unsigned char _Data[0x50];
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
    iterator erase(iterator __first, iterator __last);                 // 0x004795E0 (out of line in retail)
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);   // 0x00479630
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

template void vector<COpinionDeedReactionDef, allocator<COpinionDeedReactionDef> >::resize(unsigned int, const COpinionDeedReactionDef&);

}
