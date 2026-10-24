// std::vector<unsigned short,std::allocator<unsigned short> >::_M_insert_overflow(unsigned short* position, const unsigned short& x, const __true_type&, unsigned int fillLength, bool atEnd) -- STLport growth path for a POD vector of 16-bit values: allocate size + max(size, n) slots, memmove the prefix, fill n copies of x, memmove the suffix unless at the end, free the old block
// Leaf: _M_insert_overflow
// Retail: Fable.exe (TLC, VC7.1) @ 0x005135D0, 209 bytes (padded to 224), `ret 14h` (thiscall: position, x, the tag,
//   fillLength, atEnd). 46 call sites (0x00514AF3 in the CWorldMap TU -- its MapPlacement is a vector<unsigned short> --
//   and the wide-character users at 0x009AF9AF .. 0x00BE5451): the one COMDAT every vector<unsigned short> insert and
//   push_back grows through. Catalog label "Vector_InsertN_uint16" is descriptive.
// Callees: malloc 0x00BFEA0E, memmove 0x00BFEAE6 (x2), free 0x00BFEA14.
// Shape: STLport 4.5 `_M_insert_overflow(.., const __true_type& /*_IsPOD*/, ..)`: `__copy_trivial` (`last == first ? result :
//   memmove(result, first, n) + n`) for prefix and suffix, `fill_n` (x re-read per element: it may alias the block), and the
//   inline `_M_clear` (trivial destroy, then the allocator's null-tested free) before `_M_set`. `max` takes const
//   references, so the old size is homed in the spare frame word (which then holds the length) and the new start in
//   the dead fillLength slot. Speed TU (`add esp,4` after each cdecl call).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int n);   // 0x00BFEAE6

namespace std {
struct __true_type { __true_type() { } };

template <class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }

inline void* __copy_trivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result :
        ((char*)memmove(result, first, ((const char*)last - (const char*)first))) +
        ((const char*)last - (const char*)first);
}

template <class OutputIter, class Size, class T>
inline OutputIter fill_n(OutputIter first, Size n, const T& value)
{
    for (; n > 0; --n, ++first)
        *first = value;
    return first;
}

template <class T> class allocator {
public:
    allocator() { }
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { if (p != 0) free(p); }
};

template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
};

template <class T, class A> class _Vector_base {
public:
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<T*, A> _M_end_of_storage;          // +0x08
};

template <class T, class A = allocator<T> > class vector : protected _Vector_base<T, A> {
public:
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
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
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned int fillLength, bool atEnd);
};

template <class T, class A>
void vector<T, A>::_M_insert_overflow(T* position, const T& x, const __true_type&, unsigned int fillLength, bool atEnd)
{
    const unsigned int oldSize = size();
    const unsigned int length = oldSize + (max)(oldSize, fillLength);
    T* newStart = this->_M_end_of_storage.allocate(length);
    T* newFinish = (T*)__copy_trivial(this->_M_start, position, newStart);
    newFinish = fill_n(newFinish, fillLength, x);
    if (!atEnd)
        newFinish = (T*)__copy_trivial(position, this->_M_finish, newFinish);
    _M_clear();
    _M_set(newStart, newFinish, newStart + length);
}

template void vector<unsigned short>::_M_insert_overflow(unsigned short* position, const unsigned short& x,
    const __true_type&, unsigned int fillLength, bool atEnd);
}