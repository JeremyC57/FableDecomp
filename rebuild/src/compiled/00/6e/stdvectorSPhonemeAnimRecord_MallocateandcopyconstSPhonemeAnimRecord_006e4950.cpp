// std::vector<SPhonemeAnimRecord>::_M_allocate_and_copy<const SPhonemeAnimRecord*>(unsigned int n, const SPhonemeAnimRecord* first, const SPhonemeAnimRecord* last) -- STLport growth helper: allocate n 44-byte records and copy-construct [first, last) into the block, return it
// Leaf: ??$_M_allocate_and_copy@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E4950, 78 bytes (+ int3 padding), thiscall `ret 0Ch` (`this` is never read: the
//   allocator is empty).
// Caller: vector<SPhonemeAnimRecord>::reserve(n) @ 0x006E4F30 (call site 0x006E4F77, the non-empty branch), itself called
//   by the CTCTalk phoneme-animation update 0x006E3B90 on a local vector reserved to the lip-sync phoneme count. The
//   catalog's "Array_CopyElements_0x2c" is descriptive; the element's class name is not recoverable (a 0x2C-byte record
//   with an implicit copy constructor), so it is named by role.
// Callees: malloc @ 0x00BFEA0E (allocator::allocate).
// Shape: STLport `_M_allocate_and_copy(n, first, last) { pointer result = _M_end_of_storage.allocate(n);
//   __uninitialized_copy(first, last, result, __false_type()); return result; }` (exceptions off: no unwind); the
//   destination walks as `ebx + edx` (the source pointer plus a fixed difference) with the placement null test; the
//   implicit copy of the plain 44-byte record is `mov ecx,0Bh ; rep movsd`. A speed TU (aligned loop head).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }

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
    template <class ForwardIter>
    T* _M_allocate_and_copy(unsigned int n, ForwardIter first, ForwardIter last)
    {
        T* result = this->_M_end_of_storage.allocate(n);
        __uninitialized_copy(first, last, result, __false_type());
        return result;
    }
};

}

struct SPhonemeAnimRecord {                                 // 0x2C bytes, plain record
    long Words[11];
};

template SPhonemeAnimRecord* std::vector<SPhonemeAnimRecord>::_M_allocate_and_copy<const SPhonemeAnimRecord*>(
    unsigned int n, const SPhonemeAnimRecord* first, const SPhonemeAnimRecord* last);