#include <assert.h>
#include <stdio.h>
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS
#define FABLE_RETAIL_STLP_LIST
#include "engine/C2DBoxI.h"
#include "engine/C3DVector.h"
#include "engine/CMeshIntersectionData.h"
#include "stlport/fable_containers.h"

// Isolate the dependencies of the selected member body from the live engine.
#define FABLE_ENGINE_CWORLD_H
#define FABLE_ENGINE_CWORLDMAP_H
#define FABLE_ENGINE_CGAMECAMERABASE_H
#define FABLE_ENGINE_CTHING_H
#define FABLE_ENGINE_CTCPHYSICSBASE_H
#define FABLE_ENGINE_CTHINGGAMEOBJECT_H
#define FABLE_ENGINE_NTHINGMAP_CTHINGMAP_H
#define FABLE_ENGINE_CTCMAPWHO_H
#define FABLE_ENGINE_CTHINGSEARCHTOOLS_H
#define FABLE_ENGINE_CPHYSICSMESHOBJECT_H

namespace
{
int mesh_checks = 0;
int intersections = 0;

void CheckVector(const C3DVector& vector, float x, float y, float z)
{
    assert(vector.X == x && vector.Y == y && vector.Z == z);
}
}

class CPhysicsMeshObject
{
public:
    CPhysicsMeshObject(bool hits, const C3DVector& intersection, bool* leaves_building)
        : Hits(hits), Intersection(intersection), LeavesBuilding(leaves_building) {}

    bool GetPointIntersectionFromTo(const C3DVector& target, const C3DVector& camera,
                                    CMeshIntersectionData& collision_data) const
    {
        ++intersections;
        CheckVector(target, 2.75f, -2.75f, 1.0f);
        CheckVector(camera, 8.0f, 5.0f, 12.0f);
        if (LeavesBuilding)
            *LeavesBuilding = false;
        if (Hits)
            collision_data.IntersectionPosition = Intersection;
        return Hits;
    }

private:
    bool Hits;
    C3DVector Intersection;
    bool* LeavesBuilding;
};

class CTCPhysicsBase
{
public:
    CTCPhysicsBase(const C3DVector& center, float radius, bool has_mesh, bool hit, const C3DVector& point,
                   bool* leaves_building = 0)
        : Center(center), Radius(radius), HasMesh(has_mesh), Mesh(hit, point, leaves_building) {}

    const void* GetPhysicsMeshInfo() const
    {
        ++mesh_checks;
        return HasMesh ? this : 0;
    }

    CPhysicsMeshObject GetPhysicsMeshObject() const { return Mesh; }
    float GetRadius() const { return Radius; }
    void GetBoundingInfo(C3DVector& center, float& radius) const
    {
        center = Center;
        radius = Radius;
    }

private:
    C3DVector Center;
    float Radius;
    bool HasMesh;
    CPhysicsMeshObject Mesh;
};

class CThing
{
public:
    CThing(const C3DVector& pos, unsigned long id, const CTCPhysicsBase& physics)
        : Position(pos), UniqueID(id), Physics(&physics) {}
    const CTCPhysicsBase* GetPhysicsTC() const { return Physics; }
    const C3DVector& GetPos() const { return Position; }
    unsigned long GetUniqueID() const { return UniqueID; }

private:
    C3DVector Position;
    unsigned long UniqueID;
    const CTCPhysicsBase* Physics;
};

class CThingGameObject : public CThing
{
public:
    CThingGameObject(const C3DVector& pos, unsigned long id, const CTCPhysicsBase& physics)
        : CThing(pos, id, physics) {}
};

#include "engine/CThingFilter_MinAllowedRadius.h"

bool CThingFilter_MinAllowedRadius::operator()(const CThing& thing) const
{
    return thing.GetPhysicsTC()->GetRadius() >= MinRadius;
}

class CThingSearchTools
{
public:
    CThingSearchTools() : Count(0) {}
    void Add(CThingGameObject& thing)
    {
        assert(Count < 8);
        Things[Count++] = &thing;
    }
    void Clear() { Count = 0; }

