// Test for GetHeroTitleAsLHTSTag (0x00CBEBA7): the hero's title index 0..20 maps to its LHTS tag suffix, anything else to "".
#include <cstdio>
#include <cstring>
static const char* GetHeroTitleAsLHTSTag(long title)
{
    switch (title) {
    case 0: return "_NONE"; case 1: return "_REAPER"; case 2: return "_SHADOWHUNTER"; case 3: return "_MALEFICUS";
    case 4: return "_DEATH"; case 5: return "_ASSASSIN"; case 6: return "_NECRO"; case 7: return "_AVATAR";
    case 8: return "_PILGRIM"; case 9: return "_LIBERATOR"; case 10: return "_PALADIN"; case 11: return "_DRUID";
    case 12: return "_RANGER"; case 13: return "_RUNE"; case 14: return "_HOOD"; case 15: return "_GLADIATOR";
    case 16: return "_SABRE"; case 17: return "_ARROW"; case 18: return "_PIE"; case 19: return "_CHICKEN";
    case 20: return "_ARSE"; default: return "";
    }
}
int main()
{
    bool ok = std::strcmp(GetHeroTitleAsLHTSTag(0), "_NONE") == 0 && std::strcmp(GetHeroTitleAsLHTSTag(7), "_AVATAR") == 0
           && std::strcmp(GetHeroTitleAsLHTSTag(20), "_ARSE") == 0 && std::strcmp(GetHeroTitleAsLHTSTag(21), "") == 0
           && std::strcmp(GetHeroTitleAsLHTSTag(-1), "") == 0;
    std::printf(ok ? "00cbeba7_TEST PASS\n" : "00cbeba7_TEST FAIL\n");
    return ok ? 0 : 1;
}