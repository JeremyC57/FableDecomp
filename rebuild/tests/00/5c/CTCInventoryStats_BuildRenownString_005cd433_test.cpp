// Behaviour test for 0x005CD433 CTCInventoryStats::BuildRenownString
#include <cstdio>
#include <cstring>
class CCharString { public:
    char buf[64];
    CCharString(const char* t) { std::strcpy(buf, t); }
    CCharString& operator+=(const CCharString& o) { std::strcat(buf, o.buf); return *this; }
};
static CCharString GFIntToCharString(long v) { char b[16]; std::sprintf(b, "%ld", v); return CCharString(b); }
struct CDataBank { CCharString GetTextBySymbol(const CCharString& s) { CCharString r("<"); r += s; r += CCharString(">"); return r; } };
struct CTCHeroRenown { long RenownClassLevel; };
static CCharString BuildRenownString(CDataBank* bank, CTCHeroRenown* renown)
{
    CCharString symbol("TEXT_GUI_STATS_RENOWNCLASS_LEVEL");
    symbol += GFIntToCharString(renown->RenownClassLevel);
    return bank->GetTextBySymbol(symbol);
}
int main()
{
    CDataBank bank; CTCHeroRenown r = { 7 };
    CCharString s = BuildRenownString(&bank, &r);
    if (std::strcmp(s.buf, "<TEXT_GUI_STATS_RENOWNCLASS_LEVEL7>") != 0) { std::printf("005cd433_TEST FAIL\n"); return 1; }
    std::printf("005cd433_TEST PASS\n");
    return 0;
}