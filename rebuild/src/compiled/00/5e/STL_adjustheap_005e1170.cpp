// _STL::__adjust_heap(CCharString* first, int holeIndex, int len, CCharString val, less<CCharString> comp) -- STLport heap sift-down of a CCharString hole to a leaf, then __push_heap of val back up
// Leaf: __adjust_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E1170, 151 bytes (+ padding to 160), `ret 0Ch` (__fastcall /Gr algorithm TU: first in ecx ->
//   edi, holeIndex in edx -> ebx, then len, the by-value string and the empty comparator on the stack).
// Callees: 0x00429950 CBasicString<char>::operator<, 0x0099EFB0 CCharString::operator=, 0x0099EC30 CCharString copy ctor
//   (val copied into __push_heap's argument area), 0x005E08C0 __push_heap (same instantiation), 0x0099EAE0 ~CCharString.
// Instantiation: sort<CCharString*> @ 0x005E3A50 (called by __make_heap 0x005E1BE0 and __partial_sort 0x005E2EB0).
// Shape: STLport `top = hole; second = 2 * hole + 2; while (second < len) { if (comp(*(first + second), *(first +
//   (second - 1)))) second--; *(first + hole) = *(first + second); hole = second; second = 2 * (second + 1); }
//   if (second == len) { *(first + hole) = *(first + (second - 1)); hole = second - 1; } __push_heap(first, hole, top,
//   val, comp);` -- topIndex is spilled to the `push ecx` frame word.
// Label: catalogued as Dinkumware `_Adjust_heap<CCharString*,int,CCharString>`; the code is STLport `__adjust_heap`.
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

void __fastcall __push_heap(CCharString* first, int holeIndex, int topIndex, CCharString val, less<CCharString> comp);   // 0x005E08C0

void __fastcall __adjust_heap(CCharString* first, int holeIndex, int len, CCharString val, less<CCharString> comp)
{
    int topIndex = holeIndex;
    int secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (comp(*(first + secondChild), *(first + (secondChild - 1))))
            secondChild--;
        *(first + holeIndex) = *(first + secondChild);
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) {
        *(first + holeIndex) = *(first + (secondChild - 1));
        holeIndex = secondChild - 1;
    }
    __push_heap(first, holeIndex, topIndex, val, comp);
}