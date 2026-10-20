// Behaviour test for CTCLook::GetAnglesFromBodyToPos (0x00751C70).
#include <cstdio>
#include <math.h>

struct C3DVector { float X, Y, Z; };
static float AngleDifference(float a, float b) { return b - a; }
static float AngleBetweenPositions(const C3DVector& from, const C3DVector& to) { return (to.X - from.X) * 0.01f; }
static float AngleYZBetweenPositions(const C3DVector& from, const C3DVector& to) { return (to.Z - from.Z) * 0.01f; }
static bool g_bQueryCreatureHeadPos = false;
static float g_epsilon = 1e-4f;

struct CPhysics { float FacingXY, FacingYZ; float GetFacingAngleXY() { return FacingXY; } float GetFacingAngleYZ() { return FacingYZ; } };
struct CThing { CPhysics* PhysicsTC; };
struct CThingCreatureBase {
    unsigned char m_flags91;
    C3DVector Head;
    C3DVector GetHeadPos(bool) { return Head; }
};
template<class T> inline bool IS_THING_ALIVE(T* const& p)
{
    if (p != 0) {
        if (!(p->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CTCLook {
    CThing* ParentThing;
    CThingCreatureBase* PParentCreature;
    float RecoveryAngleToLookAtXY, RecoveryAngleToLookAtYZ;
    C3DVector CachedHeadPos;
    C3DVector GetHeadPos()
    {
        if (g_bQueryCreatureHeadPos)
            return PParentCreature->GetHeadPos(false);
        return CachedHeadPos;
    }
    void GetAnglesFromBodyToPos(const C3DVector& pos, float*, float* angleXY, float* angleYZ)
    {
        float bodyXY = ParentThing->PhysicsTC->GetFacingAngleXY();
        float bodyYZ = ParentThing->PhysicsTC->GetFacingAngleYZ();
        if (IS_THING_ALIVE(PParentCreature)) {
            C3DVector headPos = GetHeadPos();
            if ((float)fabs(pos.X - headPos.X) > g_epsilon || (float)fabs(pos.Y - headPos.Y) > g_epsilon ||
                (float)fabs(pos.Z - headPos.Z) > g_epsilon) {
                float xy = AngleBetweenPositions(headPos, pos);
                float yz = AngleYZBetweenPositions(headPos, pos);
                *angleXY = AngleDifference(bodyXY, xy);
                *angleYZ = AngleDifference(bodyYZ, yz);
                return;
            }
        }
        *angleXY = AngleDifference(bodyXY, RecoveryAngleToLookAtXY);
        *angleYZ = AngleDifference(bodyYZ, RecoveryAngleToLookAtYZ);
    }
};

static bool Near(float a, float b) { return fabs(a - b) < 1e-5f; }

int main()
{
    CPhysics phys = { 0.1f, 0.2f };
    CThing thing = { &phys };
    CThingCreatureBase creature; creature.m_flags91 = 0; creature.Head.X = 50; creature.Head.Y = 0; creature.Head.Z = 50;
    CTCLook look; look.ParentThing = &thing; look.PParentCreature = &creature;
    look.RecoveryAngleToLookAtXY = 0.5f; look.RecoveryAngleToLookAtYZ = 0.6f;
    look.CachedHeadPos.X = 0; look.CachedHeadPos.Y = 0; look.CachedHeadPos.Z = 0;
    C3DVector pos = { 10.0f, 0.0f, 20.0f };
    float xy = 0, yz = 0;
    bool ok = true;
    look.GetAnglesFromBodyToPos(pos, 0, &xy, &yz);            // cached head (0,0,0): xy 0.1-0.1, yz 0.2-0.2
    ok = ok && Near(xy, 0.0f) && Near(yz, 0.0f);
    g_bQueryCreatureHeadPos = true;                           // creature head (50,0,50): xy -0.4-0.1, yz -0.3-0.2
    look.GetAnglesFromBodyToPos(pos, 0, &xy, &yz);
    ok = ok && Near(xy, -0.5f) && Near(yz, -0.5f);
    creature.m_flags91 = 1;                                   // dead: recovery angles
    look.GetAnglesFromBodyToPos(pos, 0, &xy, &yz);
    ok = ok && Near(xy, 0.4f) && Near(yz, 0.4f);
    creature.m_flags91 = 0; pos = creature.Head;              // at the head: recovery angles
    look.GetAnglesFromBodyToPos(pos, 0, &xy, &yz);
    ok = ok && Near(xy, 0.4f) && Near(yz, 0.4f);
    if (!ok) { std::printf("00751c70_TEST FAIL\n"); return 1; }
    std::printf("00751c70_TEST PASS\n");
    return 0;
}
