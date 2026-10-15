// Behaviour test for 0x0069B650 CGameCameraManager::UpdateObservedThingInBuilding.
#include <cstdio>
static int g_push = 0, g_remove = 0;
struct CThing { unsigned char m_statusFlags; CThing* building; };
static CThing* GetPBuildingThingOccupying(CThing* t) { return t->building; }
struct M {
    bool interruptable; void* PendingMode; bool ObservedThingInBuilding; CThing* PObservedThing;
    bool IsCameraModeInterruptable() { return interruptable; }
    void PushBuildingCameraMode() { ++g_push; }
    void RemoveBuildingCameraMode() { ++g_remove; }
    void Update()
    {
        if (!IsCameraModeInterruptable() && PendingMode == 0) return;
        CThing* building;
        if (PObservedThing && !(PObservedThing->m_statusFlags & 1)
            && (building = GetPBuildingThingOccupying(PObservedThing)) != 0 && !(building->m_statusFlags & 1)) {
            if (!ObservedThingInBuilding) PushBuildingCameraMode();
        } else {
            if (ObservedThingInBuilding) RemoveBuildingCameraMode();
        }
    }
};
static int Fail() { std::printf("0069b650_TEST FAIL\n"); return 1; }
int main()
{
    CThing house; house.m_statusFlags = 0; house.building = 0;
    CThing hero; hero.m_statusFlags = 0; hero.building = &house;
    M m; m.interruptable = false; m.PendingMode = 0; m.ObservedThingInBuilding = false; m.PObservedThing = &hero;
    m.Update();
    if (g_push || g_remove) return Fail();
    m.interruptable = true; m.Update();
    if (g_push != 1) return Fail();
    m.ObservedThingInBuilding = true; m.Update();
    if (g_push != 1 || g_remove != 0) return Fail();
    hero.building = 0; m.Update();
    if (g_remove != 1) return Fail();
    m.interruptable = false; m.PendingMode = &house; hero.building = &house; house.m_statusFlags = 1; m.Update();
    if (g_remove != 2 || g_push != 1) return Fail();
    std::printf("0069b650_TEST PASS\n");
    return 0;
}