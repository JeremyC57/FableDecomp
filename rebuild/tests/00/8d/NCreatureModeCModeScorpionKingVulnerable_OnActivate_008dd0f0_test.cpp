// Self-contained C++03 behavior fixture for the Scorpion King activation.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void require(bool ok, const char* reason) {
    if (!ok) { fprintf(stderr, "%s\n", reason); exit(1); }
}

static char trace[256];
static void record(char event) {
    size_t length = strlen(trace);
    require(length + 1 < sizeof(trace), "trace overflow");
    trace[length] = event; trace[length + 1] = 0;
}

struct C3DVector {
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator*(float value) const { return C3DVector(X*value,Y*value,Z*value); }
    C3DVector& operator-=(const C3DVector& rhs) { X-=rhs.X; Y-=rhs.Y; Z-=rhs.Z; return *this; }
};
static bool equals(const C3DVector& v,float x,float y,float z) { return v.X==x && v.Y==y && v.Z==z; }

class CThing;
struct Handle {
    CThing* value;
    Handle(CThing* object=0) : value(object) {}
    Handle(const Handle& other) : value(other.value) {}
    Handle& operator=(const Handle& other) { value=other.value;return *this; }
    operator CThing*() const {return value;}
};
struct CTCBoss {};
struct CTCPhysicsBase {
    float radius;
    float GetRadius() const { record('r'); return radius; }
    void SetRadius(float value) { record('R'); radius=value; }
    C3DVector GetFacingVector() const { record('f'); return C3DVector(2,3,4); }
};
namespace NHitLocations {
struct CHitLocation {
    bool belly, enabled;
    void SetEnabled(bool value) { require(value==belly,"hit-location state"); enabled=value; record(belly?'b':'n'); }
};
}
struct CTCHitLocations {
    NHitLocations::CHitLocation hit[2];
    NHitLocations::CHitLocation* GetPHitLocationWithDefName(const char* name) {
        bool belly=!strcmp(name,"HIT_LOCATION_SCORPION_KING_BELLY");
        require(belly||!strcmp(name,"HIT_LOCATION_SCORPION_KING"),"hit-location name");
        record(belly?'B':'N'); return hit+(belly?0:1);
    }
};
struct Appearance {
    bool GetHelperPoint(const char* name,long a,unsigned long b,C3DVector& result,float scale=1.0f) {
        bool right=!strcmp(name,"weapon_pos_pincer_right");
        require(right||!strcmp(name,"weapon_pos_pincer_left"),"pincer name");
        require(!a&&!b&&scale==1.0f,"pincer arguments");
        result=right?C3DVector(8,9,-100):C3DVector(-4,-3,100); record(right?'h':'H'); return true;
    }
};

enum EAnimUpdateMode { ANIM_UPDATE_NONE };
struct CAnimationEntry {
    long BankIndex;
    unsigned long GetAnimateThingFlags() const { record('g'); return 0x12345678UL; }
};
struct CAnimInit {
    CAnimInit(unsigned char type,unsigned long type_flags,long index,EAnimUpdateMode mode,bool loop,unsigned long flags,long priority) {
        require(type==33 && !type_flags && index==87 && mode==ANIM_UPDATE_NONE && !loop &&
                flags==0x12345678UL && priority==100,"tail curl animation initialization"); record('i');
    }
};
struct CAnimateThing {
    void AddCombinationAnim(const CAnimInit&,long a,long b,bool play,bool d) {
        require(!a&&!b&&play&&!d,"combination animation arguments"); record('A');
    }
};
struct CTCAnimationComplex {
    CAnimateThing anim;
    CAnimateThing& GetAnimation(){record('a');return anim;}
};
enum ENullDef { NULL_DEF };
template<class T> struct CDefPointer {
    const T* ptr;
    CDefPointer(ENullDef) : ptr(0) {}
    ~CDefPointer() { if (ptr) ptr->Release(); }
    const T* Get() const { return ptr; }
    const T* operator->() const { return ptr; }
};

