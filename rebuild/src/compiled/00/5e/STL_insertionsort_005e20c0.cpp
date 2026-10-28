// std::__insertion_sort(CCharString* first, CCharString* last, less<CCharString> comp) -- STLport insertion sort of a CCharString range: every element after the first goes through the out-of-line __linear_insert(first, i, *i, comp)
// Leaf: ?__insertion_sort@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E20C0, 56 bytes (+ padding to 64), `ret 4` (__fastcall /Gr algorithm TU: first in ecx ->
//   ebx, last in edx -> edi, the empty comparator on the stack, loaded once into ebp).
// Callees: 0x0099EC30 CCharString copy ctor (the by-value `*i` built in the argument area), 0x005E1A40 __linear_insert.
// Instantiation: sort<CCharString*> @ 0x005E3A50 (its inlined __final_insertion_sort).
// Shape: STLport `if (first == last) return; for (i = first + 1; i != last; ++i) __linear_insert(first, i, *i, comp);`
//   -- same shape as the landed __insertion_sort 0x005E2E00 of this TU.
// Label correction: catalogued as `std_CopyStringRange`; the body is STLport `__insertion_sort`.
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

template <class T> struct less {                           // STLport less<T>: empty, passed by value
    bool operator()(const T& a, const T& b) const { return a < b; }
};

void __fastcall __linear_insert(CCharString* first, CCharString* last, CCharString val, less<CCharString> comp);   // 0x005E1A40

void __fastcall __insertion_sort(CCharString* first, CCharString* last, less<CCharString> comp)
{
    if (first == last)
        return;
    for (CCharString* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}