// Behaviour test for 0x004BC4F0 NGraphicAppearance::CTCGraphicAppearance::GetMeshObject(const CMeshInfo&, float,
// C3DMeshObject&) const: the mesh object is built from the bank's mesh info for the entry, the owner's interpolated
// render position, the current orientation when interpolation == 1 (else the interpolated one) and
// SizeX * Scale * 0.01, then assigned to the caller's object.
#include <cstdio>
#include <cmath>

class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };
class C3DMeshInfo { public: int id; };

static int g_currentCalls = 0, g_interpolatedCalls = 0, g_assigns = 0, g_live = 0;
class C3DMeshObject { public:
    const C3DMeshInfo* MeshInfo;
    C3DVector Position;
    CRightHandedSet Orientation;
    float Scale;
    C3DMeshObject() : MeshInfo(0), Scale(0) { ++g_live; }
    C3DMeshObject(const C3DMeshInfo* info, const C3DVector& p, const CRightHandedSet& o, float s)
        : MeshInfo(info), Position(p), Orientation(o), Scale(s) { ++g_live; }
    C3DMeshObject& operator=(const C3DMeshObject& o)
    {
        MeshInfo = o.MeshInfo; Position = o.Position; Orientation = o.Orientation; Scale = o.Scale; ++g_assigns;
        return *this;
    }
    ~C3DMeshObject() { --g_live; }
};

static C3DMeshInfo g_infos[4] = { { 10 }, { 11 }, { 12 }, { 13 } };
class CMeshDataBank { public: const C3DMeshInfo* GetMeshInfo(int index) { return &g_infos[index]; } };
static CMeshDataBank g_bank;
static CMeshDataBank* g_pMeshDataBank = &g_bank;

class CTCPhysicsBase { public:
    CRightHandedSet current, old;
    const CRightHandedSet& GetRHSet() const { ++g_currentCalls; return current; }
    CRightHandedSet GetRHSet(float t) const
    {
        ++g_interpolatedCalls;
        CRightHandedSet r = old;
        r.Forward.X = old.Forward.X + (current.Forward.X - old.Forward.X) * t;
        return r;
    }
};
class CThing { public: CTCPhysicsBase* PhysicsTC; C3DVector pos; };
static C3DVector GetRenderPositionForThing(const CThing& thing, float t)
{
    C3DVector p = thing.pos; p.Z += t; return p;
}
struct CMeshInfo { int Index; float SizeX; };
struct CTCGraphicAppearance {
    CThing* ParentThing;
    float Scale;
    void GetMeshObject(const CMeshInfo& info, float interpolation, C3DMeshObject& out) const
    {
        const CTCPhysicsBase* physics = ParentThing->PhysicsTC;
        C3DVector position = GetRenderPositionForThing(*ParentThing, interpolation);
        const C3DMeshInfo* meshInfo = g_pMeshDataBank->GetMeshInfo(info.Index);
        if (interpolation == 1.0f)
            out = C3DMeshObject(meshInfo, position, physics->GetRHSet(), info.SizeX * Scale * 0.01f);
        else
            out = C3DMeshObject(meshInfo, position, physics->GetRHSet(interpolation), info.SizeX * Scale * 0.01f);
    }
};

int main()
{
    int ok = 1;
    CTCPhysicsBase physics;
    physics.current.Up.X = 0; physics.current.Up.Y = 0; physics.current.Up.Z = 1;
    physics.current.Forward.X = 1; physics.current.Forward.Y = 0; physics.current.Forward.Z = 0;
    physics.old = physics.current; physics.old.Forward.X = 0;
    CThing thing; thing.PhysicsTC = &physics; thing.pos.X = 5; thing.pos.Y = 6; thing.pos.Z = 7;
    CTCGraphicAppearance appearance; appearance.ParentThing = &thing; appearance.Scale = 2.0f;
    CMeshInfo entry; entry.Index = 2; entry.SizeX = 50.0f;
    {
        C3DMeshObject out;
        appearance.GetMeshObject(entry, 1.0f, out);
        if (out.MeshInfo != &g_infos[2] || std::fabs(out.Scale - 1.0f) > 1e-5f || out.Position.Z != 8.0f ||
            out.Orientation.Forward.X != 1.0f || g_currentCalls != 1 || g_interpolatedCalls != 0 || g_assigns != 1) ok = 0;
        appearance.GetMeshObject(entry, 0.5f, out);
        if (std::fabs(out.Orientation.Forward.X - 0.5f) > 1e-5f || out.Position.Z != 7.5f || g_currentCalls != 1 ||
            g_interpolatedCalls != 1 || g_assigns != 2) ok = 0;
        if (g_live != 1) ok = 0;                               // the temporaries are gone
    }
    if (g_live != 0) ok = 0;
    if (!ok) { std::printf("004bc4f0_TEST FAIL\n"); return 1; }
    std::printf("004bc4f0_TEST PASS\n");
    return 0;
}