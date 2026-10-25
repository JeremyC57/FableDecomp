// std::vector::operator=(const vector<C3DVector>& x) -- assign a vector of 3D vectors (rigid-body bounding vertices and many others)
// Leaf: ??4?$vector@VC3DVector@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0075B8D0, 274 bytes, thiscall `ret 4`, returns this.
// Label correction: catalog "Std_Vector_Assign" -> STLport 4.x vector<C3DVector>::operator= (the landed
//   _M_allocate_and_copy 0x0075B800 names this caller; 38 call sites, e.g. CRigidBodyConfig::operator= 0x0075BA30).
// Element: C3DVector (12 bytes) with a memberwise user copy constructor and trivial destructor (no _Destroy calls;
//   _M_clear is only the free).
// Callees: _M_allocate_and_copy 0x0075B800, free 0x00BFEA14, __copy 0x0075B560, __uninitialized_copy 0x006EFDB0.
// Shape: STLport 4.6 `operator=` verbatim (see 0x00709660): grow (allocate-and-copy, inline _M_clear, new start /
//   end of storage), shrink (__copy_ptrs -> __copy with a random_access_iterator_tag and a null distance pointer) or
//   extend (__copy over size() elements, __uninitialized_copy of the rest), `_M_finish = _M_start + xlen` in each arm.
//   x._M_finish is cached in x's dead argument slot and the empty tags share it: the tag-taking callees are same-TU
//   noinline stand-ins.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);           // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) {}   // memberwise user copy (no user operator=)
};

namespace std {

struct __false_type { __false_type() {} };
struct random_access_iterator_tag { random_access_iterator_tag() {} };

template<class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template<class T> class allocator {
public:
    allocator() {}
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { if (p) free(p); }
};

#pragma auto_inline(off)
// Same-TU stand-ins (real bodies: 0x0075B560, 0x006EFDB0).
template <class _RandomAccessIter, class _OutputIter, class _Distance>
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                                                   const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class T> __declspec(noinline) T* __fastcall __uninitialized_copy(const T* first, const T* last, T* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        _Construct(result, *first);
    return result;
}
#pragma auto_inline(on)

template <class _Tp>
inline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template<class T> class vector {
public:
    T* _M_start;                                // +0x00
    T* _M_finish;                               // +0x04
    T* _M_end_of_storage;                       // +0x08
    allocator<T> _M_alloc() const { return allocator<T>(); }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start); }
#pragma auto_inline(off)
    // 0x0075B800 (stand-in)
    template<class ForwardIter>
    __declspec(noinline) T* _M_allocate_and_copy(unsigned int n, ForwardIter first, ForwardIter last)
    {
        T* result = _M_alloc().allocate(n);
        __uninitialized_copy(first, last, result, __false_type());
        return result;
    }
#pragma auto_inline(on)
    void _M_clear()
    {
        _M_alloc().deallocate(_M_start, capacity());
    }
    vector& operator=(const vector& x);
};

template<class T>
vector<T>& vector<T>::operator=(const vector<T>& __x)
{
    if (&__x != this) {
        const unsigned int __xlen = __x.size();
        if (__xlen > capacity()) {
            T* __tmp = _M_allocate_and_copy(__xlen, (const T*)__x._M_start + 0, (const T*)__x._M_finish + 0);
            _M_clear();
            this->_M_start = __tmp;
            this->_M_end_of_storage = this->_M_start + __xlen;
        }
        else if (size() >= __xlen) {
            __copy_ptrs((const T*)__x._M_start + 0, (const T*)__x._M_finish + 0, (T*)this->_M_start, __false_type());
        }
        else {
            __copy_ptrs((const T*)__x._M_start, (const T*)__x._M_start + size(), (T*)this->_M_start, __false_type());
            __uninitialized_copy((const T*)__x._M_start + size(), (const T*)__x._M_finish + 0, this->_M_finish, __false_type());
        }
        this->_M_finish = this->_M_start + __xlen;
    }
    return *this;
}

template class vector<C3DVector>;

}