    template<class T, class Container, class Filter>
    void GetAllThingsInRoughArea(const C2DBoxI& area, const Filter& filter, Container& out)
    {
        assert(area.TLX == -3 && area.TLY == -7 && area.BRX == 7 && area.BRY == 3);
        for (int i = 0; i < Count; ++i)
            if (filter(*Things[i]))
                out.push_back(static_cast<T*>(Things[i]));
    }

private:
    CThingGameObject* Things[8];
    int Count;
};

class CWorld
{
public:
    CThingSearchTools& GetThingSearchTools() { return Search; }
private:
    CThingSearchTools Search;
};

class CCamera
{
public:
    CCamera() : WorldPosition(8.0f, 5.0f, 12.0f) {}
    const C3DVector& PeekWorldPos() const { return WorldPosition; }
private:
    C3DVector WorldPosition;
};

class CGameCameraBase
{
public:
    CGameCameraBase()
        : PointingAt(2.75f, -2.75f, 1.0f), GoingIntoBuilding(false), BuildingUID(0) {}

    bool IsThingCollision();

    CCamera InternalCam;
    C3DVector PointingAt;
    CWorld World;
    bool GoingIntoBuilding;
    unsigned long BuildingUID;
    std::list<C3DVector> DrawCoordList;
    std::list<C3DVector> LocalThingPosList;
};

// Leaf: ?IsThingCollision@CGameCameraBase@@AAE_NXZ
// Flags:
#define FABLE_RETAIL_CGAMECAMERABASE_RENDER
#define FABLE_RETAIL_CGAMECAMERABASE_UPDATE
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CCAMERA_RENDER_INTERPOLATION
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_LASTPOSITION
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_LINEOFSIGHT
#define FABLE_RETAIL_CWORLDMAP_FINDLASTUNBLOCKEDGROUND
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTHINGGAMEOBJECT_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGMAP
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTCMAPWHO_PASS22_SHARED_NODE
#define FABLE_RETAIL_CTHINGMAP_LAYERINFO
#define FABLE_RETAIL_MAP_VALIDATE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_COPY
#include "engine/CWorldMap.h"
#include "fable_string.h"
#include "engine/CGameCameraBase.h"
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CWorld.h"
#include "engine/CThingGameObject.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CTCMapwho.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_MinAllowedRadius.h"
#include "engine/CThingFilter_OverlappingSphere.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CMeshIntersectionData.h"
#include "engine/CPhysicsMeshObject.h"

bool CGameCameraBase::IsThingCollision()
{
    bool has_a_collision_occured = false;
    std::list<CThingGameObject*> thing_list;
    C3DVector cam_pos = InternalCam.PeekWorldPos();
    float distance = 5.0f;
    C2DBoxI area(C2DCoordI((long)PointingAt.X, (long)PointingAt.Y), (long)distance);
    World.GetThingSearchTools().GetAllThingsInRoughArea<CThingGameObject>(
        area, GFFilter_Bind2(CThingFilter_MinAllowedRadius(0.5f),
                            CThingFilter_OverlappingSphere(PointingAt, distance)), thing_list);
    DrawCoordList.clear();
    LocalThingPosList.clear();
    std::list<CThingGameObject*>::iterator it;
    for (it = thing_list.begin(); it != thing_list.end(); ++it)
    {
        CThingGameObject& object = **it;
        const CTCPhysicsBase* physics = object.GetPhysicsTC();
        LocalThingPosList.push_back(object.GetPos());
        if (physics->GetPhysicsMeshInfo())
        {
            CMeshIntersectionData intersection_data;
            CPhysicsMeshObject physics_object = physics->GetPhysicsMeshObject();
            GoingIntoBuilding = BuildingUID > 0;
            if (physics_object.GetPointIntersectionFromTo(PointingAt, cam_pos, intersection_data))
            {
                C3DVector displacement_vec(0.0f, 0.0f, 0.0f);
                if (BuildingUID > 0 && !GoingIntoBuilding)
                    displacement_vec.Z = 5.0f;
                if (BuildingUID == object.GetUniqueID())
                    DrawCoordList.push_front(intersection_data.IntersectionPosition + displacement_vec);
                else
                    DrawCoordList.push_back(intersection_data.IntersectionPosition + displacement_vec);
                has_a_collision_occured = true;
            }
        }
    }
    return has_a_collision_occured;
}


