// CArray::DeallocateMemory() -- (CArray<CIVCountedPointer<CManagedTextureData> >) release the array's storage: when it holds any capacity, erase every element and swap the storage out into an empty array, whose destructor destroys the detached range and frees the block
// Leaf: ?DeallocateMemory@?$CArray@V?$CIVCountedPointer@VCManagedTextureData@@@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A6BE40, 111 bytes, plain `ret`; a `push ecx` frame word homes the one-byte
//   iterator / __false_type tags.
// Identity: Ego_r.pdb `?DeallocateMemory@?$CArray@V?$CIVCountedPointer@VCManagedTextureData@@@@@@QAEXXZ`; the only
//   caller is the tail jump at 0x00A6A53A of the CPreallocTexturePool member 0x00A6A4D0 on its Textures array (+0x48,
//   CArray<CIVCountedPointer<CManagedTextureData> > per the landed ~CTextureManager 0x00A6A440). Same body as the landed
//   CArray<CCountedPointer<CParticleSimulation> >::DeallocateMemory 0x00B83D30 over a 4-byte element.
// Callees (bound by the landed resize 0x00A6C020): std::__copy<CIVCountedPointer<CManagedTextureData>*, ..., int>
//   @ 0x00A6B080 (__fastcall, `ret 0Ch`), std::__destroy_aux<CIVCountedPointer<CManagedTextureData>*> @ 0x00A6B700
//   (__fastcall, `ret 4`), free (CRT import thunk) @ 0x00BFEA14.
// Shape: `if (capacity() > 0) { clear(); CArray<T>().swap(*this); }` with the STLport 4.6 bodies inline (see 0x00B83D30).
extern "C" void __cdecl free(void* block);              // 0x00BFEA14

class CManagedTextureData;
template <class T> class CIVCountedPointer {           // retail 4 bytes, user copy / assignment / destructor
public:
    T* Object;                                          // +0x00
    ~CIVCountedPointer();
    CIVCountedPointer& operator=(const CIVCountedPointer& other);
};

namespace std {
struct __true_type { __true_type() {} };
struct __false_type { __false_type() {} };
struct input_iterator_tag { input_iterator_tag() {} };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() {} };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() {} };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() {} };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);                    // 0x00A6B080
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
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x00A6B700
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
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
    typedef _Alloc allocator_type;
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

template class CArray<CIVCountedPointer<CManagedTextureData> >;