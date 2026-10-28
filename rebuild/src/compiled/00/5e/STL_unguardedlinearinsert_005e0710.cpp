// std::__unguarded_linear_insert(CCharString* last, CCharString val, less<CCharString> comp) -- STLport insertion step: shift greater strings up one slot until val's place, then store it
// Leaf: __unguarded_linear_insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E0710, 76 bytes (+ padding to 80), `ret 8` (__fastcall /Gr algorithm TU: last in ecx ->
//   edi, the by-value string and the empty comparator on the stack; the by-value string is destroyed here).
// Callees: 0x00429950 CBasicString<char>::operator<, 0x0099EFB0 CCharString::operator=, 0x0099EAE0 ~CCharString.
// Instantiation: sort<CCharString*> @ 0x005E3A50 (called from __linear_insert 0x005E1A40 and __unguarded_insertion_sort
//   0x005E19A0 of the same instantiation).
// Shape: STLport `next = last; --next; while (comp(val, *next)) { *last = *next; last = next; --next; } *last = val;`;
//   `next` lives in esi as `last - 1` (`lea esi,[edi-4]`), the inline CCharString operator< compares val (this) with
//   *next (other). The comparator's real body above the caller keeps val's payload test order (see 0x005E0650).
// Label correction: catalogued as `SortedList_Insert`; the body is STLport `__unguarded_linear_insert`.
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

void __fastcall __unguarded_linear_insert(CCharString* last, CCharString val, less<CCharString> comp)
{
    CCharString* next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}