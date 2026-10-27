// std::vector<float>::_M_allocate_and_copy(unsigned int n, const T* first, const T* last) -- STLport growth helper: allocate n elements and copy-construct [first, last) into the block, return it
// Leaf: ?_M_allocate_and_copy@?$vector@M
// Retail: Fable.exe (TLC, VC7.1) @ 0x0045F008, 50 bytes, thiscall `ret 0Ch` (`this` unused: the allocator is empty), frame pointer.
// Identity (proven): its callee is the landed __uninitialized_copy<const float*, float*> 0x0045EFE5 (which names this caller); called from vector<float>::operator= 0x0045EF2D.
// Callees: malloc 0x00BFEA0E (allocator::allocate), __uninitialized_copy(first, last, result, const __true_type&) 0x0045EFE5
//   (__fastcall; the empty tag temporary homed in the dead n slot, `lea eax,[ebp+0Bh]`).
// Shape (family K095, representative 0x0045F008): the landed STLport `_M_allocate_and_copy` (0x00AD04E0) in a frame/size TU
//   (`optimize("y", off)` + `("s", on)`) with the copy out of line.
#pragma optimize("y", off)
#pragma optimize("s", on)
extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E
namespace std {
struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class T> class allocator {
public:
    allocator() { }
    __forceinline T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }   // size TU: forced inline
};
template <class V, class T, class A> class _STLP_alloc_proxy : public A {
public:
    V _M_data;
};
template <class InputIter, class ForwardIter>
ForwardIter __fastcall __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __true_type&);   // 0x0045EFE5
template <class T, class A> class _Vector_base {
public:
    T* _M_start;                                          // +0x00
    T* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<T*, T, A> _M_end_of_storage;        // +0x08
};
template <class T, class A = allocator<T> > class vector : protected _Vector_base<T, A> {
public:
    T* _M_allocate_and_copy(unsigned int n, const T* first, const T* last)
    {
        T* result = this->_M_end_of_storage.allocate(n);
        __uninitialized_copy(first, last, result, __true_type());
        return result;
    }
};
template float* vector<float >::_M_allocate_and_copy(unsigned int, const float*, const float*);
}   // namespace std