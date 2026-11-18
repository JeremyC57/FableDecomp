#include <math.h>
#include <stdio.h>
#include <string>
#include <vector>

struct C3DVector {
    float X,Y,Z;
    C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z) {}
};

struct CThing {
    C3DVector Position;
    CThing* Vehicle;
    unsigned LifetimeFlags;
    CThing():Vehicle(0),LifetimeFlags(0) {}
    const C3DVector& GetPos() const { return Position; }
    const CThing* GetThingStandingOn() const { return Vehicle; }
};
struct ThingPointer {
    CThing* Ptr;
    CThing* operator->() const { return Ptr; }
};
bool IS_THING_ALIVE(const CThing* thing) { return thing && !(thing->LifetimeFlags&1); }
struct CVectorVector {
    C3DVector Points[2];
    C3DVector* _M_start;
    C3DVector* _M_finish;
    CVectorVector():_M_start(Points),_M_finish(Points) {}
    void push_back(const C3DVector& point) { *_M_finish++=point; }
};
struct CWorldMap {
    bool BadSlope;
    float Theme,Water;
    CWorldMap():BadSlope(false),Theme(1),Water(6) {}
    C3DVector GetGroundFaceNormalAt(const C3DVector& point) const
    { return BadSlope&&point.X>0 ? C3DVector(0,0,-1):C3DVector(0,0,1); }
    float GetGroundSizeZAt(const C3DVector& position) const { return 2+position.X; }
    float GetThemeMinCameraSizeZOffsetAt(const C3DVector&) const { return Theme; }
    float GetWaterSizeZAt(const C3DVector&) const { return Water; }
};
struct CWorld {
    CWorldMap Map;
    const CWorldMap& PeekWorldMap() const { return Map; }
};
namespace NCameraModes {
struct CTools {
    enum { TEST_THINGS=1 };
    bool Block,Invalid;
    size_t Ignores;
    unsigned Checks;
    CTools():Block(false),Invalid(false),Ignores(0),Checks(0) {}
    bool IsLineOfSight(const std::vector<const CThing*>& ignore,const C3DVector& point,const C3DVector& flat,C3DVector& hit,
        C3DVector&,float&,const CThing**,int filter,bool ignoreWater)
    {
        ++Checks;Ignores=ignore.size();
        if(filter!=1||ignoreWater||flat.Z!=0||flat.X!=point.X) Invalid=true;
        hit=C3DVector(flat.X,flat.Y,9+point.X);
        return !Block;
    }
};
}
struct CCameraManagerDef {
    float BaseHeightToKeepAboveWater,BaseHeightToKeepAboveFlatLandscape,MaxHeightRelativeToGround;
    float AngleBetweenGroundNormalAndVerticalForLandscapeToBeConsideredNotFlat;
};
struct CameraMode {
    C3DVector Position;
    const C3DVector& GetPosition() const { return Position; }
};
struct ModePointer {
    const CameraMode* Ptr;
    explicit ModePointer(const CameraMode* mode):Ptr(mode) {}
    bool IsNull() const { return !Ptr; }
    const CameraMode* operator->() const { return Ptr; }
};
struct CGameCameraManager {
    CCameraManagerDef* PDef;
    ThingPointer PObservedThing;
    CWorld& World;
    ModePointer CurrentMode;
    NCameraModes::CTools CamTools;
    float MinWorldHeight,MaxWorldHeight;
    CGameCameraManager(CCameraManagerDef& param,CThing& thing,CWorld& world)
        :PDef(&param),World(world),CurrentMode(0),MinWorldHeight(0),MaxWorldHeight(0)
    { PObservedThing.Ptr=&thing; }
    void UpdateHeightLimits();
};

