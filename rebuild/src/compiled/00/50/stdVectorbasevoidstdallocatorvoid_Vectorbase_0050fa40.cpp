// std::_Vector_base<void*,std::allocator<void*> >::_Vector_base(unsigned int n, const allocator<void*>& a) -- STLport vector base: null the three pointers, allocate room for n pointers, finish = start, end of storage = start + n
// Leaf: ??0?$_Vector_base@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0050FA40, 56 bytes, `ret 8` (thiscall: n and the allocator reference).
// Caller: 0x00D38F3E, the `vector<T*>(size_type n)` constructor at 0x00D38F30 (it zero-fills the block with `rep stosd`
//   and sets _M_finish); the base constructor is one COMDAT shared by every 4-byte-element instantiation (ICF), so the
//   element is modelled as `void*`. Catalog label "std_vector_allocate_ptrs" is descriptive.
// Callee: malloc @ 0x00BFEA0E (`__cdecl`, result kept: `add esp,4`).
// Shape: STLport `_Vector_base(n, a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0)` with the end-of-storage
//   pointer in an `_STLP_alloc_proxy` over the empty allocator; `allocate(n)` = `n != 0 ? malloc(n * 4) : 0` reuses the
//   shared zero register for the null block. A speed TU (`add esp,4`, one `xor eax,eax` zero register).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

namespace std {

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

template class _Vector_base<void*, allocator<void*> >;

}