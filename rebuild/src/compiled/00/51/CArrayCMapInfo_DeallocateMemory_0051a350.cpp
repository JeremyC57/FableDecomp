// CArray<CMapInfo>::DeallocateMemory() -- release the storage of the world map's 72-byte map-info array: when it holds any capacity, erase every element and swap the storage out into an empty array, whose destructor destroys the detached range and frees the block
// Leaf: ?DeallocateMemory@?$CArray@UCMapInfo@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051A350, length 123 bytes (+ int3 padding); the only caller is
// CArray<CMapInfo>::DeallocateUnusedMemory 0x0051B560 (the manifest's CArray<std::pair<unsigned long,...>> label is propagated;
// the 0x38E38E39 / `sar 4` reciprocal is the 72-byte stride of CMapInfo, the CWorldMap::Maps element of landed 0x005022F0).
// Callees: the random-access __copy<CMapInfo*> 0x005150E0 (landed; __fastcall: first in ecx, last in edx, result + the
//          iterator tag + the Distance* on the stack), the non-trivial __destroy_aux(__false_type) 0x00515250 (__fastcall
//          first, last, tag), free 0x00BFEA14.
// Shape: the STLport 4.6 form of the landed CArray<CCountedPointer<CParticleSimulation> >::DeallocateMemory 0x00B83D30
//   (FableWin.pdb name): `if (capacity() > 0) { clear(); CArray<T>().swap(*this); }` with clear() = erase(begin(), end()) =
//   `_M_finish = copy(end, end, begin)` (copy -> __copy_aux(__true_type) -> __copy_ptrs(__false_type) -> the out-of-line
//   random-access __copy, its tag a one-byte temporary homed in the `push ecx` frame word, the null Distance* the
//   `push 0`), the destroy of the left-over tail the out-of-line __destroy_aux(__false_type); the swap with the zeroed temporary detaches
//   the three pointers and the temporary's destructor destroys the detached range and frees the block (`if (_M_start) free`). The landed form spelled
//   the swap as an `Exchange(slot, 0)` helper.
//   Namespace `std` as the landed 0x00B83D30 model (owner decision on std vs std pending).
extern "C" void __cdecl free(void* block);              // 0x00BFEA14

struct __true_type { __true_type() {} };
struct __false_type { __false_type() {} };

struct CMapInfo {                                       // 0x48 bytes (CWorldMap::Maps element)
    unsigned char _body[0x48];
};

namespace std {
struct input_iterator_tag { input_iterator_tag() {} };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() {} };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() {} };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() {} };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);                    // 0x005150E0
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
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x00515250
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

template class CArray<CMapInfo>;
