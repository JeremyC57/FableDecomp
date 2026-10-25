// Behaviour test for CWorld::ProcessEvent (0x0049D980): the dispatch-table entry for the event's type is looked up and
// its action handler (not the local one) is called on the world with the event; an empty entry does nothing.
#include <cstdio>

struct CGameEvent { long Type; };
struct CWorld { int Id; };
struct CWorld_Methods;
typedef void (CWorld_Methods::*TWorldEventFunction)(const CGameEvent* event);
struct CGameEventDispatch {
    TWorldEventFunction ActionFunction;
    TWorldEventFunction LocalActionFunction;
};
static CGameEventDispatch GameEventDispatchTable[4];
static int g_action = 0, g_local = 0; static long g_type = -1; static int g_id = 0;

struct CWorld_Methods : CWorld {
    const CGameEventDispatch* GetEventDispatchTableEntry(long type) const { return &GameEventDispatchTable[type]; }
    void OnAction(const CGameEvent* e) { ++g_action; g_type = e->Type; g_id = Id; }
    void OnLocal(const CGameEvent*) { ++g_local; }
    void ProcessEvent(const CGameEvent* event);
};
void CWorld_Methods::ProcessEvent(const CGameEvent* event)
{
    const CGameEventDispatch* dispatch = GetEventDispatchTableEntry(event->Type);
    if (dispatch->ActionFunction)
        (this->*dispatch->ActionFunction)(event);
}

int main()
{
    for (int i = 0; i < 4; ++i) { GameEventDispatchTable[i].ActionFunction = 0; GameEventDispatchTable[i].LocalActionFunction = 0; }
    GameEventDispatchTable[2].ActionFunction = &CWorld_Methods::OnAction;
    GameEventDispatchTable[2].LocalActionFunction = &CWorld_Methods::OnLocal;
    GameEventDispatchTable[3].LocalActionFunction = &CWorld_Methods::OnLocal;
    CWorld_Methods w; w.Id = 77;
    CGameEvent e1 = { 1 }, e2 = { 2 }, e3 = { 3 };
    w.ProcessEvent(&e1);
    w.ProcessEvent(&e3);
    bool ok = g_action == 0 && g_local == 0;
    w.ProcessEvent(&e2);
    ok = ok && g_action == 1 && g_local == 0 && g_type == 2 && g_id == 77;
    if (!ok) { std::printf("0049d980_TEST FAIL\n"); return 1; }
    std::printf("0049d980_TEST PASS\n");
    return 0;
}