#define FABLE_RETAIL_CJACKOFBLADESBATTLEDEF_TYPENAME_PASS26
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22
// Leaf: ?OnActivate@CModeJackOfBladesFlash@NCreatureMode@@UAEXXZ
// Flags: (none)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_PDCB_TYPED_MEMBERS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODEBASE_PARENTTHING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_LIGHTINGFLASH
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_CREATE
#define FABLE_RETAIL_C3DVECTOR
#include "engine/CDefString.h"
#include "engine/CVectorMap.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefPointer.h"
#include "engine/NCreatureMode_CModeJackOfBladesFlash.h"
#include "engine/CJackOfBladesBattleDef.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CMainGameComponent.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CWorld.h"
#include "engine/CEnvironment.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CRightHandedSet.h"
#include "engine/CDefClassBase_GetSubDef.h"
namespace {extern long JACK_FLASH_DURATION,JACK_POWERUP_DURATION,JACK_FADE_UP_TIME,JACK_TOTAL_DURATION,JACK_GLOW_START_FRAME,JACK_FADE_DOWN_TIME;}

#include <cstdio>
#include <cmath>
#include <cstring>
static int errors=0,basecalls=0,jacklookups=0,fxlookups=0,fxqueries=0,posqueries=0,dummyqueries=0,creates=0,assigns=0,iplive=0,releases=0,themequeries=0,transitions=0;static long fps=30;static bool fxpresent=true,fxfound=true,dummyfound=true,createok=true;
#define CHECK(x) do{if(!(x)){++errors;std::printf("FAIL %d %s\n",__LINE__,#x);}}while(0)
static bool near(float a,float b){return std::fabs(a-b)<.0001f;}
namespace {long JACK_FLASH_DURATION,JACK_POWERUP_DURATION,JACK_FADE_UP_TIME,JACK_TOTAL_DURATION,JACK_GLOW_START_FRAME,JACK_FADE_DOWN_TIME;}
struct FixtureJack{mutable int refs;float FlashDuration,FlashBuildUpTime;};
struct FixtureEffects{mutable int refs;bool GetEffectFromString(const char*n,long&i)const{CHECK(!std::strcmp(n,"FLASH_SWORD_POWER_UP")&&i==0&&basecalls==1);++fxqueries;if(fxfound)i=71;return fxfound;}};
template<class T>struct FixtureDefPointer{T*p;FixtureDefPointer():p(0){}~FixtureDefPointer(){if(p){CHECK(p->refs==2);--p->refs;++releases;}}void set(T*v){CHECK(!p);p=v;if(p)++p->refs;}T*operator->()const{return p;}};
struct FixtureBaseDef{FixtureJack*jack;FixtureEffects*effects;bool GetSubDef(FixtureDefPointer<const FixtureJack>*p)const{++jacklookups;CHECK(basecalls==0);p->set(jack);return true;}bool GetSubDef(FixtureDefPointer<const FixtureEffects>*p)const{++fxlookups;CHECK(basecalls==1);if(fxpresent)p->set(effects);return fxpresent;}};
struct FixtureGraphic{bool GetDummyObject(const char*n,long i,unsigned long f,C3DVector&pos,CRightHandedSet&,float scale)const{CHECK(!std::strcmp(n,"WEAPON_FOCUS_01")&&i==0&&f==0&&scale==1&&pos.X==3&&pos.Y==4&&pos.Z==5);++dummyqueries;pos=C3DVector(8,-2,6);return dummyfound;}};
struct FixtureThing{FixtureBaseDef*def;FixtureGraphic*graphic;C3DVector position;const FixtureBaseDef&PeekBaseDef()const{return*def;}const C3DVector&GetPos()const{++posqueries;return position;}FixtureGraphic*GetTCGraphicAppearance(){return graphic;}};
template<class T>struct FixtureIP{T*p;FixtureIP():p(0){}FixtureIP(T*v):p(v){++iplive;}~FixtureIP(){if(p)--iplive;}FixtureIP&operator=(const FixtureIP&o){CHECK(o.p);++assigns;p=o.p;return*this;}};
struct FixtureEnvironment{void TransitionToThemeAllChannels(long priority,long n,float t){CHECK(priority==3&&n==83&&near(t,JACK_FADE_DOWN_TIME/30.f)&&iplive==0&&releases==0);++transitions;}};struct FixtureWorld{FixtureEnvironment env;FixtureEnvironment&GetEnvironment(){return env;}};
struct FixtureManager{static FixtureManager&Get(){static FixtureManager m;return m;}long GetDefGlobalIndexFromName(const char*s)const{CHECK(!std::strcmp(s,"ENVIRONMENT_JACK_OF_BLADES_FADE")&&basecalls==1);++themequeries;return 83;}};
struct FixtureMain{static long GetConstantFPS(){return fps;}};
struct FixtureEmitter{static FixtureThing emitter;static FixtureThing*Create(long n,const C3DVector&p,bool b){CHECK(n==71&&near(p.X,8)&&near(p.Y,-2)&&near(p.Z,6)&&!b);++creates;return createok?&emitter:0;}};FixtureThing FixtureEmitter::emitter;
struct FixtureModeManager{struct CModeBase{FixtureThing*parent;FixtureWorld*world;FixtureThing&GetParentThing(){return*parent;}FixtureWorld&GetWorld()const{return*world;}void OnActivate(){CHECK(jacklookups==1&&JACK_FLASH_DURATION==(long)(fps*parent->def->jack->FlashDuration)&&JACK_POWERUP_DURATION==(long)(fps*parent->def->jack->FlashBuildUpTime)&&JACK_TOTAL_DURATION==JACK_FLASH_DURATION+JACK_POWERUP_DURATION&&JACK_GLOW_START_FRAME==JACK_FLASH_DURATION&&JACK_FADE_DOWN_TIME==JACK_POWERUP_DURATION&&JACK_FADE_UP_TIME==10);++basecalls;}};};
namespace NCreatureMode{struct FixtureFlash:FixtureModeManager::CModeBase{long Timer,BurnCounter;FixtureThing*PCreature;FixtureIP<FixtureThing>PEmitter;void OnActivate();};}
#define CModeJackOfBladesFlash FixtureFlash
#define CJackOfBladesBattleDef FixtureJack
#define CSpecialEffectsDef FixtureEffects
#define CDefPointer FixtureDefPointer
#define CMainGameComponent FixtureMain
#define CTCCreatureModeManager FixtureModeManager
#define CThing FixtureThing
#define CTCDParticleEmitter FixtureEmitter
#define CIntelligentPointer FixtureIP
#define CGameDefinitionManager FixtureManager
void NCreatureMode::CModeJackOfBladesFlash::OnActivate()
{
 CDefPointer<const CJackOfBladesBattleDef> p_jack_def;
 GetParentThing().PeekBaseDef().GetSubDef(&p_jack_def);
 JACK_FLASH_DURATION=(long)(CMainGameComponent::GetConstantFPS()*p_jack_def->FlashDuration);
 JACK_POWERUP_DURATION=(long)(CMainGameComponent::GetConstantFPS()*p_jack_def->FlashBuildUpTime);
 JACK_TOTAL_DURATION=JACK_FLASH_DURATION+JACK_POWERUP_DURATION;
 JACK_GLOW_START_FRAME=JACK_TOTAL_DURATION-JACK_POWERUP_DURATION;
 JACK_FADE_DOWN_TIME=JACK_POWERUP_DURATION;
 JACK_FADE_UP_TIME=10;
 CTCCreatureModeManager::CModeBase::OnActivate();
 Timer=JACK_POWERUP_DURATION+JACK_FLASH_DURATION;
 BurnCounter=0;
 CDefPointer<const CSpecialEffectsDef> pspecial_fx;
 long fx_index=0;
 if(GetParentThing().PeekBaseDef().GetSubDef(&pspecial_fx)&&pspecial_fx->GetEffectFromString("FLASH_SWORD_POWER_UP",fx_index)) {
  C3DVector weapon_pos=PCreature->GetPos();
  CRightHandedSet rhs;
  PCreature->GetTCGraphicAppearance()->GetDummyObject("WEAPON_FOCUS_01",0,0,weapon_pos,rhs,1.0f);
  CThing* pEmitter=CTCDParticleEmitter::Create(fx_index,weapon_pos,false);
  if(pEmitter)PEmitter=CIntelligentPointer<CThing>(pEmitter);
 }
 long glow_def_num=CGameDefinitionManager::Get().GetDefGlobalIndexFromName("ENVIRONMENT_JACK_OF_BLADES_FADE");
 GetWorld().GetEnvironment().TransitionToThemeAllChannels(3,glow_def_num,(float)JACK_FADE_DOWN_TIME/30.0f);
}

