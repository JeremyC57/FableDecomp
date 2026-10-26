// std::sort(CCharString* first, CCharString* last) -- STLport sort of a CCharString range with the default less<CCharString>: introsort to 16-element runs, then the final insertion sort
// Leaf: ?sort@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E3A50, 115 bytes (+ padding to 128), plain `ret` (__fastcall /Gr algorithm TU: first in ecx ->
//   edi, last in edx -> ebx; no stack arguments).
// Callees: 0x005E3740 __introsort_loop, 0x005E20C0 __insertion_sort, 0x005E19A0 __unguarded_insertion_sort (all the same
//   instantiation). Callers: 0x005DD42A (the CAnimationSet TU) and 0x006A03F8.
// Shape: STLport `if (first != last) { __introsort_loop(first, last, _STLP_VALUE_TYPE(first), __lg(last - first) * 2,
//   __less(_STLP_VALUE_TYPE(first))); __final_insertion_sort(first, last, __less(...)); }` with __lg
//   (`for (k = 0; n != 1; n >>= 1) ++k;`) and __final_insertion_sort (`if (last - first > __stl_threshold) {
//   __insertion_sort(first, first + 16, comp); __unguarded_insertion_sort(first + 16, last, comp); } else
//   __insertion_sort(first, last, comp);`) inlined. The two `less<CCharString>` temporaries share the one `push ecx`
//   frame word (pushed uninitialised: `less` needs a user-written empty constructor, else `less<T>()` stores a zero
//   byte into it), and the element count computed for __lg is reused for the threshold test.
// Label correction: catalogued as `CCharStringPair_SortedRange`; the body is STLport `sort(first, last)`.
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
    less() { }                                             // user-written: `less<T>()` stores no zero byte
    bool operator()(const T& a, const T& b) const { return a < b; }
};

void __fastcall __introsort_loop(CCharString* first, CCharString* last, CCharString*, int depth_limit, less<CCharString> comp);   // 0x005E3740
void __fastcall __insertion_sort(CCharString* first, CCharString* last, less<CCharString> comp);   // 0x005E20C0
void __fastcall __unguarded_insertion_sort(CCharString* first, CCharString* last, less<CCharString> comp);   // 0x005E19A0

inline int __lg(int n)
{
    int k;
    for (k = 0; n != 1; n >>= 1)
        ++k;
    return k;
}

inline less<CCharString> __less(CCharString*)
{
    return less<CCharString>();
}

inline void __final_insertion_sort(CCharString* first, CCharString* last, less<CCharString> comp)
{
    if (last - first > 16) {
        __insertion_sort(first, first + 16, comp);
        __unguarded_insertion_sort(first + 16, last, comp);
    }
    else
        __insertion_sort(first, last, comp);
}

void __fastcall sort(CCharString* first, CCharString* last)
{
    if (first != last) {
        __introsort_loop(first, last, (CCharString*)0, __lg(last - first) * 2, __less((CCharString*)0));
        __final_insertion_sort(first, last, __less((CCharString*)0));
    }
}