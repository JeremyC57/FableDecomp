// _STL::__introsort_loop(CCharString* first, CCharString* last, CCharString*, int depth_limit, less<CCharString> comp) -- STLport introsort recursion for a CCharString range: median-of-three quicksort partitions down to 16 elements, heap sort once the depth budget is spent
// Leaf: __introsort_loop
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E3740, 142 bytes (+ padding to 144), `ret 0Ch` (__fastcall /Gr algorithm TU: first in ecx ->
//   ebx, last in edx -> edi, then the null value-type pointer, the depth budget and the empty comparator on the stack).
// Callees: 0x005E0580 __median (out of line: returns a const reference), 0x0099EC30 CCharString copy ctor (the by-value
//   pivot `_Tp(__median(...))` built in the argument area), 0x005E0650 __unguarded_partition, itself (recursion on the
//   right part), 0x005E2EB0 __partial_sort (partial_sort(first, last, last, comp) inlined to its core).
// Instantiation: sort<CCharString*> @ 0x005E3A50.
// Shape: STLport `while (last - first > __stl_threshold) { if (depth_limit == 0) { partial_sort(first, last, last, comp);
//   return; } --depth_limit; cut = __unguarded_partition(first, last, _Tp(__median(*first, *(first + (last - first)/2),
//   *(last - 1), comp)), comp); __introsort_loop(cut, last, (_Tp*)0, depth_limit, comp); last = cut; }` -- the size test
//   is `and eax,-4 ; cmp eax,40h`, the decremented budget is stored back to its argument slot, and the comparator push
//   shared by the partition and the partial_sort calls is hoisted above the depth test.
// Label correction: catalogued as `std_sort_QuicksortIntrosort`; the body is STLport `__introsort_loop`.
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

const CCharString& __fastcall __median(const CCharString& a, const CCharString& b, const CCharString& c, less<CCharString> comp);   // 0x005E0580
CCharString* __fastcall __unguarded_partition(CCharString* first, CCharString* last, CCharString pivot, less<CCharString> comp);   // 0x005E0650
void __fastcall __partial_sort(CCharString* first, CCharString* middle, CCharString* last, CCharString*, less<CCharString> comp);   // 0x005E2EB0

inline void partial_sort(CCharString* first, CCharString* middle, CCharString* last, less<CCharString> comp)
{
    __partial_sort(first, middle, last, (CCharString*)0, comp);
}

void __fastcall __introsort_loop(CCharString* first, CCharString* last, CCharString*, int depth_limit, less<CCharString> comp)
{
    while (last - first > 16) {
        if (depth_limit == 0) {
            partial_sort(first, last, last, comp);
            return;
        }
        --depth_limit;
        CCharString* cut = __unguarded_partition(first, last,
                                                 CCharString(__median(*first, *(first + (last - first) / 2), *(last - 1), comp)),
                                                 comp);
        __introsort_loop(cut, last, (CCharString*)0, depth_limit, comp);
        last = cut;
    }
}