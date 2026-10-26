#include <cstdio>
// Behaviour model of CTCPhysicsBase::GetPhysicsMeshObject (0x006B0850)
class C3DVector { public: float X, Y, Z; };
class CPhysicsMesh { public: long refs; int destroyed; };
struct CMeshPtr { CPhysicsMesh* Data; CMeshPtr(CPhysicsMesh* d) : Data(d) { if (d) ++d->refs; }
    ~CMeshPtr() { if (Data && --Data->refs == 0) ++Data->destroyed; } };
class CPhysicsMeshInfo { public: CPhysicsMesh* mesh; CMeshPtr GetMesh() const { return CMeshPtr(mesh); } };
struct CMeshBank { CPhysicsMeshInfo* infos; CPhysicsMeshInfo* GetPhysicsMeshInfo(long i) { return &infos[i]; } };
class CThingPhysicsMeshInfo { public: unsigned short BankIndex; float RenderSizeX; };
class CPhysicsMeshObject { public: CPhysicsMesh* mesh; C3DVector pos; int rhs; float scale;
    CPhysicsMeshObject(const CMeshPtr& m, const C3DVector& p, const int& r, float s) : mesh(m.Data), pos(p), rhs(r), scale(s) {} };
class CTCPhysicsBase { public:
    const CThingPhysicsMeshInfo* info; CMeshBank* bank; C3DVector Position; int rhs;
    const CThingPhysicsMeshInfo* GetPhysicsMeshInfo() { return info; } const int& GetRightHandedSet() { return rhs; }
    CPhysicsMeshObject GetPhysicsMeshObject() {
        const CThingPhysicsMeshInfo* i = GetPhysicsMeshInfo();
        CMeshBank* b = bank;
        CPhysicsMeshInfo* mi = b->GetPhysicsMeshInfo(i->BankIndex);
        return CPhysicsMeshObject(mi->GetMesh(), Position, GetRightHandedSet(), i->RenderSizeX * 0.01f);
    } };
int main()
{
    CPhysicsMesh mesh = { 1, 0 };
    CPhysicsMeshInfo infos[2] = { { 0 }, { &mesh } };
    CMeshBank bank = { infos };
    CThingPhysicsMeshInfo ti = { 1, 200.0f };
    CTCPhysicsBase p = { &ti, &bank, { 1, 2, 3 }, 7 };
    CPhysicsMeshObject o = p.GetPhysicsMeshObject();
    if (o.mesh != &mesh || o.pos.Y != 2 || o.rhs != 7 || o.scale < 1.99f || o.scale > 2.01f || mesh.refs != 1 || mesh.destroyed)
    { std::printf("006b0850_TEST FAIL\n"); return 1; }
    std::printf("006b0850_TEST PASS\n");
    return 0;
}