#include <math.h>
#include <stdio.h>
#include <vector>
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x = 0, float y = 0, float z = 0) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X-o.X,Y-o.Y,Z-o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X+o.X,Y+o.Y,Z+o.Z); }
    C3DVector& operator-=(const C3DVector& o) { X-=o.X; Y-=o.Y; Z-=o.Z; return *this; }
    C3DVector operator*(float k) const { return C3DVector(X*k,Y*k,Z*k); }
    C3DVector operator^(const C3DVector& o) const { return C3DVector(Y*o.Z-Z*o.Y,Z*o.X-X*o.Z,X*o.Y-Y*o.X); }
    float Dot(const C3DVector& o) const { return X*o.X+Y*o.Y+Z*o.Z; }
    float GetSquaredMagnitude() const { return Dot(*this); }
    void AccurateNormalise()
    {
        float length = (float)sqrt(GetSquaredMagnitude());
        X/=length; Y/=length; Z/=length;
    }
    void Normalise() { AccurateNormalise(); }
};

const float DELTA_NOTIONAL_ZERO=0.0001f;
struct CThing {
    unsigned char Flags;
    unsigned long GetTypeFlags() const { return Flags; }
    CThing() : Flags(8) {}
};
namespace NCameraModes {
struct CTools {
    enum ELOSTest { LOS_TEST_THINGS=1,LOS_TEST_LANDSCAPE=2,LOS_TEST_THEME=4,LOS_TEST_THING_FLAG=8,LOS_TEST_THEME_2=16 };
    float CameraMeshOffsetDistance,CameraCreatureOffsetDistance,CameraLandscapeOffsetDistance,CameraThemeOffsetDistance;
    float ThingDistance,ThemeDistance,Theme2Distance,GroundDistance;
    CThing Item;
    mutable unsigned Calls;
    CTools() : CameraMeshOffsetDistance(10),CameraCreatureOffsetDistance(20),CameraLandscapeOffsetDistance(30),CameraThemeOffsetDistance(40),
        ThingDistance(6),ThemeDistance(4),Theme2Distance(3),GroundDistance(2),Calls(0) {}
    bool GetThingPointIntersectionFromTo(const std::vector<const CThing*>& ignore,const C3DVector&,const C3DVector&,
        C3DVector& point,C3DVector& normal,const CThing** thing,bool,ELOSTest) const
    {
        ++Calls;
        if (ThingDistance < 0 || (!ignore.empty() && ignore[0]==&Item)) return false;
        point=C3DVector(ThingDistance,0,0);normal=C3DVector(1,0,0);*thing=&Item;
        return true;
    }
    bool GetThemePointIntersectionFromTo(const C3DVector&,const C3DVector&,C3DVector& point,C3DVector& normal,bool water) const
    {
        ++Calls;
        float distance=water?ThemeDistance:Theme2Distance;
        if (distance<0) return false;
        point=C3DVector(distance,0,0);normal=C3DVector(0,1,0);
        return true;
    }
    bool GetLandscapePointIntersectionFromTo(const C3DVector&,const C3DVector&,C3DVector& point,C3DVector& normal,bool) const
    {
        ++Calls;
        if (GroundDistance<0) return false;
        point=C3DVector(GroundDistance,0,0);normal=C3DVector(0,0,1);
        return true;
    }
    bool IsLineOfSight(const std::vector<const CThing*>&,const C3DVector&,const C3DVector&,C3DVector&,C3DVector&,float&,
        const CThing**,ELOSTest,bool) const;
};

bool CTools::IsLineOfSight(const std::vector<const CThing*>& thing_list, const C3DVector& from, const C3DVector& to, C3DVector& collision_pos,
                           C3DVector& normal_vector, float& cam_radius, const CThing** pphit_thing, ELOSTest test_flags, bool camera_test) const
{
    if (!((from - to).GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO))
        return true;
    bool intersection_occured = false;
    float best_cam_rad = 0.0f;
    const CThing* pbest_thing = 0;
    C3DVector best_pos;
    C3DVector best_normal;
    {
        C3DVector test_pos, test_normal;
        const CThing* ptest_thing;
        if ((test_flags & 9) && GetThingPointIntersectionFromTo(thing_list, from, to, test_pos, test_normal, &ptest_thing, camera_test, test_flags)) {
            float test_cam_rad = CameraMeshOffsetDistance;
            if ((ptest_thing->GetTypeFlags() & 8) && (test_flags & 8))
                test_cam_rad = CameraCreatureOffsetDistance;
            best_cam_rad = test_cam_rad;
            best_pos = test_pos;
            best_normal = test_normal;
            pbest_thing = ptest_thing;
            intersection_occured = true;
        }
    }
    if (test_flags & 4) {
        C3DVector test_pos, test_normal;
        if (GetThemePointIntersectionFromTo(from, to, test_pos, test_normal, true)) {
            float test_cam_rad = CameraThemeOffsetDistance;
            if (!intersection_occured || (test_pos - from).GetSquaredMagnitude() < (best_pos - from).GetSquaredMagnitude()) {
                best_pos = test_pos;
                best_normal = test_normal;
                best_cam_rad = test_cam_rad;
            }
            intersection_occured = true;
        }
    }
    if (test_flags & 0x10) {
        C3DVector test_pos, test_normal;
        if (GetThemePointIntersectionFromTo(from, to, test_pos, test_normal, false)) {
            float test_cam_rad = CameraThemeOffsetDistance;
            if (!intersection_occured || (test_pos - from).GetSquaredMagnitude() < (best_pos - from).GetSquaredMagnitude()) {
                best_pos = test_pos;
                best_normal = test_normal;
                best_cam_rad = test_cam_rad;
            }
            intersection_occured = true;
        }
    }
    if (test_flags & 2) {
        C3DVector test_pos, test_normal;
        if (GetLandscapePointIntersectionFromTo(from, to, test_pos, test_normal, camera_test)) {
            float test_cam_rad = CameraLandscapeOffsetDistance;
            if (!intersection_occured || (test_pos - from).GetSquaredMagnitude() < (best_pos - from).GetSquaredMagnitude()) {
                best_pos = test_pos;
                best_normal = test_normal;
                best_cam_rad = test_cam_rad;
            }
            intersection_occured = true;
        }
    }
    if (intersection_occured) {
        collision_pos = best_pos;
        normal_vector = best_normal;
        cam_radius = best_cam_rad;
        *pphit_thing = pbest_thing;
        return false;
    }
    return true;
}
}

