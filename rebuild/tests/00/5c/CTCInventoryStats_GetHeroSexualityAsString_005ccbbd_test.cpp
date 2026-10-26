// Behaviour test for 0x005CCBBD CTCInventoryStats::GetHeroSexualityAsString -- symbol chosen per sexuality value.
#include <stdio.h>
#include <string.h>
class CCharString { public:
    char text[64];
    CCharString(const char* s, long = -1) { strcpy(text, s); }
};
static CCharString operator+(const CCharString& l, const CCharString& r) { CCharString out(l.text); strcat(out.text, r.text); return out; }
class CWideString { public: char text[64]; };
struct CDataBank {
    CWideString GetTextBySymbol(const CCharString& sym) { CWideString w; strcpy(w.text, "<"); strcat(w.text, sym.text); strcat(w.text, ">"); return w; }
};
class CMainGameComponent { public: unsigned char _pad[0x14]; CDataBank* TextBank; };
static CMainGameComponent* g_pMainGameComponent;
enum EHeroSexuality { SEX_UNKNOWN_VALUE = 0, SEX_STRAIGHT_VALUE = 1, SEX_BI_VALUE = 2, SEX_GAY_VALUE = 3 };
class CTCInventoryStats { public: CWideString GetHeroSexualityAsString(long sexuality); };
CWideString CTCInventoryStats::GetHeroSexualityAsString(long sexuality)
{
    CDataBank* bank = g_pMainGameComponent->TextBank;
    if (sexuality == SEX_UNKNOWN_VALUE)
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("SEX_UNKNOWN"));
    if (sexuality == SEX_BI_VALUE)
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("SEX_BI"));
    if (sexuality == SEX_GAY_VALUE)
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("SEX_GAY"));
    if (sexuality == SEX_STRAIGHT_VALUE)
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("SEX_STRAIGHT"));
    return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("NONE"));
}
int main()
{
    CDataBank bank; CMainGameComponent comp; comp.TextBank = &bank; g_pMainGameComponent = &comp;
    CTCInventoryStats stats;
    static const char* want[] = { "<TEXT_GUI_STATS_LABEL_SEX_UNKNOWN>", "<TEXT_GUI_STATS_LABEL_SEX_STRAIGHT>",
                                  "<TEXT_GUI_STATS_LABEL_SEX_BI>", "<TEXT_GUI_STATS_LABEL_SEX_GAY>", "<TEXT_GUI_STATS_LABEL_NONE>" };
    long values[] = { 0, 1, 2, 3, 7 };
    bool ok = true;
    for (int i = 0; i < 5; ++i) {
        CWideString w = stats.GetHeroSexualityAsString(values[i]);
        if (strcmp(w.text, want[i]) != 0) ok = false;
    }
    if (!ok) { printf("005ccbbd_TEST FAIL\n"); return 1; }
    printf("005ccbbd_TEST PASS\n"); return 0;
}