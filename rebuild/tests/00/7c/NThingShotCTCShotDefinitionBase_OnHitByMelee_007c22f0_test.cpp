#include <assert.h>
#include <stdio.h>
#include <string.h>
#define FABLE_RETAIL_ASSERT(c) assert(c)
enum { TCI_TRAP=125, COLLIDE_TYPE_THING_SOLID=1 };
struct C3DVector { float x,y,z; };
struct Counted { mutable unsigned long refs; Counted():refs(0){} };
template<class T> class CDefPointer {
    T* p;
public:
    CDefPointer(T* v=0):p(v){if(p)++p->refs;}
    CDefPointer(const CDefPointer& other):p(other.p){if(p)++p->refs;}
    ~CDefPointer(){if(p)--p->refs;}
    T* operator->()const{return p;}
    void set(T* v){if(v)++v->refs;if(p)--p->refs;p=v;}
};
template<class T> class CIntelligentPointer {
    T* p;
public:
    CIntelligentPointer(T* v=0):p(v){}
    T* Get()const{return p;}
};
struct CThingMultiArrowShotDef:Counted { bool MultiArrowDeflectionByMelee; };
struct CMeleeCombatAbilityDef:Counted {};
struct CThingShotDef:Counted {
    CThingMultiArrowShotDef* child;
    bool IsDeflectedOnHitByMelee;
    float MeleeHitDeflectionDamageMultiplier;
    mutable int lookups;
    bool GetSubDef(CDefPointer<const CThingMultiArrowShotDef>* dst)const{++lookups;dst->set(child);return child!=0;}
};
namespace NThingShot { class CTCShotDefinitionBase; }
struct CThing {
    bool trap;
    int adds;
    NThingShot::CTCShotDefinitionBase* added;
    CThing():trap(false),adds(0),added(0){}
    bool HasTC(int t) const{assert(t==125);return trap;}
    NThingShot::CTCShotDefinitionBase* AddTC(const char* n,bool active,int arg){
        assert(!strcmp(n,"CTCShotDefinitionThrow")&&active&&arg==0);
        ++adds;return added;
    }
};
template<class T,class U> U* GFDebugPDynamicCast(T* p,U*) {return static_cast<U*>(p);}
namespace NThingShot {
class CTCShotDefinitionBase {
public:
    C3DVector BestCollisionPos,BestCollisionNormal;
    int BestCollisionType,collisions,initialisations;
    CIntelligentPointer<CThing> PBestCollisionThing,PShooter;
    CDefPointer<const CThingShotDef> PShotDef;
    CThing* parent;
    bool multi;
    const CThingShotDef* expectedDef;
    const CThing* expectedShooter;
    const CThing* expectedTarget;
    CTCShotDefinitionBase():BestCollisionType(-1),collisions(0),initialisations(0),parent(0),multi(false){}
    bool IsMultiArrowShotType()const{return multi;}
    CThing& GetParentThing()const{return *parent;}
    const CThingShotDef& GetShotDef()const{return *PShotDef.operator->();}
    void OnCollisionProper(bool& killed){++collisions;killed=true;}
    void Init(CDefPointer<const CThingShotDef> def,CThing* shooter,CThing* weapon,const CThing* target,float accuracy,float multiplier,float buildup,const CDefPointer<const CMeleeCombatAbilityDef>& ability){
        assert(def.operator->()==expectedDef&&def->refs==3&&shooter==expectedShooter&&weapon==0&&target==expectedTarget);
        assert(accuracy==100.0f&&multiplier==expectedDef->MeleeHitDeflectionDamageMultiplier&&buildup==0.0f&&ability.operator->()==0);
        ++initialisations;
    }
    void OnHitByMelee(CThing&,CThing*,const C3DVector&,const C3DVector&,bool,bool);
};
}
void NThingShot::CTCShotDefinitionBase::OnHitByMelee(CThing& thing_hitting, CThing* pweapon,
    const C3DVector& point_of_hit, const C3DVector& hit_direction, bool knockdown, bool blockable)
{
    if (thing_hitting.HasTC(TCI_TRAP)) {
        BestCollisionPos = point_of_hit;
        BestCollisionNormal = hit_direction;
        BestCollisionType = COLLIDE_TYPE_THING_SOLID;
        PBestCollisionThing = CIntelligentPointer<CThing>(&thing_hitting);
        bool killed_me;
        OnCollisionProper(killed_me);
        return;
    }
    CDefPointer<const CThingShotDef> pdef = PShotDef;
    bool is_deflected = pdef->IsDeflectedOnHitByMelee;
    if (IsMultiArrowShotType()) {
        const CThingShotDef& pt_shot_definition = GetShotDef();
        CDefPointer<const CThingMultiArrowShotDef> pt_multiarrow_shot_definition;
        if (!pt_shot_definition.GetSubDef(&pt_multiarrow_shot_definition)) { FABLE_RETAIL_ASSERT(false); }
        is_deflected = pt_multiarrow_shot_definition->MultiArrowDeflectionByMelee;
    }
    if (is_deflected) {
        CThing& shooter = thing_hitting;
        CThing* ptarget = PShooter.Get();
        float damage_multiplier = GetShotDef().MeleeHitDeflectionDamageMultiplier;
        CTCShotDefinitionBase* ptc_shot_defn = GFDebugPDynamicCast(GetParentThing().AddTC("CTCShotDefinitionThrow", true, 0), (CTCShotDefinitionBase*)0);
        FABLE_RETAIL_ASSERT(ptc_shot_defn!=NULL);
        ptc_shot_defn->Init(pdef, &shooter, 0, ptarget, 100.0f, damage_multiplier, 0.0f, CDefPointer<const CMeleeCombatAbilityDef>());
    }
}

