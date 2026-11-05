// std::__make_heap(SNamedPosition* first, SNamedPosition* last, CKeyPairCompareLess comp, SNamedPosition*, int*) -- STLport make_heap core over 16-byte (CCharString, C3DVector) pairs ordered by name: sift every parent down, last parent first
// Leaf: __make_heap
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077EDA0, 108 bytes, `ret 0Ch` (__fastcall /Gr algorithm TU: first in
//   ecx (kept in the `push ecx` frame word), last in edx; the empty comparator and the two null type-tag pointers on
//   the stack).
// Callees: 0x0099EC30 CCharString copy constructor (the by-value `_Tp(*(first + parent))` built in the argument area by
//   the pair's memberwise copy constructor, then the vector's three dwords), 0x0077EA60 __adjust_heap (landed, same
//   instantiation).
// Instantiation: sort<pair<CCharString, C3DVector>*, CKeyPairCompareLess<CCharString, C3DVector>> (called by
//   __partial_sort 0x0077F2C0).
// Shape: STLport `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2; for (;;) {
//   __adjust_heap(first, parent, len, _Tp(*(first + parent)), comp); if (parent == 0) return; parent--; }` -- the
//   element address is strength-reduced into edi as `&(first + parent)->second.Z` (`lea edi,[eax+ecx+0Ch]`,
//   `sub edi,10h` per step).
// Label correction: catalogued as `MakeHeap_CKeyPair`; the body is STLport `__make_heap`.
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

class CCharString { public:
    CBasicStringChar* PStringData;
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);      // 0x0099EFB0
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

#include "engine/C3DVector.h"

template <class T1, class T2> struct pair {                // STLport pair
    T1 first;
    T2 second;
    pair(const pair& p) : first(p.first), second(p.second) { }
};
typedef pair<CCharString, C3DVector> SNamedPosition;       // 16 bytes

template <class K, class V> class CKeyPairCompareLess { public:   // empty, passed by value
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<CCharString, C3DVector> CNamedPositionLess;

void __fastcall __adjust_heap(SNamedPosition* first, int holeIndex, int len, SNamedPosition val,
                              CNamedPositionLess comp);   // 0x0077EA60

void __fastcall __make_heap(SNamedPosition* first, SNamedPosition* last, CNamedPositionLess comp, SNamedPosition*, int*)
{
    if (last - first < 2)
        return;
    int len = last - first;
    int parent = (len - 2) / 2;
    for (;;) {
        __adjust_heap(first, parent, len, SNamedPosition(*(first + parent)), comp);
        if (parent == 0)
            return;
        parent--;
    }
}