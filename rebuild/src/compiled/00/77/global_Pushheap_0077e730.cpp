// _global::_Push_heap(pair<CCharString,C3DVector>*, long, long, pair<CCharString,C3DVector>, CKeyPairCompareLess<CCharString,C3DVector>) -- heap sift-up of name-keyed positions
// Leaf: _Push_heap
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077E730, 178 bytes, `ret 0x18`.
// Callees: 0x00429950 CBasicString<char>::operator<, 0x0099EFB0 CCharString::operator=, 0x0099EAE0
// ~CCharString.  Link-time __fastcall (/Gr algorithm TU): first in ecx, hole in edx, then top, the
// 16-byte pair by value and the empty comparator (4 + 0x10 + 4 = 0x18); the by-value pair is
// destroyed at the end.  The implicit pair assignment is the CCharString call plus three dword moves.
// CCharString::operator< (inline, the landed 0x009B5C70 / 0x00CB68C0 shape): same payload -> false,
// null this -> true, null other -> false, else CBasicString<char>::operator< @ 0x00429950 on the payloads.
// Shape: SGI/STLport `__push_heap` (`parent = (hole - 1) / 2; while (hole > top && comp(first[parent], value))`).
struct CBasicStringChar {
    char* PData;
    bool operator<(const CBasicStringChar& other) const;   // 0x00429950
};

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

template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
typedef pair<CCharString, C3DVector> TEntry;

template<class K, class V> class CKeyPairCompareLess { public:
    char m_tag;
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<CCharString, C3DVector> TCompare;

void __fastcall _Push_heap(TEntry* first, long hole, long top, TEntry value, TCompare comp)
{
    long parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}