int main() {
    for (unsigned bits=0; bits<256; ++bits) {
        CThingShotDef def; CThingMultiArrowShotDef child;
        NThingShot::CTCShotDefinitionBase original,created;
        CThing holder,hitter,target,weapon;
        C3DVector pos={1,2,3},normal={4,5,6},sentinel={-10,-20,-30};
        original.BestCollisionPos=original.BestCollisionNormal=sentinel;
        hitter.trap=(bits&1)!=0; original.multi=(bits&2)!=0;
        def.IsDeflectedOnHitByMelee=(bits&4)!=0;child.MultiArrowDeflectionByMelee=(bits&8)!=0;
        def.MeleeHitDeflectionDamageMultiplier=(bits&16)?2.25f:-0.25f; def.child=&child;def.lookups=0;
        created.expectedDef=&def;created.expectedShooter=&hitter;created.expectedTarget=(bits&32)?&target:0;
        holder.added=&created;original.parent=&holder;original.PShotDef.set(&def);original.PShooter=CIntelligentPointer<CThing>((bits&32)?&target:0);
        original.OnHitByMelee(hitter,(bits&64)?&weapon:0,pos,normal,(bits&128)!=0,(bits&64)!=0);
        const bool bounced=!hitter.trap&&(original.multi?child.MultiArrowDeflectionByMelee:def.IsDeflectedOnHitByMelee);
        assert(holder.adds==int(bounced)&&created.initialisations==int(bounced)&&original.collisions==int(hitter.trap));
        assert(def.lookups==int(!hitter.trap&&original.multi)&&def.refs==1&&child.refs==0);
        assert(original.BestCollisionType==(hitter.trap?COLLIDE_TYPE_THING_SOLID:-1)&&original.PBestCollisionThing.Get()==(hitter.trap?&hitter:0));
        assert(!memcmp(&original.BestCollisionPos,hitter.trap?&pos:&sentinel,sizeof(pos)));
        assert(!memcmp(&original.BestCollisionNormal,hitter.trap?&normal:&sentinel,sizeof(normal)));
    }
    puts("007c22f0_TEST PASS");
}