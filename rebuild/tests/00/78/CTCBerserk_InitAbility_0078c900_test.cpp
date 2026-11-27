#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static void check(bool value){if(!value)abort();}
static unsigned seed;static int trace[64],count,readBase,bodyEffect,hasteAdds,casts,loops;static void log(int v){check(count<64);trace[count++]=v;}
static bool bit(int i){return (seed&(1u<<i))!=0;}
struct C3DVector{float X,Y,Z;};
struct CTCBerserk;static CTCBerserk* owner;
struct CSpecialEffectsDef {bool GetEffectFromString(const char* key,long& result) const{
 int k=!strcmp(key,"SKILL_STRENGTH_INTRO")?0:!strcmp(key,"BERSERK")?1:!strcmp(key,"BERSERK_BODY_EFFECT")?2:-1;check(k>=0);log(10+k);if(!bit(4+k))return false;result=100+k;return true;}} effects;
template<class T>struct CDefPointer{T* data;CDefPointer():data(0){}T* operator->(){check(data!=0);return data;}};
struct CThing;
struct CTCDParticleEmitter{void AttachToThing(CThing&,long,const char*,long,float);};
struct CTCHaste{void SetActionSpeedMultiplier(void* p,float f){check(p==owner&&f==1.5f);log(3);}} haste;
template<class T,class U> U GFDebugPDynamicCast(T* v,U){return static_cast<U>(v);}
struct CTCSoundPlayer{
 unsigned long PlayAttachedCriteria(const char* key,bool cast,bool a,bool b,long val){
 check(!a&&!b&&val==-1);
 const char*keys[]={"HERO_ABILITY_BEZERK_CONTINUOUS_NO_POWERUP","HERO_ABILITY_BEZERK_CONTINUOUS_LEVEL_1","HERO_ABILITY_BEZERK_CONTINUOUS_LEVEL_2","HERO_ABILITY_BEZERK_CONTINUOUS_LEVEL_3"};
 if(cast){check(!strcmp(key,"HERO_ABILITY_BEZERK_CAST"));++casts;log(30);return 11;}
 for(int i=0;i<4;++i)if(!strcmp(key,keys[i])){++loops;log(32+i);return 20+i;}
 abort();return 0;}
 void SetExtraTag(const char*s){check(!strcmp(s,"BERSERK"));log(31);}
} sound;
struct Base{bool GetSubDef(CDefPointer<const CSpecialEffectsDef>* p){int index=readBase++;check(index<2);log(5+index);if(!bit(1+index))return false;p->data=&effects;return true;}} base;
static CTCDParticleEmitter emitter;
struct CThing{C3DVector pos;bool alive;
 const C3DVector& GetPos()const{return pos;}Base& PeekBaseDef(){return base;}
 bool GetTC(CTCHaste** p){log(1);if(!bit(0))return false;*p=&haste;return true;}
 bool GetTC(CTCSoundPlayer** p){log(29);if(!bit(9))return false;*p=&sound;return true;}
 bool GetTC(CTCDParticleEmitter** p){log(25);*p=&emitter;return true;}
 CTCHaste* AddTC(const char* n,bool on,void* p){check(!strcmp(n,"CTCHaste")&&on&&!p);++hasteAdds;log(2);return &haste;}
} parent,body,old;
void CTCDParticleEmitter::AttachToThing(CThing& p,long slot,const char* attach,long i,float f){check(&p==&parent&&slot==42&&!attach[0]&&!i&&!f);log(26);}
bool IS_THING_ALIVE(CThing* p){log(24);return p&&p->alive;}
struct CTCSpecialAbilities{static CThing* CreateParticleEffect(long effect,const C3DVector& pos){check(&pos==&parent.pos&&effect>=100&&effect<=102);log(20+effect-100);if(effect==102)++bodyEffect;return bit(7)?&body:0;}};
namespace NScreenEffect {enum Priority{SCREEN_EFFECT_PRIORITY_SPELL=2};}
struct CEnvironmentSpellEffectInterpolator {void* RadialBlurFadeTo(float t,NScreenEffect::Priority p,float a,float b,float c,float d,float e,float f){check(t==0&&p==NScreenEffect::SCREEN_EFFECT_PRIORITY_SPELL&&a==2&&b==3&&c==4&&d==5&&e==6&&f==7);log(0);return this;}} interpObj;
struct Env{CEnvironmentSpellEffectInterpolator& GetSpellEffectInterpolator(){return interpObj;}} env;
struct World{Env& GetEnvironment(){return env;}} world;
struct Def{float BerserkThreshold,BerserkLength,BerserkIntensity,BerserkFade,BerserkRange,BerserkTwist,SpeedMultiplier;
 bool GetSubDef(CDefPointer<const CSpecialEffectsDef>* p){log(7);if(!bit(3))return false;p->data=&effects;return true;}
} def={2,3,4,5,6,7,1.5f};
#define FABLE_RETAIL_ASSERT(v) check(v)
struct CTCBerserk{bool AbilityInitialised;void* RadialBlurFadeKey;Def* PDef;CThing* BerserkBodyEffect;unsigned long BerserkingLoopingSound;long SpellLevel;
 World& GetWorld(){check(AbilityInitialised);return world;}CThing& GetParentThing(){return parent;}void InitAbility();};
