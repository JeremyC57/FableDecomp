// Behaviour test for CTCInventoryStats::GetCreatureGroupAsString @ 0x005CC9AD: first set group bit picks the label
#include <cstdio>
#include <cstring>
static const char* Pick(unsigned long groups)
{
    if (groups & 0x1) return "CRVILLAGERS";
    if (groups & 0x2) return "CRGUARDS";
    if (groups & 0x4) return "CRBANDITS";
    if (groups & 0x8) return "CRBALVERINES";
    if (groups & 0x10) return "CRWHITEBALVERINES";
    if ((groups & 0x20) || (groups & 0x40)) return "CRHOBBES";
    if (groups & 0x80) return "CRSCREAMERS";
    if (groups & 0x100) return "CRWASPS";
    if (groups & 0x200) return "CRSCORPIONS";
    if ((groups & 0x400) || (groups & 0x800)) return "CRUNDEAD";
    if (groups & 0x1000) return "CRRIVALHEROES";
    if (groups & 0x2000) return "CRNYMPHS";
    if (groups & 0x4000) return "CRMINIONS";
    if (groups & 0x8000) return "CREARTHTROLLS";
    if (groups & 0x10000) return "CRROCKTROLLS";
    if (groups & 0x20000) return "CRDRAGONS";
    if (groups & 0x40000) return "CRBEETLES";
    if (groups & 0x80000) return "CRSCORPIONKINGS";
    if (groups & 0x100000) return "CRKRAKEN";
    if (groups & 0x400000) return "CRWASPQUEENS";
    if (groups & 0x800000) return "CRFAMILYMEMBERS";
    if (groups & 0x1000000) return "CRBIRDS";
    if (groups & 0x4000000) return "CRSUMMONER";
    if (groups & 0x2000000) return "CRICETROLL";
    return "CROTHER";
}
static int Is(unsigned long g, const char* s)
{
    char buf[64];
    std::strcpy(buf, "TEXT_GUI_STATS_LABEL_");
    std::strcat(buf, Pick(g));
    char want[64];
    std::strcpy(want, "TEXT_GUI_STATS_LABEL_");
    std::strcat(want, s);
    return std::strcmp(buf, want) == 0;
}
int main()
{
    int ok = 1;
    if (!Is(0x3, "CRVILLAGERS")) ok = 0;
    if (!Is(0x40, "CRHOBBES")) ok = 0;
    if (!Is(0x800, "CRUNDEAD")) ok = 0;
    if (!Is(0x6000000, "CRSUMMONER")) ok = 0;
    if (!Is(0x2000000, "CRICETROLL")) ok = 0;
    if (!Is(0x200000, "CROTHER")) ok = 0;
    if (!Is(0, "CROTHER")) ok = 0;
    if (!ok) { std::printf("005cc9ad_TEST FAIL\n"); return 1; }
    std::printf("005cc9ad_TEST PASS\n");
    return 0;
}