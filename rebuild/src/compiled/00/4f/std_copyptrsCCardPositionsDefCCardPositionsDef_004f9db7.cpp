// std::__copy_ptrs<CCardPositionsDef*,CCardPositionsDef*>(CCardPositionsDef* first, CCardPositionsDef* last, CCardPositionsDef* result, const __false_type&) -- STLport assignment copy of card-position defs: assign [first, last) onto result in order, return the advanced result
// Leaf: ??$__copy_ptrs@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004F9DB7, 65 bytes, `ret 8` (__fastcall: first in ecx, last in edx, result and the
//   tag reference on the stack -- a /Gr algorithm TU).
// Caller: 0x004F9D96, vector<CCardPositionsDef>::erase(first, last) (copy down, then __destroy_aux of the tail).
//   Catalog label "CActiveFile_CopyAssign_Loop" is propagated.
// Element: CCardPositionsDef (retail 0x48: CParentDefClassBase 0x39 bytes, vector<C2DVector> Offset +0x3C); its
//   assignment runs CParentDefClassBase::operator= @ 0x00431ED0 then the Offset vector operator= @ 0x0046EABD (manifest
//   "CActiveFile_Copy" is propagated), written as a `__forceinline` memberwise operator= (the size TU calls the implicit
//   one out of line). The engine header CParentDefClassBase.h carries the Ego_r donor size 0x3D, so a local model stands in.
// Shape: STLport `__copy_ptrs(first, last, result, __false_type) { return __copy(first, last, result,
//   random_access_iterator_tag(), (ptrdiff_t*)0); }` with the counted `__copy` inlined (the source walking as
//   `edi + esi`); 0x48 is not a power of two, so the count is an `idiv`. Size TU (`dec ebx ; jne`).
#pragma optimize("s", on)

class C2DVector {
public:
    float X;
    float Y;
};

namespace std {

struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class T> class allocator {
public:
    allocator() { }
};

template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
};

template <class T, class A> class _Vector_base {
public:
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<T*, A> _M_end_of_storage;          // +0x08
};

template <class T, class A = allocator<T> > class vector : protected _Vector_base<T, A> {
public:
    vector<T, A>& operator=(const vector<T, A>& x);      // vector<C2DVector>: 0x0046EABD
};

template <class RandomAccessIter, class OutputIter, class Distance>
__forceinline OutputIter __copy(RandomAccessIter first, RandomAccessIter last, OutputIter result,
                                const random_access_iterator_tag&, Distance*)
{
    for (Distance n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

template <class InputIter, class OutputIter>
OutputIter __fastcall __copy_ptrs(InputIter first, InputIter last, OutputIter result, const __false_type&)
{
    return __copy(first, last, result, random_access_iterator_tag(), (int*)0);
}

}

class CParentDefClassBase {                         // local model, retail 0x39 bytes (the engine header carries the donor 0x3D)
public:
    unsigned char _body[0x39];
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0
};

class CCardPositionsDef : public CParentDefClassBase {   // Ego_r.pdb, retail sizeof 0x48
public:
    std::vector<C2DVector> Offset;                  // +0x3C
    __forceinline CCardPositionsDef& operator=(const CCardPositionsDef& other)
    {
        CParentDefClassBase::operator=(other);
        Offset = other.Offset;
        return *this;
    }
};

template CCardPositionsDef* __fastcall std::__copy_ptrs<CCardPositionsDef*, CCardPositionsDef*>(CCardPositionsDef* first,
    CCardPositionsDef* last, CCardPositionsDef* result, const std::__false_type&);