// Behaviour test for CWorld::EAChargeQuickAccessItem (0x00629B10): gates (mode 0x2F, no/dead/busy hero), the
// payload forwarded to the GUI, and the charging mode added or removed only while the GUI is open.
#include <cstdio>
#include <cstring>
struct CMemoryBuffer {
    unsigned char Data[0x20]; long Length, ReadPos, WritePos;
    CMemoryBuffer() : Length(0), ReadPos(0), WritePos(0) {}
    void Read(unsigned long length, void* data) { std::memcpy(data, Data + ReadPos, length); ReadPos += (long)length; }
    char ReadChar() { char v; Read(1, &v); return v; }
};
struct CGameEvent {
    signed char Player; unsigned char Payload[0x20];
    long GetData(CMemoryBuffer& b) const { std::memcpy(b.Data, Payload, 0x20); return 5; }
};
struct CThingCreatureBase { unsigned char Dead : 1; bool Busy; bool IsPerformingAnyAction() { return Busy; } };
enum EPlayerMode { PLAYER_MODE_NONE = 0, PLAYER_MODE_CHARGING_QUICK_ACCESS = 0x1c, PLAYER_MODE_0x2F = 0x2f };
static int g_added, g_removed;
struct CPlayer {
    bool Blocked; CThingCreatureBase* Hero; EPlayerMode Mode;
    bool HasMode(EPlayerMode m) { return m == PLAYER_MODE_0x2F && Blocked; }
    CThingCreatureBase* GetHeroCreature() { return Hero; }
    EPlayerMode GetCurrentMode() const { return Mode; }
    void AddMode(EPlayerMode m, bool) { g_added = m; }
    void RemoveMode(EPlayerMode m) { g_removed = m; }
};
static long g_item; static int g_charge;
struct CPlayerGui { bool Open; void ToggleQuickAccessCharge(long i, bool c) { g_item = i; g_charge = c; } bool IsOpen() const { return Open; } };
static CPlayerGui g_gui; static CPlayerGui* g_pPlayerGui = &g_gui;
struct CWorld {
    CPlayer* P;
    CPlayer* GetPlayer(long) { return P; }
    void EAChargeQuickAccessItem(const CGameEvent* event)
    {
        CPlayer* eventPlayer = GetPlayer(event->Player);
        if (eventPlayer->HasMode(PLAYER_MODE_0x2F))
            return;
        CPlayer* player = GetPlayer(event->Player);
        CThingCreatureBase* hero = player->GetHeroCreature();
        if (!hero || hero->Dead || hero->IsPerformingAnyAction())
            return;
        CMemoryBuffer buffer;
        event->GetData(buffer);
        int item;   // 4-byte payload word (long on the 32-bit target)
        buffer.Read(4, &item);
        bool charge = buffer.ReadChar() != 0;
        g_pPlayerGui->ToggleQuickAccessCharge(item, charge);
        if (g_pPlayerGui->IsOpen()) {
            EPlayerMode mode = player->GetCurrentMode();
            if (charge) {
                if (mode != PLAYER_MODE_CHARGING_QUICK_ACCESS)
                    player->AddMode(PLAYER_MODE_CHARGING_QUICK_ACCESS, false);
            } else if (mode == PLAYER_MODE_CHARGING_QUICK_ACCESS)
                player->RemoveMode(mode);
        }
    }
};
static bool run(bool blocked, bool hasHero, bool dead, bool busy, bool open, EPlayerMode mode, unsigned char flag,
                long wantItem, int wantCharge, int wantAdd, int wantRemove)
{
    CThingCreatureBase hero; hero.Dead = dead; hero.Busy = busy;
    CPlayer p = { blocked, hasHero ? &hero : 0, mode }; CWorld w; w.P = &p; g_gui.Open = open;
    CGameEvent e; std::memset(&e, 0, sizeof e); int item = 42; std::memcpy(e.Payload, &item, 4); e.Payload[4] = flag;
    g_item = -1; g_charge = -1; g_added = -1; g_removed = -1;
    w.EAChargeQuickAccessItem(&e);
    return g_item == wantItem && g_charge == wantCharge && g_added == wantAdd && g_removed == wantRemove;
}
int main()
{
    bool ok = run(true, true, false, false, true, PLAYER_MODE_NONE, 1, -1, -1, -1, -1)
        && run(false, false, false, false, true, PLAYER_MODE_NONE, 1, -1, -1, -1, -1)
        && run(false, true, true, false, true, PLAYER_MODE_NONE, 1, -1, -1, -1, -1)
        && run(false, true, false, true, true, PLAYER_MODE_NONE, 1, -1, -1, -1, -1)
        && run(false, true, false, false, true, PLAYER_MODE_NONE, 5, 42, 1, 0x1c, -1)
        && run(false, true, false, false, true, PLAYER_MODE_CHARGING_QUICK_ACCESS, 1, 42, 1, -1, -1)
        && run(false, true, false, false, true, PLAYER_MODE_CHARGING_QUICK_ACCESS, 0, 42, 0, -1, 0x1c)
        && run(false, true, false, false, false, PLAYER_MODE_CHARGING_QUICK_ACCESS, 0, 42, 0, -1, -1);
    if (!ok) { std::printf("00629b10_TEST FAIL\n"); return 1; }
    std::printf("00629b10_TEST PASS\n");
    return 0;
}
