#include <stdio.h>
#include <string.h>
#define FABLE_RETAIL_ASSERT(condition) ((void)0)
struct CCharString {
    const char* text;
    CCharString(const char* value) : text(value) {}
    bool operator==(const char* value) const { return strcmp(text,value)==0; }
};
struct TestPosition { float x,y,z; };
struct CThingCreatureBase;
struct CTCDParticleEmitter;
struct CThing {
    bool alive;
    CTCDParticleEmitter* emitter;
    int lookups;
    CThing() : alive(false),emitter(0),lookups(0) {}
    bool GetTC(CTCDParticleEmitter** out) { ++lookups; if (!emitter) return false; *out=emitter; return true; }
};
template<class T> struct CIntelligentPointer {
    T* value;
    CIntelligentPointer(T* p=0) : value(p) {}
    T* operator->() const { return value; }
};
static bool IS_THING_ALIVE(CThing* thing) { return thing && thing->alive; }
static bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p) { return IS_THING_ALIVE(p.value); }
struct CThingCreatureBase : CThing {
    TestPosition pos;
    float maximum,nominal,current;
    mutable int maxCalls,nomCalls,curCalls,posCalls;
    CThingCreatureBase() : maxCalls(0),nomCalls(0),curCalls(0),posCalls(0) {}
    const TestPosition& GetPos() const { ++posCalls; return pos; }
    float GetApproxMaxMeshHeight() const { ++maxCalls; return maximum; }
    float GetNominalMeshHeight() const { ++nomCalls; return nominal; }
    float GetCurrentMeshHeight() const { ++curCalls; return current; }
};
struct Spawns {
    CThing* produced;
    CThingCreatureBase* caster;
    int requestedFlags,simple,blended,attached,always;
    float fraction;
    int bad;
} spawns;
struct CTCDParticleEmitter {
    void AttachToThing(CThingCreatureBase& caster, long flags,const CCharString& name,long index,float offset) {
        ++spawns.attached;
        if (&caster!=spawns.caster||flags!=spawns.requestedFlags||strcmp(name.text,"")||index||offset!=0) ++spawns.bad;
    }
    void SetAlwaysUpdate(bool active) { ++spawns.always; if (!active||!spawns.attached) ++spawns.bad; }
    static CThing* Create(long low,long high,float fraction,const TestPosition& pos,bool trailing) {
        ++spawns.blended; spawns.fraction=fraction;
        if(low!=17||high!=29||&pos!=&spawns.caster->pos||trailing) ++spawns.bad;
        return spawns.produced;
    }
};
struct CTCSpecialAbilities {
    static CThing* CreateParticleEffect(long low,const TestPosition& pos) {
        ++spawns.simple;
        if(low!=17||&pos!=&spawns.caster->pos) ++spawns.bad;
        return spawns.produced;
    }
};
struct CSpecialEffectsDef {
    bool lowOK,highOK;
    mutable int lookups,owners;
    bool GetEffectFromString(const CCharString& name,long& index) const {
        ++lookups;
        if (owners!=1) ++spawns.bad;
        if(name=="LOW") { index=17; if(lookups!=1) ++spawns.bad; return lowOK; }
        if(name=="HIGH") { index=29; if(lookups!=2) ++spawns.bad; return highOK; }
        ++spawns.bad; return false;
    }
};
template<class T> struct CDefPointer {
    T* value;
    CDefPointer() : value(0) {}
    ~CDefPointer() { if (value) --value->owners; }
    T* operator->() const { return value; }
};
struct TestDefinition {
    bool available;
    CSpecialEffectsDef fx;
    int calls;
    bool GetSubDef(CDefPointer<const CSpecialEffectsDef>* out) {
        ++calls;
        if(!available) return false;
        out->value=&fx; ++fx.owners; return true;
    }
};
struct CTCPhysicalShieldSpell {
    TestDefinition* PDef;
    void PlayEffect(CIntelligentPointer<CThing>*,const CCharString&,const CCharString&,CThingCreatureBase*,long);
};

