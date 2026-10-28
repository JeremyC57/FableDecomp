// std::__unguarded_partition(CWideString*, CWideString*, CWideString, NGameComponentHelpers::CCaseInsensitiveStringCompare) -- STLport quicksort partition of wide strings around a by-value pivot ordered case-insensitively (_wcsicmp); returns the cut
// Flags: /Gr
// Leaf: __unguarded_partition
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00497200, 215 bytes, `ret 8` (__fastcall: first in ecx -> esi, last in edx ->
//   edi, the 4-byte pivot string by value and the empty comparator's slot on the stack; the callee destroys its pivot).
// Callees: 0x0099AF10 CWideString::operator const wchar_t* (byte twin of the CCharString accessor, label propagated),
//   0x00F3FB84 _wcsicmp (CRT), 0x0099B720 CWideString copy ctor, 0x0099B7D0 CWideString::operator=, 0x0099B510
//   ~CWideString. Same models as the landed _Adjust_heap 0x00497160 of this sort instantiation.
// Shape: STLport `for (;;) { while (comp(*first, pivot)) ++first; --last; while (comp(pivot, *last)) --last;
//   if (!(first < last)) return first; iter_swap(first, last); ++first; }`; the inlined `__iter_swap_aux` temporary
//   lives in the `push ecx` frame slot.
// Label correction: catalogued as `CWideString_QuicksortPartition`; the body is STLport `__unguarded_partition`.
#include <string.h>                                 // _wcsicmp @ 0x00F3FB84 (CRT)

class CWideString { public:
    void* PStringData;
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    const CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    operator const wchar_t*() const;                    // 0x0099AF10
};

namespace NGameComponentHelpers {
class CCaseInsensitiveStringCompare { public:
    char m_tag;
    bool operator()(const CWideString& a, const CWideString& b) const { return _wcsicmp(a, b) < 0; }
};
}

typedef NGameComponentHelpers::CCaseInsensitiveStringCompare TCompare;

inline void __iter_swap_aux(CWideString* a, CWideString* b)
{
    CWideString tmp = *a;
    *a = *b;
    *b = tmp;
}

CWideString* __fastcall __unguarded_partition(CWideString* first, CWideString* last, CWideString pivot, TCompare comp)
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