// test for 00434a30 ReceiveFeedbackGameEvent
#include <stdio.h>
#include <string.h>
#include "engine/CDisplayEngine.h"
#include "engine/CGameEvent.h"
struct CDisplayEngine_Methods;
typedef void (CDisplayEngine_Methods::*TDisplayEventFunction)(const CGameEvent* event);
struct CGameEventDispatch {
    unsigned char         ActionFunction[0x10];          // +0x00 CWorld pointer to member
    unsigned char         LocalActionFunction[0x10];     // +0x10 CWorld pointer to member
    TDisplayEventFunction FeedbackFunction;              // +0x20
    TDisplayEventFunction ConfirmFunction;               // +0x30
};
FABLE_STATIC_ASSERT(sizeof(CGameEventDispatch) == 0x40);
extern CGameEventDispatch GameEventDispatchTable[];      // 0x013B9288
struct CDisplayEngine_Methods : CDisplayEngine {
    void ReceiveFeedbackGameEvent(const CGameEvent* event);
    void Handler(const CGameEvent* event);
};
void CDisplayEngine_Methods::ReceiveFeedbackGameEvent(const CGameEvent* event)
{
    const CGameEventDispatch& dispatch = GameEventDispatchTable[event->Type];
    if (dispatch.FeedbackFunction)
        (this->*dispatch.FeedbackFunction)(event);
}

CGameEventDispatch GameEventDispatchTable[4];
static const void* g_this; static const CGameEvent* g_ev; static int g_calls;
void CDisplayEngine_Methods::Handler(const CGameEvent* event) { g_this = this; g_ev = event; ++g_calls; }
int main()
{
    static double buf[0x4000];
    CDisplayEngine_Methods* obj = (CDisplayEngine_Methods*)buf;
    memset(GameEventDispatchTable, 0, sizeof(GameEventDispatchTable));
    GameEventDispatchTable[2].FeedbackFunction = &CDisplayEngine_Methods::Handler;
    CGameEvent ev; memset(&ev, 0, sizeof(ev)); ev.Type = 2;
    obj->ReceiveFeedbackGameEvent(&ev);
    int ok = g_calls == 1 && g_this == obj && g_ev == &ev;
    ev.Type = 1;
    obj->ReceiveFeedbackGameEvent(&ev);
    ok &= g_calls == 1;
    printf(ok ? "00434a30_TEST PASS\n" : "00434a30_TEST FAIL\n");
    return ok ? 0 : 1;
}