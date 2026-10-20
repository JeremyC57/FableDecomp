// Behaviour test for 00663ba0: body copied from src/00663ba0.cpp (CThingCreatureBase::GetMovementSpeedFromCreatureMode), the callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
static C3DVector g_offsetSeen, g_facingSeen; static long g_typeSeen = -1; static int g_speedCalls = 0;
struct CModeBase {
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {}
    virtual void v06() {}
    virtual float GetMovementSpeed(const C3DVector& offset, const C3DVector& facing, long movementType) { ++g_speedCalls; g_offsetSeen = offset; g_facingSeen = facing; g_typeSeen = movementType; return 3.5f; }
};
struct CTCCreatureModeManager { CModeBase mode; CModeBase* GetCurrentMode() { return &mode; } };
struct CTCPhysicsBase {
    virtual C3DVector GetFacing() const { return C3DVector(0.0f, 1.0f, 0.0f); }
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;
    C3DVector position;
    const C3DVector& GetPosition() const { return position; }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
};
struct CThingCreatureBase : CThing {
    float GetMovementSpeedFromCreatureMode(const C3DVector& target, long movementType);
    __forceinline CTCCreatureModeManager* GetModeManager()
    {
        CTCCreatureModeManager* modes = 0;
        if (HasTC(0x31))
            modes = GetTC<CTCCreatureModeManager>(0x31);
        return modes;
    }
};
float CThingCreatureBase::GetMovementSpeedFromCreatureMode(const C3DVector& target, long movementType)
{
    CModeBase* mode = GetModeManager()->GetCurrentMode();
    C3DVector offset = target - GetPosition();
    C3DVector facing = PhysicsTC->GetFacing();
    return mode->GetMovementSpeed(offset, facing, movementType);
}
int main()
{
    CThingCreatureBase c; memset(&c, 0, sizeof(c)); CTCCreatureModeManager modes; CTCPhysicsBase phys;
    CInterfaceMap::SEntry entries[2] = { { 0x10, (CTCBase*)0x99 }, { 0x31, (CTCBase*)&modes } };
    c.m_interfaces.first = entries; c.m_interfaces.last = entries + 2; c.m_presentBits[0x31 >> 5] = 1UL << (0x31 & 31);
    c.PhysicsTC = &phys; c.position = C3DVector(1.0f, 2.0f, 3.0f);
    // the mode manager is looked up (bit present), the offset is target - position, facing from the physics component
    float r = c.GetMovementSpeedFromCreatureMode(C3DVector(4.0f, 6.0f, 9.0f), 7);
    CHECK(r == 3.5f); CHECK(g_speedCalls == 1); CHECK(g_typeSeen == 7);
    CHECK(g_offsetSeen.X == 3.0f && g_offsetSeen.Y == 4.0f && g_offsetSeen.Z == 6.0f);
    CHECK(g_facingSeen.X == 0.0f && g_facingSeen.Y == 1.0f && g_facingSeen.Z == 0.0f);
    // the helper's miss arm yields a null manager (not dereferenced here)
    CThingCreatureBase d; memset(&d, 0, sizeof(d)); CHECK(d.GetModeManager() == 0); CHECK(c.GetModeManager() == &modes);
    if (fails == 0) printf("00663ba0_TEST PASS\n"); else printf("00663ba0_TEST FAIL\n");
    return fails;
}