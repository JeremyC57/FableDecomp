// std::vector::resize(size_type new_size, const value_type& x) -- (vector<CLipSyncDataBankEntry >) STLport two-argument resize with the range erase inline: shrink by copying the (empty) tail through the out-of-line __copy and destroying [new end, finish) through the out-of-line __destroy_aux, or grow through _M_fill_insert at the end
// Leaf: ?resize@?$vector@VCLipSyncDataBankEntry@@V?$allocator@VCLipSyncDataBankEntry@@@std@@@std@@QAEXIABVCLipSyncDataBankEntry@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A43F20, 88 bytes (+ int3 padding to 96), thiscall, `ret 8`.
// Identity (proven): std::vector<CLipSyncDataBankEntry >::resize(size_type, const value_type&); evidence: its callers are CLipSyncDataBank::NotifyEntryAdded 0x00A42DF0 and CLipSyncDataBank::NotifyBankSize 0x00A42E70 (the class's CArray<CLipSyncDataBankEntry> Entries, Ego_r.pdb); 8-byte stride (`sar eax, 3`); Ego_r.pdb / FableWin.pdb name the one-argument ?resize@?$vector@VCLipSyncDataBankEntry@@... (the two-argument resize is this out-of-line COMDAT)
// Label: the manifest's "resize" is propagated (family J022: six resize bodies over 4/8-byte non-trivial elements;
//   retail has no identical-code folding, so each address is one element type).
// Callees: std::__copy<CLipSyncDataBankEntry*, CLipSyncDataBankEntry*, int> 0x00A438E0 (`__fastcall`: last / finish in ecx / edx, first, the
//   random_access_iterator_tag reference and `(int*)0` pushed), std::__destroy_aux<CLipSyncDataBankEntry*> 0x00A43A00 (`__fastcall`,
//   the __false_type reference pushed), vector::_M_fill_insert(iterator, size_type, const value_type&) 0x00A43CD0 (thiscall).
// Body: STLport 4.6 `if (__new_size < size()) erase(begin() + __new_size, end()); else insert(end(), __new_size - size(), __x);`
//   with erase = `pointer __i = __copy_ptrs(__last, _M_finish, __first, _TrivialAss()); _Destroy(__i, _M_finish);
//   _M_finish = __i;` and insert(pos, n, x) = _M_fill_insert(pos, n, x); the empty tags are homed in the dead size slot.
// Element: CLipSyncDataBankEntry {CIVCountedPointer<CData> Data; float Duration;}, 8 bytes (Ego_r.pdb).
#include <stddef.h>
class CIVCountedPointeeBase;
template <class T> class CIVCountedPointer {                 // 4 bytes (Ego_r.pdb)
public:
    CIVCountedPointeeBase* Object;
};
class CLipSyncDataBankEntry {                                 // 8 bytes (Ego_r.pdb)
public:
    class CData;
    CIVCountedPointer<CData> Data;                            // +0x00
    float                    Duration;                        // +0x04
};
namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last,
                              _OutputIter __result, const random_access_iterator_tag&, _Distance*);   // out of line
template <class _Tp>
inline _Tp* __copy_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // out of line
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    typedef __false_type _TrivialAss;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    iterator erase(iterator __first, iterator __last)
    {
        pointer __i = __copy_ptrs(__last, this->_M_finish, __first, _TrivialAss());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);   // 0x00A43CD0
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(size_type __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
};
}
template void std::vector<CLipSyncDataBankEntry >::resize(std::vector<CLipSyncDataBankEntry >::size_type, const CLipSyncDataBankEntry&);