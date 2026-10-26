// Behaviour test for 0x005FBD20 CTCInventoryMap::TeleportHeroToTeleporter
#include <cstdio>
static int g_teleports = 0;
class CTCTeleporter { public: void TeleportHero() { ++g_teleports; } };
class CThing { public:
    unsigned int m_presentBits[9]; CTCTeleporter* tc; unsigned char IsDead;
    bool GetInterface(CTCTeleporter** out, long t)
    {
        if (m_presentBits[t >> 5] & (1u << (t & 31))) { *out = tc; return true; }
        return false;
    }
};
static void TeleportHeroToTeleporter(CThing* hero, CThing* teleporter)
{
    if (hero && !hero->IsDead) {
        CTCTeleporter* teleporterTC;
        if (teleporter->GetInterface(&teleporterTC, 0xDD))
            teleporterTC->TeleportHero();
    }
}
int main()
{
    CTCTeleporter tc; CThing hero = { { 0 }, 0, 0 }; CThing tele = { { 0 }, &tc, 0 };
    TeleportHeroToTeleporter(&hero, &tele);              // no component
    tele.m_presentBits[0xDD >> 5] = 1u << (0xDD & 31);
    if (0xDD >> 5 != 6 || (1u << (0xDD & 31)) != 0x20000000u) { std::printf("005fbd20_TEST FAIL\n"); return 1; }
    TeleportHeroToTeleporter(&hero, &tele);              // teleports
    hero.IsDead = 1; TeleportHeroToTeleporter(&hero, &tele);
    TeleportHeroToTeleporter(0, &tele);
    if (g_teleports != 1) { std::printf("005fbd20_TEST FAIL\n"); return 1; }
    std::printf("005fbd20_TEST PASS\n");
    return 0;
}