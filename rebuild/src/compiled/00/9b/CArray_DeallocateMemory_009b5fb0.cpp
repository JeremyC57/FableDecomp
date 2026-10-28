// CArray::DeallocateMemory() -- (CArray<std::pair<unsigned long, unsigned long> >) release the array's storage: when it holds any capacity, erase every element (STLport's element-wise copy of the empty tail range) and swap the storage out into an empty array, whose destructor frees the block
// Leaf: ?DeallocateMemory@?$CArray@U?$pair@KK@std@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B5FB0, 90 bytes (+ int3 padding), thiscall, no arguments, `ret`.
// Identity (proven): its one caller is CArray<pair<unsigned long, unsigned long> >::DeallocateUnusedMemory 0x009B7B10 (call at
//   0x009B7B6A), whose callers are CDefinitionManager / CBankFile code (CBankFile::PostInitEntries 0x009CFB70) and whose
//   _M_range_insert 0x009B54E0 is landed as vector<pair<unsigned long, unsigned long> >; Ego_r.pdb CBankFile::SymbolCRCIndices
//   is CVectorMap<unsigned long, unsigned long> (a CArray<pair<K,K> >), and FableWin.pdb carries
//   ?DeallocateMemory@?$CArray@U?$pair@KK@std@@@@QAEXXZ. The 8-byte stride (`sar 3`) and the two-dword copy agree.
// Label: the chunk label CArray<unsigned_short> is propagated.
// Callees: free 0x00BFEA14 (the temporary's destructor). STLport's random-access __copy is inlined (a pair has a user
//   constructor, so its type traits are __false_type and copy() takes the element-wise loop rather than memmove).
// Shape: the landed CArray<SElement12>::DeallocateMemory 0x009D3D90 model with __copy inlined (`__forceinline`): the counted
//   `for (n = last - first; n > 0; --n)` loop over end()..end() gives retail's `sar 3 ; test ; jle` and the `dec esi` loop.
extern "C" void __cdecl free(void* block);              // 0x00BFEA14

struct __true_type { __true_type() {} };
struct __false_type { __false_type() {} };


namespace std {
struct input_iterator_tag { input_iterator_tag() {} };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() {} };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() {} };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() {} };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
};
template <class _RandomAccessIter, class _OutputIter, class _Distance>
__forceinline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                                 const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_aux(_InputIter __first, _InputIter __last, _OutputIter __result, const __true_type&)
{
    return __copy_ptrs(__first, __last, __result, __false_type());
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
}

template <class T> class CArray : public std::vector<T> {
public:
    void DeallocateMemory()
    {
        if (this->capacity() > 0) {
            this->clear();
            CArray<T>().swap(*this);
        }
    }
};

template class CArray<std::pair<unsigned long, unsigned long> >;
