// Behaviour test for CTCDeadShotInThingTracker::FrameUpdate() @ 0x007E34E0: with HasThingStuckIn set, the shot's parent is
// killed when the thing is gone, dead, or its physics component is moving; nothing happens otherwise.
#include <cstdio>
static int g_kills = 0;
class CTCPhysicsBase { public: unsigned char _pad[0x3c]; unsigned char OldMoving[0x4]; };
class CThing { public:
    unsigned char   _pad0[0x60];
    CTCPhysicsBase* PhysicsTC;
    unsigned char   _pad1[0x91 - 0x64];
    unsigned char   m_flags91;
    bool IsDead() const { return (m_flags91 & 1) != 0; }
    void Kill(bool immediately) { if (immediately) ++g_kills; }
};
struct CBaseIntelligentPointer {
    virtual void v00() { }
    void* PData;
    CThing* GetPItem() const { return static_cast<CThing*>(PData); }
};
class CTCDeadShotInThingTracker { public:
    virtual void v00() { }
    CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs;
    CBaseIntelligentPointer PThingStuckIn;
    bool                    HasThingStuckIn;
    void FrameUpdate();
};
void CTCDeadShotInThingTracker::FrameUpdate()
{
    if (HasThingStuckIn) {
        if (PThingStuckIn.GetPItem() == 0
            || PThingStuckIn.GetPItem()->IsDead()
            || (PThingStuckIn.GetPItem()->PhysicsTC->OldMoving[0] & 1) != 0)
            ParentThing->Kill(true);
    }
}
int main()
{
    CThing shot, wall; CTCPhysicsBase physics; physics.OldMoving[0] = 0;
    wall.PhysicsTC = &physics; wall.m_flags91 = 0;
    CTCDeadShotInThingTracker t; t.ParentThing = &shot; t.PThingStuckIn.PData = &wall;
    t.HasThingStuckIn = false; t.PThingStuckIn.PData = 0; t.FrameUpdate();          // not stuck: nothing
    if (g_kills != 0) { std::printf("007e34e0_TEST FAIL\n"); return 1; }
    t.HasThingStuckIn = true; t.FrameUpdate();                                     // stuck in nothing: kill
    if (g_kills != 1) { std::printf("007e34e0_TEST FAIL\n"); return 1; }
    t.PThingStuckIn.PData = &wall; t.FrameUpdate();                                // alive and still: nothing
    if (g_kills != 1) { std::printf("007e34e0_TEST FAIL\n"); return 1; }
    physics.OldMoving[0] = 1; t.FrameUpdate();                                     // moving: kill
    if (g_kills != 2) { std::printf("007e34e0_TEST FAIL\n"); return 1; }
    physics.OldMoving[0] = 2; wall.m_flags91 = 1; t.FrameUpdate();                 // dead: kill
    if (g_kills != 3) { std::printf("007e34e0_TEST FAIL\n"); return 1; }
    std::printf("007e34e0_TEST PASS\n");
    return 0;
}