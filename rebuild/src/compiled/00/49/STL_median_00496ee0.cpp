// std::__median(const CWideString&, const CWideString&, const CWideString&, NGameComponentHelpers::CCaseInsensitiveStringCompare) -- STLport median of three wide strings ordered case-insensitively (_wcsicmp): b when a<b<c, else c when a<c, else a; symmetrically when !(a<b)
// Flags: /Gr
// Leaf: __median
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00496EE0, 175 bytes, `ret 8` (__fastcall: a in ecx -> edi, b in edx -> esi,
//   c on the stack -> ebx, then the empty comparator's 4-byte slot; the STLport algorithm TUs are /Gr).
// Callees: 0x0099AF10 CWideString::GetAsCharArray() (byte twin of the CCharString accessor, label propagated),
//   0x00F3FB84 _wcsicmp (CRT, __cdecl). Same models as the landed _Adjust_heap 0x00497160 / __unguarded_linear_insert
//   0x00496FD0 of this sort instantiation.
// Shape: the five compares of STLport's `__median` in source order, each `_wcsicmp(x, y) < 0` converting the SECOND string
//   first (right-to-left argument evaluation); the result is the element's address (const T&).
// Label correction: catalogued as `CCharString_MedianOfThree`; the strings are CWideStrings (the _wcsicmp comparator and
//   the CWideString copy constructor in the callers 0x00497740), the body is STLport `__median`.
#include <string.h>                                 // _wcsicmp @ 0x00F3FB84 (CRT)

class CWideString { public:
    void* PStringData;
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    const CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    const wchar_t* GetAsCharArray() const;                    // 0x0099AF10
};

namespace NGameComponentHelpers {
class CCaseInsensitiveStringCompare { public:
    char m_tag;
    bool operator()(const CWideString& a, const CWideString& b) const { return _wcsicmp(a.GetAsCharArray(), b.GetAsCharArray()) < 0; }
};
}

typedef NGameComponentHelpers::CCaseInsensitiveStringCompare TCompare;

const CWideString& __fastcall __median(const CWideString& a, const CWideString& b, const CWideString& c, TCompare comp)
{
    if (comp(a, b)) {
        if (comp(b, c))
            return b;
        else if (comp(a, c))
            return c;
        else
            return a;
    }
    else if (comp(a, c))
        return a;
    else if (comp(b, c))
        return c;
    else
        return b;
}