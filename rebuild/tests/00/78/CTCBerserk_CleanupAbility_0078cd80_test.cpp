#include <stdio.h>
#include <stdlib.h>
#include <string.h>
namespace std {template<class T> class vector { public: T data[3]; typedef T* iterator; iterator begin(){return data;} iterator end(){return data+3;} };}
static unsigned bits; static int events[32],n; static void event(int id){if(n>=32)abort();events[n++]=id;}
static bool immediateFlag; static int expectedLevel; static void* owner;
class CThing; class CThingCreatureBase;
class CTCCombat {public:void RemoveMeleeFlourishAbility(long k){if(k!=71)abort();event(2);}};
class CTCHeroMorph {public:void SetBerserkSkin(float k){if(k!=0)abort();event(4);}};
namespace NGraphicAppearance {class CTCGraphicAppearance {public:void RemoveTintColour(void* p){if(p!=owner)abort();event(5);}void SetScale(float f){if(f!=1)abort();event(6);}};}
class CTCHaste {public:void UnsetActionSpeedMultiplier(void* p){if(p!=owner)abort();event(9);}};
class CTCSoundPlayer {public:void StopAttachedSound(unsigned long k){if(k!=46)abort();event(13);}void SetExtraTag(const char* s){if(*s)abort();event(14);}void PlayAttachedCriteria(const char* s,bool a,bool b,bool c,long d){const char* names[]={"HERO_ABILITY_BEZERK_END_NO_POWERUP","HERO_ABILITY_BEZERK_END_LEVEL_1","HERO_ABILITY_BEZERK_END_LEVEL_2","HERO_ABILITY_BEZERK_END_LEVEL_3"};if(expectedLevel<0||expectedLevel>3||strcmp(s,names[expectedLevel])||!a||b||c||d!=-1)abort();event(15);}};
class CTCCreatureModeManager {public:void RemoveMode(unsigned long k){if(k!=36)abort();event(16);}};
class CThing {public: bool dead;int id;void Kill(bool k){if(!k||dead)abort();event(id);dead=true;}bool IsDead()const{return dead;}bool GetTC(CTCSoundPlayer** p){static CTCSoundPlayer s;*p=&s;return (bits&32)!=0;}};
class CCreatureAction_PlayAnimation;
class CThingCreatureBase : public CThing {public:using CThing::GetTC;
bool GetTC(CTCCombat** p){static CTCCombat s;*p=&s;return (bits&1)!=0;}bool GetTC(CTCHeroMorph** p){static CTCHeroMorph s;*p=&s;return (bits&2)!=0;}bool GetTC(CTCHaste** p){static CTCHaste s;*p=&s;return (bits&8)!=0;}bool GetTC(CTCCreatureModeManager** p){static CTCCreatureModeManager s;*p=&s;return true;}NGraphicAppearance::CTCGraphicAppearance* GetTCGraphicAppearance(){static NGraphicAppearance::CTCGraphicAppearance g;return bits&4?&g:0;}int GetPos()const{return 73;}void FinishCurrentAction(){event(17);}void SetCurrentAction(const CCreatureAction_PlayAnimation&){event(19);}};
static CThingCreatureBase creature;
static CThingCreatureBase* GFDebugPDynamicCast(CThing* p,CThingCreatureBase*){if(p!=&creature)abort();return &creature;}
template<class T> class CIntelligentPointer {public:T* p;T* Get()const{return p;}T* operator->()const{return p;}};
static bool IS_THING_ALIVE(CThing* p){return p&&!p->IsDead();} static bool IS_THING_ALIVE(CIntelligentPointer<CThing> p){return IS_THING_ALIVE(p.Get());}
class CMainGameComponent {public:static CMainGameComponent* Get(){static CMainGameComponent c;return &c;}CMainGameComponent& PeekDefinitionManager(){return *this;}long GetDefGlobalIndexFromName(const char* s){if(strcmp(s,"HERO_MELEE_COMBAT_ABILITY_BERSERK_CRAZY_MOVE"))abort();event(1);return 71;}};
class CSoundPair {public:CSoundPair(int n){if(n)abort();}};
class CCreatureAction_PlayAnimation {public:CCreatureAction_PlayAnimation(CThingCreatureBase& p,const char* s,const CSoundPair&,bool a,bool b,bool c,long d,long e){if(&p!=&creature||strcmp(s,"STRENGTH_BERSERK_EXIT")||a||!b||!c||d||e)abort();event(18);}};
class CTCSpecialAbilities {public:static void CreateParticleEffect(long e,int pos){if(e!=234||pos!=73)abort();event(12);}};
enum ENullDef {NULL_DEF};
template<class T> class CDefPointer {public:T* p;CDefPointer(ENullDef=NULL_DEF):p(0){}T* operator->()const{return p;}};
class CSpecialEffectsDef {public:bool GetEffectFromString(const char* s,long& e)const{if(strcmp(s,"BERSERK_EXIT_EFFECT")||e)abort();e=234;event(11);return (bits&128)!=0;}};
class CSpecialAbilitiesBerserkDef {public:float TimeForRadialBlurToFadeOut;bool GetSubDef(CDefPointer<const CSpecialEffectsDef>* p){static CSpecialEffectsDef s;p->p=&s;event(21);return (bits&64)!=0;}};
class CEnvironmentSpellEffectInterpolator {public: void RadialBlurFadeOut(float secs,void* key){if(secs!=(immediateFlag?0.f:2.5f)||key!=&bits)abort();event(10);}};
class CWorld {public:CWorld& GetEnvironment(){return *this;}CEnvironmentSpellEffectInterpolator& GetSpellEffectInterpolator(){static CEnvironmentSpellEffectInterpolator s;return s;}CWorld& GetOpinionReactionManager(){return *this;}void ClearOpinionSource(CThing& p,bool b){if(&p!=&creature||!b)abort();event(20);}};
class CTCBerserk {public:CThingCreatureBase& GetParentThing(){return creature;} CWorld& GetWorld(){static CWorld w;return w;}CIntelligentPointer<CThing> BerserkBodyEffect;std::vector<CIntelligentPointer<CThing> > DependentThings;CSpecialAbilitiesBerserkDef* PDef;void* RadialBlurFadeKey;unsigned long BerserkingLoopingSound;long SpellLevel;void CleanupAbility(bool immediate);};
void CTCBerserk::CleanupAbility(bool immediate)
{
    CThingCreatureBase* pcreature=GFDebugPDynamicCast(&GetParentThing(),(CThingCreatureBase*)0);
    CTCCombat* ptc_combat;
    if(pcreature->GetTC(&ptc_combat)) {
        long combat_def_index=CMainGameComponent::Get()->PeekDefinitionManager().GetDefGlobalIndexFromName("HERO_MELEE_COMBAT_ABILITY_BERSERK_CRAZY_MOVE");
        ptc_combat->RemoveMeleeFlourishAbility(combat_def_index);
    }
    if(IS_THING_ALIVE(BerserkBodyEffect)) BerserkBodyEffect->Kill(true);
    CTCHeroMorph* ptc_hero_morph;
    if(pcreature->GetTC(&ptc_hero_morph)) ptc_hero_morph->SetBerserkSkin(0.0f);
    NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance=pcreature->GetTCGraphicAppearance();
    if(ptc_graphic_appearance) {
        ptc_graphic_appearance->RemoveTintColour(this);
        ptc_graphic_appearance->SetScale(1.0f);
    }
    for(std::vector<CIntelligentPointer<CThing> >::iterator it=DependentThings.begin();it!=DependentThings.end();++it) {
        CThing* pthing=it->Get();
        if(IS_THING_ALIVE(pthing)) pthing->Kill(true);
    }
    CTCHaste* ptc_haste;
    if(pcreature->GetTC(&ptc_haste)) ptc_haste->UnsetActionSpeedMultiplier(this);
    CEnvironmentSpellEffectInterpolator& interp=GetWorld().GetEnvironment().GetSpellEffectInterpolator();
    interp.RadialBlurFadeOut(immediate?0.0f:PDef->TimeForRadialBlurToFadeOut,RadialBlurFadeKey);
    if(!immediate) {
        CDefPointer<const CSpecialEffectsDef> pspecial_effects(NULL_DEF);
        long effect=0;
        if(PDef->GetSubDef(&pspecial_effects) && pspecial_effects->GetEffectFromString("BERSERK_EXIT_EFFECT",effect))
            CTCSpecialAbilities::CreateParticleEffect(effect,pcreature->GetPos());
    }
    CTCSoundPlayer* ptc_sound_player;
    if(GetParentThing().GetTC(&ptc_sound_player)) {
        ptc_sound_player->StopAttachedSound(BerserkingLoopingSound);
        ptc_sound_player->SetExtraTag("");
        if(!immediate) switch(SpellLevel) {
        case 0: ptc_sound_player->PlayAttachedCriteria("HERO_ABILITY_BEZERK_END_NO_POWERUP",true,false,false,-1); break;
        case 1: ptc_sound_player->PlayAttachedCriteria("HERO_ABILITY_BEZERK_END_LEVEL_1",true,false,false,-1); break;
        case 2: ptc_sound_player->PlayAttachedCriteria("HERO_ABILITY_BEZERK_END_LEVEL_2",true,false,false,-1); break;
        case 3: ptc_sound_player->PlayAttachedCriteria("HERO_ABILITY_BEZERK_END_LEVEL_3",true,false,false,-1); break;

        }
    }
    CTCCreatureModeManager* ptc_mode_manager;
    pcreature->GetTC(&ptc_mode_manager);
    ptc_mode_manager->RemoveMode(0x24);
    pcreature->FinishCurrentAction();
    if(!immediate) pcreature->SetCurrentAction(CCreatureAction_PlayAnimation(*pcreature,"STRENGTH_BERSERK_EXIT",CSoundPair(0),false,true,true,0,0));
    GetWorld().GetOpinionReactionManager().ClearOpinionSource(*pcreature,true);
}

