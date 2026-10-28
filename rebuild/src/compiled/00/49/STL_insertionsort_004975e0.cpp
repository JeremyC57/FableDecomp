// std::__insertion_sort(CWideString*, CWideString*, NGameComponentHelpers::CCaseInsensitiveStringCompare) -- STLport insertion sort of wide strings ordered case-insensitively: every element from first+1 goes through __linear_insert @ 0x00497500 with a by-value copy of itself
// Flags: /Gr
// Leaf: __insertion_sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004975E0, 56 bytes, `ret 4` (__fastcall: first in ecx -> ebx, last in edx ->
//   edi, the empty comparator's slot on the stack -> ebp; no value-type tag argument in this instantiation).
// Callees: 0x0099B720 CWideString copy ctor (the `_Tp(*i)` value built in __linear_insert's argument slot:
//   `push ebp ; push ecx ; mov ecx,esp`), 0x00497500 __linear_insert (same instantiation).
// Shape: STLport `if (first == last) return; for (i = first + 1; i != last; ++i) __linear_insert(first, i, _Tp(*i), comp);`
//   (VC7.1 rotates the loop: one test before it, one at the bottom).
// Label correction: catalogued as `VectorInsert_ConstructWideStrings`; the body is STLport `__insertion_sort`.
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

void __fastcall __linear_insert(CWideString* first, CWideString* last, CWideString val, TCompare comp);   // 0x00497500

void __fastcall __insertion_sort(CWideString* first, CWideString* last, TCompare comp)
{
    if (first == last) return;
    for (CWideString* i = first + 1; i != last; ++i)
        __linear_insert(first, i, CWideString(*i), comp);
}