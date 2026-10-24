// CArray::DeallocateMemory() -- (CArray<CCountedPointer<CParticleSimulation> >) release the array's storage: when it holds any capacity, erase every element and swap the storage out into an empty array, whose destructor destroys the detached range and frees the block
// Leaf: ?DeallocateMemory@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B83D30, 111 bytes. __fastcall, plain `ret`; a `push ecx` frame word that
//   homes the one-byte iterator / __false_type tags.
// Callees: _STL::__copy<CCountedPointer<CParticleSimulation>*, ...> @ 0x00B83330 (__fastcall, `ret 0Ch`: result, the
//   random_access_iterator_tag reference and the null distance pointer on the stack), _STL::__destroy_aux<...> @ 0x00B833F0
//   (__fastcall, `ret 4`: the __false_type reference), free (CRT import thunk) @ 0x00BFEA14.
// Callers: CEngineClothSimulationMeshBase::ClearParticleSims 0x00B81D60 (call 0x00B81DEA, on ParticleSims).
// Identity: FableWin.pdb `?DeallocateMemory@?$CArray@V?$CCountedPointer@VCParticleSimulation@@@@@@QAEXXZ`; the manifest
//   label "Vector_Clear" is descriptive. CArray<T> is the engine's STLport vector<T> wrapper (12 bytes).
// Shape: `if (capacity() > 0) { clear(); CArray<T>().swap(*this); }` with the STLport bodies inline: clear() is
//   erase(begin(), end()) = `__i = copy(end, end, begin)` (copy -> __copy_aux(__true_type) -> __copy_ptrs(__false_type)
//   -> the out-of-line random-access __copy), `_Destroy(__i, _M_finish)` (-> the out-of-line __destroy_aux(__false_type)),
//   `_M_finish = __i`; the swap with the zeroed temporary detaches the three pointers, and the temporary's destructor
//   runs _Destroy over the detached range and _Vector_base's `if (_M_start) deallocate` (free).
extern "C" void __cdecl free(void* block);              // 0x00BFEA14

struct __true_type { __true_type() {} };
struct __false_type { __false_type() {} };

class CCPPointerInfo;
template <class T> class CCountedPointer {             // retail 8 bytes, user copy / assignment / destructor
public:
    T*              Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
    ~CCountedPointer();
    CCountedPointer& operator=(const CCountedPointer& other);
};
class CParticleSimulation;
typedef CCountedPointer<CParticleSimulation> CParticleSimulationPtr;

namespace _STL {
struct input_iterator_tag { input_iterator_tag() {} };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() {} };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() {} };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() {} };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);                    // 0x00B83330
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
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x00B833F0
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
        _STL::swap(this->_M_start, __x._M_start);
        _STL::swap(this->_M_finish, __x._M_finish);
        _STL::swap(this->_M_end_of_storage, __x._M_end_of_storage);
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

template <class T> class CArray : public _STL::vector<T> {
public:
    void DeallocateMemory()
    {
        if (this->capacity() > 0) {
            this->clear();
            CArray<T>().swap(*this);
        }
    }
};

template class CArray<CParticleSimulationPtr>;