#undef CModeJackOfBladesFlash
#undef CJackOfBladesBattleDef
#undef CSpecialEffectsDef
#undef CDefPointer
#undef CMainGameComponent
#undef CTCCreatureModeManager
#undef CThing
#undef CTCDParticleEmitter
#undef CIntelligentPointer
#undef CGameDefinitionManager
int main(){long rates[]={24,30,60,0};float durations[]={.5f,1.25f,-.5f,0};for(int t=0;t<128;++t){FixtureJack jack;jack.refs=1;jack.FlashDuration=durations[(t/4)%4];jack.FlashBuildUpTime=durations[(t/16)%4];FixtureEffects effects;effects.refs=1;FixtureBaseDef def;def.jack=&jack;def.effects=&effects;FixtureGraphic graphic;FixtureThing thing;thing.def=&def;thing.graphic=&graphic;thing.position=C3DVector(3,4,5);FixtureWorld world;FixtureThing old_emitter;NCreatureMode::FixtureFlash mode;mode.parent=&thing;mode.world=&world;mode.PCreature=&thing;mode.Timer=mode.BurnCounter=-777;mode.PEmitter.p=(t&8)?&old_emitter:0;fps=rates[(t/32)%4];fxpresent=t%4!=0;fxfound=t%4!=1;createok=t%4!=2;dummyfound=(t&4)!=0;basecalls=jacklookups=fxlookups=fxqueries=posqueries=dummyqueries=creates=assigns=iplive=releases=themequeries=transitions=0;JACK_FLASH_DURATION=JACK_POWERUP_DURATION=JACK_FADE_UP_TIME=JACK_TOTAL_DURATION=JACK_GLOW_START_FRAME=JACK_FADE_DOWN_TIME=-999;mode.OnActivate();bool effect=fxpresent&&fxfound;CHECK(basecalls==1&&jacklookups==1&&fxlookups==1&&fxqueries==(fxpresent?1:0)&&posqueries==(effect?1:0)&&dummyqueries==posqueries&&creates==posqueries&&assigns==((effect&&createok)?1:0));CHECK(mode.Timer==(long)(fps*jack.FlashDuration)+(long)(fps*jack.FlashBuildUpTime)&&mode.BurnCounter==0&&themequeries==1&&transitions==1&&releases==(fxpresent?2:1)&&jack.refs==1&&effects.refs==1&&iplive==0&&mode.PEmitter.p==((effect&&createok)?&FixtureEmitter::emitter:((t&8)?&old_emitter:0)));mode.PEmitter.p=0;}std::puts(errors?"008d7f10_TEST FAIL":"008d7f10_TEST PASS");return errors?1:0;}