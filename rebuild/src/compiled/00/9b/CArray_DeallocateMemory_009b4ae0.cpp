// CArray::DeallocateMemory() -- (CArray<unsigned char>) release the array's storage: when it holds any capacity, erase every element (a trivial memmove-based copy of an empty range) and swap the storage out into an empty array, whose destructor frees the block
// Leaf: ?DeallocateMemory@?$CArray@E@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B4AE0, 68 bytes (+ int3 padding), thiscall, no arguments, `ret`.
// Identity (proven): CArray<unsigned char>::DeallocateMemory(); its one caller is the landed CArray<unsigned char>::
//   DeallocateUnusedMemory 0x009B5DB0 (call at 0x009B5E04), and both PDB name sets carry ?DeallocateMemory@?$CArray@E@@QAEXXZ;
//   rebuild/oracles/reloc_bindings.tsv already binds that symbol to 0x009B4AE0.
// Label: the chunk label CArray<unsigned_char> is propagated across the two byte-identical 1-byte bodies (no ICF in retail).
// Callees: memmove 0x00BFEAE6 (clear's trivial copy, count 0 -- `last` is `end()`), free 0x00BFEA14 (the temporary's destructor).
// Shape: the landed CArray<CBankFileEntryUpdateData*>::DeallocateMemory 0x009D3160 model verbatim with a 1-byte element: the
//   capacity is a plain difference, so `capacity() > 0` is retail's `sub ecx,eax ; je`.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6
extern "C" void __cdecl free(void* block);                                          // 0x00BFEA14
namespace std {
struct __true_type { __true_type() {} };
inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __true_type&)
{
    return (_OutputIter)__copy_trivial(__first, __last, __result);
}
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_aux(_InputIter __first, _InputIter __last, _OutputIter __result, const __true_type&)
{
    return __copy_ptrs(__first, __last, __result, __true_type());
}
template <class _InputIter, class _OutputIter>
inline _OutputIter copy(_InputIter __first, _InputIter __last, _OutputIter __result)
{
    return __copy_aux(__first, __last, __result, __true_type());
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) {}
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __true_type());
}
template <class _Tp> class allocator {
public:
    allocator() {}
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) {}
};
template <class _Tp> inline void swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) {}
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) {}
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    unsigned int capacity() const { return unsigned int(this->_M_end_of_storage._M_data - this->_M_start); }
    void swap(vector<_Tp, _Alloc>& __x)
    {
        std::swap(this->_M_start, __x._M_start);
        std::swap(this->_M_finish, __x._M_finish);
        std::swap(this->_M_end_of_storage, __x._M_end_of_storage);
    }
    iterator erase(iterator __first, iterator __last)
    {
        _Tp* __i = copy(__last, this->_M_finish, __first);
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
};
}   // namespace std
template <class T> class CArray : public std::vector<T> {   // 12 bytes in retail (16 in Ego_r.pdb)
public:
    void DeallocateMemory()
    {
        if (this->capacity() > 0) {
            this->clear();
            CArray<T>().swap(*this);
        }
    }
};
template class CArray<unsigned char>;