// std::__insertion_sort(SSymbolRecord* first, SSymbolRecord* last, CCompareString comp) -- STLport insertion sort over 16-byte pair<CCharString, CSymbolMap::CSymbolInfo> records: every element after the first goes through the out-of-line __linear_insert(first, i, *i, comp)
// Leaf: __insertion_sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E7590, 110 bytes, `ret 4` (__fastcall: first in ecx, last in edx, the
// 4-byte comparator on the stack -- the SGI/STLport algorithm TUs are /Gr).
// Callees: 0x009E6FA0 __linear_insert(first, last, value, comp) (landed as SymbolMapSort_AddGossip_009e6fa0; manifest
// "CGameScriptInterface::AddGossip" is propagated), 0x0099EC30 CCharString::CCharString(const CCharString&) (x3).
// Element: pair<CCharString name, CSymbolMap::CSymbolInfo { CCharString GroupName; CCharString EntryName; long Val; }>,
// the model of the landed callee.
// Label: catalogued as `_Insertion_sort1<pair<CCharString,CSymbolMap::CSymbolInfo>*, CCompareString<..> >` (Dinkumware
// name); the code is STLport's `__insertion_sort`.
// Shape: STLport's explicit pair copy constructor builds the by-value `*i` in the argument area (`sub esp,10h ; mov
// ebx,esp`): the key's copy constructor, then CSymbolInfo's IMPLICIT memberwise copy inlined with its `this` stepped
// into ebx (`add ebx,4`) -- two string copies and the long, the sources read through a second induction pointer
// (`lea edi,[esi+0Ch]`). The `push ecx` frame word keeps `first`.
class CCharString { public:
    void* m_pData;                                      // shared string block
    CCharString(const CCharString& other);              // 0x0099EC30
};

struct CSymbolInfo {                                    // CSymbolMap::CSymbolInfo, 12 bytes (implicit copy)
    CCharString GroupName;                              // +0x00
    CCharString EntryName;                              // +0x04
    long Val;                                           // +0x08
};

struct SSymbolRecord {                                  // STLport std::pair<CCharString, CSymbolInfo>, 16 bytes
    CCharString first;                                  // +0x00 key
    CSymbolInfo second;                                 // +0x04
    SSymbolRecord(const SSymbolRecord& o) : first(o.first), second(o.second) { }
};

struct CCompareString {                                 // 4-byte comparator slot passed by value
    void* m_context;
};

void __fastcall __linear_insert(SSymbolRecord* first, SSymbolRecord* last, SSymbolRecord value, CCompareString comp);   // 0x009E6FA0

void __fastcall __insertion_sort(SSymbolRecord* first, SSymbolRecord* last, CCompareString comp)
{
    if (first == last)
        return;
    for (SSymbolRecord* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}