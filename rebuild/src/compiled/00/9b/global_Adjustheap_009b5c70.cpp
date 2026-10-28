// _global::_Adjust_heap(pair<CCharString,unsigned long>*, long, long, pair<CCharString,unsigned long>, CKeyPairCompareLess<CCharString,unsigned long>) -- heap sift-down of string-keyed pairs, then _Push_heap @ 0x009B4940
// Flags: /Gr
// Leaf: _Adjust_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B5C70, 184 bytes (+ int3 padding).
// Callees: 0x00429950 CBasicString<char>::operator<, 0x0099EFB0 CCharString::operator=, 0x0099EC30
// CCharString copy ctor, 0x0099EAE0 ~CCharString (manifest label propagated from a _Cons_val twin),
// 0x009B4940 _Push_heap (same instantiation).  Link-time __fastcall: first in ecx, hole in edx,
// then bottom, the 8-byte pair by value (built in the _Push_heap slot by the implicit pair copy
// ctor: CCharString copy ctor + dword copy) and the 4-byte comparator; the by-value pair is
// destroyed here at the end (`lea ecx,[esp+0x1c] ; call ~CCharString`).
// CCharString::operator< (inline): same payload -> false, null this -> true, null other -> false,
// else compare the shared CBasicString payloads (the shape of the landed 0x00CB68C0 comparator).
// Shape: VC7.1 <algorithm> `_Adjust_heap`; the bottom case reuses idx (`first[idx - 1]`).
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

template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
typedef pair<CCharString, unsigned long> TEntry;

template<class K, class V> class CKeyPairCompareLess { public:
    char m_tag;
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<CCharString, unsigned long> TCompare;

void __fastcall _Push_heap(TEntry* first, long hole, long top, TEntry value, TCompare pred);   // 0x009B4940

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