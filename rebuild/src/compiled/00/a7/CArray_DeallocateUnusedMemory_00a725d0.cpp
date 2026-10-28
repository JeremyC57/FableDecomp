// CArray<C3DVector>::DeallocateUnusedMemory() -- shrink the array to fit: when it has spare capacity, copy the elements into an exactly reserved temporary, release the old storage and adopt the temporary's
// Leaf: ?DeallocateUnusedMemory@?$CArray@VC3DVector@@@@QAEXXZ
// Grade: RELOCATION_MATCH (agent902) -- the agent422 recipe (0x00A74EA0): STLport vector base + `CArray<T> temp;
//   temp.reserve(size()); temp.insert(temp.begin(), begin(), end()); DeallocateMemory(); swap(temp);` with same-TU noinline
//   bodies for reserve, the forward _M_range_insert (tag never touched) and the out-of-line DeallocateMemory: VC7.1 then
//   treats the temporary as uncaptured and folds swap + ~_Vector_base into "load the old start, take the temporary's three
//   words, free the old start" (retail's order; the Exchange hand-over loaded the temporary's start first).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A725D0, length 153 bytes (+ int3 padding); called from CPhysicsMesh::GetMeshDataFromFileData (0x00A6CFEB); DeallocateMemory 0x00A71F40 is the
// landed CArray<C3DVector> body (`imul 2AAAAAABh ; sar 1` = 12-byte stride).
// Identity: CArray<C3DVector> (FableWin.pdb names CArray<C3DVector>::DeallocateUnusedMemory): the caller passes
//   CPhysicsMesh::TotalPoints (`add ecx,3Ch`; retail layout: 16-byte header, two 0x10 CVectorMaps, TotalFaceIndices 0xC).
// Callees: vector<C3DVector>::reserve 0x00920FF0 (on the temporary), vector<C3DVector>::_M_range_insert<C3DVector*>(pos, first, last, forward_iterator_tag)
// 0x00A70CB0, CArray::DeallocateMemory 0x00A71F40, free 0x00BFEA14.
class C3DVector { public: float X, Y, Z; };          // CPhysicsMesh::TotalPoints element (12 bytes)
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

namespace std {
struct forward_iterator_tag { forward_iterator_tag() { } };
template <class _Tp> inline void swap(_Tp& __a, _Tp& __b) { _Tp __tmp = __a; __a = __b; __b = __tmp; }

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    void reserve(size_type __n);
    template <class _ForwardIterator>
    void _M_range_insert(iterator __pos, _ForwardIterator __first, _ForwardIterator __last, const forward_iterator_tag&);
    template <class _InputIterator>
    void insert(iterator __pos, _InputIterator __first, _InputIterator __last)
    {
        _M_range_insert(__pos, __first, __last, forward_iterator_tag());
    }
    void swap(vector<_Tp, _Alloc>& __x)
    {
        std::swap(this->_M_start, __x._M_start);
        std::swap(this->_M_finish, __x._M_finish);
        std::swap(this->_M_end_of_storage._M_data, __x._M_end_of_storage._M_data);
    }
};

#pragma auto_inline(off)
// vector::reserve -- same-TU noinline body (STLport: allocate, copy, free the old block, adopt).
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::reserve(size_type __n)
{
    if (capacity() < __n) {
        const size_type __old_size = size();
        pointer __tmp = this->_M_end_of_storage.allocate(__n);
        for (pointer __s = this->_M_start, __d = __tmp; __s != this->_M_finish; ++__s, ++__d)
            new (__d) _Tp(*__s);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
        this->_M_start = __tmp;
        this->_M_finish = __tmp + __old_size;
        this->_M_end_of_storage._M_data = __tmp + __n;
    }
}
// vector::_M_range_insert(pos, first, last, forward_iterator_tag) -- same-TU noinline stand-in (append into the reserved
// room; never touches the tag).
template <class _Tp, class _Alloc> template <class _ForwardIterator>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_range_insert(iterator __pos, _ForwardIterator __first, _ForwardIterator __last, const forward_iterator_tag&)
{
    for (; __first != __last; ++__first, ++this->_M_finish)
        new (this->_M_finish) _Tp(*__first);
}
#pragma auto_inline(on)
}

template <class T> class CArray : public std::vector<T> {
public:
    void DeallocateMemory();
    void DeallocateUnusedMemory();
};

#pragma auto_inline(off)
// CArray::DeallocateMemory -- out of line in retail; same-TU noinline body (empty the array and free its block).
template <class T> __declspec(noinline) void CArray<T>::DeallocateMemory()
{
    if (this->capacity() > 0) {
        this->_M_finish = this->_M_start;
        T* p = this->_M_start;
        this->_M_start = 0;
        this->_M_finish = 0;
        this->_M_end_of_storage._M_data = 0;
        if (p)
            free(p);
    }
}
#pragma auto_inline(on)

template <class T> void CArray<T>::DeallocateUnusedMemory()
{
    if (this->capacity() > this->size()) {
        CArray<T> temp;
        temp.reserve(this->size());
        temp.insert(temp.begin(), this->begin(), this->end());
        DeallocateMemory();
        this->swap(temp);
    }
}
template class CArray<C3DVector>;