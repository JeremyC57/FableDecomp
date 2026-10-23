#include <cstdio>
#include <cstring>
// Behaviour test for CTCInventoryBase::AddCloseInventoryEvent (0x005BC527): same logic over a copy of the model.
struct CMemoryBuffer { };
struct CGameEvent : CMemoryBuffer {
    long Type; signed char Player; unsigned char Data[32]; unsigned char EndPos; bool Valid; bool Replacement;
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void Copy(unsigned long length, const void* data) { std::memcpy(Data + EndPos, data, length); EndPos = (unsigned char)(EndPos + length); }
    void AddUBYTE(unsigned char value) { Copy(1, &value); }
};
static CGameEvent g_last(0);
static int g_events = 0;
struct CMainGameComponent { void AddGameEvent(const CGameEvent& event) { g_last = event; ++g_events; } };
static CMainGameComponent g_component;
static CMainGameComponent* g_pMainGameComponent = &g_component;
struct CPlayerManager { long m_main; long GetMainPlayerNumber() { return m_main; } };
static CPlayerManager g_players = { 1 };
static CPlayerManager* GetPlayerManager_00686d50() { return &g_players; }
enum { GAME_EVENT_CLOSE_INVENTORY = 0x1B };
struct CTCInventoryBase_Retail {
    void AddCloseInventoryEvent()
    {
        CGameEvent event(GAME_EVENT_CLOSE_INVENTORY);
        event.Player = (signed char)GetPlayerManager_00686d50()->GetMainPlayerNumber();
        event.AddUBYTE(0);
        g_pMainGameComponent->AddGameEvent(event);
    }
};
int main()
{
    CTCInventoryBase_Retail inv;
    g_last.Data[0] = 0x55;
    inv.AddCloseInventoryEvent();
    if (g_events != 1 || g_last.Type != 0x1B || g_last.Player != 1 || g_last.EndPos != 1 || g_last.Data[0] != 0
        || !g_last.Valid || g_last.Replacement) { std::printf("005bc527_TEST FAIL\n"); return 1; }
    std::printf("005bc527_TEST PASS\n");
    return 0;
}