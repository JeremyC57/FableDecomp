// _STL::__make_heap(CCharString* first, CCharString* last, less<CCharString> comp, CCharString*, int*) -- STLport make_heap core: sift every parent of a CCharString range down, last parent first
// Leaf: __make_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E1BE0, 73 bytes (+ padding to 80), `ret 0Ch` (__fastcall /Gr algorithm TU: first in ecx ->
//   ebp, last in edx, then the empty comparator and the two null type-tag pointers on the stack).
// Callees: 0x0099EC30 CCharString copy ctor (the by-value `_Tp(*(first + parent))` built in the argument area),
//   0x005E1170 __adjust_heap (same instantiation).
// Instantiation: sort<CCharString*> @ 0x005E3A50 (called by __partial_sort 0x005E2EB0).
// Shape: STLport `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2; for (;;) {
//   __adjust_heap(first, parent, len, _Tp(*(first + parent)), comp); if (parent == 0) return; parent--; }` -- the
//   element address `first + parent` is strength-reduced into edi (`sub edi,4` per step).
// Label correction: catalogued as `STL_MakeHeap_CCharString`; the body is STLport `__make_heap`.
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

// The comparator's real body (the landed 0x00429950 source) compiled out of line ABOVE its caller: VC7.1 then knows
// the call writes no memory, so the pivot's payload pointer stays in esi across the compare calls (reloaded only
// after the swap's operator= calls), as retail. Without it the pivot word is re-read from its slot after every compare.
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

template <class T> struct less {                           // STLport less<T>: empty, passed by value
    bool operator()(const T& a, const T& b) const { return a < b; }
};

void __fastcall __adjust_heap(CCharString* first, int holeIndex, int len, CCharString val, less<CCharString> comp);   // 0x005E1170

void __fastcall __make_heap(CCharString* first, CCharString* last, less<CCharString> comp, CCharString*, int*)
{
    if (last - first < 2)
        return;
    int len = last - first;
    int parent = (len - 2) / 2;
    for (;;) {
        __adjust_heap(first, parent, len, CCharString(*(first + parent)), comp);
        if (parent == 0)
            return;
        parent--;
    }
}