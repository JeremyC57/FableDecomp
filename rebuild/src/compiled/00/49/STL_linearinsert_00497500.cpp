// _STL::__linear_insert(CWideString*, CWideString*, CWideString, NGameComponentHelpers::CCaseInsensitiveStringCompare) -- STLport insertion step over wide strings ordered case-insensitively: a value below *first shifts [first, last) up one slot and lands at first, else the unguarded linear insert @ 0x00496FD0
// Flags: /Gr
// Leaf: __linear_insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00497500, 112 bytes, `ret 8` (__fastcall: first in ecx -> esi, last in edx ->
//   edi, the 4-byte value string by value and the empty comparator's slot on the stack; the callee destroys its value).
// Callees: 0x0099AF10 CWideString::operator const wchar_t*, 0x00F3FB84 _wcsicmp (CRT), 0x00497460 copy_backward
//   (__fastcall first/last in ecx/edx, result on the stack), 0x0099B7D0 CWideString::operator=, 0x0099B720 CWideString
//   copy ctor (the by-value copy for the unguarded insert built in its argument slot), 0x00496FD0 __unguarded_linear_insert
//   (landed, same instantiation), 0x0099B510 ~CWideString.
// Shape: STLport `if (comp(val, *first)) { copy_backward(first, last, last + 1); *first = val; }
//   else __unguarded_linear_insert(last, val, comp);`; the two arms each destroy the by-value string (tail-duplicated).
// Label correction: catalogued as `CWideString_PartitionWithMedian`; the body is STLport `__linear_insert`.
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

CWideString* __fastcall copy_backward(CWideString* first, CWideString* last, CWideString* result);    // 0x00497460
void __fastcall __unguarded_linear_insert(CWideString* last, CWideString val, TCompare comp);         // 0x00496FD0

void __fastcall __linear_insert(CWideString* first, CWideString* last, CWideString val, TCompare comp)
{
    if (comp(val, *first)) {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}