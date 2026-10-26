// Behaviour test for 0075c5f0: the model below is a copy of src/0075c5f0.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
static int g_removes = 0, g_adds = 0, g_insideCalls = 0, g_onInside = 0, g_angleCalls = 0, g_managerCalls = 0;
static int g_haveManager = 1, g_inside = 0, g_angleValid = 1;
class CThing { public:};
static CThing* g_lastThing = 0;
struct CCameraPointManager {
    void RemoveCameraPoint(CThing* thing) { ++g_removes; g_lastThing = thing; }
    void AddCameraPoint(CThing* thing) { ++g_adds; g_lastThing = thing; }
};
static CCameraPointManager g_manager;
struct CCameraManagerSource {
    virtual bool GetCameraManager(CCameraPointManager** out) { ++g_managerCalls; *out = &g_manager; return g_haveManager != 0; }
};
class CWorld { public:
    unsigned char         _pad00[0x34];
    CCameraManagerSource* m_pCameraSource;
};
static CWorld g_world; static CCameraManagerSource g_source;
struct CTCCameraPointShapeActivationBase {
    virtual void vfunc_9();
    virtual bool IsSubjectInside() { ++g_insideCalls; return g_inside != 0; }
    virtual void OnSubjectInside() { ++g_onInside; }
    CThing*       ParentThing;
    unsigned char _pad08[0x3c - 0x08];
    bool          Activated;
    CWorld* GetWorld() { return &g_world; }
    bool IsAngleValid() { ++g_angleCalls; return g_angleValid != 0; }
};

void CTCCameraPointShapeActivationBase::vfunc_9()
{
    CCameraPointManager* manager;
    CWorld* world = GetWorld();
    if (!world->m_pCameraSource->GetCameraManager(&manager))
        return;
    bool inside = IsSubjectInside();
    if (inside)
        OnSubjectInside();
    if (Activated && !inside)
    {
        manager->RemoveCameraPoint(ParentThing);
        Activated = false;
    }
    else if (!Activated && inside && IsAngleValid())
    {
        manager->AddCameraPoint(ParentThing);
        Activated = true;
    }
}

static bool Run(int haveManager, bool activated, int inside, int angleValid, bool expectActivated, int expectAdds, int expectRemoves, int expectOnInside)
{
    static CThing parent;
    g_world.m_pCameraSource = &g_source; g_haveManager = haveManager; g_inside = inside; g_angleValid = angleValid;
    CTCCameraPointShapeActivationBase c; c.ParentThing = &parent; c.Activated = activated;
    g_removes = g_adds = g_insideCalls = g_onInside = g_angleCalls = g_managerCalls = 0; g_lastThing = 0;
    c.vfunc_9();
    if (g_managerCalls != 1) return false;
    if (c.Activated != expectActivated || g_adds != expectAdds || g_removes != expectRemoves || g_onInside != expectOnInside) return false;
    if ((g_adds || g_removes) && g_lastThing != &parent) return false;
    if (g_insideCalls != (haveManager ? 1 : 0)) return false;
    return true;
}
int main()
{
    bool ok = true;
    ok = ok && Run(0, true, 1, 1, true, 0, 0, 0);      // no manager: nothing happens
    ok = ok && Run(1, true, 0, 1, false, 0, 1, 0);     // activated, subject left: removed
    ok = ok && Run(1, false, 1, 1, true, 1, 0, 1);     // not activated, subject inside, angle ok: added
    ok = ok && Run(1, false, 1, 0, false, 0, 0, 1);    // inside but the angle is invalid: not added
    ok = ok && Run(1, true, 1, 1, true, 0, 0, 1);      // activated and inside: unchanged
    ok = ok && Run(1, false, 0, 1, false, 0, 0, 0);    // not activated, outside: unchanged
    if (!ok) { std::printf("0075c5f0_TEST FAIL\n"); return 1; }
    std::printf("0075c5f0_TEST PASS\n");
    return 0;
}