// std::vector<unsigned long,std::allocator<unsigned long> >::vector(unsigned int n, const unsigned long& value, const allocator<unsigned long>& a) -- STLport vector(n, value): allocate n 4-byte slots and fill them with value
// Leaf: ??0?$vector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00512630, 91 bytes, `ret 0Ch` (thiscall: n, the value reference, the allocator).
// Callers: ten sites (0x004FFAB8, 0x004FFAE7, 0x005141E4, 0x005F3C33, 0x00AB220C, 0x00F3BAE4, ...): one COMDAT shared by
//   every 4-byte POD element instantiation (ICF), modelled as `unsigned long`. Catalog label "Vector_InitFromValue" is
//   descriptive.
// Callee: malloc @ 0x00BFEA0E (`__cdecl`, result kept: `add esp,4`).
// Shape: STLport `vector(n, value, a) : _Vector_base(n, a) { _M_finish = uninitialized_fill_n(_M_start, n, value); }`
//   with the base's null initialisers stored as immediates (as the landed CSTLportVector<SHandle> 0x00B2EC80); the
//   element is POD, so `__uninitialized_fill_n(.., __true_type)` is `fill_n` -- a plain store loop with no placement
//   null test; the reference itself is loaded once before the loop and the value re-read through it every pass
//   (a dword store through `first` may alias it).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

namespace std {

struct __true_type { __true_type() { } };

template <class OutputIter, class Size, class T>
inline OutputIter fill_n(OutputIter first, Size n, const T& value)
{
    for (; n > 0; --n, ++first)
        *first = value;
    return first;
}

template <class ForwardIter, class Size, class T>
inline ForwardIter __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __true_type&)
{
    return fill_n(first, n, x);
}

template <class ForwardIter, class Size, class T>
inline ForwardIter uninitialized_fill_n(ForwardIter first, Size n, const T& x)
{
    return __uninitialized_fill_n(first, n, x, __true_type());
}

template <class T> class allocator {
public:
    allocator() { }
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};

template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
    _STLP_alloc_proxy(const A& a, V p) : A(a), _M_data(p) { }
};

template <class T, class A> class _Vector_base {
public:
    _Vector_base(unsigned int n, const A& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0)
    {
        _M_start = _M_end_of_storage.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
    }
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<T*, A> _M_end_of_storage;          // +0x08
};

template <class T, class A = allocator<T> > class vector : protected _Vector_base<T, A> {
public:
    vector(unsigned int n, const T& value, const A& a) : _Vector_base<T, A>(n, a)
    {
        this->_M_finish = uninitialized_fill_n(this->_M_start, n, value);
    }
};

template class vector<unsigned long, allocator<unsigned long> >;

}