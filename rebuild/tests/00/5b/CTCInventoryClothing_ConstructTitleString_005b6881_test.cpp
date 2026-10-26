// Behaviour test for 0x005B6881 CTCInventoryClothing::ConstructTitleString
#include <cstdio>
#include <cstring>
class CWideString { public:
    char buf[128];
    CWideString(const char* t) { std::strcpy(buf, t); }
    const CWideString& operator+=(const CWideString& o) { std::strcat(buf, o.buf); return *this; }
};
static CWideString IntToWideString(long v) { char b[32]; std::sprintf(b, "%ld", v); return CWideString(b); }
struct CDataBank { CWideString GetTextBySymbol(const char* s) { return CWideString(std::strcmp(s, "TEXT_GUI_MENU_CLOTHING_TOTAL_ARMOUR") == 0 ? "Total Armour" : "?"); } };
static CDataBank g_bank;
struct CClothing {
    long armour;
    long GetCurrentTotalArmorValue() { return armour; }
    void ConstructTitleString(CWideString& title)
    {
        {
            const char* symbol = "TEXT_GUI_MENU_CLOTHING_TOTAL_ARMOUR";
            CDataBank* textBank = &g_bank;
            title += textBank->GetTextBySymbol(symbol);
        }
        title += CWideString(" ");
        title += IntToWideString(GetCurrentTotalArmorValue());
    }
};
int main()
{
    CClothing c; c.armour = 42; CWideString t("> ");
    c.ConstructTitleString(t);
    if (std::strcmp(t.buf, "> Total Armour 42") != 0) { std::printf("005b6881_TEST FAIL\n"); return 1; }
    std::printf("005b6881_TEST PASS\n");
    return 0;
}