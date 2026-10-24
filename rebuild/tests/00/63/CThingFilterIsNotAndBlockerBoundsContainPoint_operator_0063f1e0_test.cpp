#include <cstdio>
// Test model: a thing whose physics component may report the slot-42 flag, a CanBlockNavigatingCreature answer, an optional
// physics mesh and a bounding box; the mesh object is a temporary whose destruction is counted.
struct C3DVector { float X, Y, Z; };
static int g_meshObjects = 0;
struct CPhysicsMeshObject {
    float MinX, MaxX;
    CPhysicsMeshObject(float a, float b) : MinX(a), MaxX(b) { ++g_meshObjects; }
    CPhysicsMeshObject(const CPhysicsMeshObject& o) : MinX(o.MinX), MaxX(o.MaxX) { ++g_meshObjects; }
    ~CPhysicsMeshObject() { --g_meshObjects; }
    bool IsPointContainedInBoundingBox(const C3DVector& p) const { return p.X >= MinX && p.X <= MaxX; }
};
struct CTCPhysicsBase {
    bool Flag42; bool HasMesh; float MinX, MaxX;
    bool IsFlag42() const { return Flag42; }
    const void* GetPhysicsMeshInfo() const { return HasMesh ? this : 0; }
    CPhysicsMeshObject GetPhysicsMeshObject() const { return CPhysicsMeshObject(MinX, MaxX); }
};
struct CThing {
    bool Blocks; CTCPhysicsBase* PhysicsTC;
    bool CanBlockNavigatingCreature() const { return Blocks; }
};
inline bool MeshBoundsContainPoint(const CTCPhysicsBase* tc, const C3DVector& pos)
{
    if (tc->GetPhysicsMeshInfo() == 0)
        return false;
    CPhysicsMeshObject mesh = tc->GetPhysicsMeshObject();
    return mesh.IsPointContainedInBoundingBox(pos);
}
class CThingFilter_BlockerBoundsContainPoint {
public:
    const void*      Owner;
    const C3DVector* Pos;
    bool operator()(const CThing& thing) const
    {
        CTCPhysicsBase* physics = thing.PhysicsTC;
        bool blocks = (physics && physics->IsFlag42()) ? thing.CanBlockNavigatingCreature() : thing.CanBlockNavigatingCreature();
        return blocks && MeshBoundsContainPoint(thing.PhysicsTC, *Pos);
    }
};

class CThingFilter_IsNot {
public:
    const CThing* Thing;
    bool operator()(const CThing& thing) const { return Thing != &thing; }   // 0x006A76A0
};
class CThingFilter_IsNotAndBlockerBoundsContainPoint {
public:
    const CThingFilter_IsNot*              NotFilter;
    CThingFilter_BlockerBoundsContainPoint Inner;
    bool operator()(const CThing& thing) const { return (*NotFilter)(thing) && Inner(thing); }
};
int main()
{
    C3DVector p = { 5.0f, 0.0f, 0.0f };
    CTCPhysicsBase tc = { false, true, 0.0f, 10.0f };
    CThing a = { true, &tc }, b = { true, &tc };
    CThingFilter_IsNot notA = { &a };
    CThingFilter_IsNotAndBlockerBoundsContainPoint f;
    f.NotFilter = &notA; f.Inner.Owner = 0; f.Inner.Pos = &p;
    if (f(a) || !f(b) || g_meshObjects != 0) { std::printf("0063f1e0_TEST FAIL\n"); return 1; }
    p.X = -1.0f;
    if (f(b)) { std::printf("0063f1e0_TEST FAIL\n"); return 1; }
    std::printf("0063f1e0_TEST PASS\n");
    return 0;
}