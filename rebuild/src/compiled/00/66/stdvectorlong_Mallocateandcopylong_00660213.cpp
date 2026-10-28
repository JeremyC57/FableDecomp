// std::vector<long>::_M_allocate_and_copy<long*>(unsigned int n, long* first, long* last) -- STLport growth helper: allocate n longs and copy [first, last) into the block (memmove), return the block
// Leaf: ??$_M_allocate_and_copy@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00660213, 45 bytes, thiscall `ret 0Ch` (n, first, last; `this` never read -- the
// allocator is empty). Only caller: vector<long>::reserve @ 0x00660684 (call at 0x006606AF), whose first caller is the
// CTCVillage serialiser at 0x0065CED5 reserving PlayersCrimes.size() longs (crime handles), so the element is `long`
// (any 4-byte POD instance folds onto this body).
// Callees: malloc @ 0x00BFEA0E (allocate, inline, `n != 0` test), __copy_trivial(first, last, result) @ 0x00405BA0
// (`__fastcall`, memmove; its return value is dropped).
// Label correction: the catalog "Vector_AllocateElements" is descriptive; this is STLport 4.x `_M_allocate_and_copy`.
// Shape: `result = allocate(n); __uninitialized_copy(first, last, result, __true_type()); return result;` -- the POD
// `__uninitialized_copy` collapses inline onto `__copy_trivial` (exceptions off: no unwind), result kept in esi.
// Opt: size TU (`optimize("s", on)`).
#pragma optimize("s", on)

extern "C" void* __cdecl malloc(unsigned int size);                                    // 0x00BFEA0E
void* __fastcall __copy_trivial(const void* __first, const void* __last, void* __result);   // 0x00405BA0

namespace std {

struct __true_type { };

template <class T> class allocator {
public:
    allocator() { }
    __forceinline T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};

template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
};

template <class _Tp>
__forceinline _Tp* __uninitialized_copy(const _Tp* __first, const _Tp* __last, _Tp* __result, const __true_type&)
{
    return (_Tp*)__copy_trivial(__first, __last, __result);
}

template <class T, class A> class _Vector_base {
public:
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<T*, A> _M_end_of_storage;          // +0x08
};

template <class T, class A = allocator<T> > class vector : protected _Vector_base<T, A> {
public:
    protected:
    template <class ForwardIter>
    T* _M_allocate_and_copy(unsigned int n, ForwardIter first, ForwardIter last)
    {
        T* result = this->_M_end_of_storage.allocate(n);
        __uninitialized_copy(first, last, result, __true_type());
        return result;
    }
    public:
};

template long* vector<long>::_M_allocate_and_copy<long*>(unsigned int, long*, long*);
}