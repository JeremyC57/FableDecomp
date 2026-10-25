// std::__find<CCharString*, CCharString>(CCharString* first, CCharString* last, const CCharString& val, const random_access_iterator_tag&) -- STLport random-access find: four-way unrolled scan of a CCharString range for the first string equal to val, else last
// Leaf: ??$__find@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004B8E40, 432 bytes, `__fastcall` (first ecx, last edx), `ret 8` (the value
//   reference + the empty tag reference). Callers: 12 call sites (0x004B0179, 0x004B29DA, 0x004B2FFB, 0x004B3827,
//   0x004B3AA1, 0x004B9E2A, 0x0076ED61, 0x007FEA51, 0x007FEAB5, 0x00AEBB53, 0x00AEBCFE, 0x00AEC8FF) -- `find(v.begin(),
//   v.end(), name)` over a vector<CCharString>. The catalog label "VectorFind" is descriptive.
// Callees: Compare(const char*, const char*) @ 0x00411570 (`__fastcall`, the inlined CCharString operator==).
// Layout: CCharString {CCharStringData* PStringData} (4 bytes, `>> 2` per element, `>> 4` trip count); the data block
//   {const char* chars +0, unsigned long length +4} (the landed 0x0099E900 / 0x0075D2B0 model).
// Shape: SGI/STLport `__find` verbatim (`__trip_count = (__last - __first) >> 2`, the four-way unrolled loop, then the
//   fall-through switch on `__last - __first`). The value's data block is loaded ONCE before the unrolled loop (esi)
//   but re-read in the switch: Compare's real body (landed 0x00411570) sits above under `#pragma auto_inline(off)`, so
//   VC7.1 knows the call writes no memory. The equality `Compare(..) ? false : true` gives `neg ; sbb al,al ; inc al`.

#pragma auto_inline(off)
long __fastcall Compare(const char* a, const char* b)   // 0x00411570 (the landed body)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)

struct CCharStringData {                  // the shared string block
    const char*   Chars;                  // +0x00
    unsigned long Length;                 // +0x04
};

class CCharString {
public:
    CCharStringData* PStringData;
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData && PStringData->Length == other.PStringData->Length)
            return Compare(PStringData->Chars, other.PStringData->Chars) ? false : true;
        return false;
    }
};

namespace std {
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

template <class _RandomAccessIter, class _Tp>
_RandomAccessIter __fastcall __find(_RandomAccessIter __first, _RandomAccessIter __last, const _Tp& __val,
                                    const random_access_iterator_tag&)
{
    int __trip_count = (__last - __first) >> 2;

    for ( ; __trip_count > 0 ; --__trip_count) {
        if (*__first == __val) return __first;
        ++__first;

        if (*__first == __val) return __first;
        ++__first;

        if (*__first == __val) return __first;
        ++__first;

        if (*__first == __val) return __first;
        ++__first;
    }

    switch (__last - __first) {
    case 3:
        if (*__first == __val) return __first;
        ++__first;
    case 2:
        if (*__first == __val) return __first;
        ++__first;
    case 1:
        if (*__first == __val) return __first;
        ++__first;
    case 0:
    default:
        return __last;
    }
}

template CCharString* __fastcall __find<CCharString*, CCharString>(CCharString*, CCharString*, const CCharString&,
                                                                    const random_access_iterator_tag&);
}