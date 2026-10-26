#include <cstdio>
// Test model: a thing whose physics component may report the slot-42 flag, a CanBlockNavigatingCreature answer, an optional
// physics mesh and a bounding box; the mesh object is a temporary whose destruction is counted.
class C3DVector { public: float X, Y, Z; };
static int g_meshObjects = 0;
class CPhysicsMeshObject { public:
    float MinX, MaxX;
    CPhysicsMeshObject(float a, float b) : MinX(a), MaxX(b) { ++g_meshObjects; }
    CPhysicsMeshObject(const CPhysicsMeshObject& o) : MinX(o.MinX), MaxX(o.MaxX) { ++g_meshObjects; }
    ~CPhysicsMeshObject() { --g_meshObjects; }
    bool IsPointContainedInBoundingBox(const C3DVector& p) const { return p.X >= MinX && p.X <= MaxX; }
};
class CTCPhysicsBase { public:
    bool Flag42; bool HasMesh; float MinX, MaxX;
    bool IsFlag42() const { return Flag42; }
    const void* GetPhysicsMeshInfo() const { return HasMesh ? this : 0; }
    CPhysicsMeshObject GetPhysicsMeshObject() const { return CPhysicsMeshObject(MinX, MaxX); }
};
class CThing { public:
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

int main()
{
    C3DVector p = { 5.0f, 0.0f, 0.0f };
    CTCPhysicsBase tc = { true, true, 0.0f, 10.0f };
    CThing t = { true, &tc };
    CThingFilter_BlockerBoundsContainPoint f = { 0, &p };
    if (!f(t) || g_meshObjects != 0) { std::printf("0063f150_TEST FAIL\n"); return 1; }
    tc.Flag42 = false;
    if (!f(t)) { std::printf("0063f150_TEST FAIL\n"); return 1; }
    p.X = 20.0f;
    if (f(t) || g_meshObjects != 0) { std::printf("0063f150_TEST FAIL\n"); return 1; }
    p.X = 5.0f; tc.HasMesh = false;
    if (f(t)) { std::printf("0063f150_TEST FAIL\n"); return 1; }
    tc.HasMesh = true; t.Blocks = false;
    if (f(t)) { std::printf("0063f150_TEST FAIL\n"); return 1; }
    std::printf("0063f150_TEST PASS\n");
    return 0;
}