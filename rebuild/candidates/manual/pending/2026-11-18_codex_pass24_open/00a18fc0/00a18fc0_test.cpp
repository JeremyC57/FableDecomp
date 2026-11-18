#include <math.h>
#include <stdio.h>
struct C3DVector {
    float X,Y,Z;
    C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X-o.X,Y-o.Y,Z-o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X+o.X,Y+o.Y,Z+o.Z); }
    C3DVector& operator*=(float scale) { X*=scale; Y*=scale; Z*=scale; return *this; }
    float GetSquaredMagnitude() const { return X*X+Y*Y+Z*Z; }
    float GetSquaredMagnitude2D() const { return X*X+Y*Y; }
    void Normalise() { *this *= 1.0f/(float)sqrt(X*X+Y*Y+Z*Z); }
};

struct C2DVector {
    float X,Y;
    C2DVector(float x,float y):X(x),Y(y) {}
    C2DVector(const C3DVector& v):X(v.X),Y(v.Y) {}
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X-o.X,Y-o.Y); }
};
const float DELTA_NOTIONAL_ZERO=0.001f,MIN_HEIGHT_TO_KEEP_ABOVE_GROUND_IN_FLIGHT=0.5f;
const C3DVector ADDITIONAL_HEIGHT_TO_CHECK_Z_FROM(0,0,1);
struct CANavQuadTreeNode {};
struct CNavigatorManager {
    CANavQuadTreeNode Node;
    bool HasNode,SameMap;
    unsigned Validations;
    CNavigatorManager():HasNode(false),SameMap(true),Validations(0) {}
    CANavQuadTreeNode* GetNodeAt(const C2DVector&,bool useLayer)
    {
        ++Validations; return HasNode&&!useLayer ? &Node : 0;
    }
    bool IsInSameMap(const C2DVector&,const C2DVector&) const { return SameMap; }
};
struct CNavigatorExternalInterface {
    float Ground,Flying,Radius;
    mutable bool BadProbe;
    CNavigatorExternalInterface():Ground(0),Flying(0),Radius(0),BadProbe(false) {}
    float GetThemeHeightAt(const C3DVector&) const { return Ground; }
    float FindPosZAtIncludingThemes(const C3DVector& probe,bool flag) const { BadProbe=probe.Z!=2||!flag; return Flying; }
    float PeekNavigatorRadius(bool flag) const { return flag?-100.0f:Radius; }
};
enum ENavigatorReturn { NAV_REACHED_TARGET,NAV_CANNOT_REACH_TARGET,NAV_MOVING_TO_TARGET };
struct CNavigatorFlyer {
    CNavigatorExternalInterface* PInterface;
    CNavigatorManager* NavigationManager;
    bool Interrupted,OnRoute;
    C3DVector DestinationPos;
    CNavigatorFlyer(CNavigatorExternalInterface& interface,CNavigatorManager& manager)
        :PInterface(&interface),NavigationManager(&manager),Interrupted(false),OnRoute(false) {}
    float FindPosZAtIncludingThemes(const C3DVector& pos,bool navigation) const { return PInterface->FindPosZAtIncludingThemes(pos,navigation); }
    ENavigatorReturn MoveToPosProximity(const C3DVector&,const C3DVector&,float,float,C3DVector&,bool,bool,bool,bool,bool,bool,float);
    ENavigatorReturn Step(const C3DVector& from,const C3DVector& to,float speed,C3DVector& move)
    { return MoveToPosProximity(from,to,1,speed,move,false,false,false,false,false,false,0); }
};

