#include <cstdio>
struct CTCPhysicsBase {
    virtual void v000() {} virtual void v001() {} virtual void v002() {} virtual void v003() {} virtual void v004() {} virtual void v005() {} virtual void v006() {} virtual void v007() {} virtual void v008() {} virtual void v009() {} virtual void v010() {} virtual void v011() {} virtual void v012() {} virtual void v013() {} virtual void v014() {} virtual void v015() {} virtual void v016() {} virtual void v017() {} virtual void v018() {} virtual void v019() {} virtual void v020() {} virtual void v021() {} virtual void v022() {} virtual void v023() {} virtual void v024() {} virtual void v025() {} virtual void v026() {} virtual void v027() {} virtual void v028() {} virtual void v029() {} virtual void v030() {} virtual void v031() {} virtual void v032() {} virtual void v033() {} virtual void v034() {} virtual void v035() {} virtual void v036() {} virtual void v037() {} virtual void v038() {} virtual void v039() {} virtual void v040() {} virtual void v041() {} virtual void v042() {} virtual void v043() {} virtual void v044() {} virtual void v045() {} virtual void v046() {} virtual void v047() {} virtual void v048() {} virtual void v049() {} virtual void v050() {} virtual void v051() {} virtual void v052() {} virtual void v053() {} virtual void v054() {} virtual void v055() {} virtual void v056() {}
    virtual bool IsNavigationBlocker() { return m_blocker; }
    bool m_blocker;
};
struct CThingPhysical {
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;
    unsigned char   _pad64[0x8];
    unsigned long   ThingTypeFlags;
    bool CanBlockNavigatingCreature();
};
bool CThingPhysical::CanBlockNavigatingCreature()
{
    CTCPhysicsBase* physics = PhysicsTC;
    if (physics && physics->IsNavigationBlocker() && !(ThingTypeFlags & 8))
        return true;
    return false;
}
int main()
{
    CTCPhysicsBase tc; tc.m_blocker = true;
    CThingPhysical t; t.PhysicsTC = 0; t.ThingTypeFlags = 0;
    int bad = 0;
    if (t.CanBlockNavigatingCreature()) bad = 1;
    t.PhysicsTC = &tc;
    if (!t.CanBlockNavigatingCreature()) bad = 2;
    t.ThingTypeFlags = 8;
    if (t.CanBlockNavigatingCreature()) bad = 3;
    t.ThingTypeFlags = 7; tc.m_blocker = false;
    if (t.CanBlockNavigatingCreature()) bad = 4;
    if (bad) { std::printf("006a4d90_TEST FAIL\n"); return 1; }
    std::printf("006a4d90_TEST PASS\n");
    return 0;
}
