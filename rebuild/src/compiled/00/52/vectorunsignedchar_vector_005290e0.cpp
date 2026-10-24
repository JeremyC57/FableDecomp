// vector<unsigned char>::vector(const vector& x) -- STLport copy constructor of a byte vector: allocate exactly x.size() bytes and memmove the contents across
// Leaf: ??0?$vector@EV?$allocator@E
//
// Retail 0x005290E0, 86 bytes, thiscall `ret 4`.  Callees: 0x00BFEA0E malloc, 0x00BFEAE6 memmove.
// Callers: seven sites in unrelated owners (0x0052A2D0, 0x0052B300, 0x0052B4E0, 0x0052C150, 0x0052C300, 0x009C6EE0,
//   0x009C7060) -- one body for every 1-byte POD vector the linker folded (char / unsigned char / bool); modelled as
//   vector<unsigned char>.
// Shape: STLport `vector(const _Self& x) : _Vector_base(x.size(), x.get_allocator()) { _M_finish =
//   __uninitialized_copy(x._M_start, x._M_finish, _M_start, __true_type()); }` -- _Vector_base zeroes the three words,
//   then `allocate(n)` = `n ? malloc(n) : 0` (`mov eax,0` keeps the flags of `sub edi,eax` for the `je`), finish = start,
//   eos = start + n; the POD copy is __copy_trivial (`last == first ? result : memmove(result, first, n) + n`).
// Label: catalog "Vector_CopyAssignFrom" (propagated); this is the copy constructor, not operator=.
extern "C" void* __cdecl malloc(unsigned int size);
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);

namespace std {
struct __true_type { __true_type() {} };

inline void* __copy_trivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
                           : ((char*)memmove(result, first, ((const char*)last - (const char*)first)))
                               + ((const char*)last - (const char*)first);
}
template <class _Tp> inline _Tp* __uninitialized_copy(const _Tp* first, const _Tp* last, _Tp* result, const __true_type&)
{
    return (_Tp*)__copy_trivial(first, last, result);
}

template <class T> class allocator {
public:
    allocator() {}
    allocator(const allocator<T>&) {}
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};
template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
    _STLP_alloc_proxy(const allocator<T>& a, T* p) : allocator<T>(a), _M_data(p) {}
};
template <class T, class A> class _Vector_base {
public:
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T> _M_end_of_storage;
    _Vector_base(unsigned int n, const A& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0)
    {
        _M_start = _M_end_of_storage.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
    }
};
template <class T, class A = allocator<T> > class vector : public _Vector_base<T, A> {
public:
    unsigned int size() const { return unsigned int(this->_M_finish - this->_M_start); }
    A get_allocator() const { return A(); }
    vector(const vector<T, A>& x) : _Vector_base<T, A>(x.size(), x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const T*)x._M_start, (const T*)x._M_finish, this->_M_start, __true_type());
    }
};
}

template std::vector<unsigned char>::vector(const std::vector<unsigned char>&);