int main(){for(bits=0;bits<256;bits++)for(int im=0;im<=1;im++)for(int lvl=-1;lvl<5;lvl++)for(int dep=0;dep<4;dep++){int ex[32],k=0;CTCBerserk s;CThing body,one,two;body.dead=(bits&16)==0;body.id=3;one.dead=(dep&1)==0;one.id=7;two.dead=(dep&2)==0;two.id=8;
s.BerserkBodyEffect.p=&body;s.DependentThings.data[0].p=&one;s.DependentThings.data[1].p=0;s.DependentThings.data[2].p=&two;s.RadialBlurFadeKey=&bits;s.BerserkingLoopingSound=46;s.SpellLevel=expectedLevel=lvl;CSpecialAbilitiesBerserkDef def;def.TimeForRadialBlurToFadeOut=2.5f;s.PDef=&def;owner=&s;immediateFlag=im!=0;n=0;
if(bits&1){ex[k++]=1;ex[k++]=2;}if(bits&16)ex[k++]=3;if(bits&2)ex[k++]=4;if(bits&4){ex[k++]=5;ex[k++]=6;}if(dep&1)ex[k++]=7;if(dep&2)ex[k++]=8;if(bits&8)ex[k++]=9;ex[k++]=10;if(!im){ex[k++]=21;if(bits&64){ex[k++]=11;if(bits&128)ex[k++]=12;}}if(bits&32){ex[k++]=13;ex[k++]=14;if(!im&&lvl>=0&&lvl<=3)ex[k++]=15;}ex[k++]=16;ex[k++]=17;if(!im){ex[k++]=18;ex[k++]=19;}ex[k++]=20;s.CleanupAbility(immediateFlag);if(k!=n||memcmp(ex,events,k*sizeof(int)))return 1;}puts("0078cd80_TEST PASS");return 0;}