void CTCPhysicalShieldSpell::PlayEffect(CIntelligentPointer<CThing>* effect, const CCharString& effect_name_low,
                                        const CCharString& effect_name_hi, CThingCreatureBase* creature, long flags)
{
    if (effect_name_hi == "") {
        CDefPointer<const CSpecialEffectsDef> pspecial_effects;
        long effect_index = 0;
        if (PDef->GetSubDef(&pspecial_effects) &&
            pspecial_effects->GetEffectFromString(effect_name_low, effect_index)) {
            if (!effect) {
                CThing* temporary_effect = CTCSpecialAbilities::CreateParticleEffect(effect_index, creature->GetPos());
                if (IS_THING_ALIVE(temporary_effect)) {
                    CTCDParticleEmitter* ptc_emitter;
                    if (!temporary_effect->GetTC(&ptc_emitter)) {
                        FABLE_RETAIL_ASSERT(false);
                    }
                    ptc_emitter->AttachToThing(*creature, flags, "", 0, 0.0f);
                }
            } else {
                *effect = CTCSpecialAbilities::CreateParticleEffect(effect_index, creature->GetPos());
                if (IS_THING_ALIVE((*effect))) {
                    CTCDParticleEmitter* ptc_emitter;
                    if (!(*effect)->GetTC(&ptc_emitter)) {
                        FABLE_RETAIL_ASSERT(false);
                    }
                    ptc_emitter->AttachToThing(*creature, flags, "", 0, 0.0f);
                }
            }
        }
    } else {
        CDefPointer<const CSpecialEffectsDef> pspecial_effects;
        long effect_index_lo = 0, effect_index_hi = 0;
        if (PDef->GetSubDef(&pspecial_effects) &&
            pspecial_effects->GetEffectFromString(effect_name_low, effect_index_lo) &&
            pspecial_effects->GetEffectFromString(effect_name_hi, effect_index_hi)) {
            float height_range = creature->GetApproxMaxMeshHeight() - creature->GetNominalMeshHeight();
            float interpolate = 0.0f;
            if (height_range > 0.0f)
                interpolate = (creature->GetCurrentMeshHeight() - creature->GetNominalMeshHeight()) / height_range;
            if (!effect) {
                CThing* temporary_effect = CTCDParticleEmitter::Create(effect_index_lo, effect_index_hi, interpolate, creature->GetPos(), false);
                if (IS_THING_ALIVE(temporary_effect)) {
                    CTCDParticleEmitter* ptc_emitter = 0;
                    if (!temporary_effect->GetTC(&ptc_emitter)) {
                        FABLE_RETAIL_ASSERT(false);
                    }
                    ptc_emitter->AttachToThing(*creature, flags, "", 0, 0.0f);
                    ptc_emitter->SetAlwaysUpdate(true);
                }
            } else {
                *effect = CTCDParticleEmitter::Create(effect_index_lo, effect_index_hi, interpolate, creature->GetPos(), false);
                if (IS_THING_ALIVE((*effect))) {
                    CTCDParticleEmitter* ptc_emitter = 0;
                    if (!(*effect)->GetTC(&ptc_emitter)) {
                        FABLE_RETAIL_ASSERT(false);
                    }
                    ptc_emitter->AttachToThing(*creature, flags, "", 0, 0.0f);
                    ptc_emitter->SetAlwaysUpdate(true);
                }
            }
        }
    }
}

int main() {
    unsigned long cases=0;
    for(int high=0;high<2;++high) for(int def=0;def<2;++def)
    for(int lowOK=0;lowOK<2;++lowOK) for(int highOK=0;highOK<2;++highOK)
    for(int output=0;output<3;++output) for(int spawn=0;spawn<3;++spawn)
    for(int height=-1;height<=1;++height) for(int current=-1;current<=3;++current)
    for(int flag=0;flag<3;++flag) {
        TestDefinition data;
        data.available=def!=0; data.calls=0; data.fx.lowOK=lowOK!=0; data.fx.highOK=highOK!=0;
        data.fx.lookups=0; data.fx.owners=0;
        CTCPhysicalShieldSpell spell; spell.PDef=&data;
        CThingCreatureBase caster; caster.maximum=(float)(4+height*2); caster.nominal=4;
        caster.current=(float)(4+current); caster.pos.x=1; caster.pos.y=2; caster.pos.z=-4;
        CThing effect,old; CTCDParticleEmitter emitter; effect.alive=spawn==2; effect.emitter=&emitter; old.alive=true;
        CIntelligentPointer<CThing> holder(output==2?&old:0);
        CThing* previous=holder.value;
        memset(&spawns,0,sizeof(spawns)); spawns.produced=spawn?&effect:0; spawns.caster=&caster;
        spawns.requestedFlags=flag==0?0:(flag==1?0x24:-2);
        spell.PlayEffect(output?&holder:0,"LOW",high?"HIGH":"",&caster,spawns.requestedFlags);
        bool create=def&&lowOK&&(!high||highOK); bool attach=create&&spawn==2;
        if(spawns.bad||data.calls!=1||data.fx.owners!=0||data.fx.lookups!=(def?1+(high&&lowOK):0)) return 1;
        if(spawns.simple!=int(create&&!high)||spawns.blended!=int(create&&high)) return 2;
        if(caster.posCalls!=int(create)||caster.maxCalls!=int(create&&high)) return 3;
        bool divide=create&&high&&height>0;
        if(caster.nomCalls!=int(create&&high)+int(divide)||caster.curCalls!=int(divide)) return 4;
        if(create&&high&&spawns.fraction!=(divide?(float)current*0.5f:0.0f)) return 5;
        if(holder.value!=((output&&create)?spawns.produced:previous)) return 6;
        if(effect.lookups!=int(attach)||spawns.attached!=int(attach)||spawns.always!=int(attach&&high)) return 7;
        ++cases;
    }
    if(cases!=6480) return 8;
    puts("0079deb0_TEST PASS"); return 0;
}