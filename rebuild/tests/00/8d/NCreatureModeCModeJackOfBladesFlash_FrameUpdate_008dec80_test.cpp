#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
static void ck(bool ok){if(!ok)exit(2);}
#define FABLE_RETAIL_ASSERT(v) ((void)0)
static int trace[64],used,chosen;static void event(int e){trace[used++]=e;ck(used<64);}
struct C3DVector{float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){} C3DVector operator-(const C3DVector& b)const{return C3DVector(X-b.X,Y-b.Y,Z-b.Z);} void Normalise(){float n=(float)sqrt(X*X+Y*Y+Z*Z);if(n){X/=n;Y/=n;Z/=n;}}float Dot(const C3DVector& v)const{return X*v.X+Y*v.Y+Z*v.Z;}};
struct CRightHandedSet{C3DVector up,forward;CRightHandedSet(){} CRightHandedSet(const C3DVector& u,const C3DVector& f):up(u),forward(f){}};
struct Physics{void ResetRHSet(const CRightHandedSet& s){ck(s.up.X==0&&s.up.Y==0&&s.up.Z==1&&s.forward.Z==0&&fabs(s.forward.X-.6)<.0001&&fabs(s.forward.Y-.8)<.0001);event(10);}};
struct CTCSpotLight{void SetAngle(float n){ck(fabs(n-(float)acos(-chosen/sqrt(25.0+chosen*chosen))/(2.0*3.141592653589793))<.0001);event(11);}};
static bool ready,has,found,dummy;static long JACK_FLASH_DURATION,JACK_FADE_UP_TIME,JACK_GLOW_START_FRAME;
struct CTCBoss{bool IsReadyForPhaseChange(){event(12);return ready;}};
struct CTCCreatureModeManager{struct CModeBase{void FrameUpdate(){event(1);}};void RemoveMode(long x){ck(x==99);event(13);}};
enum {MODE_JACK_OF_BLADES_FLASH=99};
struct CSpecialEffectsDef{bool GetEffectFromString(const char* n,long& id)const{ck(!strcmp(n,"FLASH_SWORD_GLOW"));id=35;event(17);return found;}};
static CSpecialEffectsDef fx;
template<class T> struct CDefPointer {T* value;T* operator->() const{return value;}};
struct Def{bool GetSubDef(CDefPointer<const CSpecialEffectsDef>* out){event(16);out->value=&fx;return has;}};
struct Appear{bool GetDummyObject(const char* n,long a,long b,C3DVector& v,CRightHandedSet&){ck(!strcmp(n,"WEAPON_FOCUS_01")&&!a&&!b&&v.X==7);v=C3DVector(17,18,19);event(18);return dummy;}};
struct CCreatureAction_PlayCombatAnimation{template<class T> CCreatureAction_PlayCombatAnimation(T&,const char*n,bool f){ck(f&&!strcmp(n,"ST_STRIKE_LIGHT_OUTOF"));}};
static int emitterState,markerState;static Physics phys;static CTCSpotLight spot;static CTCBoss boss;static CTCCreatureModeManager modes;static Def def;static Appear appear;
struct CThing{bool dead;int id;C3DVector pos;bool IsDead()const{return dead;}void Kill(bool b){ck(b);event(id==1?3:4);dead=true;}Def& PeekBaseDef(){return def;}C3DVector GetPos()const{return pos;}Physics* GetPhysicsTC(){return &phys;}Appear* GetTCGraphicAppearance(){return &appear;}bool GetTC(CTCSpotLight** p){*p=&spot;return true;}bool GetTC(CTCBoss** p){*p=&boss;return true;}bool GetTC(CTCCreatureModeManager** p){*p=&modes;return true;}void SetCurrentAction(const CCreatureAction_PlayCombatAnimation&){event(15);}};
static CThing oldE,oldS,newE,newS,creature;
static bool IS_THING_ALIVE(CThing* v){return v&&!v->dead;}
struct CTCDParticleEmitter{static CThing* Create(long id,const C3DVector& pos,bool b){ck(id==35&&pos.X==17&&!b);event(19);return emitterState?&newE:0;}};
struct CThingMarker{static CThing* Create(long id,const C3DVector& pos,const char* n){ck(id==42&&pos.Z==19&&!*n);event(20);return markerState?&newS:0;}};
struct CGameDefinitionManager{static CGameDefinitionManager& Get(){static CGameDefinitionManager d;return d;}long GetDefGlobalIndexFromName(const char* s){if(!strcmp(s,"ENVIRONMENT_JACK_OF_BLADES_GLOW"))return 40;if(!strcmp(s,"ENVIRONMENT_JACK_OF_BLADES_FADE"))return 41;ck(!strcmp(s,"MARKER_SPOT_LIGHT_JACK_OF_BLADES"));return 42;}};
struct Environment{void TransitionToThemeAllChannels(long c,long id,float dt){ck(c==3);if(id==40){ck(fabs(dt-JACK_FADE_UP_TIME/30.0)<.00001);event(5);}else{ck(id==41&&dt==.75f);event(6);}}};
struct Camera{C3DVector PeekCurrentPos(){return C3DVector(10,12,(float)(9+chosen));}};
struct World{Environment e;Camera c;Environment& GetEnvironment(){return e;}Camera* GetGameCamera(){return &c;}} world;
static void GFLimit(float& f,float a,float b){if(f<a)f=a;else if(f>b)f=b;}static float GFArcCos(float f){return (float)(acos(f)/(2*3.141592653589793));}
namespace NCreatureMode {
struct CModeJackOfBladesFlash:CTCCreatureModeManager::CModeBase{long Timer;CThing *PEmitter,*PSpotLight,*PCreature;void Flash(){event(2);}void ExtinguishFlames(){event(14);}CThing& GetParentThing(){return creature;}World& GetWorld(){return world;}void FrameUpdate();};
void CModeJackOfBladesFlash::FrameUpdate()
{
    CTCCreatureModeManager::CModeBase::FrameUpdate();
    long glow_def_num = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("ENVIRONMENT_JACK_OF_BLADES_GLOW");
    --Timer;
    if (Timer < JACK_FLASH_DURATION)
        Flash();
    if (Timer == JACK_GLOW_START_FRAME) {
        if (IS_THING_ALIVE(PEmitter))
            PEmitter->Kill(true);
        GetWorld().GetEnvironment().TransitionToThemeAllChannels(3, glow_def_num, (float)(JACK_FADE_UP_TIME / 30.0f));
        CDefPointer<const CSpecialEffectsDef> pspecial_fx;
        if (GetParentThing().PeekBaseDef().GetSubDef(&pspecial_fx)) {
            long fx_index;
            if (pspecial_fx->GetEffectFromString("FLASH_SWORD_GLOW", fx_index)) {
                C3DVector weapon_pos = PCreature->GetPos();
                CRightHandedSet rhs;
                if (!(PCreature->GetTCGraphicAppearance()->GetDummyObject("WEAPON_FOCUS_01", 0, 0, weapon_pos, rhs))) { FABLE_RETAIL_ASSERT(false); }
                CThing* pemitter = CTCDParticleEmitter::Create(fx_index, weapon_pos, false);
                if (IS_THING_ALIVE(pemitter))
                    PEmitter = pemitter;
                long def_num = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("MARKER_SPOT_LIGHT_JACK_OF_BLADES");
                CThing* plight_marker = CThingMarker::Create(def_num, weapon_pos, "");
                if (IS_THING_ALIVE(plight_marker))
                    PSpotLight = plight_marker;
            }
        }
    } else if (Timer == 0) {
        long fade_def_num = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("ENVIRONMENT_JACK_OF_BLADES_FADE");
        GetWorld().GetEnvironment().TransitionToThemeAllChannels(3, fade_def_num, 0.75f);
        if (IS_THING_ALIVE(PEmitter))
            PEmitter->Kill(true);
        if (IS_THING_ALIVE(PSpotLight))
            PSpotLight->Kill(true);
        ExtinguishFlames();
        PCreature->SetCurrentAction(CCreatureAction_PlayCombatAnimation(*PCreature, "ST_STRIKE_LIGHT_OUTOF", true));
        CTCCreatureModeManager* tc_mode_manager;
        if (!(PCreature->GetTC(&tc_mode_manager))) { FABLE_RETAIL_ASSERT(false); }
        tc_mode_manager->RemoveMode(MODE_JACK_OF_BLADES_FLASH);
        return;
    }
    if (IS_THING_ALIVE(PSpotLight)) {
        C3DVector camera_pos = GetWorld().GetGameCamera()->PeekCurrentPos();
        C3DVector dir_to_camera = camera_pos - GetParentThing().GetPos();
        dir_to_camera.Normalise();
        C3DVector dir_to_camera_prime = dir_to_camera;
        dir_to_camera_prime.Z = 0.0f;
        dir_to_camera_prime.Normalise();
        PSpotLight->GetPhysicsTC()->ResetRHSet(CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), dir_to_camera_prime));
        CTCSpotLight* ptc_spot_light = 0;
        if (!(PSpotLight->GetTC(&ptc_spot_light))) { FABLE_RETAIL_ASSERT(false); }
        float cos_theta = dir_to_camera.Dot(C3DVector(0.0f, 0.0f, -1.0f));
        GFLimit(cos_theta, -1.0f, 1.0f);
        float theta = GFArcCos(cos_theta);
        ptc_spot_light->SetAngle(theta);
    }
    CTCBoss* pTCBoss = 0;
    if (!(PCreature->GetTC(&pTCBoss))) { FABLE_RETAIL_ASSERT(false); }
    if (pTCBoss->IsReadyForPhaseChange()) {
        CTCCreatureModeManager* tc_mode_manager = 0;
        if (!(PCreature->GetTC(&tc_mode_manager))) { FABLE_RETAIL_ASSERT(false); }
        tc_mode_manager->RemoveMode(MODE_JACK_OF_BLADES_FLASH);
    }
}
}
int main(){
for(int flags=0;flags<32;++flags)for(int frame=0;frame<4;++frame)for(int o=0;o<9;++o)for(int n=0;n<9;++n){
    NCreatureMode::CModeJackOfBladesFlash m;used=0;JACK_FLASH_DURATION=2;JACK_FADE_UP_TIME=47;JACK_GLOW_START_FRAME=frame==3?0:2;
    int timer=frame==0?2:frame==1||frame==3?0:1;m.Timer=timer+1;chosen=(flags&16)?3:-2;
    ready=(flags&1)!=0;has=(flags&2)!=0;found=(flags&4)!=0;dummy=(flags&8)!=0;
    oldE.id=newE.id=1;oldS.id=newS.id=2;int oe=o%3,os=o/3;emitterState=n%3;markerState=n/3;
    oldE.dead=oe==1;oldS.dead=os==1;newE.dead=emitterState==1;newS.dead=markerState==1;
    m.PEmitter=oe?&oldE:0;m.PSpotLight=os?&oldS:0;m.PCreature=&creature;creature.pos=C3DVector(7,8,9);
    int want[64],k=0;want[k++]=1;if(timer<2)want[k++]=2;
    CThing* exE=m.PEmitter;CThing* exS=m.PSpotLight;bool aim=os==2;
    if(timer==JACK_GLOW_START_FRAME){if(oe==2)want[k++]=3;want[k++]=5;want[k++]=16;if(has){want[k++]=17;if(found){want[k++]=18;want[k++]=19;want[k++]=20;if(emitterState==2)exE=&newE;if(markerState==2){exS=&newS;aim=true;}}}}
    bool end=timer==0&&timer!=JACK_GLOW_START_FRAME;
    if(end){want[k++]=6;if(oe==2)want[k++]=3;if(os==2)want[k++]=4;want[k++]=14;want[k++]=15;want[k++]=13;}
    else{if(aim){want[k++]=10;want[k++]=11;}want[k++]=12;if(ready)want[k++]=13;}
    m.FrameUpdate();ck(m.Timer==timer&&k==used&&m.PEmitter==exE&&m.PSpotLight==exS);for(int i=0;i<k;++i)ck(want[i]==trace[i]);
}
puts("008dec80_TEST PASS");return 0;}