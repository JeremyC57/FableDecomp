// 008dcc80. Flash is pasted verbatim from the production row. The shared vector
// math runs against two deterministic seed tables. The fixture records calls
// and validates every damage packet at its receiving virtual method.
#if !defined(_MSC_VER)
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#ifndef __fastcall
#define __fastcall
#endif
#endif
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARE_ORIGINAL_FORMAL
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION
#define FABLE_RETAIL_FLASH_SQRT_SEED_PASS30
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_GROUPED_SUM
#include "engine/C3DVector.h"

static int scenario;
static void check(bool ok, int line)
{
    if (!ok) { std::printf("case %d failed at line %d\n", scenario, line); std::exit(1); }
}
#define CHECK(expr) check((expr), __LINE__)
unsigned long GFastOneOverSqrtTable[128];
enum { TCI_BURNT_EFFECT=137, DAMAGE_FIRE=3 };
class CThing;
struct CTCBurntEffect {};
struct CTCPerceiveThing {
    CThing* perceived;
    bool sight;
    CThing* GetPerceivedThing() const { return perceived; }
    bool IsThingLineOfSight() const { return sight; }
};
struct CJackOfBladesBattleDef { float BurnDamage; };
template <class T> struct CDefPointer {
    T* obj;
    CDefPointer():obj(0) {}
    T* operator->() const { CHECK(obj!=0); return obj; }
};
struct ThingDef {
    CJackOfBladesBattleDef def;
    bool GetSubDef(CDefPointer<const CJackOfBladesBattleDef>* out) const { out->obj=&def; return true; }
};
struct CHitParameters {
    CThing* hitter;
    C3DVector position,direction;
    bool response, explicitResponse;
    int attr;
    float magnitude;
    CHitParameters(void* loc,CThing* h,CThing* weapon,const C3DVector& p,const C3DVector& d,bool k,bool b,bool f,bool c):
        hitter(h),position(p),direction(d),response(true),explicitResponse(false),attr(-1),magnitude(-1)
        { CHECK(!loc && !weapon && !k && !b && !f && !c); }
    void SetPlayHitResponse(bool x) { CHECK(!explicitResponse); response=x; explicitResponse=true; }
    void AddDamageWithAttribute(int a,float m) { CHECK(explicitResponse && attr==-1); attr=a; magnitude=m; }
};
static std::vector<int> events;
static bool same(const C3DVector& a,const C3DVector& b) { return a.X==b.X && a.Y==b.Y && a.Z==b.Z; }
class CThing {
public:
    int id;
    bool dead,unconscious,burnt;
    CTCPerceiveThing* perception;
    CTCBurntEffect effect;
    C3DVector pos,wantedDir;
    float expectedDamage;
    CThing* expectedHitter;
    ThingDef def;
    CThing():id(0),dead(false),unconscious(false),burnt(false),perception(0),pos(0,0,0),wantedDir(0,0,0),expectedDamage(-1),expectedHitter(0) {}
    const ThingDef& PeekBaseDef() const { return def; }
    const C3DVector& GetPos() const { return pos; }
    bool GetTC(CTCPerceiveThing** p) { if(perception) *p=perception; return perception!=0; }
    bool GetTC(CTCBurntEffect** p) { if(burnt) *p=&effect; return burnt; }
    void AddTC(const char* n,bool enabled,void* params) { CHECK(!std::strcmp(n,"CTCBurntEffect") && !enabled && !params && !burnt); burnt=true; events.push_back(10*id+1); }
    void RemoveTC(int t) { CHECK(t==137 && burnt); burnt=false; events.push_back(10*id+2); }
    virtual void OnHit(CHitParameters& h) {
        CHECK(burnt && h.hitter==expectedHitter && h.attr==3 && h.magnitude==expectedDamage);
        CHECK(h.explicitResponse && !h.response && same(h.position,pos) && same(h.direction,wantedDir));
        events.push_back(10*id+3);
    }
};
struct CThingAICreature : public CThing {};
template<class T> static bool IS_THING_ALIVE(T p){ return p && !p->dead; }
class CThingFilter_IsNot {
    const CThing& excluded;
public:
    CThingFilter_IsNot(const CThing& x):excluded(x) {}
    bool operator()(const CThing* q)const {return q!=&excluded;}
};
struct Search {
    std::vector<CThingAICreature*> candidates;
    template<class T> void GetAllThings(const CThingFilter_IsNot& f,std::vector<T*>& result)const {
        CHECK(result.empty()); events.push_back(90);
        for(unsigned int i=0;i<candidates.size();++i) if(f(candidates[i])) result.push_back(candidates[i]);
    }
};
struct World {Search tools; const Search& PeekThingSearchTools()const {return tools;} };
namespace NCreatureMode {
static bool IsUnconscious(const CThing& q) { CHECK(!q.dead);events.push_back(10*q.id+4);return q.unconscious;}
class CModeJackOfBladesFlash {
public:
    World world;
    CThing* PCreature;
    CThing* PTarget;
    long BurnCounter;
    CThing& GetParentThing()const {return *PCreature;}
    const World& GetWorld()const { return world; }
    void Flash();
};
}

