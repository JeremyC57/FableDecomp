#include <cstdio>
class CThing { public:
    bool m_forceFrameUpdate; int m_calls;
    void SetForceFrameUpdate(bool update) { m_forceFrameUpdate = update; ++m_calls; }
};
class CTCExplodingObject { public:
    CThing* ParentThing; float MaxDamage; float Radius; long FireDamage; long CountdownTimer;
    bool TriggeredOnCreatureProximity; float TriggerRadius;
    void OnCreate() { if (TriggeredOnCreatureProximity) ParentThing->SetForceFrameUpdate(true); }
};
int main()
{
    CThing t; t.m_forceFrameUpdate = false; t.m_calls = 0;
    CTCExplodingObject o; o.ParentThing = &t; o.TriggeredOnCreatureProximity = false;
    o.OnCreate();
    bool off = t.m_calls == 0 && !t.m_forceFrameUpdate;
    o.TriggeredOnCreatureProximity = true; o.OnCreate();
    if (!off || t.m_calls != 1 || !t.m_forceFrameUpdate) { std::printf("00750b80_TEST FAIL\n"); return 1; }
    std::printf("00750b80_TEST PASS\n");
    return 0;
}