int main()
{
    NCameraModes::CTools tools;
    typedef NCameraModes::CTools::ELOSTest Test;
    std::vector<const CThing*> ignored;
    C3DVector a,b(12,0,0),position(-1,0,0),normal;
    const CThing* hit=0;
    float result=-1;
    if (!tools.IsLineOfSight(ignored,a,a,position,normal,result,&hit,static_cast<Test>(31),false) || tools.Calls || result!=-1)
        return 1;
    if (tools.IsLineOfSight(ignored,a,b,position,normal,result,&hit,static_cast<Test>(31),false) ||
        tools.Calls!=4 || result!=30 || position.X!=2 || normal.Z!=1 || hit!=&tools.Item)
        return 2;
    if (tools.IsLineOfSight(ignored,a,b,position,normal,result,&hit,static_cast<Test>(9),false) ||
        result!=20 || position.X!=6 || normal.X!=1 || hit!=&tools.Item)
        return 3;
    ignored.push_back(&tools.Item);
    if (tools.IsLineOfSight(ignored,a,b,position,normal,result,&hit,static_cast<Test>(21),false) ||
        result!=40 || position.X!=3 || normal.Y!=1 || hit)
        return 4;
    if (!tools.IsLineOfSight(ignored,a,b,position,normal,result,&hit,static_cast<Test>(1),false) ||
        result!=40 || position.X!=3 || hit)
        return 5;
    puts("00877840_TEST PASS");
    return 0;
}