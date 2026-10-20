// Behaviour test for CWorld::EACreatureSprint 0x0062C3E0: sprint flag from the event toggles SetSprinting only on a change.
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
static int g_sets = 0;
struct CTCHeroSpecialMovement {
    bool Sprinting;
    void SetSprinting(bool s) { ++g_sets; Sprinting = s; }
};
struct CThing {
    bool Dead; CTCHeroSpecialMovement* Movement;
    bool HasTC(long type) const { return type == 0xDF && Movement != 0; }
    template<class T> T* GetTC(long) { return Movement; }
};
static CThing* g_hero = 0;
struct CPlayer { CThing* GetHeroThing() { return g_hero; } };
static CPlayer g_player;
struct CWorld {
    CPlayer* GetPlayer(long) { return &g_player; }
    void EACreatureSprint(const CGameEvent* event);
};

void CWorld::EACreatureSprint(const CGameEvent* event)
{
    bool sprint;
    {
        CMemoryBuffer buffer;
        event->GetData(buffer);
        long value;
        buffer.Read(4, &value);
        sprint = value != 0;
    }
    CThing* hero = GetPlayer(event->Player)->GetHeroThing();
    if (hero && !hero->Dead && hero->HasTC(0xDF))
    {
        CTCHeroSpecialMovement* movement = hero->GetTC<CTCHeroSpecialMovement>(0xDF);
        if (sprint)
        {
            if (!movement->Sprinting)
                movement->SetSprinting(true);
        }
        else if (movement->Sprinting)
            movement->SetSprinting(false);
    }
}

static void Set(CGameEvent& e, long v) { std::memset(e.Data, 0, 32); std::memcpy(e.Data, &v, 4); }

int main()
{
    CWorld w; CGameEvent e; e.Player = 0;
    CTCHeroSpecialMovement m; m.Sprinting = false; CThing hero; hero.Dead = false; hero.Movement = &m;
    bool ok = true;
    Set(e, 5); w.EACreatureSprint(&e); if (g_sets != 0) ok = false;        // no hero
    g_hero = &hero;
    w.EACreatureSprint(&e); if (g_sets != 1 || !m.Sprinting) ok = false;
    w.EACreatureSprint(&e); if (g_sets != 1) ok = false;                   // already sprinting
    Set(e, 0); w.EACreatureSprint(&e); if (g_sets != 2 || m.Sprinting) ok = false;
    w.EACreatureSprint(&e); if (g_sets != 2) ok = false;
    hero.Dead = true; Set(e, 1); w.EACreatureSprint(&e); if (g_sets != 2) ok = false;
    if (!ok) { std::printf("0062c3e0_TEST FAIL\n"); return 1; }
    std::printf("0062c3e0_TEST PASS\n");
    return 0;
}
