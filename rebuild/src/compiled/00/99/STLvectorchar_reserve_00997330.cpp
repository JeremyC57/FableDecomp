// std::vector<char>::reserve(unsigned int n) -- grow a byte vector's capacity to at least n: allocate n bytes, move the current bytes over (when any), free the old block, keep the size
// Leaf: ?reserve@?$vector@D
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00997330, length 140 bytes (+ int3 padding), `ret 4`.
// Callees: malloc @ 0x00BFEA0E, memmove @ 0x00BFEAE6, free @ 0x00BFEA14 (the STLport allocator).
// Caller: CMemoryDataOutputStream's reserve forwarder @ 0x00996BC0 (`add ecx,8 ; jmp` -- Data, the CArray<char> at +0x08;
// the landed 0x00996BC0 source names it a _Cons_val forwarder), which eight game sites call.
// Label: the manifest's "GrowableBuffer_Resize" is descriptive.
// Shape: STLport 4.6 vector::reserve -- `if (capacity() < n) { old = size(); tmp = start ? _M_allocate_and_copy(n, start,
// finish) [+ _M_clear()] : allocate(n); _M_set(tmp, tmp + old, tmp + n); }`; allocate is `n ? malloc(n) : 0`, the POD
// copy is __copy_trivial (`last == first ? result : memmove(...)`), deallocate frees a non-null block; both arms share the
// _M_set tail (the null-start arm jumps into it with the old size still in ebp).
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);  // 0x00BFEA0E
extern "C" void __cdecl free(void* p);          // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dest, const void* source, size_t count);   // 0x00BFEAE6

namespace std {

inline void* __copy_trivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : ((char*)memmove(result, first, ((const char*)last - (const char*)first))) + ((const char*)last - (const char*)first);
}

template <class T> class allocator {
public:
    allocator() { }
    T* allocate(size_t n) { return n != 0 ? static_cast<T*>(malloc(n * sizeof(T))) : 0; }
    void deallocate(T* p, size_t) { if (p != 0) free(p); }
};

template <class Value, class T, class Alloc> class _STLP_alloc_proxy : public Alloc {
public:
    Value _M_data;
};

template <class T, class Alloc> class _Vector_base {
public:
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, T, Alloc> _M_end_of_storage;
};

template <class T, class Alloc = allocator<T> > class vector : public _Vector_base<T, Alloc> {
public:
    size_t size() const { return size_t(this->_M_finish - this->_M_start); }
    size_t capacity() const { return size_t(this->_M_end_of_storage._M_data - this->_M_start); }
    T* _M_allocate_and_copy(size_t n, const T* first, const T* last)
    {
        T* result = this->_M_end_of_storage.allocate(n);
        __copy_trivial(first, last, result);
        return result;
    }
    void _M_clear()
    {
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(T* s, T* f, T* e)
    {
        this->_M_start = s;
        this->_M_finish = f;
        this->_M_end_of_storage._M_data = e;
    }
    void reserve(size_t n);
};

template <class T, class Alloc> void vector<T, Alloc>::reserve(size_t n)
{
    if (capacity() < n) {
        const size_t oldSize = size();
        T* tmp;
        if (this->_M_start) {
            tmp = _M_allocate_and_copy(n, this->_M_start, this->_M_finish);
            _M_clear();
        }
        else
            tmp = this->_M_end_of_storage.allocate(n);
        _M_set(tmp, tmp + oldSize, tmp + n);
    }
}

}   // namespace std

template void std::vector<char>::reserve(size_t);