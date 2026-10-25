// std::__unguarded_partition(CCharString* first, CCharString* last, CCharString pivot, less<CCharString> comp) -- STLport quicksort partition of a CCharString range around a by-value pivot; returns the cut
// Leaf: __unguarded_partition
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E0650, 141 bytes, `ret 8` (__fastcall /Gr algorithm TU: first in ecx ->
//   edi, last in edx -> ebx, the 4-byte pivot string by value and the empty comparator's slot on the stack; the callee
//   destroys its by-value pivot at the end).
// Callees: 0x00429950 CBasicString<char>::operator<, 0x0099EC30 CCharString copy ctor, 0x0099EFB0 CCharString::operator=,
//   0x0099EAE0 ~CCharString.
// Instantiation: sort<CCharString*>(first, last) @ 0x005E3A50 (STLport `__less(T*)` = less<CCharString>), whose
//   __introsort_loop @ 0x005E3740 builds the pivot from __median @ 0x005E0580 and calls this body.
// CCharString::operator< (inline, the landed 0x00AF2F10 / 0x00A408E0 comparator): same payload -> false, null this ->
//   true, null other -> false, else CBasicString<char>::operator< on the shared payloads.
// Shape: STLport `for (;;) { while (comp(*first, pivot)) ++first; --last; while (comp(pivot, *last)) --last;
//   if (!(first < last)) return first; iter_swap(first, last); ++first; }`; the inlined `__iter_swap_aux` temporary
//   lives in the `push ecx` frame slot (same shape as the landed CWideString partition 0x00497200).
// Label correction: catalogued as `QuickSort_Partition_StringKey`; the body is STLport `__unguarded_partition`.
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

inline void __iter_swap_aux(CCharString* a, CCharString* b)
{
    CCharString tmp = *a;
    *a = *b;
    *b = tmp;
}

CCharString* __fastcall __unguarded_partition(CCharString* first, CCharString* last, CCharString pivot, less<CCharString> comp)
{
    for (;;) {
        while (comp(*first, pivot))
            ++first;
        --last;
        while (comp(pivot, *last))
            --last;
        if (!(first < last))
            return first;
        __iter_swap_aux(first, last);
        ++first;
    }
}