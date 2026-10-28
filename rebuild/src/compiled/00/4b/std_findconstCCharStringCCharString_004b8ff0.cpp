// std::__find<const CCharString*, CCharString>(const CCharString* first, const CCharString* last, const CCharString& val, const random_access_iterator_tag&) -- STLport random-access find over a const CCharString range: four-way unrolled scan for the first string equal to val, else last
// Leaf: ??$__find@PBVCCharString@@V1@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004B8FF0, 427 bytes, `__fastcall` (first ecx, last edx), `ret 8` (the value reference
//   + the empty tag reference).
// Identity (probable): byte-identical to the landed __find<CCharString*, CCharString> 0x004B8E40, but a separate COMDAT
//   with its own callers; the first caller is CQuestManager::IsQuestRegistered 0x004B00C0, which the PDB name set spells
//   const (`?IsQuestRegistered@CQuestManager@@QBE_NABVCCharString@@@Z`), so its `find(v.begin(), v.end(), name)` runs
//   on const_iterators = `const CCharString*`, while the non-const OnQuestFinishScreenDisplayed 0x004B0160 calls
//   0x004B8E40. Other callers: 0x004B9E4A, CTCTalk 0x006E2D1E / 0x006E3616, 0x0092F5F2 / 0x0092F67F.
//   The catalog label "vector_find_pair_by_string" is descriptive.
// Callees: CBasicString<char>::Compare(const char*, const char*) @ 0x00411570 (`__fastcall`, the inlined operator==).
// Shape: the landed 0x004B8E40 source verbatim with const iterators (SGI/STLport `__find`, trip count `>> 2`, unrolled
//   loop, fall-through switch); Compare's real body sits above under `#pragma auto_inline(off)` so VC7.1 knows the call
//   writes no memory.
template <class T> class CBasicString {
protected:
    static long __fastcall Compare(const T* a, const T* b);   // 0x00411570 (?Compare@?$CBasicString@D@@KIJPBD0@Z)
};

#pragma auto_inline(off)
template <> long __fastcall CBasicString<char>::Compare(const char* a, const char* b)   // the landed 0x00411570 body
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

class CCharStringData { public:                  // the shared string block
    const char*   Chars;                  // +0x00
    unsigned long Length;                 // +0x04
};

class CCharString : public CBasicString<char> {
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

template const CCharString* __fastcall __find<const CCharString*, CCharString>(const CCharString*, const CCharString*,
                                                                                const CCharString&,
                                                                                const random_access_iterator_tag&);
}