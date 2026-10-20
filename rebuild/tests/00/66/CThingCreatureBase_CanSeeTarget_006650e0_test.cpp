// behaviour test for CThingCreatureBase::CanSeeTarget (0x006650E0): look-component forwarding, else 2*|angle diff| <= fov
#include <cstdio>
#include <cmath>
struct C3DVector { float X, Y, Z; C3DVector() {} C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} };
struct CTCBase;
struct CInterfaceMap { struct SEntry { long type; CTCBase* component; }; SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) const { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; } };
struct CTCPhysicsBase { float m_facing; float GetFacingAngleXY() { return m_facing; } };
static int g_lookCalls = 0;
struct CTCLook { bool m_answer; bool IsPosInViewRange(const C3DVector&, float fov, bool a, bool b) { ++g_lookCalls; return m_answer && fov > 0 && a && !b; } };
static float AngleBetweenPositions(const C3DVector& from, const C3DVector& to) { return (float)std::atan2(to.Y - from.Y, to.X - from.X); }
static float AngleDifference(float a, float b) { return b - a; }
class CThing { public:
    unsigned long UsingComponents[9]; CInterfaceMap Components; CTCPhysicsBase* PhysicsTC;
    template<class T> T* GetTC(long interfaceType)
    { long type = interfaceType; const CInterfaceMap* map = &Components; CInterfaceMap::SEntry* it = map->LowerBound(&type);
      if (it == map->last || interfaceType < it->type) it = map->last; return (T*)it->component; } };
class CThingCreatureBase : public CThing { public:
    C3DVector m_head;
    C3DVector GetHeadPos(bool) { return m_head; }
    bool CanSeeTarget(const C3DVector& position, float fieldOfView, bool checkRange, bool checkObstruction)
    {
        if (UsingComponents[0x43 >> 5] & (1UL << (0x43 & 31)))
            return GetTC<CTCLook>(0x43)->IsPosInViewRange(position, fieldOfView, checkRange, checkObstruction);
        float facing = PhysicsTC->GetFacingAngleXY();
        if (std::fabs(AngleDifference(facing, AngleBetweenPositions(GetHeadPos(true), position))) * 2.0f <= fieldOfView) return true;
        return false;
    } };
int main()
{
    CTCPhysicsBase phys; phys.m_facing = 0.0f;
    CTCLook look; look.m_answer = true;
    CInterfaceMap::SEntry e[1] = { { 0x43, (CTCBase*)&look } };
    CThingCreatureBase c; for (int i = 0; i < 9; ++i) c.UsingComponents[i] = 0; c.Components.first = e; c.Components.last = e + 1;
    c.PhysicsTC = &phys; c.m_head = C3DVector(0, 0, 0);
    int bad = 0;
    C3DVector ahead(10, 1, 0), side(0, 10, 0);
    if (!c.CanSeeTarget(ahead, 0.5f, false, false) || !c.CanSeeTarget(side, 4.0f, true, true)) bad = 1;   // angles ~0.0997 and pi/2
    if (c.CanSeeTarget(side, 3.0f, false, false) || c.CanSeeTarget(ahead, 0.1f, false, false)) bad = 1;
    if (g_lookCalls != 0) bad = 1;
    c.UsingComponents[2] = 1UL << 3;                         // TC 0x43 present: forwarded
    if (!c.CanSeeTarget(side, 1.0f, true, false) || c.CanSeeTarget(side, 1.0f, true, true) || g_lookCalls != 2) bad = 1;
    if (bad) { std::printf("006650e0_TEST FAIL\n"); return 1; }
    std::printf("006650e0_TEST PASS\n");
    return 0;
}