// std::__unguarded_partition(SNamedPosition* first, SNamedPosition* last, SNamedPosition pivot, CKeyPairCompareLess comp) -- STLport quicksort partition of 16-byte (CCharString, C3DVector) pairs around a by-value pivot (ordered by name); returns the cut
// Leaf: __unguarded_partition
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077E8F0, 110 bytes (+ int3 padding to 112), `ret 14h` (__fastcall /Gr
//   algorithm TU: first in ecx -> edi, last in edx -> ebx, the 16-byte by-value pivot and the empty comparator on the
//   stack; the callee destroys its by-value pivot's string at the end).
// Callees: 0x00429950 CBasicString<char>::operator< (the inlined CCharString::operator<), 0x0077E0C0 iter_swap(a, b)
//   (out of line for the 16-byte pair: copy constructor + two assignments; catalogued as the propagated
//   `swap<C3DSkeletalBoneMorph>`), 0x0099EAE0 ~CCharString.
// Instantiation: sort<pair<CCharString, C3DVector>*, CKeyPairCompareLess<CCharString, C3DVector>> (the same TU as the
//   landed __adjust_heap 0x0077EA60); called by __introsort_loop 0x0077F370.
// Shape: STLport `for (;;) { while (comp(*first, pivot)) ++first; --last; while (comp(pivot, *last)) --last;
//   if (!(first < last)) return first; iter_swap(first, last); ++first; }` -- the pivot's payload pointer stays in esi
//   across the compare calls and is re-read only after the out-of-line iter_swap (the comparator's real body is
//   compiled above the caller, auto_inline off, as in the landed CCharString partition 0x005E0650).
// Label correction: catalogued as `Partition_ThreeWay_Quicksort`; the body is STLport `__unguarded_partition`.
struct CBasicStringChar {                                  // CBasicString<char>
    char* PData;
    __forceinline long Compare(const CBasicStringChar& other) const
    {
        const char* b = other.PData;
        const char* a = PData;
        for (;;) {
            if (*a == 0 && *b == 0)
                return 0;
            if (*a < *b)
                return -1;
            if (*a > *b)
                return 1;
            ++a;
            ++b;
        }
    }
    bool operator<(const CBasicStringChar& other) const;   // 0x00429950
};

#pragma auto_inline(off)
bool CBasicStringChar::operator<(const CBasicStringChar& other) const
{
    return Compare(other) < 0;
}
#pragma auto_inline(on)

struct CCharString {
    CBasicStringChar* PStringData;
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
    CCharString& operator=(const CCharString& other);      // 0x0099EFB0
    bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData)
            return false;
        if (!PStringData)
            return true;
        if (!other.PStringData)
            return false;
        return *PStringData < *other.PStringData;
    }
};

struct C3DVector {                                         // floats, moved as dwords
    unsigned long X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

template <class T1, class T2> struct pair {                // STLport pair
    T1 first;
    T2 second;
    pair(const pair& p) : first(p.first), second(p.second) { }
};
typedef pair<CCharString, C3DVector> SNamedPosition;       // 16 bytes

template <class K, class V> struct CKeyPairCompareLess {   // empty, passed by value
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<CCharString, C3DVector> CNamedPositionLess;

void __fastcall iter_swap(SNamedPosition* a, SNamedPosition* b);   // 0x0077E0C0

SNamedPosition* __fastcall __unguarded_partition(SNamedPosition* first, SNamedPosition* last, SNamedPosition pivot,
                                                 CNamedPositionLess comp)
{
    for (;;) {
        while (comp(*first, pivot))
            ++first;
        --last;
        while (comp(pivot, *last))
            --last;
        if (!(first < last))
            return first;
        iter_swap(first, last);
        ++first;
    }
}