void CTCBerserk::InitAbility()
{
    AbilityInitialised = true;
    CEnvironmentSpellEffectInterpolator& interp = GetWorld().GetEnvironment().GetSpellEffectInterpolator();
    RadialBlurFadeKey = interp.RadialBlurFadeTo(
        0.0f, NScreenEffect::SCREEN_EFFECT_PRIORITY_SPELL, PDef->BerserkThreshold, PDef->BerserkLength, PDef->BerserkIntensity,
        PDef->BerserkFade, PDef->BerserkRange, PDef->BerserkTwist);

    CTCHaste* ptc_haste;
    if (!GetParentThing().GetTC(&ptc_haste))
        ptc_haste = GFDebugPDynamicCast(GetParentThing().AddTC("CTCHaste", true, 0), (CTCHaste*)0);
    ptc_haste->SetActionSpeedMultiplier(this, PDef->SpeedMultiplier);

    CDefPointer<const CSpecialEffectsDef> pspecial_effects;
    long effect = 0;
    if (GetParentThing().PeekBaseDef().GetSubDef(&pspecial_effects) && pspecial_effects->GetEffectFromString("SKILL_STRENGTH_INTRO", effect)) {
        CThing* pemitter = CTCSpecialAbilities::CreateParticleEffect(effect, GetParentThing().GetPos());
    }
    if (GetParentThing().PeekBaseDef().GetSubDef(&pspecial_effects) && pspecial_effects->GetEffectFromString("BERSERK", effect)) {
        CThing* pemitter = CTCSpecialAbilities::CreateParticleEffect(effect, GetParentThing().GetPos());
    }
    if (PDef->GetSubDef(&pspecial_effects) && pspecial_effects->GetEffectFromString("BERSERK_BODY_EFFECT", effect)) {
        BerserkBodyEffect = CTCSpecialAbilities::CreateParticleEffect(effect, GetParentThing().GetPos());
        if (IS_THING_ALIVE(BerserkBodyEffect)) {
            CTCDParticleEmitter* ptcdParticleEmitter;
            if (!BerserkBodyEffect->GetTC(&ptcdParticleEmitter)) { FABLE_RETAIL_ASSERT(false); }
            ptcdParticleEmitter->AttachToThing(GetParentThing(), 42, "", 0, 0.0f);
        }
    }

    CTCSoundPlayer* ptc_sound_player;
    if (GetParentThing().GetTC(&ptc_sound_player)) {
        ptc_sound_player->PlayAttachedCriteria("HERO_ABILITY_BEZERK_CAST", true, false, false, -1);
        ptc_sound_player->SetExtraTag("BERSERK");
        switch (SpellLevel) {
        case 0:
            BerserkingLoopingSound = ptc_sound_player->PlayAttachedCriteria("HERO_ABILITY_BEZERK_CONTINUOUS_NO_POWERUP", false, false, false, -1);
            break;
        case 1:
            BerserkingLoopingSound = ptc_sound_player->PlayAttachedCriteria("HERO_ABILITY_BEZERK_CONTINUOUS_LEVEL_1", false, false, false, -1);
            break;
        case 2:
            BerserkingLoopingSound = ptc_sound_player->PlayAttachedCriteria("HERO_ABILITY_BEZERK_CONTINUOUS_LEVEL_2", false, false, false, -1);
            break;
        case 3:
            BerserkingLoopingSound = ptc_sound_player->PlayAttachedCriteria("HERO_ABILITY_BEZERK_CONTINUOUS_LEVEL_3", false, false, false, -1);
            break;
        }
    }
}
int main(){
 for(seed=0;seed<65536;++seed){CTCBerserk x;owner=&x;x.PDef=&def;x.AbilityInitialised=false;x.RadialBlurFadeKey=0;x.BerserkBodyEffect=&old;x.BerserkingLoopingSound=99;x.SpellLevel=(long)((seed>>10)%8)-2;body.alive=bit(8);
  count=readBase=bodyEffect=hasteAdds=casts=loops=0;x.InitAbility();
  int want[64],n=0;want[n++]=0;want[n++]=1;if(!bit(0))want[n++]=2;want[n++]=3;
  for(int i=0;i<3;++i){want[n++]=5+i;if(!bit(1+i))continue;want[n++]=10+i;if(!bit(4+i))continue;want[n++]=20+i;if(i==2){want[n++]=24;if(bit(7)&&bit(8)){want[n++]=25;want[n++]=26;}}}
  want[n++]=29;if(bit(9)){want[n++]=30;want[n++]=31;if(x.SpellLevel>=0&&x.SpellLevel<4)want[n++]=32+x.SpellLevel;}
  check(n==count);for(int j=0;j<n;++j)check(want[j]==trace[j]);
  bool made=bit(3)&&bit(6),played=bit(9)&&x.SpellLevel>=0&&x.SpellLevel<4;
  check(x.AbilityInitialised&&x.RadialBlurFadeKey==&interpObj&&readBase==2&&bodyEffect==int(made)&&hasteAdds==int(!bit(0))&&casts==int(bit(9))&&loops==int(played));
  check(x.BerserkBodyEffect==(made?(bit(7)?&body:0):&old));check(x.BerserkingLoopingSound==(played?20+x.SpellLevel:99));}
 puts("0078c900_TEST PASS");return 0;}