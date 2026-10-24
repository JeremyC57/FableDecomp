// _STL::sort(CWideString*, CWideString*, NGameComponentHelpers::CCaseInsensitiveStringCompare) -- STLport sort of wide strings ordered case-insensitively: introsort loop to depth 2*lg(n), then the inline final insertion sort (guarded over the first 16, unguarded over the rest)
// Flags: /Gr
// Leaf: ?sort@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004977D0, 110 bytes, `ret 4` (__fastcall: first in ecx -> edi, last in edx ->
//   ebx, the empty comparator's slot on the stack -> ebp).
// Callees: 0x00497740 __introsort_loop (value-type tag 0, depth `lea eax,[ecx+ecx]`), 0x004975E0 __insertion_sort,
//   0x004972E0 __unguarded_insertion_sort (all the same instantiation).
// Shape: STLport `if (first != last) { __introsort_loop(first, last, VALUE_TYPE, __lg(last - first) * 2, comp);
//   __final_insertion_sort(first, last, comp); }` with `__lg` (the shift-count loop) and `__final_insertion_sort`
//   (`if (last - first > __stl_threshold) { __insertion_sort(first, first + 16, comp); __unguarded_insertion_sort(first
//   + 16, last, comp); } else __insertion_sort(first, last, comp);`) both inlined; the element count stays in esi.
// Label correction: catalogued as `StdSort_Introsort` (right idea); the body is STLport `sort<CWideString*, Compare>`
//   (unlike the 12-byte-element twin 0x008701D0, __final_insertion_sort is inline here).
#include <string.h>                                 // _wcsicmp @ 0x00F3FB84 (CRT)

struct CWideString {
    void* PStringData;
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    operator const wchar_t*() const;                    // 0x0099AF10
};

namespace NGameComponentHelpers {
struct CCaseInsensitiveStringCompare {
    char m_tag;
    bool operator()(const CWideString& a, const CWideString& b) const { return _wcsicmp(a, b) < 0; }
};
}

typedef NGameComponentHelpers::CCaseInsensitiveStringCompare TCompare;

void __fastcall __introsort_loop(CWideString* first, CWideString* last, CWideString*, int depth_limit, TCompare comp);   // 0x00497740
void __fastcall __insertion_sort(CWideString* first, CWideString* last, TCompare comp);                                  // 0x004975E0
void __fastcall __unguarded_insertion_sort(CWideString* first, CWideString* last, TCompare comp);                        // 0x004972E0

template <class _Size>
inline _Size __lg(_Size __n)
{
    _Size __k;
    for (__k = 0; __n != 1; __n >>= 1) ++__k;
    return __k;
}

inline void __final_insertion_sort(CWideString* first, CWideString* last, TCompare comp)
{
    if (last - first > 16) {
        __insertion_sort(first, first + 16, comp);
        __unguarded_insertion_sort(first + 16, last, comp);
    }
    else
        __insertion_sort(first, last, comp);
}

void __fastcall sort(CWideString* first, CWideString* last, TCompare comp)
{
    if (first != last) {
        __introsort_loop(first, last, (CWideString*)0, __lg(last - first) * 2, comp);
        __final_insertion_sort(first, last, comp);
    }
}