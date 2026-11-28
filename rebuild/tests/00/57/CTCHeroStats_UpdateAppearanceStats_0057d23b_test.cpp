#include <stdio.h>
#include <list>
#include <vector>

static int failures;
static void check(bool ok, const char* label) {
    if (!ok) { printf("failed %s\n", label); ++failures; }
}
enum EHeroAttachableAppearanceModifierType { APPEARANCE_HAIR, APPEARANCE_HORN, APPEARANCE_CLOTHING };
struct Lifetime { mutable int references; Lifetime() : references(0) {} };
template<class T> class CDefPointer {
    T* ptr;
public:
    CDefPointer() : ptr(0) {}
    explicit CDefPointer(T* p) : ptr(p) { if (ptr) ++ptr->references; }
    CDefPointer(const CDefPointer& other) : ptr(other.ptr) { if (ptr) ++ptr->references; }
    CDefPointer& operator=(const CDefPointer& other) {
        if (other.ptr) ++other.ptr->references;
        if (ptr) --ptr->references;
        ptr=other.ptr; return *this;
    }
    ~CDefPointer() { if (ptr) --ptr->references; }
    T* operator->() const { return ptr; }
};
struct Scaling { float factor; };
struct CHeroStatsDef { Scaling AppearanceModifierScaling; };
struct Appearance : Lifetime {
    float attr, scary, good, SoundRadiusMultiplier, VisibilityMultiplier;
    float GetTypeBalancedAttractiveness(const Scaling& s) const { return attr*s.factor; }
    float GetTypeBalancedScariness(const Scaling& s) const { return scary*s.factor; }
    float GetTypeBalancedGoodstrength(const Scaling& s) const { return good*s.factor; }
};
struct CAppearanceModifierDef : Appearance {};
struct CTattooDef : Appearance { long CoversBodyAreaFlags; };
struct CThingObjectDef : Lifetime {
    CAppearanceModifierDef mod;
    void GetSubDef(CDefPointer<const CAppearanceModifierDef>* out) const {
        *out=CDefPointer<const CAppearanceModifierDef>(&mod);
    }
};
struct Definitions {
    CHeroStatsDef stats; CThingObjectDef items[2]; int queries[2];
    const CHeroStatsDef& PeekHeroStatsDef() const { return stats; }
    void GetDef(long id,CDefPointer<const CThingObjectDef>* out) {
        check(id==0||id==1,"clothing ID");
        if(id==0||id==1) { ++queries[id]; *out=CDefPointer<const CThingObjectDef>(&items[id]); }
    }
};
struct CTCHeroAttachableAppearanceModifiers {
    unsigned long changed; long covered; bool populated;
    mutable int categories, checks, reads;
    unsigned long GetFrameLastChanged() const { ++reads; return changed; }
    void GetAppearanceModifierList(std::list<long>& list,EHeroAttachableAppearanceModifierType type) const {
        categories=categories*10+int(type)+1;
        if(populated) list.push_back(type==APPEARANCE_CLOTHING ? 0 : 1);
    }
    bool IsWearingAppearanceModifierCoveringBodyArea(long flags,EHeroAttachableAppearanceModifierType type) const {
        ++checks; check(type==APPEARANCE_CLOTHING,"coverage category"); return (flags&covered)!=0;
    }
};
struct CTCHeroMorph {
    CTattooDef tattoo[2]; long changed; int n; mutable int reads, enumerations;
    long GetFrameTattoosLastChanged() const { ++reads; return changed; }
    void GetTattoos(std::vector<CDefPointer<const CTattooDef> >& out) const {
        ++enumerations;
        for(int i=0;i<n;++i) out.push_back(CDefPointer<const CTattooDef>(&tattoo[i]));
    }
};
struct Parent {
    CTCHeroMorph morph; CTCHeroAttachableAppearanceModifiers dressing;
    bool clothesPresent,morphPresent; mutable int morphLookups;
    bool PeekTC(const CTCHeroMorph** out) const { ++morphLookups; *out=&morph; return morphPresent; }
    bool PeekTC(const CTCHeroAttachableAppearanceModifiers** out) const { *out=&dressing; return clothesPresent; }
};
struct CWorld { long frame; long GetFrame() { return ++frame; } };
struct CTCHeroStats {
    Definitions defs; Parent thing; CWorld world;
    float Attractiveness, Scariness, InherentAttractiveness, InherentScariness, AppearanceGoodstrength, SoundRadiusMultiplier, VisibilityMultiplier;
    long FrameUpdatedClothingStats,FrameUpdatedTattooStats;
    Parent& GetParentThing() { return thing; }
    CWorld& PeekWorld() { return world; }
    Definitions& GetDefinitionManager() { return defs; }
    void UpdateAppearanceStats(bool force_update);
};
template<class T,class L,class U> void GFLimit(T& v,const L& lower,const U& upper) { if(v<lower)v=lower; else if(v>upper)v=upper; }
template<class T,class L> void GFLimitLower(T& v,const L& lower) { if(v<lower)v=lower; }