static int effects[3], currentEffect, lookupCount, obstructionMask, obstructionCount;
static bool gotEffects, finalSuccess;
struct CSpecialEffectsDef {
    mutable int refs;
    void Release() const { require(refs>0,"live special effects reference"); record(--refs==0?'X':'j'); }
    bool GetEffectFromString(const char* name,long& index) const {
        static const char* const names[]={"EYE_VULNERABLE","EYE_VULNERABLE_FLASH","STING_IDLE","STING_IDLE"};
        require(lookupCount<4&&!strcmp(name,names[lookupCount]),"effect name/order");
        currentEffect=lookupCount++;
        if(currentEffect==3) {
            require(index==701,"tail effect prior value");record('Z');
            if(finalSuccess)index=91;
            return finalSuccess;
        }
        require(index==0,"effect index reset");record("123"[currentEffect]);
        if(!effects[currentEffect])return false;
        index=40+currentEffect;return true;
    }
};
static CSpecialEffectsDef effectDef;
struct CThingBaseDef {
    bool GetSubDef(CDefPointer<const CSpecialEffectsDef>* target) const {
        require(!target->Get(),"null special-effect definition");record('D');
        if(gotEffects) { ++effectDef.refs; target->ptr=&effectDef; }
        return gotEffects;
    }
};
struct CTCDPhysicalObstruction {
    int id;
    void SetLifeTimeSeconds(float duration){require(duration==60.0f,"obstruction lifetime");record(id?'L':'l');}
    static CThing* Create(const C3DVector&,const C3DVector&,float);
};
struct CTCDParticleEmitter {
    int id;
    static CThing* Create(long,const C3DVector&,bool);
    void AttachToThing(CThing&,long,const char*,long,float);
};
struct CThing {
    C3DVector pos;
    bool dead;
    CTCBoss* boss; CTCHitLocations* hits; CTCAnimationComplex* animation;
    CTCDPhysicalObstruction* obstruction; CTCDParticleEmitter* emitter;
    CTCPhysicsBase physics; Appearance appearance; CThingBaseDef baseDef;
    CThing() : dead(false),boss(0),hits(0),animation(0),obstruction(0),emitter(0){}
    bool GetTC(CTCBoss** p){record('K');*p=boss;return boss!=0;}
    bool GetTC(CTCHitLocations** p){record('T');*p=hits;return hits!=0;}
    bool GetTC(CTCAnimationComplex** p){record('V');*p=animation;return animation!=0;}
    bool GetTCDriver(CTCDPhysicalObstruction** p) {require(obstruction!=0,"required obstruction driver"); *p=obstruction;record(obstruction->id?'O':'o');return true;}
    bool GetTCDriver(CTCDParticleEmitter** p){record('d');if(emitter)*p=emitter;return emitter!=0;}
    CTCPhysicsBase* GetPhysicsTC(){return &physics;}
    Appearance* GetTCGraphicAppearance(){return &appearance;}
    const C3DVector& GetPos() const {record('p');return pos;}
    CThingBaseDef& PeekBaseDef(){return baseDef;}
};
class CThingAICreature : public CThing {
public:
    CAnimationEntry animationEntry;
    void SuspendAI(bool state,void* requester){require(state&&!requester,"AI suspend");record('S');}
    const CAnimationEntry& GetAnimFromName(const char* name,long index) const {
        require(!strcmp(name,"TAIL_CURL")&&!index,"animation lookup");record('t');return animationEntry;
    }
};
template<class From,class To> To GFDebugPDynamicCast(From p,To){return static_cast<To>(p);}
static bool IS_THING_ALIVE(CThing* p){record('v');return p&&!p->dead;}
class CMainGameComponent { public: static int fps;static long GetConstantFPS(){return fps;} };
int CMainGameComponent::fps=0;
struct ThingList {
    CThing* things[3];int count;
    void push_back(const Handle& thing){require(count<3,"obstruction capacity");things[count++]=thing;record('+');}
};
struct CModeBase {
    CThing* parent;
    void OnActivate(){record('M');}
    CThing& GetParentThing(){return *parent;}
};
namespace NCreatureMode {
struct CModeScorpionKingVulnerable : CModeBase {
    CThingAICreature* PCreature;
    Handle PEyeEmitter, PStingEmitter;
    ThingList Obstructions;
    float CollisionRadius;
    long RecoveryCount,FrameCount,TailEffectIndex;
    bool TailCurlFinished,EffectsDone;
    void OnActivate();
};
}
static CThing obstructionThings[2], emittedThings[3], ownerThing;
static CTCDPhysicalObstruction obstructionDrivers[2];
static CTCDParticleEmitter emitterDrivers[3];
CThing* CTCDPhysicalObstruction::Create(const C3DVector& start,const C3DVector& end,float radius) {
    require(obstructionCount<2,"two pincers");int id=obstructionCount++;
    require(equals(end,10,11,12)&&equals(start,id?-4.0f:8.0f,id?-3.0f:9.0f,12)&&radius==0.5f,"obstruction geometry");
    record(id?'C':'c');return obstructionMask&(1<<id)?&obstructionThings[id]:0;
}
CThing* CTCDParticleEmitter::Create(long fx,const C3DVector& pos,bool attached) {
    require(currentEffect>=0&&currentEffect<3&&fx==40+currentEffect&&!attached&&equals(pos,24,25,26),"effect creation");
    record('e');return effects[currentEffect]==1?0:&emittedThings[currentEffect];
}
void CTCDParticleEmitter::AttachToThing(CThing& parent,long attachment,const char* point,long flags,float delay) {
    require(&parent==&ownerThing&&id==currentEffect&&attachment==10&&!strcmp(point,id==2?"EMITTER_STING":"EMITTER_EYE")&&!flags&&delay==0.0f,"emitter attachment");record('x');
}
#define FABLE_RETAIL_ASSERT(c) ((void)0)

