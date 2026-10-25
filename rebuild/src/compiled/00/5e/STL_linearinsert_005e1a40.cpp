// std::__linear_insert(CCharString* first, CCharString* last, CCharString val, less<CCharString> comp) -- STLport insertion-sort step: a new minimum shifts the whole prefix up one slot, anything else goes through the unguarded insert
// Leaf: ?__linear_insert@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E1A40, 112 bytes, `ret 8` (__fastcall /Gr algorithm TU: first in ecx ->
//   esi, last in edx -> edi, the by-value string and the empty comparator on the stack; val is destroyed on both paths).
// Callees: 0x00429950 CBasicString<char>::operator<, 0x005E0FE0 copy_backward, 0x0099EFB0 CCharString::operator=,
//   0x0099EC30 CCharString copy ctor (val copied into the callee's argument area), 0x005E0710 __unguarded_linear_insert,
//   0x0099EAE0 ~CCharString.
// Instantiation: sort<CCharString*> @ 0x005E3A50 (called by __insertion_sort 0x005E20C0).
// Shape: STLport `if (comp(val, *first)) { copy_backward(first, last, last + 1); *first = val; } else
//   __unguarded_linear_insert(last, val, comp);` -- the by-value val's destructor is duplicated into both exits.
// Label correction: catalogued as `SortedList_Insert_CCharString`; the body is STLport `__linear_insert`.
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

CCharString* __fastcall copy_backward(CCharString* first, CCharString* last, CCharString* result);   // 0x005E0FE0
void __fastcall __unguarded_linear_insert(CCharString* last, CCharString val, less<CCharString> comp);   // 0x005E0710

void __fastcall __linear_insert(CCharString* first, CCharString* last, CCharString val, less<CCharString> comp)
{
    if (comp(val, *first)) {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}