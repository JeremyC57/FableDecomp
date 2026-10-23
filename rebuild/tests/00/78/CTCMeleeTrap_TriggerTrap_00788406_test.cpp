// 00788406 behaviour test: CTCMeleeTrap::TriggerTrap (final.cpp body over mock defs)
#include <stdio.h>
#include <string.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
static const char* g_meleeTrapTriggerAnimName = "trigger";
static const char* g_meleeTrapSecondAnimName = "second";
struct CAnimFilterAll { };
struct CAnimationEntry { int AnimationIndex; };
struct CAnimationTable {
    CAnimationEntry Trigger, Second; bool HasSecond;
    const CAnimationEntry* GetAnimation(const char* name, int, const CAnimFilterAll&) const { return strcmp(name, "trigger") == 0 ? &Trigger : &Second; }
    bool HasAnimation(const char* name, const CAnimFilterAll&) const { return strcmp(name, "second") == 0 && HasSecond; }
};
struct CMeleeTrapDef { int RefCount; CAnimationTable Animations; };
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) --Object->RefCount; }
    T* operator->() const { return Object; }
};
struct CDefClassBase {
    CMeleeTrapDef* Sub;
    bool GetSubDef(CDefPointer<CMeleeTrapDef>* out) const { out->Object = Sub; if (Sub) ++Sub->RefCount; return Sub != 0; }
};
struct C3DAnimationInfo { float Duration; };
struct CMeshDataBank { C3DAnimationInfo Infos[4]; C3DAnimationInfo* GetAnimation(int i) { return &Infos[i]; } };
struct CWorld { CMeshDataBank* Bank; };
struct CThing { CDefClassBase* PDef; };
struct CTCMeleeTrap {
    CThing* ParentThing; CWorld* World;
    const char* InitName; bool InitLoop; float Trigger, Second; int Strikes;
    CWorld* GetWorld() { return World; }
    void InitialiseAnimation(const char* n, bool loop) { InitName = n; InitLoop = loop; }
    void SetStrikeTimes(float t, float s) { Trigger = t; Second = s; ++Strikes; }
    void TriggerTrap();
};
void CTCMeleeTrap::TriggerTrap()
{
    CDefPointer<CMeleeTrapDef> def;
    ParentThing->PDef->GetSubDef(&def);
    InitialiseAnimation(g_meleeTrapTriggerAnimName, false);
    const CAnimationEntry* trigger = def->Animations.GetAnimation(g_meleeTrapTriggerAnimName, 0, CAnimFilterAll());
    CMeshDataBank* bank = GetWorld()->Bank;
    float triggerTime = bank->GetAnimation(trigger->AnimationIndex)->Duration;
    float secondTime = 0.0f;
    if (def->Animations.HasAnimation(g_meleeTrapSecondAnimName, CAnimFilterAll())) {
        const CAnimationEntry* second = def->Animations.GetAnimation(g_meleeTrapSecondAnimName, 0, CAnimFilterAll());
        CMeshDataBank* secondBank = GetWorld()->Bank;
        secondTime = secondBank->GetAnimation(second->AnimationIndex)->Duration;
    }
    SetStrikeTimes(triggerTime, secondTime);
}
int main()
{
    CMeshDataBank bank; for (int i = 0; i < 4; ++i) bank.Infos[i].Duration = 1.5f * (i + 1);
    CWorld world = { &bank };
    CMeleeTrapDef sub; sub.RefCount = 1; sub.Animations.Trigger.AnimationIndex = 1; sub.Animations.Second.AnimationIndex = 3; sub.Animations.HasSecond = true;
    CDefClassBase cls = { &sub }; CThing thing = { &cls };
    CTCMeleeTrap trap; trap.ParentThing = &thing; trap.World = &world; trap.Strikes = 0;
    trap.TriggerTrap();
    CHECK(trap.Strikes == 1 && trap.Trigger == 3.0f && trap.Second == 6.0f);
    CHECK(strcmp(trap.InitName, "trigger") == 0 && !trap.InitLoop);
    CHECK(sub.RefCount == 1);
    sub.Animations.HasSecond = false;
    trap.TriggerTrap();
    CHECK(trap.Strikes == 2 && trap.Trigger == 3.0f && trap.Second == 0.0f && sub.RefCount == 1);
    if (!g_fail) printf("00788406_TEST PASS\n");
    return g_fail;
}