void NCreatureMode::CModeScorpionKingVulnerable::OnActivate()
{
    float random_seconds = 5.0f;
    CTCBoss* ptc_boss = 0;
    PCreature->GetTC(&ptc_boss);
    RecoveryCount = static_cast<long>(CMainGameComponent::GetConstantFPS() * random_seconds);
    CModeBase::OnActivate();
    FABLE_RETAIL_ASSERT(PCreature->GetType() == THING_TYPE_AI_CREATURE);
    CThingAICreature* pai_creature = GFDebugPDynamicCast(PCreature, (CThingAICreature*)0);
    pai_creature->SuspendAI(true, 0);
    CollisionRadius = pai_creature->GetPhysicsTC()->GetRadius();
    pai_creature->GetPhysicsTC()->SetRadius(CollisionRadius * 0.5f);
    CTCHitLocations* p_TC_hit_locations = 0;
    PCreature->GetTC(&p_TC_hit_locations);
    NHitLocations::CHitLocation* p_hit_location_belly = p_TC_hit_locations->GetPHitLocationWithDefName("HIT_LOCATION_SCORPION_KING_BELLY");
    NHitLocations::CHitLocation* p_hit_location_normal = p_TC_hit_locations->GetPHitLocationWithDefName("HIT_LOCATION_SCORPION_KING");
    FABLE_RETAIL_ASSERT(p_hit_location_belly);
    FABLE_RETAIL_ASSERT(p_hit_location_normal);
    p_hit_location_belly->SetEnabled(true);
    p_hit_location_normal->SetEnabled(false);

    C3DVector claw_pos, shoulder_pos(PCreature->GetPos());
    float claw_radius = 0.5f;
    shoulder_pos -= PCreature->GetPhysicsTC()->GetFacingVector() * 1.0f;
    PCreature->GetTCGraphicAppearance()->GetHelperPoint("weapon_pos_pincer_right", 0, 0, claw_pos);
    claw_pos.Z = shoulder_pos.Z;
    CThing* p_obstruction_thing = CTCDPhysicalObstruction::Create(claw_pos, shoulder_pos, claw_radius);
    if (p_obstruction_thing) {
        CTCDPhysicalObstruction* ptcd_obstruction = 0;
        p_obstruction_thing->GetTCDriver(&ptcd_obstruction);
        ptcd_obstruction->SetLifeTimeSeconds(60.0f);
        Obstructions.push_back(p_obstruction_thing);
    }

    PCreature->GetTCGraphicAppearance()->GetHelperPoint("weapon_pos_pincer_left", 0, 0, claw_pos);
    claw_pos.Z = shoulder_pos.Z;
    p_obstruction_thing = CTCDPhysicalObstruction::Create(claw_pos, shoulder_pos, claw_radius);
    if (p_obstruction_thing) {
        CTCDPhysicalObstruction* ptcd_obstruction = 0;
        p_obstruction_thing->GetTCDriver(&ptcd_obstruction);
        ptcd_obstruction->SetLifeTimeSeconds(60.0f);
        Obstructions.push_back(p_obstruction_thing);
    }

    CTCAnimationComplex* ptc_animation = 0;
    GetParentThing().GetTC(&ptc_animation);
    const CAnimationEntry& anim_entry = pai_creature->GetAnimFromName("TAIL_CURL", 0);
    CAnimInit anim_init(0x21, 0, anim_entry.BankIndex, ANIM_UPDATE_NONE, false, anim_entry.GetAnimateThingFlags(), 100);
    CAnimateThing& anim = ptc_animation->GetAnimation();
    anim.AddCombinationAnim(anim_init, 0, 0, true, false);
    TailCurlFinished = false;

    C3DVector emitter_pos(GetParentThing().GetPos());
    CDefPointer<const CSpecialEffectsDef> pspecial_fx(NULL_DEF);
    long fx_index = 0;
    GetParentThing().PeekBaseDef().GetSubDef(&pspecial_fx);
    if (pspecial_fx.Get()) {
        if (pspecial_fx->GetEffectFromString("EYE_VULNERABLE", fx_index)) {
            CThing* p_emitter = CTCDParticleEmitter::Create(fx_index, emitter_pos, false);
            if (IS_THING_ALIVE(p_emitter)) {
                PEyeEmitter = p_emitter;
                CTCDParticleEmitter* ptcd_emitter = 0;
                if (p_emitter->GetTCDriver(&ptcd_emitter))
                    ptcd_emitter->AttachToThing(GetParentThing(), 10, "EMITTER_EYE", 0, 0.0f);
            }
        }
        fx_index = 0;
        if (pspecial_fx->GetEffectFromString("EYE_VULNERABLE_FLASH", fx_index)) {
            CThing* p_emitter = CTCDParticleEmitter::Create(fx_index, emitter_pos, false);
            if (IS_THING_ALIVE(p_emitter)) {
                CTCDParticleEmitter* ptcd_emitter = 0;
                if (p_emitter->GetTCDriver(&ptcd_emitter))
                    ptcd_emitter->AttachToThing(GetParentThing(), 10, "EMITTER_EYE", 0, 0.0f);
            }
        }
        fx_index = 0;
        if (pspecial_fx->GetEffectFromString("STING_IDLE", fx_index)) {
            CThing* p_emitter = CTCDParticleEmitter::Create(fx_index, emitter_pos, false);
            if (IS_THING_ALIVE(p_emitter)) {
                PStingEmitter = p_emitter;
                CTCDParticleEmitter* ptcd_emitter = 0;
                if (p_emitter->GetTCDriver(&ptcd_emitter))
                    ptcd_emitter->AttachToThing(GetParentThing(), 10, "EMITTER_STING", 0, 0.0f);
            }
        }
        pspecial_fx->GetEffectFromString("STING_IDLE", TailEffectIndex);
    }
    FrameCount = 0;
    EffectsDone = false;
}