ENavigatorReturn CNavigatorFlyer::MoveToPosProximity(const C3DVector& current_pos, const C3DVector& dest_pos, float proximity,
                                                     float current_speed, C3DVector& resulting_vector, bool avoid_dynamic_obstacles, bool avoid_static_obstacles, bool reduce_speed_when_near, bool fail_on_dynamic_blockage, bool high_priority, bool ignore_path_preferability, float distance_to_lod_centre)
{
    if (Interrupted || !OnRoute ||
        (float)fabs(dest_pos.X - DestinationPos.X) > DELTA_NOTIONAL_ZERO ||
        (float)fabs(dest_pos.Y - DestinationPos.Y) > DELTA_NOTIONAL_ZERO ||
        (float)fabs(dest_pos.Z - DestinationPos.Z) > DELTA_NOTIONAL_ZERO) {
        Interrupted = false;
        DestinationPos = dest_pos;
        if (!NavigationManager->GetNodeAt(dest_pos, false) ||
            PInterface->GetThemeHeightAt(dest_pos) - DELTA_NOTIONAL_ZERO > dest_pos.Z ||
            !NavigationManager->IsInSameMap(current_pos, dest_pos)) {
            OnRoute = false;
            return NAV_CANNOT_REACH_TARGET;
        }
        OnRoute = true;
    }
    float range = PInterface->PeekNavigatorRadius(false) + proximity;
    float prox_and_rad_sq = range * range;
    C3DVector straight_path = dest_pos - current_pos;
    if (straight_path.GetSquaredMagnitude() <= prox_and_rad_sq) {
        resulting_vector.X = 0.0f;
        resulting_vector.Y = 0.0f;
        resulting_vector.Z = 0.0f;
        OnRoute = false;
        return NAV_REACHED_TARGET;
    }
    if (straight_path.GetSquaredMagnitude2D() <= prox_and_rad_sq &&
        PInterface->GetThemeHeightAt(dest_pos) + DELTA_NOTIONAL_ZERO < dest_pos.Z) {
        resulting_vector.X = 0.0f;
        resulting_vector.Y = 0.0f;
        resulting_vector.Z = 0.0f;
        OnRoute = false;
        return NAV_REACHED_TARGET;
    }
    float speed_sq = current_speed * current_speed;
    if (straight_path.GetSquaredMagnitude() > speed_sq) {
        straight_path.Normalise();
        straight_path *= current_speed;
    }
    C3DVector new_pos = straight_path + current_pos;
    float ground_height = FindPosZAtIncludingThemes(new_pos + ADDITIONAL_HEIGHT_TO_CHECK_Z_FROM, true);
    C2DVector flat = C2DVector(dest_pos) - C2DVector(current_pos);
    if (flat.X * flat.X + flat.Y * flat.Y > speed_sq) {
        if (new_pos.Z - ground_height < MIN_HEIGHT_TO_KEEP_ABOVE_GROUND_IN_FLIGHT) {
            new_pos.Z = MIN_HEIGHT_TO_KEEP_ABOVE_GROUND_IN_FLIGHT + ground_height;
            straight_path = new_pos - current_pos;
            straight_path.Normalise();
            straight_path *= current_speed;
        }
    }
    if (new_pos.Z < ground_height) {
        new_pos.Z = ground_height + MIN_HEIGHT_TO_KEEP_ABOVE_GROUND_IN_FLIGHT;
        straight_path = new_pos - current_pos;
    }
    resulting_vector = straight_path;
    return NAV_MOVING_TO_TARGET;
}

int main()
{
    const C3DVector from(0,0,1),far(4,0,1),near(1,0,1),above(0,0,5);
    C3DVector move(7,7,7);
    CNavigatorExternalInterface world;
    CNavigatorManager manager;
    CNavigatorFlyer f(world,manager);
    if(f.Step(from,far,2,move)!=NAV_CANNOT_REACH_TARGET || f.OnRoute || move.X!=7)
        return 1;
    manager.HasNode=true;manager.SameMap=false;
    if(f.Step(from,far,2,move)!=NAV_CANNOT_REACH_TARGET)
        return 2;
    manager.SameMap=true;world.Ground=2;
    if(f.Step(from,far,2,move)!=NAV_CANNOT_REACH_TARGET)
        return 3;
    world.Ground=0;
    if(f.Step(from,near,2,move)!=NAV_REACHED_TARGET || f.OnRoute || move.X!=0 ||
        f.Step(from,above,2,move)!=NAV_REACHED_TARGET || f.OnRoute || move.Z!=0)
        return 4;
    if(f.Step(from,far,2,move)!=NAV_MOVING_TO_TARGET || !f.OnRoute || move.X!=2 || move.Z!=0 || world.BadProbe)
        return 5;
    unsigned count=manager.Validations;
    world.Flying=2;
    if(f.Step(from,far,2,move)!=NAV_MOVING_TO_TARGET || move.Z<=0 || manager.Validations!=count || world.BadProbe)
        return 6;
    f.Interrupted=true;
    if(f.Step(from,far,5,move)!=NAV_MOVING_TO_TARGET || move.Z!=1.5f || manager.Validations!=count+1 || f.Interrupted)
        return 7;
    puts("00a18fc0_TEST PASS");
    return 0;
}
