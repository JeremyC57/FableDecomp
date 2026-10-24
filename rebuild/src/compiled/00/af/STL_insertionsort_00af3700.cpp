// _STL::__insertion_sort(SNamedPairRecord* first, SNamedPairRecord* last, CKeyPairCompareLess comp) -- STLport insertion sort over 16-byte pair<CCharString, {long, long, CCharString}> records: every element after the first goes through the out-of-line __linear_insert(first, i, *i, comp)
// Leaf: __insertion_sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF3700, 100 bytes, `ret 4` (__fastcall: first in ecx, last in edx, the
// 4-byte comparator on the stack -- the SGI/STLport algorithm TUs are /Gr).
// Callees: 0x00AF35A0 __linear_insert(first, last, value, comp) (manifest "TreeNode_InsertSorted" is propagated; only
// this body calls it), 0x0099EC30 CCharString::CCharString(const CCharString&) (x2).
// Element: the callee orders on the CCharString at +0 (CBasicString<char>::operator< @ 0x00429950 behind the shared-block
// pointer tests), then assigns the string, two dwords and a second string at +0xC -- 16 bytes, two strings.
// Label: catalogued as `_Insertion_sort1<pair<float,CCharString>*, CKeyPairCompareLess<float,CCharString> >`
// (propagated; a pair<float, CCharString> is 8 bytes); the code is STLport's `__insertion_sort`.
// Shape: STLport's explicit pair copy constructor builds the by-value `*i` in the argument area (`sub esp,10h ; mov
// esi,esp`): the key string, then the nested second's implicit memberwise copy (two dwords and the string), its
// sources read through a second induction pointer at the inner string (`lea ebx,[edi+0Ch]`, `[ebx-8]`, `[ebx-4]`).
struct CCharString {
    void* m_pData;                                      // shared string block
    CCharString(const CCharString& other);              // 0x0099EC30
};

struct SNamedPayload {                                  // second half of the record (implicit memberwise copy)
    long m_value0;                                      // +0x00
    long m_value1;                                      // +0x04
    CCharString m_name;                                 // +0x08
};

struct SNamedPairRecord {                               // STLport std::pair<CCharString, SNamedPayload>, 16 bytes
    CCharString first;                                  // +0x00 key
    SNamedPayload second;                               // +0x04
    SNamedPairRecord(const SNamedPairRecord& o) : first(o.first), second(o.second) { }
};

struct CKeyPairCompareLess {                            // 4-byte comparator slot passed by value
    void* m_context;
};

void __fastcall __linear_insert(SNamedPairRecord* first, SNamedPairRecord* last, SNamedPairRecord value, CKeyPairCompareLess comp);   // 0x00AF35A0

void __fastcall __insertion_sort(SNamedPairRecord* first, SNamedPairRecord* last, CKeyPairCompareLess comp)
{
    if (first == last)
        return;
    for (SNamedPairRecord* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}