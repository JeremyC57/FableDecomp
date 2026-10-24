// _STL::__push_heap(CCharString* first, int holeIndex, int topIndex, CCharString val, less<CCharString> comp) -- STLport heap sift-up of a CCharString: move smaller parents down, then store val in the hole
// Leaf: __push_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E08C0, 124 bytes (+ padding to 128), `ret 0Ch` (__fastcall /Gr algorithm TU: first in ecx ->
//   ebp, holeIndex in edx -> edi, then topIndex, the by-value string and the empty comparator on the stack; the by-value
//   string is destroyed here).
// Callees: 0x00429950 CBasicString<char>::operator<, 0x0099EFB0 CCharString::operator=, 0x0099EAE0 ~CCharString.
// Instantiation: sort<CCharString*> @ 0x005E3A50 (the heap tail of __adjust_heap 0x005E1170).
// Shape: STLport `parent = (hole - 1) / 2; while (hole > top && comp(*(first + parent), val)) { *(first + hole) =
//   *(first + parent); hole = parent; parent = (hole - 1) / 2; } *(first + hole) = val;` -- the signed halving is
//   `cdq ; sub eax,edx ; sar 1`. The comparator's real body above the caller keeps val's payload test in registers
//   (see 0x005E0650).
// Label: catalogued as Dinkumware `_Push_heap<CCharString*,int,CCharString>`; the code is STLport `__push_heap`.
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

void __fastcall __push_heap(CCharString* first, int holeIndex, int topIndex, CCharString val, less<CCharString> comp)
{
    int parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(*(first + parent), val)) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = val;
}