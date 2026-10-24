// _STL::__partial_sort(CCharString* first, CCharString* middle, CCharString* last, CCharString*, less<CCharString> comp) -- STLport partial_sort core for a CCharString range: heapify [first, middle), pop every smaller later string into the heap, then sort the heap
// Leaf: __partial_sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E2EB0, 144 bytes, `ret 0Ch` (__fastcall /Gr algorithm TU: first in ecx ->
//   esi, middle in edx -> ebx, then last, the null value-type pointer and the empty comparator on the stack).
// Callees: 0x005E1BE0 __make_heap (first, middle, comp, (T*)0, (int*)0), 0x00429950 CBasicString<char>::operator<,
//   0x0099EC30 CCharString copy ctor, 0x0099EFB0 CCharString::operator=, 0x005E1170 __adjust_heap, 0x0099EAE0
//   ~CCharString, 0x005E27F0 sort_heap (same instantiation).
// Instantiation: sort<CCharString*> @ 0x005E3A50 (the depth-limit fallback of __introsort_loop 0x005E3740).
// Shape: STLport `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first))
//   __pop_heap(first, middle, i, _Tp(*i), comp, _STLP_DISTANCE_TYPE(first, _RandomAccessIter)); sort_heap(first,
//   middle, comp);` -- here (unlike the 8- and 20-byte instantiations) __pop_heap is INLINED: its by-value `val` is
//   built straight from *i in the dead `_Tp*` argument slot (`lea ecx,[esp+20h]`), `*result = *first` follows, and
//   `__adjust_heap(first, 0, middle - first, val, comp)` copies val again into the argument area.
// Label correction: catalogued as `std_sort_AdjustHeap`; the body is STLport `__partial_sort`.
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

void __fastcall __make_heap(CCharString* first, CCharString* last, less<CCharString> comp, CCharString*, int*);   // 0x005E1BE0
void __fastcall __adjust_heap(CCharString* first, int holeIndex, int len, CCharString val, less<CCharString> comp);   // 0x005E1170
void __fastcall sort_heap(CCharString* first, CCharString* last, less<CCharString> comp);   // 0x005E27F0

inline void make_heap(CCharString* first, CCharString* last, less<CCharString> comp)
{
    __make_heap(first, last, comp, (CCharString*)0, (int*)0);
}

inline void __pop_heap(CCharString* first, CCharString* last, CCharString* result, CCharString val, less<CCharString> comp, int*)
{
    *result = *first;
    __adjust_heap(first, 0, last - first, val, comp);
}

void __fastcall __partial_sort(CCharString* first, CCharString* middle, CCharString* last, CCharString*, less<CCharString> comp)
{
    make_heap(first, middle, comp);
    for (CCharString* i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, CCharString(*i), comp, (int*)0);
    sort_heap(first, middle, comp);
}