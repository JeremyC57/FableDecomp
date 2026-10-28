// _global::_Adjust_heap(CWideString*, long, long, CWideString, NGameComponentHelpers::CCaseInsensitiveStringCompare) -- heap sift-down of wide strings ordered by _wcsicmp, then _Push_heap @ 0x00497060
// Flags: /Gr
// Leaf: _Adjust_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00497160, 160 bytes.
// Callees: 0x0099AF10 CWideString::operator const wchar_t* (byte twin of the CCharString accessor,
// label propagated), 0x00F3FB84 _wcsicmp (CRT, __cdecl), 0x0099B7D0 CWideString::operator=,
// 0x0099B720 CWideString copy ctor, 0x0099B510 ~CWideString, 0x00497060 _Push_heap (same
// instantiation).  Link-time __fastcall: first in ecx, hole in edx, then bottom, the 4-byte string
// by value (copy-constructed in the _Push_heap slot: `push ecx ; mov ecx,esp`) and the empty
// comparator; the by-value string is destroyed by this callee at the end.
// CCaseInsensitiveStringCompare: `_wcsicmp(a, b) < 0` (the second argument is evaluated first).
// Shape: VC7.1 <algorithm> `_Adjust_heap`; the bottom case reuses idx (`first[idx - 1]`).
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

void __fastcall _Push_heap(CWideString* first, long hole, long top, CWideString value, TCompare pred);   // 0x00497060

void __fastcall _Adjust_heap(CWideString* first, long hole, long bottom, CWideString value, TCompare pred)
{
    long top = hole;
    long idx = 2 * hole + 2;
    for (; idx < bottom; idx = 2 * idx + 2) {
        if (pred(first[idx], first[idx - 1]))
            --idx;
        first[hole] = first[idx], hole = idx;
    }
    if (idx == bottom) {
        first[hole] = first[idx - 1];
        hole = idx - 1;
    }
    _Push_heap(first, hole, top, value, pred);
}