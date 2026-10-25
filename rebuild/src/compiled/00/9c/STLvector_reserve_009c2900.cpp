// std::vector::reserve(unsigned int n) -- vector<C2DLineF>::reserve: when the capacity is below n, copy the lines into a fresh n-element block (malloc, uninitialized copy, free the old block) or just allocate when empty
// Leaf: ?reserve@?$vector@VC2DLineF@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C2900, 171 bytes (+ int3 padding), thiscall `ret 4`.
// Identity: STLport 4.x `vector<_Tp>::reserve` over a 16-byte element; its uninitialized-copy callee 0x0040DDC0 carries
//   the donor name `_Copy_impl<C2DLineF*,C2DLineF*>` (the element-wise placement copy with the null test = STLport
//   `__uninitialized_copy(first, last, result, const __false_type&)`, landed as a descriptive SElement16 copy). One
//   caller, 0x009C0408. The catalog label "vector_reserve_C2DLineF" is right.
// Callees: malloc @ 0x00BFEA0E (allocator::allocate, null for n == 0), __uninitialized_copy @ 0x0040DDC0 (`__fastcall`,
//   first/last in ecx/edx, the result and the __false_type tag on the stack), free @ 0x00BFEA14 (the inlined _M_clear).
// Shape: STLport `reserve` verbatim with `_M_allocate_and_copy` inline; C2DLineF has user constructors, so STLport's
//   type traits make it non-POD (`_IsPODType` = __false_type) and the copy is the out-of-line element loop. The new block
//   lives in n's dead argument slot, which also serves as the tag temporary's address; the old finish pointer is spilled
//   to the `push ecx` word. That frame assignment needs the same-TU stand-in body of the out-of-line copy (it never
//   reads its tag); declared only, VC7.1 swaps the two slots.
#include <new>
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

class C2DVector {                                           // 8 bytes
public:
    float X;                                                // +0x00
    float Y;                                                // +0x04
};
class C2DLineF {                                            // Ego_r.pdb, 16 bytes
public:
    C2DVector Point1;                                       // +0x00
    C2DVector Point2;                                       // +0x08
    C2DLineF(const C2DVector& point1, const C2DVector& point2) : Point1(point1), Point2(point2) { }
};

namespace std {
struct __false_type { __false_type() { } };

// Same-TU stand-in body of the out-of-line element copy 0x0040DDC0 (it never reads the tag).
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                                                  const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        new (&*__cur) C2DLineF(*__first);
    return __cur;
}
#pragma auto_inline(on)

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
    typedef __false_type _IsPODType;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    template <class ForwardIter>
    T* _M_allocate_and_copy(unsigned int n, ForwardIter first, ForwardIter last)
    {
        T* result = this->_M_end_of_storage.allocate(n);
        __uninitialized_copy(first, last, result, _IsPODType());
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

template void std::vector<C2DLineF>::reserve(unsigned int n);