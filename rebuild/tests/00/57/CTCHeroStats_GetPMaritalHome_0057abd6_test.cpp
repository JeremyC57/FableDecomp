#include <cstdio>
// Behaviour of CTCHeroStats::GetPMaritalHome (0x0057ABD6): the first marriage to this wife whose home UID resolves to a
// building (type 3); records for other wives, missing homes and non-buildings are skipped; null when none.
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
struct CThing { unsigned char ThingType; u64 UniqueID; };
static CThing* g_things[4]; static int g_lookups = 0;
struct CWorld {
    CThing* GetThingFromUID(u64 uid)
    {
        ++g_lookups;
        for (int i = 0; i < 4; ++i)
            if (g_things[i] && g_things[i]->UniqueID == uid) return g_things[i];
        return 0;
    }
};
static CWorld g_world;
struct CHeroMarriageInfoNode { CHeroMarriageInfoNode* Next; u64 WifeUID; u64 MaritalHomeUID; };
struct CTCHeroStats_Retail {
    CHeroMarriageInfoNode* Marriages_Head;
    CWorld* GetWorld() const { return &g_world; }
    CThing* GetPMaritalHome(const CThing* wife) const
    {
        CHeroMarriageInfoNode* head = Marriages_Head;
        for (CHeroMarriageInfoNode* node = head->Next; node != head; node = node->Next) {
            if (node->WifeUID == wife->UniqueID) {
                CThing* home = GetWorld()->GetThingFromUID(node->MaritalHomeUID);
                if (home && home->ThingType == 3)
                    return home;
            }
        }
        return 0;
    }
};
int main()
{
    CThing wife; wife.ThingType = 1; wife.UniqueID = ((u64)7 << 32) | 5;
    CThing other; other.ThingType = 1; other.UniqueID = 5;               // same low word, different wife
    CThing shed; shed.ThingType = 2; shed.UniqueID = 100;
    CThing house; house.ThingType = 3; house.UniqueID = 200;
    g_things[0] = &shed; g_things[1] = &house; g_things[2] = 0; g_things[3] = 0;
    CHeroMarriageInfoNode head, a, b, c, d;
    head.Next = &a; a.Next = &b; b.Next = &c; c.Next = &d; d.Next = &head;
    a.WifeUID = other.UniqueID; a.MaritalHomeUID = 200;    // other wife
    b.WifeUID = wife.UniqueID; b.MaritalHomeUID = 999;     // home gone
    c.WifeUID = wife.UniqueID; c.MaritalHomeUID = 100;     // not a building
    d.WifeUID = wife.UniqueID; d.MaritalHomeUID = 200;     // the house
    CTCHeroStats_Retail s; s.Marriages_Head = &head;
    if (s.GetPMaritalHome(&wife) != &house || g_lookups != 3) { std::printf("0057abd6_TEST FAIL\n"); return 1; }
    CThing stranger; stranger.ThingType = 1; stranger.UniqueID = 12345;
    if (s.GetPMaritalHome(&stranger) != 0) { std::printf("0057abd6_TEST FAIL\n"); return 1; }
    head.Next = &head;
    if (s.GetPMaritalHome(&wife) != 0) { std::printf("0057abd6_TEST FAIL\n"); return 1; }
    std::printf("0057abd6_TEST PASS\n");
    return 0;
}