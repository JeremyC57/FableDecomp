// _STL::__unguarded_linear_insert(CWideString* last, CWideString val, NGameComponentHelpers::CCaseInsensitiveStringCompare comp) -- SGI unguarded linear insert of a wide string: shift every predecessor that compares case-insensitively greater (_wcsicmp) up one slot, store the value in the hole, destroy the by-value copy
// Leaf: __unguarded_linear_insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00496FD0, 106 bytes, `ret 8` (__fastcall: last in ecx; the 4-byte string by
// value and the empty comparator's slot on the stack -- the SGI/STLport algorithm TUs are /Gr; the callee destroys its
// by-value string).
// Callees: 0x0099AF10 CWideString::operator const wchar_t* (byte twin of the CCharString accessor, label propagated),
// 0x00F3FB84 _wcsicmp (CRT), 0x0099B7D0 CWideString::operator=, 0x0099B510 ~CWideString. Same models as the landed
// _Adjust_heap 0x00497160 of this TU.
// Shape: `next = last; --next; while (comp(val, *next)) { *last = *next; last = next; --next; } *last = val;` with
// CCaseInsensitiveStringCompare `_wcsicmp(a, b) < 0` (the second argument converted first); VC7.1 rotates the loop
// (one test before it, one at the bottom).
// Label correction: catalogued as `_Med3<CWideString*, NGameComponentHelpers::CCaseInsensitiveStringCompare>`
// (propagated); the body is the SGI unguarded linear insert of that instantiation.
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

void __fastcall __unguarded_linear_insert(CWideString* last, CWideString val, TCompare comp)
{
    CWideString* next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}