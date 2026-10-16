// Behaviour test for 0x005AE972 CTCInventoryTrade::OpenActualBuyInventoryPC
#include <cstdio>
#include <cstring>
struct CCharString {
    const char* PStringData;
    bool operator==(const char* text) const
    {
        if (PStringData == 0) return std::strcmp(text, "") == 0;
        return std::strcmp(PStringData, text) == 0;
    }
};
struct CComponent { const char* name; CCharString GetName() const { CCharString s = { name }; return s; } };
struct CTrade {
    CComponent* PCurrentOpenTradeMenu; int deleted, buy, special;
    void DeleteComponents() { ++deleted; }
    void ConstructActualBuyInventoryPC() { ++buy; }
    void ConstructActualSpecialInventoryPC() { ++special; }
    void Open()
    {
        DeleteComponents();
        bool topLevel = PCurrentOpenTradeMenu->GetName() == "PC_TOP_LEVEL_TRADE_LIST";
        if (topLevel) ConstructActualBuyInventoryPC(); else ConstructActualSpecialInventoryPC();
    }
};
int main()
{
    CComponent top = { "PC_TOP_LEVEL_TRADE_LIST" }, other = { "PC_SPECIAL" }, unnamed = { 0 };
    CTrade t = { &top, 0, 0, 0 };
    t.Open(); t.PCurrentOpenTradeMenu = &other; t.Open(); t.PCurrentOpenTradeMenu = &unnamed; t.Open();
    bool ok = t.deleted == 3 && t.buy == 1 && t.special == 2;
    if (!ok) { std::printf("005ae972_TEST FAIL\n"); return 1; }
    std::printf("005ae972_TEST PASS\n");
    return 0;
}