void CTCHeroStats::UpdateAppearanceStats(bool force_update)
{
    const CTCHeroAttachableAppearanceModifiers* ptc_clothing;
    if (!GetParentThing().PeekTC(&ptc_clothing))
        return;
    const CTCHeroMorph* ptc_hero_morph;
    if (!GetParentThing().PeekTC(&ptc_hero_morph))
        return;
    if (!force_update && FrameUpdatedClothingStats != 0 && ptc_clothing->GetFrameLastChanged() < (unsigned long)FrameUpdatedClothingStats &&
        FrameUpdatedTattooStats != 0 && ptc_hero_morph->GetFrameTattoosLastChanged() < FrameUpdatedTattooStats)
        return;
    Attractiveness = InherentAttractiveness;
    Scariness = InherentScariness;
    AppearanceGoodstrength = 0.0f;
    SoundRadiusMultiplier = 1.0f;
    VisibilityMultiplier = 1.0f;
    std::list<long> clothing_list;
    ptc_clothing->GetAppearanceModifierList(clothing_list, APPEARANCE_CLOTHING);
    ptc_clothing->GetAppearanceModifierList(clothing_list, APPEARANCE_HAIR);
    const CHeroStatsDef& hero_stats_def = GetDefinitionManager().PeekHeroStatsDef();
    for (std::list<long>::const_iterator it = clothing_list.begin(); it != clothing_list.end(); ++it) {
        CDefPointer<const CThingObjectDef> pdef;
        GetDefinitionManager().GetDef(*it, &pdef);
        CDefPointer<const CAppearanceModifierDef> pappearance_modifier_def;
        pdef->GetSubDef(&pappearance_modifier_def);
        Attractiveness += pappearance_modifier_def->GetTypeBalancedAttractiveness(hero_stats_def.AppearanceModifierScaling);
        Scariness += pappearance_modifier_def->GetTypeBalancedScariness(hero_stats_def.AppearanceModifierScaling);
        AppearanceGoodstrength += pappearance_modifier_def->GetTypeBalancedGoodstrength(hero_stats_def.AppearanceModifierScaling);
        SoundRadiusMultiplier *= pappearance_modifier_def->SoundRadiusMultiplier;
        VisibilityMultiplier *= pappearance_modifier_def->VisibilityMultiplier;
    }
    std::vector<CDefPointer<const CTattooDef> > tattoos;
    ptc_hero_morph->GetTattoos(tattoos);
    for (long c1 = 0; c1 < tattoos.size(); ++c1) {
        CDefPointer<const CTattooDef> ptattoo = tattoos[c1];
        if (!ptc_clothing->IsWearingAppearanceModifierCoveringBodyArea(ptattoo->CoversBodyAreaFlags,
                                                                    APPEARANCE_CLOTHING)) {
            Attractiveness += ptattoo->GetTypeBalancedAttractiveness(hero_stats_def.AppearanceModifierScaling);
            Scariness += ptattoo->GetTypeBalancedScariness(hero_stats_def.AppearanceModifierScaling);
            AppearanceGoodstrength += ptattoo->GetTypeBalancedGoodstrength(hero_stats_def.AppearanceModifierScaling);
            VisibilityMultiplier *= ptattoo->VisibilityMultiplier;
        }
    }
    GFLimit(Attractiveness, -1.0f, 1.0f);
    GFLimit(Scariness, -1.0f, 1.0f);
    GFLimit(AppearanceGoodstrength, -2.0f, 2.0f);
    GFLimitLower(SoundRadiusMultiplier, 0.0f);
    GFLimitLower(VisibilityMultiplier, 0.0f);
    FrameUpdatedClothingStats = PeekWorld().GetFrame();
    FrameUpdatedTattooStats = PeekWorld().GetFrame();
}

