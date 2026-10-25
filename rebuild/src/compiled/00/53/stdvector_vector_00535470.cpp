// std::vector::vector(const vector& x) -- (vector<unsigned long >) STLport copy constructor for a trivially copyable 4-byte element: allocate x.size() elements through the inline _Vector_base (none for an empty x), then memmove x's elements and set finish past them
// Leaf: ??0?$vector@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00535470, 95 bytes, thiscall, `ret 4`, returns this.
// Identity: vector<unsigned long > -- proven; evidence: the landed map<CCharString, vector<unsigned long> >::operator[] 0x009D4810 and its tree's _M_insert 0x009D40B0 copy the mapped value with it (CBankFile::WriteSymbolFile push_backs unsigned loop indices into that vector); it is also the CGroupArray / CArray<unsigned long> copy of the landed CVertexGroupBlock code.
// Label: the manifest's "Std_Vector_Construct_Copy" is propagated across byte-identical bodies; this is STLport 4.6
//   `vector(const _Self& x)`.
// Callees: malloc 0x00BFEA0E (the inline allocator::allocate, `n ? malloc(n * 4) : 0`), memmove 0x00BFEAE6 (the inline
//   STLport __copy_trivial: `last == first ? result : (char*)memmove(result, first, last - first) + (last - first)`).
// Body: STLport 4.x `vector(const _Self& __x) : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
//   { _M_finish = __uninitialized_copy(__x.begin(), __x.end(), _M_start, _IsPODType()); }` with every step inline.
#include <stddef.h>
extern "C" void* __cdecl malloc(size_t size);                                  // 0x00BFEA0E
extern "C" void* __cdecl memmove(void* dst, const void* src, size_t count);    // 0x00BFEAE6

namespace std {
struct __true_type { __true_type() { } };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class T> class allocator {
public:
    allocator() {}
    allocator(const allocator<T>&) {}
    T* allocate(size_t n, const void* = 0) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};
inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
        : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) + ((const char*)__last - (const char*)__first);
}
template <class _Tp>
inline _Tp* __uninitialized_copy(const _Tp* __first, const _Tp* __last, _Tp* __result, const __true_type&)
{
    return (_Tp*)__copy_trivial(__first, __last, __result);
}
template <class Value, class Tp, class MaybeReboundAlloc>
class _STLP_alloc_proxy : public MaybeReboundAlloc {
public:
    Value _M_data;
    _STLP_alloc_proxy(const MaybeReboundAlloc& a, Value p) : MaybeReboundAlloc(a), _M_data(p) {}
};
template <class T, class Alloc>
class _Vector_base {
public:
    _Vector_base(size_t n, const Alloc& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0)
    {
        _M_start = _M_end_of_storage.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
    }
    T* _M_start;                                           // +0x00
    T* _M_finish;                                          // +0x04
    _STLP_alloc_proxy<T*, T, Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    allocator_type get_allocator() const { return allocator_type(); }
    size_t size() const { return (size_t)(this->_M_finish - this->_M_start); }
    const _Tp* begin() const { return this->_M_start; }
    const _Tp* end() const { return this->_M_finish; }
    vector(const vector<_Tp, _Alloc>& __x) : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy(__x.begin(), __x.end(), this->_M_start, __true_type());
    }
};
typedef unsigned long _Elem;
template vector<_Elem>::vector(const vector<_Elem>& __x);
}