namespace NCreatureMode {
void CModeJackOfBladesFlash::Flash()
{
    CDefPointer<const CJackOfBladesBattleDef> p_jack_def;
    GetParentThing().PeekBaseDef().GetSubDef(&p_jack_def);
    float flash_damage = p_jack_def->BurnDamage;
    if (IS_THING_ALIVE(PTarget)) {
        CTCPerceiveThing* ptc_perceived_thing = 0;
        if (PCreature->GetTC(&ptc_perceived_thing)) {
            if (IS_THING_ALIVE(ptc_perceived_thing->GetPerceivedThing()) &&
                PTarget == ptc_perceived_thing->GetPerceivedThing()) {
                CTCBurntEffect* ptc_burnt = 0;
                bool has_burning_fx = PTarget->GetTC(&ptc_burnt);
                if (ptc_perceived_thing->IsThingLineOfSight()) {
                    if (!has_burning_fx)
                        PTarget->AddTC("CTCBurntEffect", false, 0);
                    long damage = 0;
                    if (BurnCounter > 5)
                        damage = (long)flash_damage;
                    ++BurnCounter;
                    C3DVector dir = PTarget->GetPos() - GetParentThing().GetPos();
                    dir.Normalise();
                    CHitParameters hit_params(0, &GetParentThing(), 0, PTarget->GetPos(), dir,
                                              false, false, false, false);
                    hit_params.SetPlayHitResponse(false);
                    hit_params.AddDamageWithAttribute(DAMAGE_FIRE, (float)damage);
                    PTarget->OnHit(hit_params);
                } else if (has_burning_fx) {
                    PTarget->RemoveTC(TCI_BURNT_EFFECT);
                }
            }
        }
    }
    std::vector<CThingAICreature*> creatures;
    GetWorld().PeekThingSearchTools().GetAllThings<CThingAICreature>(CThingFilter_IsNot(*PCreature), creatures);
    long num_things_to_burn = creatures.size();
    for (long n = 0; n < num_things_to_burn; ++n) {
        if (IS_THING_ALIVE(creatures[n]) && !IsUnconscious(*creatures[n])) {
            CTCBurntEffect* ptc_burnt = 0;
            bool has_burning_fx = creatures[n]->GetTC(&ptc_burnt);
            if (!has_burning_fx)
                creatures[n]->AddTC("CTCBurntEffect", false, 0);
            long damage = 0;
            if (BurnCounter > 5)
                damage = (long)flash_damage;
            C3DVector dir = creatures[n]->GetPos() - GetParentThing().GetPos();
            dir.Normalise();
            CHitParameters hit_params(0, &GetParentThing(), 0, creatures[n]->GetPos(), dir,
                                      false, false, false, false);
            hit_params.SetPlayHitResponse(false);
            hit_params.AddDamageWithAttribute(DAMAGE_FIRE, (float)damage);
            creatures[n]->OnHit(hit_params);
        }
    }
}
}

static void run(int targetState,int perceivedState,bool seen,bool burning,bool extraBurnt,int counter,int seed,float power)
{
    CThingAICreature parent,target,other,sleeping,dead;
    parent.id=1; target.id=2; other.id=3; sleeping.id=4; dead.id=5;
    parent.pos=C3DVector(4,4,4); target.pos=C3DVector(4,6,4); other.pos=C3DVector(3,4,4);
    const float unit=seed ? 1.0f : 11.0f/16.0f;
    target.wantedDir=C3DVector(0,11.0f/16.0f,0); other.wantedDir=C3DVector(-unit,0,0);
    target.burnt=burning; other.burnt=extraBurnt; sleeping.unconscious=true; dead.dead=true;
    parent.def.def.BurnDamage=power;
    target.dead=targetState==2;
    CTCPerceiveThing perception; perception.sight=seen;
    perception.perceived=perceivedState==0 ? 0 : perceivedState==1 ? &target : perceivedState==2 ? &other : &dead;
    parent.perception=perceivedState==4 ? 0 : &perception;
    NCreatureMode::CModeJackOfBladesFlash m;
    m.PCreature=&parent; m.PTarget=targetState==0 ? 0 : &target; m.BurnCounter=counter;
    m.world.tools.candidates.push_back(&parent); m.world.tools.candidates.push_back(0);
    m.world.tools.candidates.push_back(&dead); m.world.tools.candidates.push_back(&sleeping);
    m.world.tools.candidates.push_back(&other);
    const bool selected=targetState==1 && perceivedState==1;
    const int expectedCounter=counter+(selected && seen ? 1 : 0);
    target.expectedDamage=counter>5 ? (float)(long)power : 0.0f;
    other.expectedDamage=expectedCounter>5 ? (float)(long)power : 0.0f;
    target.expectedHitter=&parent; other.expectedHitter=&parent;
    std::vector<int> want;
    if (selected && seen) {if(!burning) want.push_back(21);want.push_back(23);}
    if (selected && !seen && burning) want.push_back(22);
    want.push_back(90);want.push_back(44);want.push_back(34);
    if(!extraBurnt) want.push_back(31);
    want.push_back(33);
    GFastOneOverSqrtTable[64]=seed ? 256UL : 0UL;
    events.clear(); m.Flash();
    CHECK(events==want && m.BurnCounter==expectedCounter);
    CHECK(target.burnt==(selected ? seen : burning) && other.burnt);
    CHECK(!parent.burnt && !dead.burnt && !sleeping.burnt);
    // No search results and no primary target.
    m.world.tools.candidates.clear(); m.PTarget=0; events.clear(); m.Flash();
    CHECK(events.size()==1 && events[0]==90 && m.BurnCounter==expectedCounter);
    ++scenario;
}
int main()
{
    long counts[]={0,4,5,6}; float powers[]={7.75f,-3.5f};
    for(int t=0;t<3;++t)for(int p=0;p<5;++p)for(int seen=0;seen<2;++seen)
    for(int burnt=0;burnt<2;++burnt)for(int extra=0;extra<2;++extra)
    for(int c=0;c<4;++c)for(int seed=0;seed<2;++seed)for(int pow=0;pow<2;++pow)
        run(t,p,seen!=0,burnt!=0,extra!=0,counts[c],seed,powers[pow]);
    CHECK(scenario==1920);
    std::puts("008dcc80_TEST PASS");
    return 0;
}