// Behaviour test for 0x007e4330 CTCWillResponse::SetThingIsEpicSpellable(CThing*, bool): add-on-clear, remove-when-default.
#include <cstdio>

struct WillResponse {
    bool IsForcePushable : 1;
    bool IsLifeDrainable : 1;
    bool IsLightningable : 1;
    bool IsNotBloodyWillable : 1;
    bool IsAbleToBeLifted : 1;
    bool IsEpicPowerExplodable : 1;
    bool IsEpicSpellable : 1;
    WillResponse() : IsForcePushable(true), IsLifeDrainable(true), IsLightningable(true), IsNotBloodyWillable(false),
                     IsAbleToBeLifted(true), IsEpicPowerExplodable(true), IsEpicSpellable(true) { }
    bool AllDefault() const
    {
        return IsForcePushable && IsLifeDrainable && IsLightningable && IsAbleToBeLifted && IsEpicPowerExplodable &&
               IsEpicSpellable && !IsNotBloodyWillable;
    }
};

struct Thing {
    bool has;
    WillResponse tc;
    int adds, removes;
    Thing() : has(false), adds(0), removes(0) { }
    WillResponse* AddTC() { has = true; tc = WillResponse(); ++adds; return &tc; }
    void RemoveTC() { has = false; ++removes; }
};

static void Set(Thing* thing, bool value)
{
    if (!value) {
        WillResponse* tc = thing->has ? &thing->tc : thing->AddTC();
        tc->IsEpicSpellable = false;
    } else if (thing->has) {
        WillResponse* tc = &thing->tc;
        tc->IsEpicSpellable = value;
        if (tc->AllDefault())
            thing->RemoveTC();
    }
}

int main()
{
    Thing t;
    Set(&t, true);                                     // no component: nothing happens
    if (t.has || t.adds != 0) { std::printf("007e4330_TEST FAIL\n"); return 1; }
    Set(&t, false);                                    // adds the component and clears the flag
    if (!t.has || t.adds != 1 || t.tc.IsEpicSpellable) { std::printf("007e4330_TEST FAIL\n"); return 1; }
    Set(&t, false);                                    // reuses it
    if (t.adds != 1 || t.tc.IsEpicSpellable) { std::printf("007e4330_TEST FAIL\n"); return 1; }
    t.tc.IsNotBloodyWillable = true;
    Set(&t, true);                                     // set back, but another flag is non-default: keep
    if (!t.has || !t.tc.IsEpicSpellable || t.removes != 0) { std::printf("007e4330_TEST FAIL\n"); return 1; }
    t.tc.IsNotBloodyWillable = false;
    Set(&t, false);
    Set(&t, true);                                     // all defaults again: removed
    if (t.has || t.removes != 1) { std::printf("007e4330_TEST FAIL\n"); return 1; }
    std::printf("007e4330_TEST PASS\n");
    return 0;
}