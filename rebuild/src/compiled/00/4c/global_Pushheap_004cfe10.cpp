// _global::_Push_heap(pair<float,CCharString>*, long, long, pair<float,CCharString>, CKeyPairCompareLess<float,CCharString>) -- heap sift-up of (score, name) pairs by score
// Leaf: _Push_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004CFE10, 119 bytes, `ret 0x10`.
// Called by the landed _Adjust_heap @ 0x004D0590 (same instantiation; its model is reused here).
// Callees: 0x0099EFB0 CCharString::operator=, 0x0099EAE0 ~CCharString.
// Link-time __fastcall (/Gr algorithm TU): first in ecx, hole in edx, then top, the 8-byte pair by
// value and the empty comparator (4 + 8 + 4 = 0x10); the by-value pair is destroyed at the end.
// CKeyPairCompareLess<float,...>: `a.first < b.first` (`fld [parent] ; fcomp [value] ; test ah,5 ; jp`).
// The implicit pair assignment moves the float through eax inside the loop but through the FPU
// (`fld [esp+18h] ... fstp`) for the final drop from the value slot -- both are the same source.
// Shape: SGI/STLport `__push_heap` (`parent = (hole - 1) / 2; while (hole > top && comp(first[parent], value))`).
class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
    CCharString& operator=(const CCharString& other);      // 0x0099EFB0
};

template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
typedef pair<float, CCharString> TEntry;

template<class K, class V> class CKeyPairCompareLess { public:
    char m_tag;
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<float, CCharString> TCompare;

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