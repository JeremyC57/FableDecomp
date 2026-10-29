// std::__partial_sort(CWideString*, CWideString*, CWideString*, CWideString*, NGameComponentHelpers::CCaseInsensitiveStringCompare) -- STLport partial sort of wide strings ordered case-insensitively: heapify [first, middle), pop every smaller element of [middle, last) into the heap, then sort_heap
// Flags: /Gr
// Leaf: __partial_sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00497620, 146 bytes, `ret 0Ch` (__fastcall: first in ecx -> esi, middle in
//   edx -> ebx, then last, the value-type tag pointer and the empty comparator on the stack; the comparator is held in ebp).
// Callees: 0x00497350 __make_heap (same instantiation, `push 0 ; push 0` tag pointers), 0x0099AF10 CWideString::operator
//   const wchar_t*, 0x00F3FB84 _wcsicmp (CRT), 0x0099B720 CWideString copy ctor, 0x0099B7D0 CWideString::operator=,
//   0x00497160 __adjust_heap (landed _global::_Adjust_heap), 0x0099B510 ~CWideString, 0x00497570 sort_heap.
// Shape: STLport `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first))
//   __pop_heap(first, middle, i, _Tp(*i), comp, DISTANCE_TYPE); sort_heap(first, middle, comp);` with the inline
//   `__pop_heap` (`*result = *first; __adjust_heap(first, 0, last - first, val, comp);`): its by-value `_Tp(*i)` lands in
//   the dead comparator argument slot and is copied again into __adjust_heap's argument slot.
// Label correction: catalogued as `Heapsort_Main_CWideString`; the body is STLport `__partial_sort`.
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

void __fastcall __make_heap(CWideString* first, CWideString* last, TCompare comp, CWideString*, int*);      // 0x00497350
void __fastcall __adjust_heap(CWideString* first, int holeIndex, int len, CWideString val, TCompare comp); // 0x00497160
void __fastcall sort_heap(CWideString* first, CWideString* last, TCompare comp);                          // 0x00497570

inline void __pop_heap(CWideString* first, CWideString* last, CWideString* result, CWideString val, TCompare comp, int*)
{
    *result = *first;
    __adjust_heap(first, 0, int(last - first), val, comp);
}

void __fastcall __partial_sort(CWideString* first, CWideString* middle, CWideString* last, CWideString*, TCompare comp)
{
    __make_heap(first, middle, comp, (CWideString*)0, (int*)0);
    for (CWideString* i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, CWideString(*i), comp, (int*)0);
    sort_heap(first, middle, comp);
}