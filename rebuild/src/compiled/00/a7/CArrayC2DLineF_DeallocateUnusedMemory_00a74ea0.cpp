// CArray<C2DLineF>::DeallocateUnusedMemory() -- shrink a C2DLineF array to fit: when it has spare capacity, copy the lines into an exactly reserved temporary, release the old storage and swap the temporary in (its destructor frees what it got)
// Leaf: ?DeallocateUnusedMemory@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A74EA0, 173 bytes, thiscall `ret`.
// Caller: 0x00A7416C (the CPhysicsMesh navigation-line TU). PDB name set: CArray<T> has DeallocateMemory and
//   DeallocateUnusedMemory (Ego_r.pdb); the catalog's "Vector_ShrinkToFit_C2DLineF" is descriptive.
// Callees: vector<C2DLineF>::reserve 0x00A74940 (this chunk, on the temporary), vector<C2DLineF>::_M_range_insert(pos,
//   first, last, const forward_iterator_tag&) 0x00A74AC0, vector<C2DLineF>::erase(first, last) 0x00A748E0 (inside the
//   inline DeallocateMemory, landed 0x00A74C60), free 0x00BFEA14.
// Shape: `if (capacity() > size()) { CArray<T> temp; temp.reserve(size()); temp.insert(temp.begin(), begin(), end());
//   DeallocateMemory(); swap(temp); }` -- capacity/size from one start load (unsigned `jbe`), the temporary's inline
//   ctor zeroes its three words after the size push, the empty forward_iterator_tag at frame byte 3, DeallocateMemory
//   inline (erase + detach + free), then STLport vector::swap and the temporary's ~_Vector_base (deallocate = `if (p)
//   free(p)`) folded into "load the old start, take the temporary's three words, free the old start".
//   That folding (no stores back into the temporary, the old start loaded FIRST) needs the three callees as same-TU
//   noinline bodies (reserve = this chunk's body without its out-of-line helpers; the range insert and erase are
//   minimal stand-ins that never touch the tag): declaration-only, the temporary counts as captured and the hand-over
//   spelling schedules the start loads the other way (173 v 173), the swap spelling keeps dead stores.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

class C2DVector {
public:
    float X;
    float Y;
};
class C2DLineF {                                            // 16 bytes: two end points
public:
    C2DVector Start;                                        // +0x00
    C2DVector End;                                          // +0x08
};

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
    void reserve(size_type __n);                                                                    // 0x00A74940
    void _M_range_insert(iterator __pos, const_iterator __first, const_iterator __last, const forward_iterator_tag&);   // 0x00A74AC0
    void insert(iterator __pos, const_iterator __first, const_iterator __last)
    {
        _M_range_insert(__pos, __first, __last, forward_iterator_tag());
    }
    iterator erase(iterator __first, iterator __last);                                              // 0x00A748E0
    void swap(vector<_Tp, _Alloc>& __x)
    {
        std::swap(this->_M_start, __x._M_start);
        std::swap(this->_M_finish, __x._M_finish);
        std::swap(this->_M_end_of_storage._M_data, __x._M_end_of_storage._M_data);
    }
};

#pragma auto_inline(off)
// 0x00A74940 (this chunk): reserve -- same-TU noinline body.
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
// 0x00A74AC0: the forward range insert -- same-TU noinline stand-in (append into reserved room; never touches the tag).
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_range_insert(iterator __pos, const_iterator __first, const_iterator __last, const forward_iterator_tag&)
{
    for (; __first != __last; ++__first, ++this->_M_finish)
        new (this->_M_finish) _Tp(*__first);
}
// 0x00A748E0: erase(first, last) -- same-TU noinline body (POD lines: copy the tail down).
template <class _Tp, class _Alloc>
__declspec(noinline) _Tp* vector<_Tp, _Alloc>::erase(iterator __first, iterator __last)
{
    pointer __i = __first;
    for (pointer __s = __last; __s != this->_M_finish; ++__s, ++__i)
        *__i = *__s;
    this->_M_finish = __i;
    return __first;
}
#pragma auto_inline(on)
}

template <class T> class CArray : public std::vector<T> {
public:
    void DeallocateMemory()
    {
        if (this->capacity() > 0) {
            this->erase(this->begin(), this->end());
            T* p = this->_M_start;
            this->_M_start = 0;
            this->_M_finish = 0;
            this->_M_end_of_storage._M_data = 0;
            if (p)
                free(p);
        }
    }
    void DeallocateUnusedMemory();
};

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

template class CArray<C2DLineF>;