// _STL::__introsort_loop(CWideString*, CWideString*, CWideString*, int, NGameComponentHelpers::CCaseInsensitiveStringCompare) -- STLport introsort loop over wide strings ordered case-insensitively: median-of-three partition and recurse on the right part until 16 or fewer remain, falling back to partial_sort when the depth limit runs out
// Flags: /Gr
// Leaf: __introsort_loop
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00497740, 142 bytes, `ret 0Ch` (__fastcall: first in ecx -> ebx, last in edx
//   -> edi, then the value-type tag pointer, the depth limit and the empty comparator on the stack; the comparator in ebp).
// Callees: 0x00496EE0 __median, 0x0099B720 CWideString copy ctor (the by-value pivot `_Tp(__median(...))` built in the
//   partition's argument slot: `push ecx ; mov ecx,esp ; push eax`), 0x00497200 __unguarded_partition, 0x00497740 itself,
//   0x00497620 __partial_sort (all the same instantiation).
// Shape: STLport `while (last - first > __stl_threshold) { if (depth_limit == 0) { partial_sort(first, last, last, comp);
//   return; } --depth_limit; cut = __unguarded_partition(first, last, _Tp(__median(*first, *(first + (last - first) / 2),
//   *(last - 1), comp)), comp); __introsort_loop(cut, last, (_Tp*)0, depth_limit, comp); last = cut; }` with the inline
//   `partial_sort` forwarding to __partial_sort (value-type tag 0). The decremented depth is stored back to its slot.
// Label correction: catalogued as `Heapsort_Recursive`; the body is STLport `__introsort_loop`.
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

const CWideString& __fastcall __median(const CWideString& a, const CWideString& b, const CWideString& c, TCompare comp);  // 0x00496EE0
CWideString* __fastcall __unguarded_partition(CWideString* first, CWideString* last, CWideString pivot, TCompare comp);  // 0x00497200
void __fastcall __partial_sort(CWideString* first, CWideString* middle, CWideString* last, CWideString*, TCompare comp);  // 0x00497620

inline void partial_sort(CWideString* first, CWideString* middle, CWideString* last, TCompare comp)
{
    __partial_sort(first, middle, last, (CWideString*)0, comp);
}

void __fastcall __introsort_loop(CWideString* first, CWideString* last, CWideString*, int depth_limit, TCompare comp)
{
    while (last - first > 16) {
        if (depth_limit == 0) {
            partial_sort(first, last, last, comp);
            return;
        }
        --depth_limit;
        CWideString* cut =
            __unguarded_partition(first, last,
                                  CWideString(__median(*first,
                                                       *(first + (last - first) / 2),
                                                       *(last - 1), comp)),
                                  comp);
        __introsort_loop(cut, last, (CWideString*)0, depth_limit, comp);
        last = cut;
    }
}