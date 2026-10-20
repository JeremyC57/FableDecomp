// Behaviour test for CWorld::EARemoveCurrentMode 0x0062C4B0: RemoveMode(event mode) always, ranged-attack reset for a live hero with the TC.
#include <cstdio>
#include <cstring>

struct CMemoryBuffer {
    unsigned char Data[0x20];
    long Length, ReadPos, WritePos;
    CMemoryBuffer() : Length(0), ReadPos(0), WritePos(0) { }
    void Read(unsigned long length, void* data) { std::memcpy(data, Data + ReadPos, length); ReadPos += (long)length; }
};
struct CGameEvent {
    signed char Player;
    unsigned char Data[32];
    long GetData(CMemoryBuffer& buffer) const { std::memcpy(buffer.Data, Data, 32); buffer.Length = 32; return 32; }
};
enum EPlayerMode { PLAYER_MODE_NONE = 0, PLAYER_MODE_X = 9 };
static int g_resets = 0, g_removes = 0; static EPlayerMode g_lastMode = PLAYER_MODE_NONE;
struct CTCHeroRangedAttacks { void ResetProjectileWeaponMode() { ++g_resets; } };
struct CThing {
    bool Dead; CTCHeroRangedAttacks* Ranged;
    bool HasTC(long type) const { return type == 0x81 && Ranged != 0; }
    template<class T> T* GetTC(long) { return Ranged; }
};
static CThing* g_hero = 0;
struct CPlayer {
    CThing* GetHeroThing() { return g_hero; }
    void RemoveMode(EPlayerMode m) { ++g_removes; g_lastMode = m; }
};
static CPlayer g_player;
struct CWorld {
    CPlayer* GetPlayer(long) { return &g_player; }
    void EARemoveCurrentMode(const CGameEvent* event);
};

void CWorld::EARemoveCurrentMode(const CGameEvent* event)
{
    CPlayer* player = GetPlayer(event->Player);
    CThing* hero = player->GetHeroThing();
    {
        CMemoryBuffer buffer;
        event->GetData(buffer);
        EPlayerMode mode;
        buffer.Read(4, &mode);
        player->RemoveMode(mode);
    }
    if (hero && !hero->Dead && hero->HasTC(0x81))
        hero->GetTC<CTCHeroRangedAttacks>(0x81)->ResetProjectileWeaponMode();
}

int main()
{
    CWorld w; CGameEvent e; e.Player = 0; long m = 9; std::memset(e.Data, 0, 32); std::memcpy(e.Data, &m, 4);
    CTCHeroRangedAttacks ranged; CThing hero; hero.Dead = false; hero.Ranged = &ranged;
    bool ok = true;
    w.EARemoveCurrentMode(&e); if (g_removes != 1 || g_lastMode != PLAYER_MODE_X || g_resets != 0) ok = false;
    g_hero = &hero; w.EARemoveCurrentMode(&e); if (g_removes != 2 || g_resets != 1) ok = false;
    hero.Dead = true; w.EARemoveCurrentMode(&e); if (g_removes != 3 || g_resets != 1) ok = false;
    hero.Dead = false; hero.Ranged = 0; w.EARemoveCurrentMode(&e); if (g_removes != 4 || g_resets != 1) ok = false;
    if (!ok) { std::printf("0062c4b0_TEST FAIL\n"); return 1; }
    std::printf("0062c4b0_TEST PASS\n");
    return 0;
}