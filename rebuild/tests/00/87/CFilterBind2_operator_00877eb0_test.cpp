#include <stdio.h>
#ifndef _MSC_VER
#define __forceinline inline
#endif
// CFilter_Bind2::operator()(thing) -- Require both the first predicate and sphere overlap.
// Leaf: ??$?RVCUnprovenThing_00877eb0@@@?$CFilter_Bind2@VCThingFilter_IsOfType@@VCThingFilter_OverlappingSphere@@@@QBE_NABVCUnprovenThing_00877eb0@@@Z
#pragma pack(push,1)
class C3DVector {
public:
    float X,Y,Z;
    C3DVector() {}
    C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
    C3DVector operator-(const C3DVector& rhs) const {return C3DVector(X-rhs.X,Y-rhs.Y,Z-rhs.Z);}
    float GetLengthSquared()const{return X*X+(Y*Y+Z*Z);}
};
#pragma pack(pop)
class CTCPhysicsBase {public:void GetBoundingInfo(C3DVector&,float&)const;};
// Layout: retail CThing PhysicsTC is+0x60; the available donor CThing header is quarantined.
class CThing {public:unsigned char _body[0x60];CTCPhysicsBase* PhysicsTC;};
class CThingGameObject : public CThing {};
class CUnprovenThing_00877eb0 : public CThing {};
class CUnprovenThing_00877f40 : public CThing {};
class CThingFilter_MinAllowedRadius {public:float MinRadius;bool operator()(const CThing&)const;};
class CThingFilter_IsOfType {public:unsigned long Type;bool operator()(const CThing&)const;};
class CThingFilter_OverlappingSphere {
public:
    C3DVector SpherePos;float Radius;
    __forceinline bool operator()(const CThing& thing)const {
        C3DVector center;float radius;
        thing.PhysicsTC->GetBoundingInfo(center,radius);
        C3DVector delta=center-SpherePos;
        float sum=radius+Radius;
        return delta.GetLengthSquared()<sum*sum;
    }
};
template<class A,class B> class CFilter_Bind2 {
public:
    const A* F1;const B* F2;
    template<class T> bool operator()(const T& thing)const {return (*F1)(thing) && (*F2)(thing);}
};
template bool CFilter_Bind2<CThingFilter_IsOfType,CThingFilter_OverlappingSphere>::operator()<CUnprovenThing_00877eb0>(const CUnprovenThing_00877eb0&)const;

static int first_calls,bounds_calls,bad;static bool first_result;
static const CThing* expected_thing;static const CTCPhysicsBase* expected_physics;
static C3DVector bound_center;static float bound_radius;
bool CThingFilter_MinAllowedRadius::operator()(const CThing& thing)const {++first_calls;if(&thing!=expected_thing)bad=1;return first_result;}
bool CThingFilter_IsOfType::operator()(const CThing& thing)const {++first_calls;if(&thing!=expected_thing)bad=1;return first_result;}
void CTCPhysicsBase::GetBoundingInfo(C3DVector& center,float& radius)const {++bounds_calls;if(this!=expected_physics)bad=1;center=bound_center;radius=bound_radius;}
int main() {
    CUnprovenThing_00877eb0 thing;CTCPhysicsBase physics;thing.PhysicsTC=&physics;expected_thing=&thing;expected_physics=&physics;
    CThingFilter_IsOfType first;CThingFilter_OverlappingSphere sphere;sphere.SpherePos=C3DVector(0.0f,0.0f,0.0f);sphere.Radius=1.0f;
    CFilter_Bind2<CThingFilter_IsOfType,CThingFilter_OverlappingSphere> predicate;predicate.F1=&first;predicate.F2=&sphere;
    bound_center=C3DVector(0.0f,0.0f,0.0f);bound_radius=1.0f;first_result=false;
    int ok=!predicate(thing) && first_calls==1 && bounds_calls==0;
    first_result=true;ok=ok && predicate(thing) && bounds_calls==1;
    bound_center=C3DVector(2.0f,0.0f,0.0f);ok=ok && !predicate(thing);
    bound_center=C3DVector(0.0f,0.0f,3.0f);ok=ok && !predicate(thing);
    bound_center=C3DVector(1.0f,1.0f,1.0f);ok=ok && predicate(thing);
    ok=ok && !bad && first_calls==5 && bounds_calls==4;
    puts(ok ? "00877eb0_TEST PASS" : "00877eb0_TEST FAIL");return ok ? 0 : 1;
}