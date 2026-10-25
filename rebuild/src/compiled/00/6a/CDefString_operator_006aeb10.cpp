// CDefString::operator==(const CCharString& text) const -- whether the def-table string equals a CCharString, ignoring case (_stricmp; a null string compares as "")
// Leaf: ??8CDefString@@QBE_NABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AEB10, 50 bytes (+ int3 padding), thiscall `ret 4`.
// Identity: single caller 0x00895411, the landed CGameScriptInterface::IsHeroPerformingExpression 0x008953A0, which
//   compares the current expression's GetDefName() result against the script's name through this address
//   (`??8CDefString@@QBE_NABVCCharString@@@Z` is in Ego_r.pdb / FableWin.pdb). The catalog label
//   "std::_Iterator_base12::_Getcont" is propagated and wrong.
// Callees: CDefStringTable::GetStringAsCharArray(long) @ 0x009D49F0 on the global table @ 0x013CA828, _stricmp @
//   0x00BFEBA8 (MSVCR71 import thunk; verified against the IAT slot 0x014400EC).
// Shape: `_stricmp(table.GetStringAsCharArray(TablePos), text.GetChars()) == 0` -- VC evaluates the right-hand argument
//   first (the null-case-first CCharString accessor, "" @ 0x0129AAF4), then the table call; `== 0` is `neg/sbb/inc`.
// Note: the landed twin 0x004B7980 (strcmp intrinsic, 107 bytes) carries the same symbol; one of the two is mislabelled
//   (two different bodies cannot share one inline symbol) -- see NOTES.md / LESSONS.md.
extern "C" int __cdecl _stricmp(const char* a, const char* b);   // 0x00BFEBA8

struct CCharStringData {
    const char*   m_pChars;                         // +0x00
    unsigned long m_length;                         // +0x04
};
extern const char g_emptyLiteral_0129aaf4[];        // 0x0129AAF4: ""
class CCharString {                                 // PDB def types are classes (mangled V)
public:
    CCharStringData* m_pStringData;
    const char* GetChars() const { return m_pStringData == 0 ? g_emptyLiteral_0129aaf4 : m_pStringData->m_pChars; }
};

struct CDefStringTable {
    const char* GetStringAsCharArray(long id);      // 0x009D49F0
};

extern CDefStringTable g_DefStringTable;            // 0x013CA828

class CDefString {
public:
    long TablePos;                                  // +0 (PDB CDefString: 4 bytes)
    bool operator==(const CCharString& text) const;
};

bool CDefString::operator==(const CCharString& text) const
{
    return _stricmp(g_DefStringTable.GetStringAsCharArray(TablePos), text.GetChars()) == 0;
}