#include <cstdio>
#include <string>
#include <cmath>
#include <cwchar>
// Behaviour test for CTCInventoryStats::GetFatnessAsString (0x005CC862): same body as src/, with the strings and the
// text bank modelled by std::string (the bank echoes the symbol).
class CCharString { public:
    std::string s;
    CCharString(const char* text, long = -1) : s(text) { }
    CCharString(const std::string& t) : s(t) { }
};
CCharString operator+(const CCharString& l, const CCharString& r) { return CCharString(l.s + r.s); }
class CWideString { public:
    std::string s;
    CWideString(const std::string& t) : s(t) { }
    CWideString(const wchar_t* text) { for (; *text; ++text) s += (char)*text; }
};
struct CDataBank { CWideString GetTextBySymbol(const CCharString& sym) { return CWideString(sym.s); } };
class CMainGameComponent { public: CDataBank* TextBank; };
static CDataBank g_bank; static CMainGameComponent g_mgc = { &g_bank };
static CMainGameComponent* g_pMainGameComponent = &g_mgc;
namespace NInventory {
struct CTCInventoryStats { CWideString GetFatnessAsString(float fatness); };
CWideString CTCInventoryStats::GetFatnessAsString(float fatness)
{
    CDataBank* bank = g_pMainGameComponent->TextBank;
    if (fatness < 0.2)
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("FATNESS1"));
    if (fatness >= 0.2 && fatness < 0.4)
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("FATNESS2"));
    if (fatness >= 0.4 && fatness < 0.6)
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("FATNESS3"));
    if (fatness >= 0.6 && fatness < 0.8)
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("FATNESS4"));
    if (fatness >= 0.8)
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("FATNESS5"));
    return CWideString(L"Unbelievably Fat");
}
}

int main()
{
    int fails = 0;
    NInventory::CTCInventoryStats st;
    const char* p = "TEXT_GUI_STATS_LABEL_FATNESS";
    struct { float f; char band; } cases[] = { { -1.0f, '1' }, { 0.0f, '1' }, { 0.19f, '1' }, { 0.2f, '2' }, { 0.21f, '2' },
        { 0.39f, '2' }, { 0.4f, '3' }, { 0.59f, '3' }, { 0.6f, '4' }, { 0.79f, '4' }, { 0.8f, '5' }, { 3.0f, '5' } };
    for (unsigned i = 0; i < sizeof cases / sizeof cases[0]; ++i)
        if (st.GetFatnessAsString(cases[i].f).s != std::string(p) + cases[i].band) { std::printf("case %u\n", i); ++fails; }
    union { unsigned long u; float f; } qnan; qnan.u = 0x7fc00000UL;            // quiet NaN (VC7.1 has no nanf)
    if (st.GetFatnessAsString(qnan.f).s != "Unbelievably Fat") ++fails;
    if (fails) { std::printf("005cc862_TEST FAIL %d\n", fails); return 1; }
    std::printf("005cc862_TEST PASS\n");
    return 0;
}