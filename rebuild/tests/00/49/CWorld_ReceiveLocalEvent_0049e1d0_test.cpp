// test for 0049e1d0 CWorld::ReceiveLocalEvent
#include <stdio.h>
#include <string.h>
#include "engine/CWorld.h"
#include "engine/CGameEvent.h"
struct CWorld_Methods;
struct CDisplayEngine;
typedef void (CWorld_Methods::*TWorldEventFunction)(const CGameEvent* event);
struct CGameEventDispatch {
    TWorldEventFunction ActionFunction;                  // +0x00
    TWorldEventFunction LocalActionFunction;             // +0x10
    unsigned char       FeedbackFunction[0x10];          // +0x20 CDisplayEngine pointer to member
    unsigned char       ConfirmFunction[0x10];           // +0x30 CDisplayEngine pointer to member
};
FABLE_STATIC_ASSERT(sizeof(CGameEventDispatch) == 0x40);
extern CGameEventDispatch GameEventDispatchTable[];      // 0x013B9288
struct CWorld_Methods : CWorld {
    const CGameEventDispatch* GetEventDispatchTableEntry(long type) const { return &GameEventDispatchTable[type]; }
public:
    void ReceiveLocalEvent(const CGameEvent* event);
    void Handler(const CGameEvent* event);
};
void CWorld_Methods::ReceiveLocalEvent(const CGameEvent* event)
{
    const CGameEventDispatch* dispatch = GetEventDispatchTableEntry(event->Type);
    if (dispatch->LocalActionFunction)
        (this->*dispatch->LocalActionFunction)(event);
}

CGameEventDispatch GameEventDispatchTable[4];
static const void* g_this; static const CGameEvent* g_ev; static int g_calls;
void CWorld_Methods::Handler(const CGameEvent* event) { g_this = this; g_ev = event; ++g_calls; }
int main()
{
    static double buf[0x4000];
    CWorld_Methods* obj = (CWorld_Methods*)buf;
    memset(GameEventDispatchTable, 0, sizeof(GameEventDispatchTable));
    GameEventDispatchTable[2].LocalActionFunction = &CWorld_Methods::Handler;
    CGameEvent ev; memset(&ev, 0, sizeof(ev)); ev.Type = 2;
    obj->ReceiveLocalEvent(&ev);
    int ok = g_calls == 1 && g_this == obj && g_ev == &ev;
    ev.Type = 1;
    obj->ReceiveLocalEvent(&ev);
    ok &= g_calls == 1;
    printf(ok ? "0049e1d0_TEST PASS\n" : "0049e1d0_TEST FAIL\n");
    return ok ? 0 : 1;
}