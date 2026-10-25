// std::vector<unsigned char>::reserve(unsigned int n) -- STLport reserve for a byte vector: when the capacity is below n, move the bytes into a fresh n-byte block (malloc + memmove, then free the old block) or just allocate when empty
// Leaf: ?reserve@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B4E20, 140 bytes, thiscall `ret 4`. Catalog name "Vector_Reserve_Reallocate_Byte"
// (descriptive). Caller 0x009B5DE3.
// Callees: malloc @ 0x00BFEA0E (allocator::allocate), memmove @ 0x00BFEAE6 (__copy_trivial), free @ 0x00BFEA14 (the
// inlined _M_clear).
// Shape: STLport 4.x `reserve` verbatim with `_M_allocate_and_copy` inlined: a POD element takes
// __uninitialized_copy(..., __true_type) = __copy_trivial (`last == first ? result : memmove(result, first, last - first)`);
// old_size is spilled to the `push ecx` slot across the calls.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6

namespace std {

struct __true_type { __true_type() { } };

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
        : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) + ((const char*)__last - (const char*)__first);
}

template <class InputIter, class ForwardIter>
inline ForwardIter __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __true_type&)
{
    return (ForwardIter)__copy_trivial(first, last, result);
}

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
    T* _M_allocate_and_copy(unsigned int n, ForwardIter first, ForwardIter last)
    {
        T* result = this->_M_end_of_storage.allocate(n);
        __uninitialized_copy(first, last, result, __true_type());
        return result;
    }
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

template void std::vector<unsigned char>::reserve(unsigned int n);