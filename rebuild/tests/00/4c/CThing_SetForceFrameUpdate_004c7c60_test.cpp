// Behaviour test for CThing::SetForceFrameUpdate @ 0x004C7C60: sets the flag; a forced live thing goes on the update list.
#include <cstdio>
class CThing;
class CThingManager { public:
    int calls; CThing* last;
    void AddToUpdateList(CThing* thing) { ++calls; last = thing; }
};
static CThingManager g_manager;
static CThingManager* g_pThingManager = &g_manager;
class CThing { public:
    unsigned char Dead : 1;
    unsigned char ForceFrameUpdate : 1;
    void SetForceFrameUpdate(bool force)
    {
        ForceFrameUpdate = force;
        if (ForceFrameUpdate && !Dead)
            g_pThingManager->AddToUpdateList(this);
    }
};
int main()
{
    g_manager.calls = 0; g_manager.last = 0;
    CThing t; t.Dead = 0; t.ForceFrameUpdate = 0;
    t.SetForceFrameUpdate(true);
    if (!t.ForceFrameUpdate || g_manager.calls != 1 || g_manager.last != &t) { std::printf("004c7c60_TEST FAIL\n"); return 1; }
    t.SetForceFrameUpdate(false);
    if (t.ForceFrameUpdate || g_manager.calls != 1) { std::printf("004c7c60_TEST FAIL\n"); return 1; }
    t.Dead = 1; t.SetForceFrameUpdate(true);
    if (!t.ForceFrameUpdate || g_manager.calls != 1) { std::printf("004c7c60_TEST FAIL\n"); return 1; }
    std::printf("004c7c60_TEST PASS\n");
    return 0;
}