// _STL::vector<SRecord12>::reserve(unsigned int n) -- STLport reserve for 12-byte records: when the capacity is below n, move the elements into a fresh n-element block (allocate-and-copy, then free the old block) or just allocate when empty
// Leaf: ?reserve@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B4D80, 147 bytes, thiscall `ret 4`. Catalog name "Deque_Reserve" is wrong (STLport
// vector::reserve). Callers 0x009AEDE2, 0x009B79FA.
// Callees: _M_allocate_and_copy<const SRecord12*> @ 0x009B2780 (this chunk), free @ 0x00BFEA14 (the inlined _M_clear:
// trivial destroy + deallocate), malloc @ 0x00BFEA0E (allocator::allocate).
// Shape: STLport 4.x `reserve` verbatim: capacity() and size() as pointer differences divided by 12 (the 0x2AAAAAAB
// reciprocal), the unsigned `capacity() < n` test, `_M_clear()` inlined, `_M_set(tmp, tmp + old_size, tmp + n)`.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

namespace _STL {

template <class T> class allocator {
public:
    allocator() { }
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { if (p) free(p); }
};

template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
};

template <class T> inline void _Destroy(T*, T*) { }

template <class T, class A> class _Vector_base {
public:
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    _STLP_alloc_proxy<T*, A> _M_end_of_storage;             // +0x08
};

template <class T, class A = allocator<T> > class vector : protected _Vector_base<T, A> {
public:
    typedef unsigned int size_type;
    typedef T* pointer;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    template <class ForwardIter>
    T* _M_allocate_and_copy(unsigned int n, ForwardIter first, ForwardIter last);   // 0x009B2780
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer s, pointer f, pointer e)
    {
        this->_M_start = s;
        this->_M_finish = f;
        this->_M_end_of_storage._M_data = e;
    }
    void reserve(size_type n);
};

template <class T, class A>
void vector<T, A>::reserve(size_type n)
{
    if (capacity() < n) {
        const size_type old_size = size();
        pointer tmp;
        if (this->_M_start) {
            tmp = _M_allocate_and_copy(n, (const T*)this->_M_start, (const T*)this->_M_finish);
            _M_clear();
        } else {
            tmp = this->_M_end_of_storage.allocate(n);
        }
        _M_set(tmp, tmp + old_size, tmp + n);
    }
}

}

struct SRecord12 {                                          // 12-byte element (three words; type not recoverable)
    long Words[3];
};

template void _STL::vector<SRecord12>::reserve(unsigned int n);