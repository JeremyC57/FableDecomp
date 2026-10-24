// _global::_Adjust_heap(pair<float,CCharString>*, long, long, pair<float,CCharString>, CKeyPairCompareLess<float,CCharString>) -- heap sift-down of (score, name) pairs by score, then _Push_heap @ 0x004CFE10
// Flags: /Gr
// Leaf: _Adjust_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D0590, 163 bytes.
// Callees: 0x0099EFB0 CCharString::operator=, 0x0099EC30 CCharString copy ctor, 0x0099EAE0
// ~CCharString, 0x004CFE10 _Push_heap (same instantiation).  Link-time __fastcall: first in ecx,
// hole in edx, then bottom, the 8-byte pair by value (built in the _Push_heap slot by the implicit
// pair copy ctor: the float stored, then the CCharString copy ctor on slot+4) and the 4-byte
// comparator; the by-value pair is destroyed here at the end.  `top` lives in the `push ecx` slot.
// CKeyPairCompareLess<float,...>: `a.first < b.first` (`fld ; fcomp ; test ah,5 ; jp`).
// Shape: VC7.1 <algorithm> `_Adjust_heap`; the bottom case reuses idx (`first[idx - 1]`).
struct CCharString {
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

template<class K, class V> struct CKeyPairCompareLess {
    char m_tag;
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<float, CCharString> TCompare;

void __fastcall _Push_heap(TEntry* first, long hole, long top, TEntry value, TCompare pred);   // 0x004CFE10

void __fastcall _Adjust_heap(TEntry* first, long hole, long bottom, TEntry value, TCompare pred)
{
    long top = hole;
    long idx = 2 * hole + 2;
    for (; idx < bottom; idx = 2 * idx + 2) {
        if (pred(first[idx], first[idx - 1]))
            --idx;
        first[hole] = first[idx], hole = idx;
    }
    if (idx == bottom) {
        first[hole] = first[idx - 1];
        hole = idx - 1;
    }
    _Push_heap(first, hole, top, value, pred);
}