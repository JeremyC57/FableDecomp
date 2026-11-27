#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_CROSSWITHUP
#define FABLE_RETAIL_CTCWEAPONTRAIL_UPDATE
#include "engine/CEnginePrimitiveWeaponTrail.h"
C3DVector& C3DVector::AccurateNormalise(){
    float n=(float)sqrt(X*X+Y*Y+Z*Z);
    if(n){X/=n;Y/=n;Z/=n;}
    return *this;
}
struct CTCPhysicsBase {float radius; float GetRadius() const{return radius;}};
struct CThingBaseDef {float MeshHeight;};
struct CThing {
    C3DVector pos; CTCPhysicsBase physics; CThingBaseDef def;
    const C3DVector& GetPos() const{return pos;}
    const CTCPhysicsBase* GetPhysicsTC() const{return &physics;}
    const CThingBaseDef& PeekBaseDef() const{return def;}
};
static long frame, fps;
struct CWorld {static long GetFrame(){return frame;}};
struct CMainGameComponent {static long GetConstantFPS(){return fps;}};
namespace NCreatureHitNotification {
class CFlash {public:std::list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry> FlashGeometry; long CreationFrame;};
}
class CTCCreatureHitNotification {
public:
    CThing me; CWorld world; std::vector<NCreatureHitNotification::CFlash> Flashes;
    CThing& GetParentThing(){return me;}
    CWorld& GetWorld(){return world;}
    void SetHitNotification(const CThing&);
};
void CTCCreatureHitNotification::SetHitNotification(const CThing& attacker)
{
    const C3DVector& my_pos = GetParentThing().GetPos();
    const C3DVector& attacker_pos = attacker.GetPos();
    long const_fps = CMainGameComponent::GetConstantFPS();
    const CTCPhysicsBase* ptc_phys_base = GetParentThing().GetPhysicsTC();
    C3DVector line = attacker_pos - my_pos;
    line.Z = 0.0f;
    line.AccurateNormalise();
    C3DVector perp = line.CrossWithUp();
    C3DVector up(0, 0, 1);
    NCreatureHitNotification::CFlash flash;
    flash.CreationFrame = GetWorld().GetFrame();
    float xy_size = ptc_phys_base->GetRadius();
    float z_size = GetParentThing().PeekBaseDef().MeshHeight;
    C3DVector bottom, top;
    float time = ((float)flash.CreationFrame) / const_fps;
    bottom = my_pos + line * xy_size + perp * xy_size;
    top = my_pos + line * xy_size + perp * xy_size + up * z_size;
    flash.FlashGeometry.push_back(CEnginePrimitiveWeaponTrail::CWeaponTrailEntry(top, bottom, time));
    bottom = my_pos + line * xy_size - perp * xy_size;
    top = my_pos + line * xy_size - perp * xy_size + up * z_size;
    flash.FlashGeometry.push_back(CEnginePrimitiveWeaponTrail::CWeaponTrailEntry(top, bottom, time));
    Flashes.push_back(flash);
}
static void near_value(float a,float b){if(fabs(a-b)>0.0001){printf("expected %g, got %g",b,a);exit(1);}}
int main(){
    for(int i=0;i<420;++i){
        CTCCreatureHitNotification c;
        for(int j=0;j<5;++j){
            CThing attacker;
            c.me.pos=C3DVector((float)(i%9-4),(float)(j-2),(float)(i%7));
            float dx=(float)((i+j)%5-2),dy=(float)((i/5+j)%5-2);
            attacker.pos=C3DVector(c.me.pos.X+dx,c.me.pos.Y+dy,c.me.pos.Z+77);
            c.me.physics.radius=(float)(i%4);c.me.def.MeshHeight=(float)(i%8);
            fps=20+i%17;frame=i*5+j;
            c.SetHitNotification(attacker);
            if(c.Flashes.size()!=unsigned(j+1))return 2;
            for(int k=0;k<=j;++k)if(c.Flashes[k].CreationFrame!=i*5+k)return 3;
            NCreatureHitNotification::CFlash& flash=c.Flashes[j];
            std::list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry>::iterator it=flash.FlashGeometry.begin();
            float n=(float)sqrt(dx*dx+dy*dy);
            if(n){dx/=n;dy/=n;}
            for(int side=1;side>=-1;side-=2,++it){
                if(it==flash.FlashGeometry.end())return 4;
                const CEnginePrimitiveWeaponTrail::CWeaponTrailEntry& e=*it;
                float x=c.me.pos.X+c.me.physics.radius*(dx+side*dy);
                float y=c.me.pos.Y+c.me.physics.radius*(dy-side*dx);
                near_value(e.Bottom.X,x);near_value(e.Bottom.Y,y);near_value(e.Bottom.Z,c.me.pos.Z);
                near_value(e.Top.X,x);near_value(e.Top.Y,y);near_value(e.Top.Z,c.me.pos.Z+c.me.def.MeshHeight);
                near_value(e.CreationTime,(float)frame/fps);
            }
            if(it!=flash.FlashGeometry.end())return 5;
        }
    }
    puts("008651b0_TEST PASS");return 0;
}