int main()
{
    CGameCameraBase camera;
    CThingSearchTools& search = camera.World.GetThingSearchTools();
    camera.DrawCoordList.push_back(C3DVector(99.0f, 99.0f, 99.0f));
    camera.LocalThingPosList.push_back(C3DVector(99.0f, 99.0f, 99.0f));
    assert(!camera.IsThingCollision());
    assert(camera.DrawCoordList.empty() && camera.LocalThingPosList.empty());

    const C3DVector near(2.0f, -2.0f, 1.0f);
    const C3DVector other_hit(12.0f, 7.0f, 4.0f);
    const C3DVector building_hit(8.0f, 1.0f, -1.0f);
    const CTCPhysicsBase no_mesh(near, 1.0f, false, false, building_hit);
    const CTCPhysicsBase misses(near, 1.0f, true, false, building_hit);
    const CTCPhysicsBase hits(near, 1.0f, true, true, other_hit);
    const CTCPhysicsBase building(near, 1.0f, true, true, building_hit);
    const CTCPhysicsBase small(near, 0.25f, true, true, building_hit);
    const CTCPhysicsBase distant(C3DVector(40.0f, 0.0f, 0.0f), 1.0f, true, true, building_hit);
    CThingGameObject object_without_mesh(near, 1, no_mesh);
    CThingGameObject object_with_miss(near, 2, misses);
    CThingGameObject object_with_hit(other_hit, 3, hits);
    CThingGameObject building_with_hit(building_hit, 4, building);
    CThingGameObject small_object(near, 5, small);
    CThingGameObject distant_object(near, 6, distant);

    search.Add(object_without_mesh);
    search.Add(object_with_miss);
    search.Add(small_object);
    search.Add(distant_object);
    assert(!camera.IsThingCollision());
    assert(mesh_checks == 2 && intersections == 1 && !camera.GoingIntoBuilding);
    assert(camera.DrawCoordList.empty());
    std::list<C3DVector>::iterator it = camera.LocalThingPosList.begin();
    CheckVector(*it++, 2.0f, -2.0f, 1.0f);
    CheckVector(*it++, 2.0f, -2.0f, 1.0f);
    assert(it == camera.LocalThingPosList.end());

    search.Clear();
    search.Add(object_with_hit);
    search.Add(building_with_hit);
    camera.BuildingUID = 4;
    assert(camera.IsThingCollision());
    assert(mesh_checks == 4 && intersections == 3 && camera.GoingIntoBuilding);
    it = camera.DrawCoordList.begin();
    CheckVector(*it++, 8.0f, 1.0f, -1.0f);
    CheckVector(*it++, 12.0f, 7.0f, 4.0f);
    assert(it == camera.DrawCoordList.end());
    it = camera.LocalThingPosList.begin();
    CheckVector(*it++, 12.0f, 7.0f, 4.0f);
    CheckVector(*it++, 8.0f, 1.0f, -1.0f);
    assert(it == camera.LocalThingPosList.end());

    search.Clear();
    CTCPhysicsBase leaves_building(near, 1.0f, true, true, building_hit, &camera.GoingIntoBuilding);
    CThingGameObject object_while_leaving(near, 7, leaves_building);
    search.Add(object_while_leaving);
    assert(camera.IsThingCollision());
    assert(!camera.GoingIntoBuilding && mesh_checks == 5 && intersections == 4);
    it = camera.DrawCoordList.begin();
    CheckVector(*it++, 8.0f, 1.0f, 4.0f);
    assert(it == camera.DrawCoordList.end());
    puts("006fda20_TEST PASS");
}