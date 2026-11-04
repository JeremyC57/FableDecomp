// std::__unguarded_linear_insert(pair<unsigned long, C3DVector>* last, pair<unsigned long, C3DVector> val, CKeyPairCompareLess<unsigned long, C3DVector> comp) -- shift larger-keyed elements up one slot until val's place is found, then store val
// Leaf: ??$__unguarded_linear_insert@
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00769420, 80 bytes, `ret 14h`.
// Callers: 0x00769C9E (__unguarded_insertion_sort_aux 0x00769C70) and 0x0076AB3F (the __linear_insert of the same
//   instantiation). No callees.
// Element: the 16-byte (unsigned long key, C3DVector) pair of the landed heap helpers 0x00769470 / 0x00769D50
//   (`shl ..,4`, UNSIGNED key compare); C3DVector has a memberwise user copy constructor and no user operator=, so
//   element assignment is a four-dword copy.
// Shape: STLport `__unguarded_linear_insert(last, val, comp)` from an /Gr algorithm TU (__fastcall: last in ecx, the
//   16-byte value and the 4-byte comparator on the stack, `ret 14h`): next = last - 1; while comp(val, *next)
//   { *last = *next; last = next; --next; } *last = val. VC rotates the loop (`jae` exit test before it, `jb`
//   at the bottom) and keeps val.first in edx.
#include "engine/C3DVector.h"

namespace std {

template <class _T1, class _T2>
class pair {
public:
    _T1 first;                                              // +0x00 sort key
    _T2 second;                                             // +0x04
    pair(const pair& o) : first(o.first), second(o.second) { }
};

}

template <class K, class V> class CKeyPairCompareLess { public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

typedef std::pair<unsigned long, C3DVector> SKeyedVector;                 // 16 bytes
typedef CKeyPairCompareLess<unsigned long, C3DVector> SKeyedVectorLess;

namespace std {

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}

template void __fastcall __unguarded_linear_insert<SKeyedVector*, SKeyedVector, SKeyedVectorLess>(SKeyedVector*, SKeyedVector, SKeyedVectorLess);

}