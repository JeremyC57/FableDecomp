// std::__copy_ptrs<CAttackHistoryCombo*,CAttackHistoryCombo*>(CAttackHistoryCombo* first, CAttackHistoryCombo* last, CAttackHistoryCombo* result, const __false_type&) -- STLport assignment copy of attack combos: assign [first, last) onto result in order, return the advanced result
// Leaf: ??$__copy_ptrs@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004F94D2, 54 bytes, `ret 8` (__fastcall: first in ecx, last in edx, result and
//   the tag reference on the stack -- a /Gr algorithm TU).
// Callers: 0x004F939D and 0x004F93B7, both in vector<CAttackHistoryCombo>::operator= 0x004F932C (the shrink and the
//   overlap-assign branches). Catalog label "VectorReallocateCopyElements" is descriptive.
// Element: CAttackHistoryCombo {vector<pair<EAttackHistoryType, EDamageAttribute> > AttackCombo +0; float
//   Multiplier +0xC} (retail 16 bytes): its assignment calls the pair vector's operator= (0x004F9508) and copies the
//   float through the FPU -- written as a `__forceinline` memberwise operator= (the size TU calls the implicit one
//   out of line).
// Shape: STLport `__copy_ptrs(first, last, result, __false_type) { return __copy(first, last, result,
//   random_access_iterator_tag(), (ptrdiff_t*)0); }` with the counted `__copy` inlined (`for (n = last - first; n > 0;
//   --n) { *result = *first; ++first; ++result; }`, the source walking as `edi + esi`). Size TU (`dec ebx ; jne`).
#pragma optimize("s", on)

enum EAttackHistoryType { EAttackHistoryType_Force32 = 0x7fffffff };
enum EDamageAttribute { EDamageAttribute_Force32 = 0x7fffffff };

namespace std {

struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};

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
    vector<T, A>& operator=(const vector<T, A>& x);      // pair vector: 0x004F9508
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

class CAttackHistoryCombo {                         // Ego_r.pdb (retail 0x10 bytes)
public:
    std::vector<std::pair<EAttackHistoryType, EDamageAttribute> > AttackCombo;   // +0x00
    float Multiplier;                                                           // +0x0C
    __forceinline CAttackHistoryCombo& operator=(const CAttackHistoryCombo& other)
    {
        AttackCombo = other.AttackCombo;
        Multiplier = other.Multiplier;
        return *this;
    }
};

template CAttackHistoryCombo* __fastcall std::__copy_ptrs<CAttackHistoryCombo*, CAttackHistoryCombo*>(CAttackHistoryCombo* first,
    CAttackHistoryCombo* last, CAttackHistoryCombo* result, const std::__false_type&);