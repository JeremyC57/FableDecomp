// std::__make_heap(CWideString*, CWideString*, NGameComponentHelpers::CCaseInsensitiveStringCompare, CWideString*, int*) -- STLport make_heap over wide strings ordered case-insensitively: sift every parent down from (len-2)/2 to 0 through __adjust_heap @ 0x00497160
// Flags: /Gr
// Leaf: __make_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00497350, 73 bytes, `ret 0Ch` (__fastcall: first in ecx -> ebp, last in edx,
//   then the empty comparator, the value-type and distance-type tag pointers on the stack).
// Callees: 0x0099B720 CWideString copy ctor (the by-value `_Tp(*(first + parent))` built in the callee's argument slot:
//   `push ecx ; mov ecx,esp`), 0x00497160 __adjust_heap (landed as _global::_Adjust_heap, same instantiation).
// Shape: STLport `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2; for (;;) {
//   __adjust_heap(first, parent, len, _Tp(*(first + parent)), comp); if (parent == 0) return; parent--; }`; VC7.1
//   strength-reduces `first + parent` into edi.
// Label correction: catalogued as `Heapify_CWideString`; the body is STLport `__make_heap`.
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

void __fastcall __adjust_heap(CWideString* first, int holeIndex, int len, CWideString val, TCompare comp);   // 0x00497160

void __fastcall __make_heap(CWideString* first, CWideString* last, TCompare comp, CWideString*, int*)
{
    if (last - first < 2) return;
    int len = last - first;
    int parent = (len - 2) / 2;

    for (;;) {
        __adjust_heap(first, parent, len, CWideString(*(first + parent)), comp);
        if (parent == 0) return;
        parent--;
    }
}