#include <math.h>
#include <stdio.h>
#include <string.h>
#include <vector>
struct C3DVector{
    float x,y,z; C3DVector(float a=0,float b=0,float c=0):x(a),y(b),z(c){}
    C3DVector operator+(const C3DVector& b)const{return C3DVector(x+b.x,y+b.y,z+b.z);}
    C3DVector operator*(float n)const{return C3DVector(x*n,y*n,z*n);}
};
struct CRightHandedSet {
    C3DVector up,forward;CRightHandedSet():up(0,0,1),forward(0,1,0){}
    const C3DVector& GetUp()const{return up;} const C3DVector& GetForward()const{return forward;}
    void Rotate(const C3DVector&,float angle){const double v=6.283185307179586*angle;forward=C3DVector((float)-sin(v),(float)cos(v),0);}
};
struct CTCPhysicsBase {CRightHandedSet basis;const CRightHandedSet& GetRHSet()const{return basis;}};
struct CThing {
    CTCPhysicsBase phys;long player;C3DVector pos;bool dead;
    CThing():player(0),dead(false){}
    CTCPhysicsBase* GetPhysicsTC(){return &phys;} const C3DVector& GetPos()const{return pos;}
    long GetPlayerNumber()const{return player;}
};
bool IS_THING_ALIVE(const CThing* p){return p && !p->dead;}
struct CCharString{const char* value; bool operator==(const char* text)const{return strcmp(value,text)==0;}};
struct CThingShotDef{long SubShotType, NumSubShots, ObjectDef;float SubShotMaxAngle;const char* ShotDefinitionClassName;};
template<class T>struct CDefPointer {T* ptr;CDefPointer():ptr(0){}T* operator->()const{return ptr;}};
struct CGameDefinitionManager {
    CThingShotDef sub;CCharString category;int lookup,categories;
    void GetDef(long n,CDefPointer<const CThingShotDef>* out){if(n!=14)lookup+=100; ++lookup;out->ptr=&sub;}
    const CCharString& GetDefClassTypeNameFromGlobalIndex(long index){if(index!=42)categories+=100; ++categories;return category;}
};
struct CMainGameComponent{CGameDefinitionManager defs;static CMainGameComponent* Get();CGameDefinitionManager& PeekDefinitionManager(){return defs;}};
static CMainGameComponent game;
CMainGameComponent* CMainGameComponent::Get(){return &game;}
namespace NThingShot {
struct CTCShotDefinitionBase {
    CThing* shooter;CThing* weapon;C3DVector pos;float precision,mult,time;
    int* ability;const CThingShotDef* def;int inits;std::vector<CThing*> avoid;
    CTCShotDefinitionBase():shooter(0),weapon(0),precision(0),mult(0),time(0),ability(0),def(0),inits(0){}
    void Init(CDefPointer<const CThingShotDef> d,CThing* s,CThing* w,const C3DVector& p,float a,float m,float t,int* b){
        ++inits;shooter=s;weapon=w;pos=p;precision=a;mult=m;time=t;ability=b;def=d.ptr;
    }
    void AddThingToAvoid(CThing& thing){avoid.push_back(&thing);}
};
}
static int calls,mask,errorflag;static long players[4];static C3DVector origin;
struct CThingObject {
    NThingShot::CTCShotDefinitionBase shot;
    static CThingObject* Create(long,const C3DVector&,long,const CRightHandedSet*,int,const char*);
    NThingShot::CTCShotDefinitionBase* AddTC(const char* name,bool enable,int zero){
        if(strcmp(name,"Child")!=0 || !enable || zero!=0)errorflag=1;return &shot;
    }
};
static CThingObject objects[4];
CThingObject* CThingObject::Create(long def,const C3DVector& pos,long player,const CRightHandedSet* basis,int owner,const char* label) {
    if(def!=42 || pos.x!=origin.x || pos.y!=origin.y || pos.z!=origin.z ||
       basis->GetForward().y!=1 || basis->GetUp().z!=1 || owner!=0 || *label!=0)errorflag=1;
    players[calls]=player;int at=calls++;return mask&(1<<at)?0:&objects[at];
}
namespace NThingShot {
struct CTCShotDefinitionProjectilePredictive {
    CThingShotDef main_def; CThing parent;CThing* shooter;CThing* weapon;
    float DamageMultiplier,ShotBuildupTime; int* PShotAbilityDef; std::vector<CThing*> HitThingsList;
    CThingShotDef& GetShotDef(){return main_def;} CThing& GetParentThing(){return parent;}
    CThing* GetPShooter(){return shooter;} CThing* GetPWeapon(){return weapon;}
    long GetShotAccuracyPercentage(){return 83;}void OnSubShotTrigger();
};
void CTCShotDefinitionProjectilePredictive::OnSubShotTrigger()
{
    long def_index = GetShotDef().SubShotType;
    if (def_index >= 0) {
        CDefPointer<const CThingShotDef> pshot_def;
        CMainGameComponent::Get()->PeekDefinitionManager().GetDef(def_index, &pshot_def);
        long obj_def_index = pshot_def->ObjectDef;
        const CCharString& class_type_name = CMainGameComponent::Get()->PeekDefinitionManager().GetDefClassTypeNameFromGlobalIndex(obj_def_index);
        CRightHandedSet main_rhs = GetParentThing().GetPhysicsTC()->GetRHSet();
        long nshots = GetShotDef().NumSubShots;
        if (nshots > 0) {
            float rotate_angle = GetShotDef().SubShotMaxAngle * -1.0f;
            float rotate_step = 2.0f * GetShotDef().SubShotMaxAngle / (nshots - 1);
            for (long n = 0; n < nshots; ++n) {
                if (class_type_name == "OBJECT") {
                    CThingObject* pobject;
                    {
                        CRightHandedSet rhs_to_use = GetParentThing().GetPhysicsTC()->GetRHSet();
                        long player_number = GetParentThing().GetPlayerNumber();
                        if (IS_THING_ALIVE(GetPShooter())) player_number = GetPShooter()->GetPlayerNumber();
                        pobject = CThingObject::Create(obj_def_index, GetParentThing().GetPos(), player_number, &rhs_to_use, 0, "");
                    }
                    if (pobject) {
                        CTCShotDefinitionBase* ptc_shot = (CTCShotDefinitionBase*)pobject->AddTC(pshot_def->ShotDefinitionClassName, true, 0);
                        CRightHandedSet rhs = main_rhs;
                        rhs.Rotate(main_rhs.GetUp(), rotate_angle);
                        C3DVector target_direction = rhs.GetForward();
                        C3DVector target_pos = GetParentThing().GetPos() + target_direction * 30.0f;
                        ptc_shot->Init(pshot_def, GetPShooter(), GetPWeapon(), target_pos, (float)GetShotAccuracyPercentage(), DamageMultiplier, ShotBuildupTime, PShotAbilityDef);
                        for (long hit_thing_index = 0; hit_thing_index < HitThingsList.size(); ++hit_thing_index)
                            ptc_shot->AddThingToAvoid(*HitThingsList[hit_thing_index]);
                    }
                }
                rotate_angle += rotate_step;
            }
        }
    }
}
}
int main(){
    const int counts[]={-1,0,1,2,3};CThing shooter,weapon,hit[2];int ability=4;
    for(int enabled=0;enabled<2;++enabled) for(int cat=0;cat<2;++cat) for(int idx=0;idx<5;++idx)
    for(int who=0;who<3;++who)for(mask=0;mask<8;++mask)for(int hits=0;hits<3;++hits){
        NThingShot::CTCShotDefinitionProjectilePredictive shot; shot.main_def.SubShotType=enabled?14:-1;
        shot.main_def.NumSubShots=counts[idx];shot.main_def.SubShotMaxAngle=0.25f;
        shot.DamageMultiplier=1.5f;shot.ShotBuildupTime=0.75f;shot.PShotAbilityDef=&ability;
        shot.parent.pos=origin=C3DVector(4,-2,9);shot.parent.player=12;shooter.player=23;shooter.dead=who==2;
        shot.shooter=who?&shooter:0;shot.weapon=&weapon;
        for(int i=0;i<hits;++i)shot.HitThingsList.push_back(hit+i);
        for(int j=0;j<4;++j)objects[j].shot=NThingShot::CTCShotDefinitionBase();
        game.defs.lookup=game.defs.categories=calls=errorflag=0;
        game.defs.sub.ObjectDef=42;game.defs.sub.ShotDefinitionClassName="Child";game.defs.category.value=cat?"OBJECT":"OTHER";
        shot.OnSubShotTrigger();
        int count=(enabled && cat && counts[idx]>0)?counts[idx]:0;
        if(calls!=count || game.defs.lookup!=enabled || game.defs.categories!=enabled || errorflag)return 1;
        for(int j=0;j<4;++j){
            NThingShot::CTCShotDefinitionBase& child=objects[j].shot;
            if(j<count && players[j]!=(who==1?23:12))return 2;
            if(j>=count || (mask&(1<<j))){if(child.inits || child.avoid.size())return 3;continue;}
            const int direction=(j==0)?1:(j+1==count?-1:0);
            if(child.inits!=1 || child.shooter!=shot.shooter || child.weapon!=&weapon ||
               child.precision!=83 || child.mult!=1.5f || child.time!=0.75f ||
               child.ability!=&ability || child.def!=&game.defs.sub || (int)child.avoid.size()!=hits)return 4;
            if(fabs(child.pos.x-(4+30*direction))>0.0001 || fabs(child.pos.y-(-2+(direction==0?30:0)))>0.0001 || child.pos.z!=9)return 5;
            for(int i=0;i<hits;++i)if(child.avoid[i]!=hit+i)return 6;
        }
    }
    puts("007bd527_TEST PASS");return 0;
}