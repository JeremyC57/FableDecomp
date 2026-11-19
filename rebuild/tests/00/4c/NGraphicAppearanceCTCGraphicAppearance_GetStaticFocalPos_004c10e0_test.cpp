// Behaviour test for NGraphicAppearance::CTCGraphicAppearance::GetStaticFocalPos() const (0x004C10E0): the row body over a model of
// its callees. Kills: flag not set in either arm, offset not cached / wrong sign, zero offset not stored, wrong helper name / index /
// flags / time, render position not returned in the miss arm, helper position not returned in the hit arm, render call count.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static void check(bool v){if(!v){puts("FAIL");exit(1);}}
class C3DVector {public:float X,Y,Z;C3DVector(){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}C3DVector(const C3DVector& v):X(v.X),Y(v.Y),Z(v.Z){}
 const C3DVector operator-(const C3DVector& o)const{return C3DVector(X-o.X,Y-o.Y,Z-o.Z);}};
static bool same(float a,float b){return memcmp(&a,&b,4)==0;}
static void equal(const C3DVector& a,const C3DVector& b){check(same(a.X,b.X)&&same(a.Y,b.Y)&&same(a.Z,b.Z));}
class CCharString {public:int id;CCharString(int i):id(i){}};
const CCharString MAIN_FOCAL_POSITION_NAME(7);
class CThing {public:int tag;};
static const CThing* expected_thing;static int render_calls,helper_calls;static C3DVector render_position,helper_position;static bool helper_found;
static C3DVector GetRenderPositionForThing(const CThing& thing,float interpolation)
{check(&thing==expected_thing);check(same(interpolation,1.0f));++render_calls;return render_position;}
namespace NGraphicAppearance {
class CTCGraphicAppearance {public:
 CThing* ParentThing;
 C3DVector CachedStaticFocalPosOffset;
 unsigned char SelfIlluminating:1,ForceResendEveryRenderFrame:1,ResendPrimsEveryRenderFrame:1,DisableOcclusionTesting:1,DrawAfterWater:1,CachedStaticFocalPos:1,RenderedLastFrame:1,PrimitivesDrawn:1;
 const CThing& GetParentThing()const{return *ParentThing;}
 bool GetHelperPoint(const CCharString& name,long index,unsigned long flags,C3DVector& point,float time)const
 {++helper_calls;check(&name==&MAIN_FOCAL_POSITION_NAME&&index==0&&flags==4&&same(time,1.0f));if(helper_found)point=helper_position;return helper_found;}
 C3DVector GetStaticFocalPos()const;
};
C3DVector CTCGraphicAppearance::GetStaticFocalPos() const
{
    C3DVector pos;
    if (GetHelperPoint(MAIN_FOCAL_POSITION_NAME, 0, 4, pos, 1.0f)) {
        const_cast<CTCGraphicAppearance*>(this)->CachedStaticFocalPos = true;
        const_cast<CTCGraphicAppearance*>(this)->CachedStaticFocalPosOffset = pos - GetRenderPositionForThing(GetParentThing(), 1.0f);
        return pos;
    }
    const_cast<CTCGraphicAppearance*>(this)->CachedStaticFocalPos = true;
    const_cast<CTCGraphicAppearance*>(this)->CachedStaticFocalPosOffset = C3DVector(0.0f, 0.0f, 0.0f);
    pos = GetRenderPositionForThing(GetParentThing(), 1.0f);
    return pos;
}
}
int main(){
 const float vals[]={0.0f,-0.0f,1.5f,-2.25f,1000.0f,3.0e-5f};long cases=0;
 for(int found=0;found!=2;++found)for(int pre=0;pre!=2;++pre)for(int a=0;a!=6;++a)for(int b=0;b!=6;++b)for(int c=0;c!=6;++c){
  CThing thing;thing.tag=a;expected_thing=&thing;
  NGraphicAppearance::CTCGraphicAppearance app;memset(&app,0,sizeof app);app.ParentThing=&thing;
  app.CachedStaticFocalPosOffset=C3DVector(-9,-8,-7);app.CachedStaticFocalPos=pre;app.DrawAfterWater=1;app.PrimitivesDrawn=pre;
  helper_found=found!=0;helper_position=C3DVector(vals[a],vals[b],vals[c]);render_position=C3DVector(vals[c]+1.0f,vals[a]*2.0f,vals[b]-3.0f);
  render_calls=helper_calls=0;
  C3DVector got=app.GetStaticFocalPos();
  check(helper_calls==1&&render_calls==1);
  check(app.CachedStaticFocalPos==1&&app.DrawAfterWater==1&&app.PrimitivesDrawn==(unsigned)pre&&app.SelfIlluminating==0&&app.RenderedLastFrame==0);
  if(found){equal(got,helper_position);equal(app.CachedStaticFocalPosOffset,C3DVector(helper_position.X-render_position.X,helper_position.Y-render_position.Y,helper_position.Z-render_position.Z));}
  else{equal(got,render_position);equal(app.CachedStaticFocalPosOffset,C3DVector(0.0f,0.0f,0.0f));}
  ++cases;
 }
 check(cases==864);puts("004c10e0_TEST PASS");return 0;
}