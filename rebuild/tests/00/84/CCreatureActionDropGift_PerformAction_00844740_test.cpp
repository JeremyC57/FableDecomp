#include <stdio.h>
#include <string.h>
#include <math.h>
#include "rebuild_abi.h"
#include "engine/GFSin.h"
struct C3DVector { float X,Y,Z; C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){} };
struct COpinionOfHeroDef { long ThreatenGift; };
struct CThingCreatureBase;
static bool has_def,creation_works,wrong;
static unsigned int calls;
static char trace[32];
static void event(char c){trace[calls++]=c;}
static unsigned long seed;
static float angles[2];
static unsigned int rng_index;
static float GFFloatRandom(float range,unsigned long* s){
    event('R');if (range!=1 || s!=&seed || rng_index>=2){wrong=true;return 0;}return angles[rng_index++];
}
struct CWorld { unsigned long* GetWorldSeed()const{return &seed;} };
static CWorld world;
static COpinionOfHeroDef def;
template<class T>struct CDefPointer {
    T* p;
    CDefPointer():p(0){}
    ~CDefPointer(){if(p)event('D');}
    T* operator->()const{return p;}
};
struct CThingBaseDef {
    bool GetSubDef(CDefPointer<const COpinionOfHeroDef>* out)const{
        event('L');if(has_def)out->p=&def;return has_def;
    }
};
static CThingBaseDef base;
static C3DVector position_value,acceleration_value;
static float orientation;
struct CThingCreatureBase {
    const CThingBaseDef& PeekBaseDef()const{return base;}
    const C3DVector& GetPos()const{return position_value;}
};
static CThingCreatureBase creature;
struct CTCPhysicsBase {
    void ApplyAcceleration(const C3DVector& a){event('A');acceleration_value=a;}
    void SetFacingAngleXY(float a){event('F');orientation=a;}
};
static CTCPhysicsBase physics;
struct CTCSolidToThing {
    void AddThingNotSolidTo(const CThingCreatureBase* p){event('N');if(p!=&creature)wrong=true;}
};
static CTCSolidToThing solid;
struct CThingObject {
    static CThingObject* Create(long i,const C3DVector& p,long player,const void* rot,unsigned long uid,const char* script){
        static CThingObject gift;
        event('C');
        if(i!=def.ThreatenGift || p.X!=position_value.X || p.Y!=position_value.Y || p.Z!=position_value.Z ||
            player!=5 || rot || uid || *script)wrong=true;
        return creation_works?&gift:0;
    }
    CTCPhysicsBase* GetPhysicsTC(){return &physics;}
    CTCSolidToThing* AddTC(const char* name,bool activate,const void* params){
        event('T');if(strcmp(name,"CTCSolidToThing") || activate || params)wrong=true;return &solid;
    }
};
template<class From,class To> static To GFDebugPDynamicCast(From p,To){return static_cast<To>(p);}
struct CCreatureAction_DropGift {
    struct CreaturePtr{
        CThingCreatureBase* Get()const{return &creature;}
        CThingCreatureBase* operator->()const{return Get();}
    } PCreature;
    CWorld& GetWorld(){return world;}
    const CWorld& PeekWorld()const{return world;}
    void PerformAction();
};
void CCreatureAction_DropGift::PerformAction()
{
    CDefPointer<const COpinionOfHeroDef> pop_def;
    if (PCreature->PeekBaseDef().GetSubDef(&pop_def)) {
        long def_index = pop_def->ThreatenGift;
        C3DVector position = PCreature->GetPos();
        float angle = GFFloatRandom(1.0f, GetWorld().GetWorldSeed());
        float xy_ampli = 0.25f;
        C3DVector acceleration((float)(xy_ampli * GFSin(angle)), (float)(xy_ampli * GFSin(angle)), 1.0f);
        CThingObject* new_thing = CThingObject::Create(def_index, position, 5, 0, 0, "");
        if (new_thing) {
            new_thing->GetPhysicsTC()->ApplyAcceleration(acceleration);
            float xy = GFFloatRandom(1.0f, PeekWorld().GetWorldSeed());
            new_thing->GetPhysicsTC()->SetFacingAngleXY(xy);
            CTCSolidToThing* tc_solid_to_thing = GFDebugPDynamicCast(new_thing->AddTC("CTCSolidToThing", false, 0), (CTCSolidToThing*)0);
            tc_solid_to_thing->AddThingNotSolidTo(PCreature.Get());
        }
    }
}

int main(){
    for(int flags=0;flags<4;++flags)for(int sample=0;sample<4;++sample){
        has_def=(flags&1)!=0;creation_works=(flags&2)!=0;
        angles[0]=sample*0.25f;angles[1]=(3-sample)*0.25f;
        calls=rng_index=0;wrong=false;
        def.ThreatenGift=123+flags;
        position_value=C3DVector(7,-2,5);
        acceleration_value=C3DVector(9,8,7);orientation=-3;
        CCreatureAction_DropGift action;action.PerformAction();trace[calls]=0;
        const char* expected=has_def ? (creation_works?"LRCARFTND":"LRCD"):"L";
        if(wrong || strcmp(trace,expected))return 1;
        if(has_def && creation_works){
            float target=(float)(0.25*sin((double)angles[0]*(double)6.2831855f));
            if(fabs(acceleration_value.X-target)>1e-6 || acceleration_value.Y!=acceleration_value.X ||
                acceleration_value.Z!=1 || orientation!=angles[1])return 2;
        }else if(acceleration_value.Z!=7 || orientation!=-3)return 3;
    }
    puts("00844740_TEST PASS");return 0;
}