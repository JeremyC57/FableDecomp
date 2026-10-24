// _global::_Push_heap(pair<CCharString,unsigned long>*, long, long, pair<CCharString,unsigned long>, CKeyPairCompareLess<CCharString,unsigned long>) -- heap sift-up of string-keyed dwords
// Leaf: _Push_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B4940, 141 bytes, `ret 0x10`.
// Called by the landed _Adjust_heap @ 0x009B5C70 (same instantiation; its model is reused here).
// Callees: 0x00429950 CBasicString<char>::operator<, 0x0099EFB0 CCharString::operator=, 0x0099EAE0
// ~CCharString.  Link-time __fastcall (/Gr algorithm TU): first in ecx, hole in edx, then top, the
// 8-byte pair by value and the empty comparator (4 + 8 + 4 = 0x10); the by-value pair is destroyed
// at the end.  The implicit pair assignment is the CCharString call plus one dword.
// CCharString::operator< (inline, the landed 0x009B5C70 / 0x00CB68C0 shape): same payload -> false,
// null this -> true, null other -> false, else CBasicString<char>::operator< @ 0x00429950 on the payloads.
// Shape: SGI/STLport `__push_heap` (`parent = (hole - 1) / 2; while (hole > top && comp(first[parent], value))`).
struct CBasicStringChar {
    char* PData;
    bool operator<(const CBasicStringChar& other) const;   // 0x00429950
};

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

template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
typedef pair<CCharString, unsigned long> TEntry;

template<class K, class V> struct CKeyPairCompareLess {
    char m_tag;
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<CCharString, unsigned long> TCompare;

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