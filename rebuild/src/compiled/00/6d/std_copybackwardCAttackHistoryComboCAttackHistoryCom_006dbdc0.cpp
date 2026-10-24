// std::__copy_backward<CAttackHistoryCombo*,CAttackHistoryCombo*,int>(CAttackHistoryCombo* first, CAttackHistoryCombo* last, CAttackHistoryCombo* result, const random_access_iterator_tag&, int*) -- SGI/STLport backward assignment copy of attack-history combos, returns the moved-down result
// Leaf: ??$__copy_backward@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DBDC0, length 61 bytes (+ int3 padding), `__fastcall` (first in ecx,
// last in edx; result, the tag reference and the null Distance* on the stack = `ret 0Ch`, a /Gr algorithm TU).
// Callers: 0x006DCD37 inside vector<CAttackHistoryCombo>::_M_fill_insert 0x006DCCC0 (its value copy goes through the
// landed pair-vector copy ctor 0x004F9408) and 0x006DC65C (0x006DC550). The manifest label
// "_Copy_backward<CLipSyncPhoneme*,CLipSyncPhoneme*>" is propagated: CLipSyncPhoneme is 12 bytes, the element here is
// 16 bytes whose assignment calls vector<pair<EAttackHistoryType, EDamageAttribute>>::operator= @ 0x004F9508 (as in
// the landed CAttackHistoryCombo __copy_ptrs 0x004F94D2) and then copies the dword at +0xC.
// Element: CAttackHistoryCombo {AttackCombo vector +0 (retail 12-byte STLport vector), float Multiplier +0xC}.
// Shape: `for (Distance n = last - first; n > 0; --n) *--result = *--last;` with the class's IMPLICIT operator=
// (the vector's out-of-line operator=, then the float copied as a plain dword move -- a user memberwise operator=
// is not needed in this speed TU).
enum EAttackHistoryType { EAttackHistoryType_Force32 = 0x7fffffff };
enum EDamageAttribute { EDamageAttribute_Force32 = 0x7fffffff };

namespace std {

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

template <class RandomAccessIter, class BidirectionalIter, class Distance>
BidirectionalIter __fastcall __copy_backward(RandomAccessIter first, RandomAccessIter last, BidirectionalIter result,
                                             const random_access_iterator_tag&, Distance*)
{
    for (Distance n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

}

class CAttackHistoryCombo {                         // Ego_r.pdb (retail 0x10 bytes)
public:
    std::vector<std::pair<EAttackHistoryType, EDamageAttribute> > AttackCombo;   // +0x00
    float Multiplier;                                                           // +0x0C
};

template CAttackHistoryCombo* __fastcall std::__copy_backward<CAttackHistoryCombo*, CAttackHistoryCombo*, int>(
    CAttackHistoryCombo* first, CAttackHistoryCombo* last, CAttackHistoryCombo* result,
    const std::random_access_iterator_tag&, int*);