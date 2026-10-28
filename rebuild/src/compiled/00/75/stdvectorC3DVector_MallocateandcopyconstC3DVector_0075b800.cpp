// std::vector<C3DVector>::_M_allocate_and_copy<const C3DVector*>(unsigned int n, const C3DVector* first, const C3DVector* last) -- STLport growth helper: allocate n vectors and copy-construct [first, last) into the block, return it
// Leaf: ??$_M_allocate_and_copy@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075B800, 88 bytes, thiscall `ret 0Ch` (`this` is never read: the
//   allocator is empty).
// Caller: vector<C3DVector>::operator= 0x0075B8D0 (call site 0x0075B922; the landed CRigidBodyConfig::operator=
//   0x0075BA30 assigns BoundingVertices through it). Callee: malloc 0x00BFEA0E (allocator::allocate).
// Shape: the landed STLport `_M_allocate_and_copy` of 0x006E4950: `result = allocate(n)` (`n != 0 ? malloc(n * 12) :
//   0`), then `__uninitialized_copy(first, last, result, __false_type())` with the placement null test. C3DVector's
//   memberwise user copy constructor (the landed 0x0075BA30 model) gives the re-based destination pointer at
//   result + 8 and the third word read through the fixed source-destination difference (`mov ebx,[esi+ecx]`).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }   // memberwise user copy (no user operator=)
};

namespace std {

struct __false_type { __false_type() { } };

template <class T> class allocator {
public:
    allocator() { }
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};

template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
};

template <class T1, class T2>
inline void _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}

template <class InputIter, class ForwardIter>
inline ForwardIter __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}

template <class T, class A> class _Vector_base {
public:
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    _STLP_alloc_proxy<T*, A> _M_end_of_storage;             // +0x08
};

template <class T, class A = allocator<T> > class vector : protected _Vector_base<T, A> {
public:
    protected:
    template <class ForwardIter>
    T* _M_allocate_and_copy(unsigned int n, ForwardIter first, ForwardIter last)
    {
        T* result = this->_M_end_of_storage.allocate(n);
        __uninitialized_copy(first, last, result, __false_type());
        return result;
    }
    public:
};

}

template C3DVector* std::vector<C3DVector>::_M_allocate_and_copy<const C3DVector*>(
    unsigned int n, const C3DVector* first, const C3DVector* last);