// Behaviour test for 0x0072DB00 CTCInventoryMagic::GetScreenName
#include <cstdio>
#include <cstring>
struct CWideString { char buf[32]; };
struct CGroupEntry {
    long refs; const char* text;
    CWideString GetText() const { CWideString w; std::strcpy(w.buf, text); return w; }
};
struct CDataBank {
    CGroupEntry entries[4];
    CGroupEntry* GetPGroupEntry(unsigned long tag, bool) { ++entries[tag].refs; return &entries[tag]; }
};
struct CInventoryDef { long refs; long MagicScreenNameTag; };
static CWideString GetScreenName(CDataBank* bank, CInventoryDef* pdef)
{
    ++pdef->refs;
    CGroupEntry* e = bank->GetPGroupEntry(pdef->MagicScreenNameTag, true);
    CWideString r = e->GetText();
    --e->refs;
    --pdef->refs;
    return r;
}
int main()
{
    CDataBank bank = { { { 0, "a" }, { 0, "b" }, { 0, "Magic" }, { 0, "d" } } };
    CInventoryDef def = { 1, 2 };
    CWideString w = GetScreenName(&bank, &def);
    bool ok = std::strcmp(w.buf, "Magic") == 0 && def.refs == 1 && bank.entries[2].refs == 0;
    if (!ok) { std::printf("0072db00_TEST FAIL\n"); return 1; }
    std::printf("0072db00_TEST PASS\n");
    return 0;
}