void CGameCameraManager::UpdateHeightLimits()
{
    const float WATER_OFFSET = PDef->BaseHeightToKeepAboveWater;
    const float GROUND_OFFSET = PDef->BaseHeightToKeepAboveFlatLandscape;
    const float MAX_HEIGHT_RELATIVE_TO_GROUND = PDef->MaxHeightRelativeToGround;
    const float MAX_ANGLE_BETWEEN_GROUND_NORMAL_AND_UPWARD_VERTICAL_FOR_CONSIDERATION =
        PDef->AngleBetweenGroundNormalAndVerticalForLandscapeToBeConsideredNotFlat * 0.0027777778f;
    const C3DVector& player_pos = PObservedThing->GetPos();
    const CWorldMap& wmap = World.PeekWorldMap();
    const CThing* pmesh_to_check = PObservedThing->GetThingStandingOn();
    std::vector<const CThing*> standing_on_things;
    if (IS_THING_ALIVE(pmesh_to_check))
        standing_on_things.push_back(pmesh_to_check);
    C3DVector cam_pos = player_pos;
    if (!CurrentMode.IsNull())
        cam_pos = CurrentMode->GetPosition();
    float max_min_absolute_height = player_pos.Z;
    float min_max_absolute_height = player_pos.Z + MAX_HEIGHT_RELATIVE_TO_GROUND;
    std::vector<C3DVector> posns_to_check;
    posns_to_check.push_back(player_pos);
    posns_to_check.push_back(C3DVector(cam_pos));
    for (std::vector<C3DVector>::iterator it = posns_to_check.begin(); it != posns_to_check.end(); ++it) {
        const C3DVector& r_pos = *it;
        C3DVector normal = wmap.GetGroundFaceNormalAt(r_pos);
        float cos_thet = (normal.X + normal.Y) * 0.0f + normal.Z;
        if (cos_thet > cos(MAX_ANGLE_BETWEEN_GROUND_NORMAL_AND_UPWARD_VERTICAL_FOR_CONSIDERATION * 6.2831854820251465)) {
            float ground_height = wmap.GetGroundSizeZAt(r_pos);
            float theme_height = wmap.GetThemeMinCameraSizeZOffsetAt(r_pos) + ground_height;
            float water_height = wmap.GetWaterSizeZAt(r_pos);
            float mesh_height = ground_height;
            C3DVector coll_pos;
            C3DVector coll_normal;
            float cam_rad;
            const CThing* phit_thing;
            if (!CamTools.IsLineOfSight(standing_on_things, r_pos, C3DVector(r_pos.X, r_pos.Y, 0.0f),
                coll_pos, coll_normal, cam_rad, &phit_thing, NCameraModes::CTools::TEST_THINGS, false))
                mesh_height = coll_pos.Z;
            float min_absolute_height;
            if (mesh_height > theme_height) {
                if (mesh_height > water_height)
                    min_absolute_height = mesh_height + GROUND_OFFSET;
                else
                    min_absolute_height = water_height + WATER_OFFSET;
            } else if (theme_height > water_height)
                min_absolute_height = theme_height + GROUND_OFFSET;
            else
                min_absolute_height = water_height + WATER_OFFSET;
            float max_absolute_height = min_absolute_height + MAX_HEIGHT_RELATIVE_TO_GROUND;
            if (min_absolute_height > max_min_absolute_height)
                max_min_absolute_height = min_absolute_height;
            if (max_absolute_height > min_max_absolute_height)
                min_max_absolute_height = max_absolute_height;
        }
    }
    MinWorldHeight = max_min_absolute_height;
    MaxWorldHeight = min_max_absolute_height;
}

int main()
{
    CThing target,vehicle;
    target.Position=C3DVector(0,0,1);target.Vehicle=&vehicle;
    CWorld world;
    CCameraManagerDef params={2,1,4,90};
    CGameCameraManager manager(params,target,world);
    manager.UpdateHeightLimits();
    if(manager.MinWorldHeight!=8||manager.MaxWorldHeight!=12||manager.CamTools.Checks!=2||
        manager.CamTools.Ignores!=1||manager.CamTools.Invalid)
        return 1;
    CameraMode mode={C3DVector(10,0,1)};
    manager.CurrentMode.Ptr=&mode;
    vehicle.LifetimeFlags=1;
    manager.UpdateHeightLimits();
    if(manager.MinWorldHeight!=14||manager.MaxWorldHeight!=18||manager.CamTools.Checks!=4||manager.CamTools.Ignores!=0)
        return 2;
    world.Map.BadSlope=true;manager.CamTools.Block=true;
    manager.UpdateHeightLimits();
    if(manager.MinWorldHeight!=10||manager.MaxWorldHeight!=14||manager.CamTools.Checks!=5)
        return 3;
    puts("00699670_TEST PASS");
    return 0;
}
