// std::_Sort_heap(pair<CCharString,unsigned long>* first, pair<CCharString,unsigned long>* last, CKeyPairCompareLess<CCharString,unsigned long> pred) -- Dinkumware _Sort_heap over string-keyed pairs: pop the root to the back until one element remains
// Leaf: _Sort_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B8170, length 169 bytes (+ int3 padding), `ret 4`.
// Identity: catalog `_Sort_heap<pair<CCharString,unsigned long>*,CKeyPairCompareLess<CCharString,unsigned long>>`,
// consistent with the bytes: the callee is the landed `_Adjust_heap` @ 0x009B5C70 of the same instantiation
// (twin of 0x00A41960 / 0x00A408E0).
// Callees: _Adjust_heap @ 0x009B5C70 (out of line, __fastcall: first in ecx, hole 0 in edx), CCharString copy
// constructor @ 0x0099EC30 (x2), CCharString::operator= @ 0x0099EFB0, CCharString::~CCharString @ 0x0099EAE0.
// Shape: /Gr Dinkumware TU with _Pop_heap_0 / _Pop_heap inline and the loop calling _Pop_heap_0 directly
// (8-byte stride: `and -8 ; cmp 8`). `_Ty(*(_Last - 1))` is an 8-byte frame temporary (VC aligns it: the
// `push ebp ; mov ebp,esp ; and esp,-8` prologue, the comparator read through [ebp+8]) built by the implicit pair
// copy (string copy constructor + dword); `*_Dest = *_First` is the implicit assignment (CCharString::operator=
// + dword), the value argument is copied from the temporary, which is destroyed after the call (no /EHsc).
// Model of the landed 0x009B5C70 source.
struct CBasicStringChar {
    char* PData;
    bool operator<(const CBasicStringChar& other) const;   // 0x00429950
};

class CCharString { public:
    CBasicStringChar* PStringData;
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
    CCharString& operator=(const CCharString& other);      // 0x0099EFB0
};

template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
typedef pair<CCharString, unsigned long> TEntry;

template<class K, class V> class CKeyPairCompareLess { public:
    char m_tag;
};
typedef CKeyPairCompareLess<CCharString, unsigned long> TCompare;

void __fastcall _Adjust_heap(TEntry* first, long hole, long bottom, TEntry value, TCompare pred);   // 0x009B5C70

inline void _Pop_heap(TEntry* first, TEntry* last, TEntry* dest, TEntry value, TCompare pred, long*)
{
    *dest = *first;
    _Adjust_heap(first, 0, long(last - first), value, pred);
}

inline void _Pop_heap_0(TEntry* first, TEntry* last, TCompare pred, TEntry*)
{
    _Pop_heap(first, last - 1, last - 1, TEntry(*(last - 1)), pred, (long*)0);
}

void __fastcall _Sort_heap(TEntry* first, TEntry* last, TCompare pred)
{
    for (; 1 < last - first; --last)
        _Pop_heap_0(first, last, pred, (TEntry*)0);
}