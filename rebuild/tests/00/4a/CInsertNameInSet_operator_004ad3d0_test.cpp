#include <cstdio>

// Behaviour model of CInsertNameInSet::operator()(const CThing*) (0x004AD3D0): a thing with a physics component that
// has a mesh, whose mesh object carries no navigation line-list info, gets its def name inserted into the set.
static int g_meshObjects = 0, g_meshObjectsAlive = 0;
struct CPhysicsMeshObject {
    bool nav;
    CPhysicsMeshObject(bool n) : nav(n) { ++g_meshObjects; ++g_meshObjectsAlive; }
    CPhysicsMeshObject(const CPhysicsMeshObject& o) : nav(o.nav) { ++g_meshObjectsAlive; }
    ~CPhysicsMeshObject() { --g_meshObjectsAlive; }
    bool HasNavigationLineListInfo() const { return nav; }
};
struct CTCPhysicsBase {
    const void* meshInfo; bool nav;
    const void* GetPhysicsMeshInfo() { return meshInfo; }
    CPhysicsMeshObject GetPhysicsMeshObject() const { return CPhysicsMeshObject(nav); }
};
struct CDefString { long TablePos; const char* GetString() const { static const char* names[] = { "TREE", "ROCK", "WALL" }; return names[TablePos]; } };
struct CThing { CTCPhysicsBase* PhysicsTC; CDefString def; CDefString GetDefName() const { return def; } };
struct CCharStringSet { const char* items[8]; int n; void insert(const char* s) { for (int i = 0; i < n; ++i) if (items[i] == s) return; items[n++] = s; } };
struct CInsertNameInSet {
    CCharStringSet* DefNames;
    void operator()(const CThing* thing) const
    {
        CTCPhysicsBase* physics = thing->PhysicsTC;
        if (physics && physics->GetPhysicsMeshInfo()) {
            CPhysicsMeshObject mesh = thing->PhysicsTC->GetPhysicsMeshObject();
            if (!mesh.HasNavigationLineListInfo())
                DefNames->insert(thing->GetDefName().GetString());
        }
    }
};

int main()
{
    int info = 1;
    CTCPhysicsBase withMesh = { &info, false }, navMesh = { &info, true }, noMesh = { 0, false };
    CThing tree = { &withMesh, { 0 } }, rock = { &navMesh, { 1 } }, wall = { &noMesh, { 2 } }, ghost = { 0, { 2 } }, tree2 = { &withMesh, { 0 } };
    CCharStringSet set; set.n = 0;
    CInsertNameInSet f = { &set };
    f(&tree); f(&rock); f(&wall); f(&ghost); f(&tree2);
    if (set.n != 1 || set.items[0] != tree.GetDefName().GetString()) { std::printf("004ad3d0_TEST FAIL\n"); return 1; }
    if (g_meshObjects != 3 || g_meshObjectsAlive != 0) { std::printf("004ad3d0_TEST FAIL\n"); return 1; }   // mesh object only for the two meshed things + tree2, all destroyed
    std::printf("004ad3d0_TEST PASS\n");
    return 0;
}