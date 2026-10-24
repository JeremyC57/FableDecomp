// Behaviour test for 0x004FB370 BuildingHasVolume(building): false for a null building, a building without a physics
// component or a component without a mesh; otherwise the mesh object's HasContainmentVolume("BUILDING_BOUNDRY").
#include <cstdio>
#include <cstring>

static int g_mesh_objects_alive = 0;

struct MeshObjectModel {
    const char* const* Volumes;
    MeshObjectModel(const char* const* volumes) : Volumes(volumes) { ++g_mesh_objects_alive; }
    MeshObjectModel(const MeshObjectModel& o) : Volumes(o.Volumes) { ++g_mesh_objects_alive; }
    ~MeshObjectModel() { --g_mesh_objects_alive; }
    bool HasContainmentVolume(const char* name) const
    {
        for (const char* const* v = Volumes; v && *v; ++v)
            if (std::strcmp(*v, name) == 0) return true;
        return false;
    }
};

struct PhysicsModel {
    bool HasMesh;
    const char* const* Volumes;
    const void* GetPhysicsMesh() const { return HasMesh ? this : 0; }
    MeshObjectModel GetPhysicsMeshObject() const { return MeshObjectModel(Volumes); }
};

struct BuildingModel {
    PhysicsModel* PhysicsTC;
};

static bool model(const BuildingModel* building)
{
    if (building) {
        PhysicsModel* physics = building->PhysicsTC;
        if (physics && physics->GetPhysicsMesh()) {
            MeshObjectModel mesh = physics->GetPhysicsMeshObject();
            return mesh.HasContainmentVolume("BUILDING_BOUNDRY");
        }
    }
    return false;
}

int main()
{
    static const char* const withBoundary[] = { "DOOR", "BUILDING_BOUNDRY", 0 };
    static const char* const without[] = { "DOOR", 0 };
    PhysicsModel p1 = { true, withBoundary };
    PhysicsModel p2 = { true, without };
    PhysicsModel p3 = { false, withBoundary };
    BuildingModel b1 = { &p1 }, b2 = { &p2 }, b3 = { &p3 }, b4 = { 0 };
    if (!model(&b1) || model(&b2) || model(&b3) || model(&b4) || model(0) || g_mesh_objects_alive != 0) {
        std::printf("004fb370_TEST FAIL\n");
        return 1;
    }
    std::printf("004fb370_TEST PASS\n");
    return 0;
}