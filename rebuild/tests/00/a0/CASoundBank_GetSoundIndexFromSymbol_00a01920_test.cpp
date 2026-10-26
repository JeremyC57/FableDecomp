#include <cstdio>
// Model of CASoundBank::GetSoundIndexFromSymbol @ 0x00A01920: 0 without a table, else the table's lookup result.
class CCharString { public: const char* s; };
struct CSoundSymbolTable { long answer; void Lookup(const CCharString& sym, long* index) { if (sym.s[0] == 'A') *index = answer; } };
class CASoundBank { public:
    CSoundSymbolTable* m_pSymbolTable;
    long GetSoundIndexFromSymbol(const CCharString& symbol) { long index = 0; if (m_pSymbolTable != 0) m_pSymbolTable->Lookup(symbol, &index); return index; }
};
int main()
{
    CSoundSymbolTable t = { 17 }; CASoundBank b = { 0 }; CCharString a = { "A" }, z = { "Z" };
    if (b.GetSoundIndexFromSymbol(a) != 0) { std::printf("00a01920_TEST FAIL\n"); return 1; }
    b.m_pSymbolTable = &t;
    if (b.GetSoundIndexFromSymbol(a) != 17 || b.GetSoundIndexFromSymbol(z) != 0) { std::printf("00a01920_TEST FAIL\n"); return 1; }
    std::printf("00a01920_TEST PASS\n"); return 0;
}