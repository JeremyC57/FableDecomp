// std::__insertion_sort(SNamedRecord* first, SNamedRecord* last, SNameLess comp) -- STLport insertion sort over 0x38-byte pair<CCharString, C3DDummyObject-sized POD> records: every element after the first goes through the out-of-line __linear_insert(first, i, *i, comp)
// Leaf: __insertion_sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A71810, 97 bytes, `ret 4` (__fastcall: first in ecx, last in edx, the
// 4-byte comparator on the stack -- the SGI/STLport algorithm TUs are /Gr).
// Callees: 0x00A70FF0 __linear_insert(first, last, value, comp) (landed as global_linearinsert_00a70ff0; manifest
// "CEnvironmentSpellEffectInterface::SetRadialBlurParams" is propagated), 0x0099EC30 CCharString::CCharString(const
// CCharString&).
// Label: catalogued as `_Insertion_sort1<pair<CCharString,C3DDummyObject>*, CCompareString<C3DDummyObject> >`
// (Dinkumware name); the code is STLport's `__insertion_sort`. Element model shared with the landed callee.
// Shape: STLport's explicit pair copy constructor builds the by-value `*i` in the argument area (`sub esp,38h ; mov
// esi,esp`): the string copy constructor, then the 52-byte POD second as a 13-dword `rep movsd` from a second
// induction pointer (`lea ebp,[ebx+4]`); with ebp taken the comparator is re-read from its slot every pass.
struct CCharString {
    void* m_pData;                                      // shared string block
    CCharString(const CCharString& other);              // 0x0099EC30
};

struct SRecordData {                                    // 52 bytes, POD (C3DDummyObject-sized payload)
    long m_words[13];
};

struct SNamedRecord {                                   // STLport std::pair<CCharString, SRecordData>, 0x38 bytes
    CCharString first;                                  // +0x00 key
    SRecordData second;                                 // +0x04
    SNamedRecord(const SNamedRecord& o) : first(o.first), second(o.second) { }   // STLport's explicit pair copy constructor
};

struct SNameLess {                                      // 4-byte comparator slot passed by value
    void* m_context;
};

void __fastcall __linear_insert(SNamedRecord* first, SNamedRecord* last, SNamedRecord value, SNameLess comp);   // 0x00A70FF0

void __fastcall __insertion_sort(SNamedRecord* first, SNamedRecord* last, SNameLess comp)
{
    if (first == last)
        return;
    for (SNamedRecord* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}