static void configure(Appearance& a,float factor,float sound,float visibility) {
    a.attr=factor; a.scary=-factor; a.good=2.0f*factor; a.SoundRadiusMultiplier=sound; a.VisibilityMultiplier=visibility;
}
static void exercise(int availability,bool force,long clothingFrame,long tattooFrame,unsigned long changed,long inkChanged,
                     int amount,long covered,float scale,int sign) {
    CTCHeroStats hero;
    Parent& p=hero.thing;
    p.clothesPresent=(availability&1)!=0; p.morphPresent=(availability&2)!=0; p.morphLookups=0;
    p.dressing.changed=changed; p.dressing.covered=covered; p.dressing.populated=amount>0;
    p.dressing.categories=p.dressing.checks=p.dressing.reads=0;
    p.morph.changed=inkChanged; p.morph.n=amount; p.morph.reads=p.morph.enumerations=0;
    hero.FrameUpdatedClothingStats=clothingFrame; hero.FrameUpdatedTattooStats=tattooFrame; hero.world.frame=50;
    hero.defs.stats.AppearanceModifierScaling.factor=scale; hero.defs.queries[0]=hero.defs.queries[1]=0;
    hero.Attractiveness=hero.Scariness=hero.AppearanceGoodstrength=hero.SoundRadiusMultiplier=hero.VisibilityMultiplier=8.0f;
    hero.InherentAttractiveness=0.25f; hero.InherentScariness=-0.25f;
    configure(hero.defs.items[0].mod,sign*0.25f,2.0f,0.5f);
    configure(hero.defs.items[1].mod,sign*0.5f,sign*0.25f,2.0f);
    configure(p.morph.tattoo[0],sign*0.125f,0.0f,-0.5f); p.morph.tattoo[0].CoversBodyAreaFlags=1;
    configure(p.morph.tattoo[1],sign*0.25f,0.0f,2.0f); p.morph.tattoo[1].CoversBodyAreaFlags=2;
    const bool both=availability==3;
    const bool clothingRead=both && !force && clothingFrame!=0;
    const bool inkRead=clothingRead && changed<(unsigned long)clothingFrame && tattooFrame!=0;
    const bool run=both && !(inkRead && inkChanged<tattooFrame);
    hero.UpdateAppearanceStats(force);
    check(p.morphLookups==int(p.clothesPresent),"second lookup short circuit");
    check(p.dressing.reads==int(clothingRead) && p.morph.reads==int(inkRead),"timestamp short circuits");
    check(p.dressing.categories==(run?31:0) && p.morph.enumerations==int(run),"collection order");
    check(p.dressing.checks==(run?amount:0),"tattoo coverage calls");
    check(hero.defs.queries[0]==int(run && amount>0) && hero.defs.queries[1]==int(run && amount>0),"clothing/hair lookup counts");
    float a=8.0f,b=8.0f,g=8.0f,noise=8.0f,vis=8.0f;
    if(run) {
        float changes=amount?0.75f:0.0f;
        if(amount>=1 && !(covered&1)) changes+=0.125f;
        if(amount==2 && !(covered&2)) changes+=0.25f;
        float before=0.25f+sign*scale*changes;
        a=before>1?1:(before< -1?-1:before); b=-a;
        float alignment=2.0f*sign*scale*changes; g=alignment>2?2:(alignment< -2?-2:alignment);
        noise=amount?(sign>0?0.5f:0.0f):1.0f;
        vis=amount && !(covered&1)?0.0f:(amount==2 && !(covered&2)?2.0f:1.0f);
    }
    check(hero.Attractiveness==a && hero.Scariness==b && hero.AppearanceGoodstrength==g,"stats and clamping");
    check(hero.VisibilityMultiplier==vis && hero.SoundRadiusMultiplier==noise,"multipliers");
    check(hero.FrameUpdatedClothingStats==(run?51:clothingFrame) && hero.FrameUpdatedTattooStats==(run?52:tattooFrame),"two separate frame reads");
    for(int i=0;i<2;++i)check(hero.defs.items[i].references==0 && hero.defs.items[i].mod.references==0 && p.morph.tattoo[i].references==0,"balanced releases");
}
int main() {
    for(int av=0;av<4;++av) for(int force=0;force<2;++force)
        for(int timing=0;timing<7;++timing) for(int n=0;n<3;++n) for(int covered=0;covered<4;++covered)
        for(int s=-1;s<=1;s+=2) for(int large=0;large<2;++large) {
            const long c=timing==0?0:(timing==6?-2:10), t=timing==1?0:20;
            const unsigned long changed=timing==2?10:timing==3?11:9;
            const long tattoo=timing==4?20:timing==5?21:19;
            exercise(av,force!=0,c,t,changed,tattoo,n,covered,large?4.0f:0.5f,s);
        }
    if(failures) return 1;
    puts("0057d23b_TEST PASS");
    return 0;
}