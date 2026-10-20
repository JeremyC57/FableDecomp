// Behaviour test for CTCCameraCollision::GetCameraPhysicsMeshObject (0x007E30A0).
#include <cstdio>

struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up, Forward; };
static int g_deleted = 0;
struct CPhysicsMesh {
    virtual void DeleteThis() { ++g_deleted; }
    long m_refCount;
    void DecRef() { --m_refCount; }
    long GetRefCount() const { return m_refCount; }
    void Release() { DecRef(); if (GetRefCount() == 0) DeleteThis(); }
};
template <class T> struct CIVCountedPointer {
    T* m_pObject;
    CIVCountedPointer() : m_pObject(0) {}
    CIVCountedPointer(const CIVCountedPointer& o) : m_pObject(o.m_pObject) { if (m_pObject) ++m_pObject->m_refCount; }
    ~CIVCountedPointer() { if (m_pObject) m_pObject->Release(); }
};
struct CBaseClass { CBaseClass() {} virtual ~CBaseClass() {} };
static const C3DVector* g_pos = 0; static const CRightHandedSet* g_axes = 0;
struct CPhysicsMeshObject : CBaseClass {
    CIVCountedPointer<CPhysicsMesh> Mesh; float PositionMatrix[12]; float Scale; float OO_Scale;
    CPhysicsMeshObject(const CIVCountedPointer<CPhysicsMesh>& mesh, const C3DVector& position, const CRightHandedSet& orientation, float scale)
        : Mesh(mesh)
    {
        g_pos = &position; g_axes = &orientation;
        for (int i = 0; i < 12; ++i) PositionMatrix[i] = (float)i;
        Scale = scale; OO_Scale = 1.0f / scale;
    }
    ~CPhysicsMeshObject() {}
};
static CPhysicsMesh g_mesh;
struct CPhysicsMeshInfo {
    CIVCountedPointer<CPhysicsMesh> GetMesh() const { CIVCountedPointer<CPhysicsMesh> p; p.m_pObject = &g_mesh; ++g_mesh.m_refCount; return p; }
};
static CPhysicsMeshInfo g_info; static long g_askedIndex = -1;
struct CMeshBank { CPhysicsMeshInfo* GetPhysicsMeshInfo(long index) { g_askedIndex = index; return &g_info; } };
struct CWorld { CMeshBank* PMainMeshBank; };
struct CThingPhysicsMeshInfo { float Pad; float Radius; };
static CRightHandedSet g_orientation;
struct CThingPhysics {
    CThingPhysicsMeshInfo MeshInfo; C3DVector Position;
    virtual const CRightHandedSet& GetOrientation() const { return g_orientation; }
    virtual const CThingPhysicsMeshInfo* GetPhysicsMeshInfo() { return &MeshInfo; }
};
struct CThing { CThingPhysics* PPhysics; };
struct CCameraCollisionDef { long PhysicsMesh; };
static CWorld* g_world = 0;
class CTCCameraCollision {
public:
    CThing* ParentThing; const CCameraCollisionDef* CameraCollisionDef;
    CWorld* GetWorld() const { return g_world; }
    CPhysicsMeshObject GetCameraPhysicsMeshObject() const
    {
        CThingPhysics* physics = ParentThing->PPhysics;
        const CThingPhysicsMeshInfo* thingMesh = physics->GetPhysicsMeshInfo();
        const CCameraCollisionDef* def = CameraCollisionDef;
        CMeshBank* bank = GetWorld()->PMainMeshBank;
        CPhysicsMeshInfo* info = bank->GetPhysicsMeshInfo(def->PhysicsMesh);
        CPhysicsMeshObject object(info->GetMesh(), physics->Position, physics->GetOrientation(), thingMesh->Radius * 0.01f);
        return object;
    }
};

int main()
{
    g_mesh.m_refCount = 1;
    CMeshBank bank; CWorld world; world.PMainMeshBank = &bank; g_world = &world;
    CThingPhysics physics; physics.MeshInfo.Radius = 200.0f; physics.Position.X = 1.0f;
    CThing thing; thing.PPhysics = &physics;
    CCameraCollisionDef def; def.PhysicsMesh = 17;
    CTCCameraCollision c; c.ParentThing = &thing; c.CameraCollisionDef = &def;
    bool ok;
    {
        CPhysicsMeshObject o = c.GetCameraPhysicsMeshObject();
        float expect = 200.0f * 0.01f;
        ok = g_askedIndex == 17 && g_pos == &physics.Position && g_axes == &g_orientation && o.Mesh.m_pObject == &g_mesh
            && g_mesh.m_refCount == 2 && o.Scale == expect && o.PositionMatrix[11] == 11.0f;
    }
    ok = ok && g_mesh.m_refCount == 1 && g_deleted == 0;
    if (!ok) { std::printf("007e30a0_TEST FAIL\n"); return 1; }
    std::printf("007e30a0_TEST PASS\n");
    return 0;
}