static void exercise(bool available,int mask,int eye,int flash,int sting,bool finalFound,int fps) {
    CThing sentinel;
    CThingAICreature creature;
    CTCBoss boss;
    CTCHitLocations hit;
    CTCAnimationComplex animation;
    NCreatureMode::CModeScorpionKingVulnerable mode;
    for(int n=0;n<2;++n) {
        obstructionDrivers[n].id=n;
        obstructionThings[n].obstruction=&obstructionDrivers[n];
        hit.hit[n].belly=(n==0);hit.hit[n].enabled=(n!=0);
    }
    effects[0]=eye;effects[1]=flash;effects[2]=sting;
    for(int n=0;n<3;++n) {
        emitterDrivers[n].id=n;
        emittedThings[n].dead=(effects[n]==2);
        emittedThings[n].emitter=effects[n]==4?&emitterDrivers[n]:0;
    }
    creature.boss=&boss;creature.hits=&hit;creature.pos=C3DVector(12,14,16);
    creature.physics.radius=(mask&1)?7.5f:0.0f;creature.animationEntry.BankIndex=87;
    ownerThing.animation=&animation;ownerThing.pos=C3DVector(24,25,26);
    mode.PCreature=&creature;mode.parent=&ownerThing;mode.PEyeEmitter=&sentinel;mode.PStingEmitter=&sentinel;
    mode.FrameCount=17;mode.RecoveryCount=-1;mode.CollisionRadius=-9.0f;mode.TailEffectIndex=701;
    mode.EffectsDone=true;mode.TailCurlFinished=true;mode.Obstructions.count=1;mode.Obstructions.things[0]=&sentinel;
    CMainGameComponent::fps=fps;obstructionMask=mask;gotEffects=available;
    finalSuccess=finalFound;lookupCount=0;obstructionCount=0;currentEffect=-1;trace[0]=0;
    const float initialRadius=creature.physics.radius;
    effectDef.refs=(mask&1)?0:3;
    mode.OnActivate();
    require(effectDef.refs==((mask&1)?0:3),"special effect temporary release");
    require(mode.RecoveryCount==5*fps&&mode.CollisionRadius==initialRadius&&creature.physics.radius==initialRadius*0.5f,"recovery and collision");
    require(hit.hit[0].enabled&&!hit.hit[1].enabled&&!mode.TailCurlFinished&&!mode.EffectsDone&&!mode.FrameCount,"activation state");
    require(mode.TailEffectIndex==(available&&finalFound?91:701),"stored tail effect");
    require(mode.PEyeEmitter==(available&&eye>=3?&emittedThings[0]:&sentinel),"eye handle");
    require(mode.PStingEmitter==(available&&sting>=3?&emittedThings[2]:&sentinel),"sting handle");
    char expected[256]="KMSrRTBNbnpfhc";
    int expectedObs=1;
    require(mode.Obstructions.things[0]==&sentinel,"existing obstruction");
    if(mask&1) {strcat(expected,"ol+");require(mode.Obstructions.things[expectedObs++]==&obstructionThings[0],"right obstruction");}
    strcat(expected,"HC");
    if(mask&2) {strcat(expected,"OL+");require(mode.Obstructions.things[expectedObs++]==&obstructionThings[1],"left obstruction");}
    require(mode.Obstructions.count==expectedObs&&obstructionCount==2,"obstruction count");
    strcat(expected,"VtgiaApD");
    if(available) {
        require(lookupCount==4,"effect lookup count");
        for(int n=0;n<3;++n) {
            char key[2] = {static_cast<char>('1'+n),0};strcat(expected,key);
            if(effects[n]>=1)strcat(expected,"ev");
            if(effects[n]>=3)strcat(expected,"d");
            if(effects[n]==4)strcat(expected,"x");
        }
        strcat(expected,(mask&1)?"ZX":"Zj");
    } else require(lookupCount==0,"missing subdefinition");
    if(strcmp(trace,expected)) {fprintf(stderr,"actual %s expected %s\n",trace,expected);exit(1);}
}
int main() {
    int cases=0;
    for(int mask=0;mask<4;++mask) {
        exercise(false,mask,4,4,4,true,0);++cases;
        for(int a=0;a<5;++a)for(int b=0;b<5;++b)for(int c=0;c<5;++c)for(int final=0;final<2;++final) {
            exercise(true,mask,a,b,c,final!=0,30+a);++cases;
        }
    }
    require(cases==1004,"scenario count");
    puts("008dd0f0_TEST PASS");
    return 0;
}