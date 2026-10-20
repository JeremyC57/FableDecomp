// behaviour test for CThingCreatureBase::IsThingInFOV (0x00665190): default/perception radius and fov, squared-range gate, angle test forwarding
#include <cstdio>
struct C3DVector { float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} float GetLengthSquared() const { return X * X + Y * Y + Z * Z; } };
inline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
struct CTCBase;
struct CInterfaceMap { struct SEntry { long type; CTCBase* component; }; SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) const { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; } };
struct CTCPerceiveThing { float m_radius, m_fov; float GetExtendedSightRadius() { return m_radius; } float GetFieldOfView_007445d0() { return m_fov; } };
class CThing { public:
    unsigned long UsingComponents[9]; CInterfaceMap Components; C3DVector m_pos;
    CThing() : m_pos(0, 0, 0) {}
    const C3DVector& GetPosition() const { return m_pos; }
    template<class T> T* GetTC(long interfaceType)
    { long type = interfaceType; const CInterfaceMap* map = &Components; CInterfaceMap::SEntry* it = map->LowerBound(&type);
      if (it == map->last || interfaceType < it->type) it = map->last; return (T*)it->component; } };
static float g_lastFov = 0; static int g_angleCalls = 0;
class CThingCreatureBase : public CThing { public:
    bool IsThingInViewAngle_00664e50(CThing*, float fov, bool flag) { ++g_angleCalls; g_lastFov = fov; return flag; }
    bool IsThingInFOV(CThing* thing, bool flag)
    {
        float sightRadius = 10.0f; float fieldOfView = 0.5f;
        if (UsingComponents[0x34 >> 5] & (1UL << (0x34 & 31))) {
            CTCPerceiveThing* perceive = GetTC<CTCPerceiveThing>(0x34);
            sightRadius = perceive->GetExtendedSightRadius(); fieldOfView = perceive->GetFieldOfView_007445d0();
        }
        if ((GetPosition() - thing->GetPosition()).GetLengthSquared() <= sightRadius * sightRadius)
            return IsThingInViewAngle_00664e50(thing, fieldOfView, flag);
        return false;
    } };
int main()
{
    CTCPerceiveThing perceive; perceive.m_radius = 20.0f; perceive.m_fov = 1.25f;
    CInterfaceMap::SEntry e[1] = { { 0x34, (CTCBase*)&perceive } };
    CThingCreatureBase c; for (int i = 0; i < 9; ++i) c.UsingComponents[i] = 0; c.Components.first = e; c.Components.last = e + 1;
    CThing near_, far_; near_.m_pos = C3DVector(6, 8, 0); far_.m_pos = C3DVector(0, 12, 9);   // distances 10 (on the edge) and 15
    int bad = 0;
    if (!c.IsThingInFOV(&near_, true) || g_lastFov != 0.5f || c.IsThingInFOV(&near_, false)) bad = 1;
    if (c.IsThingInFOV(&far_, true) || g_angleCalls != 2) bad = 1;          // out of the default range: no angle test
    c.UsingComponents[1] = 1UL << 20;                                      // perception present: radius 20, fov 1.25
    if (!c.IsThingInFOV(&far_, true) || g_lastFov != 1.25f || g_angleCalls != 3) bad = 1;
    if (bad) { std::printf("00665190_TEST FAIL\n"); return 1; }
    std::printf("00665190_TEST PASS\n");
    return 0;
}
