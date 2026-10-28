// _global::_Push_heap(CWideString*, long, long, CWideString, NGameComponentHelpers::CCaseInsensitiveStringCompare) -- heap sift-up of wide strings ordered by _wcsicmp
// Leaf: _Push_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00497060, 124 bytes, `ret 0xc`.
// Called by the landed _Adjust_heap @ 0x00497160 (same instantiation; its model is reused here).
// Callees: 0x0099AF10 CWideString::operator const wchar_t*, 0x00F3FB84 _wcsicmp (CRT, __cdecl),
// 0x0099B7D0 CWideString::operator=, 0x0099B510 ~CWideString.
// Link-time __fastcall (/Gr algorithm TU): first in ecx, hole in edx, then top, the 4-byte string by
// value and the empty comparator (4 + 4 + 4 = 0xc); the callee destroys its by-value string at the
// end (`lea ecx,[esp+14h] ; call 0x0099B510`).
// CCaseInsensitiveStringCompare: `_wcsicmp(a, b) < 0` -- VC evaluates the arguments right to left, so
// the value's text is fetched before the parent's (`lea ecx,[esp+18h]` then `mov ecx,ebx`).
// Shape: SGI/STLport `__push_heap` (`parent = (hole - 1) / 2; while (hole > top && comp(first[parent], value))`).
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

void __fastcall _Push_heap(CWideString* first, long hole, long top, CWideString value, TCompare comp)
{
    long parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}