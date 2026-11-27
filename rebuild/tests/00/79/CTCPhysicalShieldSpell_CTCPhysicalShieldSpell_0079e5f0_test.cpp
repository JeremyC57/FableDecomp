#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <set>
static void check(bool x){if(!x)exit(2);}
typedef std::string CCharString;
struct CRGBColour {unsigned char R,G,B,A;CRGBColour(){}CRGBColour(unsigned char r,unsigned char g,unsigned char b,unsigned char a):R(r),G(g),B(b),A(a){}};
enum EHeroAbility {HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4};
struct CTCInventoryAbilities{long level;long GetAbilityLevel(EHeroAbility h)const{check(h==HERO_ABILITY_PHYSICAL_SHIELD_SPELL);return level;}};
struct CThing{unsigned long type;unsigned long GetType()const{return type;}};
static std::string events;
struct CThingCreatureBase:CThing {
    const CTCInventoryAbilities* abilities;
    bool PeekTC(const CTCInventoryAbilities** p){events+='A';if(abilities)*p=abilities;return abilities!=0;}
    void MakeNoise(CCharString name){check(name=="SIMPLE_NOISE");events+='N';}
};
struct CTCBase{CThing* ParentThing;CTCBase(CThing& t):ParentThing(&t){}CThing& GetParentThing(){return *ParentThing;}};
template<class T,class U>static U GFDebugPDynamicCast(T v,U){return static_cast<U>(v);}
struct CSpecialAbilitiesPhysicalShieldDef{
    float WobbleRate,FadeOutPhysicalShieldTimeSecs;
    float ColorStartLevel1[4],ColorStartLevel2[4],ColorStartLevel3[4],ColorStartLevel4[4];
    CCharString LoopingSound[5];
};
template<class T>struct CDefPointer{T* p;CDefPointer(T* v):p(v){}T* operator->()const{return p;}};
struct CGameDefinitionManager{
    CSpecialAbilitiesPhysicalShieldDef def;
    static CGameDefinitionManager& Get(){static CGameDefinitionManager m;return m;}
    long GetGlobalIndexFromClassIndex(CCharString name,long index){check(name=="SPECIAL_ABILITIES_PHYSICAL_SHIELD_DEF" && index==1);events+='I';return 53;}
    void GetDef(long index,CDefPointer<const CSpecialAbilitiesPhysicalShieldDef>* p){check(index==53);p->p=&def;events+='D';}
};
struct CMainGameComponent {static long GetConstantFPS(){return 30;}};
struct CTCPhysicalShieldSpell:CTCBase{
    CDefPointer<const CSpecialAbilitiesPhysicalShieldDef> PDef;long SpellLevel,ShieldFadeOutTimer,CurShieldFadeOutTimer,DurationTimer;
    float ShieldHealth,CurWobbleTimer,WobbleDelta;CRGBColour ShieldColor;CThing* ShieldEffect;CThing* SwirlyEffect;unsigned long LoopingSound;bool Pausable,Paused,CutSceneCancelled;std::set<CThing*> AllowReactionAttackers;
    CTCPhysicalShieldSpell(CThing&);
    unsigned long PlaySoundW(CCharString s,bool b){check(s==PDef->LoopingSound[SpellLevel] && b==(ParentThing->GetType()==2));events+='S';return 731;}
    void PlayEffect(void* p,CCharString tag,CCharString end,CThingCreatureBase* c,long n){check(!p && tag=="CAST" && end.empty() && c==ParentThing && n==8);events+='E';}
    void PlayShieldEffects(CThingCreatureBase* c){check(c==ParentThing);events+='H';}
};
CTCPhysicalShieldSpell::CTCPhysicalShieldSpell(CThing& thing)
    : CTCBase(thing), PDef((const CSpecialAbilitiesPhysicalShieldDef*)0), SpellLevel(0), ShieldFadeOutTimer(0), CurShieldFadeOutTimer(0), DurationTimer(-1),
      ShieldHealth(0.0f), CurWobbleTimer(0.0f), WobbleDelta(0.0f), ShieldEffect(0), SwirlyEffect(0), Pausable(true), Paused(false), CutSceneCancelled(false)
{
    long def_index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("SPECIAL_ABILITIES_PHYSICAL_SHIELD_DEF", 1);
    CGameDefinitionManager::Get().GetDef(def_index,&PDef);
    WobbleDelta = PDef->WobbleRate;
    CThingCreatureBase* pcreature = GFDebugPDynamicCast(&GetParentThing(),(CThingCreatureBase*)0);
    const CTCInventoryAbilities* ptc_inventory_abilities;
    if (pcreature->PeekTC(&ptc_inventory_abilities))
        SpellLevel = ptc_inventory_abilities->GetAbilityLevel(HERO_ABILITY_PHYSICAL_SHIELD_SPELL);
    switch (SpellLevel) {
    case 0:
        ShieldColor = CRGBColour((unsigned char)PDef->ColorStartLevel1[0], (unsigned char)PDef->ColorStartLevel1[1], (unsigned char)PDef->ColorStartLevel1[2], (unsigned char)PDef->ColorStartLevel1[3]);
        break;
    case 1:
        ShieldColor = CRGBColour((unsigned char)PDef->ColorStartLevel2[0], (unsigned char)PDef->ColorStartLevel2[1], (unsigned char)PDef->ColorStartLevel2[2], (unsigned char)PDef->ColorStartLevel2[3]);
        break;
    case 2:
        ShieldColor = CRGBColour((unsigned char)PDef->ColorStartLevel3[0], (unsigned char)PDef->ColorStartLevel3[1], (unsigned char)PDef->ColorStartLevel3[2], (unsigned char)PDef->ColorStartLevel3[3]);
        break;
    case 3:
        ShieldColor = CRGBColour((unsigned char)PDef->ColorStartLevel4[0], (unsigned char)PDef->ColorStartLevel4[1], (unsigned char)PDef->ColorStartLevel4[2], (unsigned char)PDef->ColorStartLevel4[3]);
        break;
    default:
        ShieldColor = CRGBColour((unsigned char)PDef->ColorStartLevel1[0], (unsigned char)PDef->ColorStartLevel1[1], (unsigned char)PDef->ColorStartLevel1[2], (unsigned char)PDef->ColorStartLevel1[3]);
        break;
    }
    LoopingSound = PlaySoundW(PDef->LoopingSound[SpellLevel],pcreature->GetType()==2);
    PlayEffect(0,"CAST","",pcreature,8);
    PlayShieldEffects(pcreature);
    pcreature->MakeNoise("SIMPLE_NOISE");
    ShieldFadeOutTimer = CurShieldFadeOutTimer = (long)(CMainGameComponent::GetConstantFPS()*PDef->FadeOutPhysicalShieldTimeSecs);
}
int main(){
    CSpecialAbilitiesPhysicalShieldDef& def=CGameDefinitionManager::Get().def;
    float* colors[]={def.ColorStartLevel1,def.ColorStartLevel2,def.ColorStartLevel3,def.ColorStartLevel4};
    const char* sounds[]={"one","two","three","four","fallback fixture"};
    for(int v=0;v<5;++v){def.LoopingSound[v]=sounds[v];if(v<4)for(int k=0;k<4;++k)colors[v][k]=10*v+k+.75f;}
    for(int n=-1;n<=4;++n)for(int player=0;player<=1;++player){
        CThingCreatureBase creature;CTCInventoryAbilities inventory;inventory.level=n;creature.abilities=n<0?0:&inventory;creature.type=player?2:8;
        def.WobbleRate=.375f;def.FadeOutPhysicalShieldTimeSecs=1.25f;events="";
        CTCPhysicalShieldSpell shield(creature);
        int level=n<0?0:n;int row=level>=4?0:level;
        check(shield.SpellLevel==level && shield.PDef.p==&def && events=="IDASEHN" && shield.ParentThing==&creature);
        check(shield.ShieldColor.R==10*row && shield.ShieldColor.G==10*row+1 && shield.ShieldColor.B==10*row+2 && shield.ShieldColor.A==10*row+3);
        check(shield.WobbleDelta==.375f && shield.ShieldHealth==0 && shield.CurWobbleTimer==0 && shield.DurationTimer==-1);
        check(shield.ShieldFadeOutTimer==37 && shield.CurShieldFadeOutTimer==37 && !shield.ShieldEffect && !shield.SwirlyEffect);
        check(shield.LoopingSound==731 && shield.Pausable && !shield.Paused && !shield.CutSceneCancelled && shield.AllowReactionAttackers.empty());
    }
    puts("0079e5f0_TEST PASS");return 0;
}