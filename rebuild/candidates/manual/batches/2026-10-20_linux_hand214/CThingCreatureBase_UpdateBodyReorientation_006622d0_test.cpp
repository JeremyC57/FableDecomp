// behaviour test for CThingCreatureBase::UpdateBodyReorientation (0x006622D0): reorienter update/stop and the YZ pitch reset gates
#include <cstdio>
#include <cmath>
static const float g_epsilon_0129ba3c = 1e-4f;
static int g_updates = 0, g_setFlagCalls = 0, g_setYZ = 0;
struct CTCPhysicsBase { float m_yz; float GetFacingAngleYZ() { return m_yz; } void SetFacingAngleYZ(float a) { ++g_setYZ; m_yz = a; } };
struct CThingBodyReorienter { bool m_need; bool m_flag;
    bool IsUpdateNeeded() { return m_need; } void UpdateBodyReorienation() { ++g_updates; } void SetFlag_00863260(bool v) { ++g_setFlagCalls; m_flag = v; } };
template<class T> struct scoped_ptr { T* px; T* get() const { return px; } T* operator->() const { return px; } };
struct CCreatureActionBase { bool Finished; };
struct CThing { CTCPhysicsBase* PhysicsTC; unsigned char Dead : 1; bool m_lying, m_incap; bool IsAlive() const { return Dead ? false : true; } };
static bool IsLyingDown(CThing* t) { return t->m_lying; }
static bool IsIncapacitated(CThing* t) { return t->m_incap; }
struct CThingCreatureBase : CThing {
    CCreatureActionBase* PCurrentAction_Data; scoped_ptr<CThingBodyReorienter> BodyReorienter;
    bool HadFacingAngleYZSetByBodyOrientation : 1;
    bool IsPerformingAnyAction() { CCreatureActionBase* a = PCurrentAction_Data; if (a != 0) { if (!a->Finished) return true; } return false; }
    void UpdateBodyReorientation()
    {
        if (BodyReorienter.get() && BodyReorienter->IsUpdateNeeded()) {
            HadFacingAngleYZSetByBodyOrientation = false;
            BodyReorienter->UpdateBodyReorienation();
            if (!IsPerformingAnyAction()) BodyReorienter->SetFlag_00863260(false);
        }
        if (IsAlive() && HadFacingAngleYZSetByBodyOrientation) {
            if (std::fabs(PhysicsTC->GetFacingAngleYZ()) > g_epsilon_0129ba3c) {
                if (!IsPerformingAnyAction() && !IsLyingDown(this) && !IsIncapacitated(this)) {
                    PhysicsTC->SetFacingAngleYZ(0.0f);
                    HadFacingAngleYZSetByBodyOrientation = false;
                }
            }
        }
    }
};
int main()
{
    CTCPhysicsBase phys; phys.m_yz = -0.5f;
    CThingBodyReorienter reo; reo.m_need = true; reo.m_flag = true;
    CCreatureActionBase act; act.Finished = false;
    CThingCreatureBase c; c.PhysicsTC = &phys; c.Dead = 0; c.m_lying = false; c.m_incap = false;
    c.PCurrentAction_Data = &act; c.BodyReorienter.px = &reo; c.HadFacingAngleYZSetByBodyOrientation = true;
    int bad = 0;
    c.UpdateBodyReorientation();             // busy action: update runs, flag cleared, reorienter kept, no reset
    if (g_updates != 1 || g_setFlagCalls != 0 || c.HadFacingAngleYZSetByBodyOrientation || g_setYZ != 0) bad = 1;
    c.BodyReorienter.px = 0; c.HadFacingAngleYZSetByBodyOrientation = true; act.Finished = true;
    c.UpdateBodyReorientation();             // idle, alive, tilted: pitch levelled
    if (g_setYZ != 1 || phys.m_yz != 0.0f || c.HadFacingAngleYZSetByBodyOrientation || g_updates != 1) bad = 1;
    c.HadFacingAngleYZSetByBodyOrientation = true; phys.m_yz = 0.5f; c.m_lying = true;
    c.UpdateBodyReorientation();             // lying down: no reset
    if (g_setYZ != 1 || !c.HadFacingAngleYZSetByBodyOrientation) bad = 1;
    c.m_lying = false; phys.m_yz = 0.00001f;
    c.UpdateBodyReorientation();             // within epsilon: no reset
    if (g_setYZ != 1) bad = 1;
    phys.m_yz = 0.5f; c.Dead = 1;
    c.UpdateBodyReorientation();             // dead: no reset
    if (g_setYZ != 1) bad = 1;
    c.Dead = 0; c.BodyReorienter.px = &reo; c.PCurrentAction_Data = 0;
    c.UpdateBodyReorientation();             // no action: reorienter stopped, flag cleared, nothing more to level
    if (g_updates != 2 || g_setFlagCalls != 1 || reo.m_flag || g_setYZ != 1) bad = 1;
    if (bad) { std::printf("006622d0_TEST FAIL\n"); return 1; }
    std::printf("006622d0_TEST PASS\n");
    return 0;
}
