// Behaviour test for 0x0049D8C0 CWorld::EventIsWorldEvent: an event is a world event when its type's dispatch entry has an action handler.
#include <cstdio>
class CGameEvent { public: long Type; signed char Player; };
class CWorld;
typedef void (CWorld::*TWorldEventFunction)(const CGameEvent* event);
class CGameEventDispatch { public:
    TWorldEventFunction ActionFunction;
    TWorldEventFunction LocalActionFunction;
    unsigned char       FeedbackFunction[0x10];
    unsigned char       ConfirmFunction[0x10];
};
CGameEventDispatch GameEventDispatchTable[4];
class CWorld { public:
    void OnAction(const CGameEvent*) { }
    bool EventIsWorldEvent(const CGameEvent* event) const
    {
        const CGameEventDispatch* dispatch = GetEventDispatchTableEntry(event->Type);
        return dispatch->ActionFunction != 0;
    }
protected:
    const CGameEventDispatch* GetEventDispatchTableEntry(long type) const { return &GameEventDispatchTable[type]; }
};
int main()
{
    for (int i = 0; i < 4; ++i) { GameEventDispatchTable[i].ActionFunction = 0; GameEventDispatchTable[i].LocalActionFunction = 0; }
    GameEventDispatchTable[2].ActionFunction = &CWorld::OnAction;
    GameEventDispatchTable[3].LocalActionFunction = &CWorld::OnAction;     // local-only handler: not a world event
    CWorld world;
    CGameEvent e;
    int ok = 1;
    e.Type = 0; if (world.EventIsWorldEvent(&e)) ok = 0;
    e.Type = 2; if (!world.EventIsWorldEvent(&e)) ok = 0;
    e.Type = 3; if (world.EventIsWorldEvent(&e)) ok = 0;
    if (!ok) { std::printf("0049d8c0_TEST FAIL\n"); return 1; }
    std::printf("0049d8c0_TEST PASS\n");
    return 0;
}