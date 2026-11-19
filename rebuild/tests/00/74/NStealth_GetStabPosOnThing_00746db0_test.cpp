// Behaviour test for NStealth::GetStabPosOnThing (0x00746DB0): the row body over models of its callees. Kills: the helper hit not
// returned / the miss arm run on a hit, the wrong helper / dummy name, index, flags or time, the dummy miss not falling back to the
// position, the wrong midpoint (scale, operand), and the dummy queried on a helper hit.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static void check(bool v){if(!v){puts("FAIL");exit(1);}}
static bool same(float a,float b){return memcmp(&a,&b,4)==0;}
class C3DVector {public:float X,Y,Z;C3DVector(){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}C3DVector(const C3DVector& v):X(v.X),Y(v.Y),Z(v.Z){}
 const C3DVector operator+(const C3DVector& o)const{return C3DVector(X+o.X,Y+o.Y,Z+o.Z);}
 const C3DVector operator*(float s)const{return C3DVector(X*s,Y*s,Z*s);}};
static void equal(const C3DVector& a,const C3DVector& b){check(same(a.X,b.X)&&same(a.Y,b.Y)&&same(a.Z,b.Z));}
class CRightHandedSet {public:C3DVector Up,Forward;};
class CCharString {public:int id;CCharString(int i):id(i){}};
const CCharString STAB_POS(1),EYE_LEFT_DUMMY_OBJECT_NAME(2);
static bool helper_hit,dummy_hit;static int helper_calls,dummy_calls;static C3DVector helper_pos,dummy_pos,thing_pos;
namespace NGraphicAppearance {
class CTCGraphicAppearance {public:
 bool GetHelperPoint(const CCharString& name,long index,unsigned long flags,C3DVector& pos,float time)const
 {++helper_calls;check(&name==&STAB_POS&&index==0&&flags==0&&same(time,1.0f));if(helper_hit)pos=helper_pos;return helper_hit;}
 bool GetDummyObject(const CCharString& name,long index,unsigned long flags,C3DVector& pos,CRightHandedSet& set,float scale)const
 {++dummy_calls;check(&name==&EYE_LEFT_DUMMY_OBJECT_NAME&&index==0&&flags==0&&same(scale,1.0f));set.Up=C3DVector(0,0,1);if(dummy_hit)pos=dummy_pos;return dummy_hit;}
};}
class CThing {public:NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;
 NGraphicAppearance::CTCGraphicAppearance* GetTCGraphicAppearance(){return PTCGraphicAppearance;}
 const C3DVector& GetPos()const{return thing_pos;}};
namespace NStealth {
C3DVector GetStabPosOnThing(CThing& thing)
{
    C3DVector pos;
    if (!thing.GetTCGraphicAppearance()->GetHelperPoint(STAB_POS, 0, 0, pos, 1.0f)) {
        C3DVector target_pos = thing.GetPos();
        C3DVector eye_pos;
        CRightHandedSet rhset;
        if (!thing.GetTCGraphicAppearance()->GetDummyObject(EYE_LEFT_DUMMY_OBJECT_NAME, 0, 0, eye_pos, rhset, 1.0f))
            eye_pos = target_pos;
        pos = (eye_pos + target_pos) * 0.5f;
    }
    return pos;
}
}
int main(){
 const float v[]={0.0f,-0.0f,1.0f,-3.5f,1000.25f,7.0e-4f};long cases=0;
 NGraphicAppearance::CTCGraphicAppearance app;CThing thing;thing.PTCGraphicAppearance=&app;
 for(int h=0;h!=2;++h)for(int d=0;d!=2;++d)for(int a=0;a!=6;++a)for(int b=0;b!=6;++b)for(int c=0;c!=6;++c){
  helper_hit=h!=0;dummy_hit=d!=0;helper_pos=C3DVector(v[a],v[b],v[c]);dummy_pos=C3DVector(v[b],v[c],v[a]+2.0f);thing_pos=C3DVector(v[c]*3.0f,v[a],v[b]-1.0f);
  helper_calls=dummy_calls=0;
  C3DVector got=NStealth::GetStabPosOnThing(thing);
  check(helper_calls==1&&dummy_calls==(h?0:1));
  if(h)equal(got,helper_pos);
  else if(d)equal(got,C3DVector((dummy_pos.X+thing_pos.X)*0.5f,(dummy_pos.Y+thing_pos.Y)*0.5f,(dummy_pos.Z+thing_pos.Z)*0.5f));
  else equal(got,C3DVector((thing_pos.X+thing_pos.X)*0.5f,(thing_pos.Y+thing_pos.Y)*0.5f,(thing_pos.Z+thing_pos.Z)*0.5f));
  ++cases;
 }
 check(cases==864);puts("00746db0_TEST PASS");return 0;
}