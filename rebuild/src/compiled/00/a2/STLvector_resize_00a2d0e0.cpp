// std::vector::resize(unsigned int new_size, CMeshDataBankEntry* const& x) -- resize the mesh bank's entry-pointer table: erase the surplus tail (inline memmove copy-down), or fill-insert copies of x at the end
// Leaf: ?resize@?$vector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A2D0E0, 87 bytes, thiscall `ret 8`.
// Callers: one, 0x00A26681 in CMeshDataBank::NotifyBankSize 0x00A26660 (landed: `MeshesAndAnimations.resize(noEntries, 0)`).
// Callees: memmove 0x00BFEAE6 (MSVCR71 thunk), vector<CMeshDataBankEntry*>::_M_fill_insert(pos, n, x) 0x00A2C0F0.
// Label: catalog "vector<unsigned int>::resize" is propagated (the element is a pointer).
// Shape: STLport 4.6 `resize(n, x) { if (n < size()) erase(begin() + n, end()); else insert(end(), n - size(), x); }`:
//   erase inline over a POD element -- `__copy_ptrs(last, _M_finish, first, __true_type())` = __copy_trivial, `(last ==
//   first) ? result : (char*)memmove(result, first, bytes) + bytes` (the `cmp eax,eax` is `_M_finish == _M_finish`), a
//   trivial _Destroy, `_M_finish = i` -- and insert forwarding to the out-of-line _M_fill_insert.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6

class CMeshDataBankEntry;

namespace std {
struct __true_type { __true_type() { } };

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
        : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first)))
              + ((const char*)__last - (const char*)__first);
}

template <class _Tp>
inline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __true_type&)
{
    return (_Tp*)__copy_trivial(__first, __last, __result);
}

template <class _ForwardIterator> inline void _Destroy(_ForwardIterator, _ForwardIterator) { }

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
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x8
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    iterator erase(iterator __first, iterator __last)
    {
        _Tp* __i = __copy_ptrs(__last, this->_M_finish, __first, __true_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void _M_fill_insert(iterator __pos, unsigned int __n, const _Tp& __x);     // 0x00A2C0F0
    void insert(iterator __pos, unsigned int __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(unsigned int __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
};

template void vector<CMeshDataBankEntry*>::resize(unsigned int, CMeshDataBankEntry* const&);
}