// Behaviour test for CWorld::EAUseQuickAccessItemInCutscene 0x0062AD90: live hero + inventory TC -> slot from the event.
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
static int g_uses = 0; static long g_lastSlot = -1;
struct CTCInventory { void UseQuickAccessItemInCutscene(long slot) { ++g_uses; g_lastSlot = slot; } };
struct CThing {
    bool Dead;
    bool HasInventory;
    CTCInventory* Inventory;
    bool HasTC(long type) const { return type == 0x11 && HasInventory; }
    template<class T> T* GetTC(long) { return Inventory; }
};
static CThing* g_hero = 0;
struct CPlayer { CThing* GetHeroThing() { return g_hero; } };
static CPlayer g_player;
struct CWorld {
    CPlayer* GetPlayer(long) { return &g_player; }
    void EAUseQuickAccessItemInCutscene(const CGameEvent* event);
};

void CWorld::EAUseQuickAccessItemInCutscene(const CGameEvent* event)
{
    CThing* hero = GetPlayer(event->Player)->GetHeroThing();
    if (hero && !hero->Dead)
    {
        CMemoryBuffer buffer;
        event->GetData(buffer);
        if (hero->HasTC(0x11))
        {
            CTCInventory* inventory = hero->GetTC<CTCInventory>(0x11);
            if (inventory)
            {
                long slot;
                buffer.Read(4, &slot);
                inventory->UseQuickAccessItemInCutscene(slot);
            }
        }
    }
}

int main()
{
    CWorld w; CGameEvent e; e.Player = 0; long slot = 3; std::memset(e.Data, 0, 32); std::memcpy(e.Data, &slot, 4);
    CTCInventory inv; CThing hero; hero.Dead = false; hero.HasInventory = true; hero.Inventory = &inv;
    bool ok = true;
    w.EAUseQuickAccessItemInCutscene(&e); if (g_uses != 0) ok = false;          // no hero
    g_hero = &hero;
    w.EAUseQuickAccessItemInCutscene(&e); if (g_uses != 1 || g_lastSlot != 3) ok = false;
    hero.Dead = true; w.EAUseQuickAccessItemInCutscene(&e); if (g_uses != 1) ok = false;
    hero.Dead = false; hero.HasInventory = false; w.EAUseQuickAccessItemInCutscene(&e); if (g_uses != 1) ok = false;
    hero.HasInventory = true; hero.Inventory = 0; w.EAUseQuickAccessItemInCutscene(&e); if (g_uses != 1) ok = false;
    if (!ok) { std::printf("0062ad90_TEST FAIL\n"); return 1; }
    std::printf("0062ad90_TEST PASS\n");
    return 0;
}
