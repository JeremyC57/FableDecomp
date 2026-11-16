// Self-contained C++03 behaviour test; actual shared classes and normal lifetimes.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <math.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __int64 long long
#define __declspec(x)
#define FABLE_PORTABLE
#include <strings.h>
#define _stricmp strcasecmp
enum ESaveGameLoadStatus {};enum ERegionLoadStatus {};enum EPlayerSpawnStatus {};enum ESaveType {};
enum ESex {};enum ECombatZone {};enum EControlledMovementType {};enum EMovementBandType {};
#endif
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_STLP_HOST_TAGS_PASS23
#define FABLE_RETAIL_CGAME_CORE_PASS23
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE_CTOR_DECL
#include "engine/EPlayerMode.h"
#define private public
#define protected public
// 00683c10 CTCCreatureOpinionOfHero::GetGreetDialogueGroupIndex
// Flags: none
// Leaf: ?GetGreetDialogueGroupIndex@CTCCreatureOpinionOfHero@@QAEKW4EOpinionAttitudeType@@@Z
// Identity: both original PDBs; FableWin01f47e40 call graph and native callers.
#define FABLE_RETAIL_GREETING_PASS23
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_WORLDSEED
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_GREET
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCOPINIONSOURCE
#define FABLE_RETAIL_CTCWIFE
#define FABLE_RETAIL_CTCWIFE_GREET
#define FABLE_RETAIL_CHEROTITLEDEF
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CBANKFILE_FINDINDEXBYSYMBOLCRC
#include "fable_compiler.h"
#include "engine/ETCInterfaceType.h"
#include "engine/EOpinion.h"
#include "engine/EOpinionAttitudeType.h"
#include "engine/EHeroTrainingStatus.h"
#include "engine/CHeroTitleDef.h"
#include "engine/CTCCreatureOpinionOfHero.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCHero.h"
#include "engine/CTCDrunkenness.h"
#include "engine/CTCOpinionSource.h"
#include "engine/CTCWife.h"
#include "engine/COpinionReactionManager.h"
#include "engine/CWorld.h"
#include "engine/CPlayer.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"
// Both original name sets and donor call01f48152; native004978a0.
float FABLE_FASTCALL GFFloatRandom(float,unsigned long*);

unsigned long CTCCreatureOpinionOfHero::GetGreetDialogueGroupIndex(EOpinionAttitudeType for_attitude)
{
    if (!(GetParentThing().GetTypeFlags() & 8)) return 0;
    const NGameText::CDataBank& text_bank=CMainGameComponent::Get()->GetTextBank();
    const CThingPlayerCreature* pplayer_thing=GetWorld().PeekMyPlayer()->PeekPControlledCreature();
    const CTCHeroStats* ptc_hero_stats=0;
    EHeroTrainingStatus hero_training_status=(pplayer_thing && pplayer_thing->PeekTC(&ptc_hero_stats))
        ?ptc_hero_stats->GetTrainingStatus():TRAINING_STATUS_GRADUATED;
    if (GFDebugPDynamicCast(&GetParentThing(),(CThingCreatureBase*)0)->GetProperty()!=3) {
        CCharString group_name("");
        const CTCDrunkenness* ptc_drunk=0;
        if (GetParentThing().PeekTC(&ptc_drunk) && ptc_drunk->IsDrunk()) {
            group_name="TEXT_AI_OPINION_ACT_GREET_TO_HERO_DRUNK";
            return text_bank.FindIndexBySymbolCRC(group_name.GetCRC());
        }
        const CTCOpinionSource* ptc_source=0;
        if (pplayer_thing) pplayer_thing->PeekTC(&ptc_source);
        const COpinionReactionManager& reaction_manager=PeekWorld().PeekOpinionReactionManager();
        EOpinionAttitudeType attitude=for_attitude;
        if (attitude==OPINION_ATTITUDE_TYPE_LAST) {
            attitude=reaction_manager.GetAttitudeType(GetParentThing(),ptc_source);
            if (!ptc_source) attitude=reaction_manager.GetBlurredAttitude(attitude);
        }
        const CTCWife* ptc_wife=0;
        if (!(attitude==OPINION_ATTITUDE_TYPE_DISDAIN || attitude==OPINION_ATTITUDE_TYPE_OFFENSIVE || attitude==OPINION_ATTITUDE_TYPE_FEAR)) {
            float prob_of_title_greet=reaction_manager.GetThingOpinionOfPlayer(GetParentThing(),OPINION_RENOWN);
            prob_of_title_greet=prob_of_title_greet*prob_of_title_greet*0.35f;
            if (!ptc_source && GFFloatRandom(1.0f,GetPWorldSeed())<prob_of_title_greet) {
                if (!(GetParentThing().HasTC(TCI_WIFE) && GetParentThing().GetTC(&ptc_wife) && ptc_wife->IsMarriedToThePlayer())) {
                    const CTCHero* ptc_hero=0;
                    if (pplayer_thing->PeekTC(&ptc_hero) && ptc_hero->HasHeroTitleSubDef() && ptc_hero->PeekHeroTitleSubDef().HeroTitleGreetToHeroTag>0)
                        return ptc_hero->PeekHeroTitleSubDef().HeroTitleGreetToHeroTag;
                }
            }
        } else if (GetParentThing().PeekTC(&ptc_wife) && ptc_wife->IsMarriedToThePlayer() && ptc_wife->GetGreetDialogueGroupName(group_name)) {
            unsigned long group_index=text_bank.FindIndexBySymbolCRC(group_name.GetCRC());
            return group_index;
        }
        if (hero_training_status!=TRAINING_STATUS_GRADUATED) {
            group_name=GreetedFlag?"TEXT_AI_OPINION_ACT_REGREET_TO_HERO":"TEXT_AI_OPINION_ACT_GREET_TO_HERO";
            if (hero_training_status==TRAINING_STATUS_BOY) group_name+="_ASBOY";
            else if (hero_training_status==TRAINING_STATUS_APPRENTICE) group_name+="_ASAPPRENTICE";
            unsigned long group_index=text_bank.FindIndexBySymbolCRC(group_name.GetCRC());
            return group_index;
        }
        const CCharString& att_str=reaction_manager.GetAttitudeString(attitude);
        if (att_str!="") {
            group_name=GreetedFlag?"TEXT_AI_OPINION_ACT_REGREET_TO_HERO_":"TEXT_AI_OPINION_ACT_GREET_TO_HERO_";
            group_name+=att_str;
            unsigned long group_index=text_bank.FindIndexBySymbolCRC(group_name.GetCRC());
            if (group_index>0) return group_index;
        }
    } else {
        CCharString group_name(GreetedFlag?"TEXT_AI_GUARD_REGREET_TO_HERO":"TEXT_AI_GUARD_GREET_TO_HERO");
        if (hero_training_status==TRAINING_STATUS_BOY) group_name+="_ASBOY";
        else if (hero_training_status==TRAINING_STATUS_APPRENTICE) group_name+="_ASAPPRENTICE";
        unsigned long group_index=text_bank.FindIndexBySymbolCRC(group_name.GetCRC());
        return group_index;
    }
    return 0;
}

#include "engine/CGame.h"
#include "engine/CMainGameComponentInit.h"
#include "engine/CCamera.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CDefString.h"
#undef private
#undef protected
void CBaseObject::Validate() const { abort(); }
void CClass::Save(CCharString&) { abort(); }
bool CMainGameComponent::Run(CGameComponent**) { abort(); }
void CMainGameComponent::ChangeTextureColourDepth(long) { abort(); }
void CMainGameComponent::OnPreDeviceReset() { abort(); }
bool CMainGameComponent::OnPostDeviceReset() { abort(); }
bool CResource::UnloadResource() { abort(); }
unsigned long CResource::GetUnloadableMemory() const { abort(); }
bool CResource::IsResourceLoaded() const { abort(); }
void CResource::OnResourceLoaded() { abort(); }
bool CResource::CanAbortResourceLoad(long) const { abort(); }
void CResource::OnResourceLoadAborted() { abort(); }
unsigned long CResource::GetDebugMemoryInUse() const { abort(); }
void CResource::ResetDebugMemoryInUse() { abort(); }
bool CDefClassBase::UnloadResource() { abort(); }
CDefString CDefClassBase::GetInstantiationName() const { abort(); }
void CDefClassBase::SetInstantiationName(CDefString) { abort(); }
void CDefClassBase::AddSubDef(const CCharString&, long, long) { abort(); }
void CDefClassBase::RemoveSubDef(const CCharString&) { abort(); }
bool CDefClassBase::IsParentDef() const { abort(); }
void CDefClassBase::PostTransfer() { abort(); }
void CDefClassBase::InternalSaveBinary(CDataOutputStream&) { abort(); }
void CDefClassBase::InternalLoadBinary(CDataInputStream&) { abort(); }
void CDefClassBase::Validate(const CValidateDefinitionInitBase&, const CCharString&) const { abort(); }
CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> >* CDefClassBase::GetPSubDefInfoMap() { abort(); }
void CWorld::PostLoadInit() { abort(); }
void CWorld::LoadWorldFromFile(CAFile& file, long version) { abort(); }
CThingManager& CWorld::GetThingManager() { abort(); }
const CThingManager& CWorld::PeekThingManager() const { abort(); }
CCamera CWorld::GetCurrentRenderCamera() const { abort(); }
CCamera CWorld::GetCurrentRenderCamera(const CInterpolationInfo& info) const { abort(); }
void CWorld::PostInit(CDisplayEngine& displayEngine) { abort(); }
C3DVector CWorld::GetGameLODCentre() const { abort(); }
void CWorld::Init() { abort(); }
void CWorld::Uninitialise() { abort(); }
void CWorld::PreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo& info) { abort(); }
CWorldMap& CWorld::GetWorldMap() { abort(); }
const CWorldMap& CWorld::PeekWorldMap() const { abort(); }
const CProceduralTexture* CWorld::DrawGetProceduralTexture() const { abort(); }
const CIDrawWorldMap* CWorld::DrawGetWorldMap() const { abort(); }
const CIDrawEnvironment* CWorld::DrawGetEnvironment() const { abort(); }
void CThingGameObject::OnHit(CHitParameters&) { abort(); }
void CThingCreatureBase::OnHit(CHitParameters&) { abort(); }
bool CThingCreatureBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) { abort(); }
bool CThingPlayerCreature::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) { abort(); }
bool CThingPlayerCreature::SetActionForMelee(const CCreatureAbilityDef& ability) { abort(); }
void CTCBase::OnSerialise(CPersistContext&, bool) { abort(); }
void CTCBase::OnRemove() { abort(); }
void CTCBase::ConstructFromParams(const CTCParamsBase&) { abort(); }
void CTCBase::OnCreate() { abort(); }
void CTCBase::InitialActivate() { abort(); }
void CTCBase::OnInitialActivate() { abort(); }
void CTCBase::RenderUpdate(float) { abort(); }
bool CTCBase::HasFrameUpdate() const { abort(); }
void CTCBase::FrameUpdate() { abort(); }
void CTCBase::BulletTimeFrameUpdate() { abort(); }
void CTCBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&) { abort(); }
void CTCBase::Copy(const CTCBase&) { abort(); }
void CTCBase::OnDie() { abort(); }
void CTCBase::OnKill() { abort(); }
void CTCBase::OnLevelUnload() { abort(); }
void CTCBase::OnSetInLimbo(bool) { abort(); }
bool CTCBase::Validate(CCharString&) const { abort(); }
bool CTCBase::IsFreeForDeletion() const { abort(); }
bool CTCBase::IsSerialiseCreatable() const { abort(); }
bool CTCBase::IsSerialisableForHeroState() const { abort(); }
bool CTCBase::IsComponentSerialisable() const { abort(); }
bool CTCBase::IsDriver() const { abort(); }
bool CTCBase::IsCopyable() const { abort(); }
void CTCBase::GlobalUpdate(bool) { abort(); }
void CTCBase::HandleMessage(const NTCMessages::CMessageBase&) { abort(); }

static unsigned stringLive=0,crcCalls=0,lookupCalls=0,randomCalls=0,attitudeCalls=0,blurCalls=0,opinionCalls=0,wifeCalls=0;
static unsigned long propertyValue=0,seed=123;
static EOpinionAttitudeType selectedAttitude=OPINION_ATTITUDE_TYPE_FRIENDLINESS;
static float opinionValue=1.0f,randomValue=0.0f;
static bool married=false,wifeHasText=false,bankMissing=false;
static CWorld* activeWorld;static CPlayer* activePlayer;static CThingPlayerCreature* activeHero;
static CThingCreatureBase* activeCreature;static COpinionReactionManager* activeManager;static NGameText::CDataBank* activeBank;
static char lastText[256];
static unsigned long hashText(const char*s){unsigned long h=2166136261UL;while(*s){h=((h^(unsigned char)*s++)*16777619UL)&0xffffffffUL;}return h?h:1;}
static CCharStringData* textData(const char*x,long n){if(n<0)n=(long)strlen(x);if(!n)return 0;CCharStringData*d=new CCharStringData;d->Data.PData=(char*)malloc(n+1);assert(d->Data.PData);memcpy(d->Data.PData,x,n);d->Data.PData[n]=0;d->Data.StringLength=(unsigned long)n;d->Data.DataLength=(unsigned long)n+1;d->Data.UseFastExtend=false;d->NoRefs=1;++stringLive;return d;}
static void dropText(CCharStringData*&p){if(p){assert(p->NoRefs==1);free(p->Data.PData);delete p;p=0;--stringLive;}}
const char CCharString::NullChar=0;
CCharString::CCharString():PStringData(0){}
CCharString::CCharString(const char*s,long n):PStringData(textData(s,n)){}
CCharString::CCharString(const CCharString&s):PStringData(textData(s.GetAsCharArray(),-1)){}
CCharString::~CCharString(){dropText(PStringData);}
const CCharString& CCharString::operator=(const char*s){CCharStringData*p=textData(s,-1);dropText(PStringData);PStringData=p;return*this;}
const CCharString& CCharString::operator+=(const char*s){char b[256];assert(strlen(GetAsCharArray())+strlen(s)<sizeof b);strcpy(b,GetAsCharArray());strcat(b,s);return operator=(b);}
const CCharString& CCharString::operator+=(const CCharString&s){return operator+=(s.GetAsCharArray());}
bool CCharString::operator!=(const char*s)const{return strcmp(GetAsCharArray(),s)!=0;}
unsigned long FABLE_FASTCALL CCRC::Calc(unsigned long initial,const void*data,unsigned long count){assert(initial==0&&count<sizeof lastText);memcpy(lastText,data,count);lastText[count]=0;++crcCalls;return hashText(lastText);}
unsigned long CBankFile::FindIndexBySymbolCRC(unsigned long crc)const{assert(this==activeBank&&crc==hashText(lastText));++lookupCalls;return bankMissing?0:crc;}
const CCharString COpinionReactionManager::AttitudeStrings[18]={"","RESPECT","AWE","DISDAIN","FEAR","FRIENDLINESS","WORSHIP","RIDICULOUS","OFFENSIVE","AGREEABLE","UGLY","ATTRACTED","LOVE","WIFE_LOVE","WIFE_LIKE","WIFE_NEUTRAL","WIFE_DISLIKE","WIFE_HATE"};
EOpinionAttitudeType COpinionReactionManager::GetAttitudeType(const CThing&t,const CTCOpinionSource*)const{assert(this==activeManager&&&t==activeCreature);++attitudeCalls;return selectedAttitude;}
EOpinionAttitudeType COpinionReactionManager::GetBlurredAttitude(EOpinionAttitudeType a)const{assert(this==activeManager);++blurCalls;return a;}
float COpinionReactionManager::GetThingOpinionOfPlayer(const CThing&t,EOpinion op)const{assert(this==activeManager&&&t==activeCreature&&op==OPINION_RENOWN);++opinionCalls;return opinionValue;}
float FABLE_FASTCALL GFFloatRandom(float range,unsigned long*p){assert(range==1.0f&&p==&seed);++randomCalls;return randomValue;}
unsigned long* CTCBase::GetPWorldSeed()const{return &seed;}
CWorld& CTCBase::GetWorld()const{return *activeWorld;}
const CWorld& CTCBase::PeekWorld()const{return *activeWorld;}
const CPlayer* CWorld::PeekMyPlayer()const{assert(this==activeWorld);return activePlayer;}
const CThingPlayerCreature* CPlayer::PeekPControlledCreature()const{assert(this==activePlayer);return activeHero;}
unsigned long CThingCreatureBase::GetProperty()const{assert(this==activeCreature);return propertyValue;}
bool CTCWife::IsMarriedToThePlayer()const{++wifeCalls;return married;}
bool CTCWife::GetGreetDialogueGroupName(CCharString&s)const{if(wifeHasText)s="SPOUSE_GROUP";return wifeHasText;}
CWideString::CWideString():PStringData(0){}CWideString::~CWideString(){assert(!PStringData);}
CInitBaseClass::CInitBaseClass():Valid(false){}CInitBaseClass::~CInitBaseClass(){}
CBaseObject::CBaseObject():IntelligentPointer(0){}CBaseObject::~CBaseObject(){}
CBaseClass::CBaseClass(){}CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){}CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CGame::CGame():CurrentGameComponent(0),Quit(false){}CGame::~CGame(){}
CGameComponent::CGameComponent(CGame&g):Quit(false),Running(false),Game(g){}CGameComponent::~CGameComponent(){}
void CGameComponent::Init(){}void CGameComponent::SetQuit(){Quit=true;}
CMainGameComponent::CMainGameComponent(CGame&g,const CMainGameComponentInit&):CGameComponent(g){}CMainGameComponent::~CMainGameComponent(){}
CMainGameComponent* CMainGameComponent::PMainGameComponent=0;
CMainGameComponentInit::CMainGameComponentInit(){}CMainGameComponentInit::~CMainGameComponentInit(){}
CWorld::~CWorld(){}CIDrawWorld::~CIDrawWorld(){}
CThingPlayerCreature::CThingPlayerCreature(){}
CTCBase::CTCBase(CThing&h):ParentThing(&h),ToBeRemoved(false),IsInGlobalUpdateTCs(false){}CTCBase::~CTCBase(){}
CTCHeroStats::CTCHeroStats(CThing&t):CTCBase(t),TrainingStatus(TRAINING_STATUS_GRADUATED){}
CTCHero::CTCHero(CThing&t):CTCBase(t){}void CTCHero::FrameUpdate(){abort();}
CTCOpinionSource::CTCOpinionSource(CThing&t):CTCBase(t){}
CTCDrunkenness::CTCDrunkenness(CThing&t):CTCBase(t),Drunk(false),Drunkenness(0),FrameLastUpdatedSobering(0),SecondsPerFrame(0.25f),ThisDrunkennessThreshold(0){}
CTCWife::CTCWife(CThing&t):CTCBase(t){}CTCWife::~CTCWife(){}
CTCCreatureOpinionOfHero::CTCCreatureOpinionOfHero(CThing&t):CTCBase(t),GreetedFlag(false){}
void CTCCreatureOpinionOfHero::OnSerialise(CPersistContext&,bool){abort();}void CTCCreatureOpinionOfHero::OnInitialActivate(){abort();}bool CTCCreatureOpinionOfHero::HasFrameUpdate()const{abort();return false;}void CTCCreatureOpinionOfHero::FrameUpdate(){abort();}void CTCCreatureOpinionOfHero::OnKill(){abort();}ETCInterfaceType CTCCreatureOpinionOfHero::GetInterfaceType()const{return TCI_CREATURE_OPINION_OF_HERO;}CCharString CTCCreatureOpinionOfHero::GetClassTypeName()const{abort();return CCharString();}bool CTCCreatureOpinionOfHero::IsComponentSerialisable()const{abort();return false;}
void CTCWife::OnSerialise(CPersistContext&,bool){abort();}void CTCWife::OnCreate(){abort();}void CTCWife::OnInitialActivate(){abort();}bool CTCWife::HasFrameUpdate()const{abort();return false;}void CTCWife::FrameUpdate(){abort();}bool CTCWife::IsSerialiseCreatable()const{abort();return false;}ETCInterfaceType CTCWife::GetInterfaceType()const{return TCI_WIFE;}CCharString CTCWife::GetClassTypeName()const{abort();return CCharString();}bool CTCWife::IsComponentSerialisable()const{abort();return false;}void CTCWife::GlobalUpdate(bool){abort();}
CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&){abort();return 0;}
const CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&)const{abort();return 0;}
CResource::CResource():ResourceList(0),PrevResource(0),NextResource(0),ResourceSize(0),LastUsedFrame(0){IVRefCount=1;}CResource::~CResource(){}
CDefClassBase::CDefClassBase():PDefManager(0),GlobalIndex(0),Setup(false),Template(false),DefaultValsApplied(false){}CSubDefClassBase::CSubDefClassBase(){}
void CIVCountedPointeeBase::Delete()const{abort();}
CBaseIntelligentPointer::CBaseIntelligentPointer():PData(0){}CBaseIntelligentPointer::~CBaseIntelligentPointer(){assert(!PData);}
// Concrete test subclasses only implement otherwise-unreached pure virtuals.
template<class T>class FixtureComponent:public T {public:FixtureComponent(CThing&t):T(t){}ETCInterfaceType GetInterfaceType()const{return T::StatGetInterfaceType();}CCharString GetClassTypeName()const{abort();return CCharString();}};
class FixtureTitle:public CHeroTitleDef {public:void Transfer(CPersistContext&){abort();}void Copy(const CDefClassBase*){abort();}unsigned long GetSizeofClass(){abort();return 0;}};
typedef CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> > FixtureMap;
static void fixtureSet(CThing&t,unsigned n,bool value){unsigned long mask=t.UsingComponents._S_maskbit(n);if(value)t.UsingComponents._M_w[t.UsingComponents._S_whichword(n)]|=mask;else t.UsingComponents._M_w[t.UsingComponents._S_whichword(n)]&=~mask;}
static void initMap(CThing&t){memset(t.UsingComponents._M_w,0,sizeof t.UsingComponents._M_w);t.Components._M_start=t.Components._M_end_of_storage.allocate(8);t.Components._M_finish=t.Components._M_start;t.Components._M_end_of_storage._M_data=t.Components._M_start+8;}
static void addComponent(CThing&t,ETCInterfaceType type,CTCBase&value){FixtureMap::value_type*p=t.Components._M_finish++;std::_Construct(p,FixtureMap::value_type(type,&value));fixtureSet(t,type,true);}
int main(){
 CGame game;CMainGameComponentInit init;CMainGameComponent mainComponent(game,init);CWorld world;CPlayer player;CThingPlayerCreature hero;CThingCreatureBase creature;NGameText::CDataBank bank;COpinionReactionManager manager;
 activeWorld=&world;activePlayer=&player;activeHero=&hero;activeCreature=&creature;activeBank=&bank;activeManager=&manager;CMainGameComponent::PMainGameComponent=&mainComponent;NGameText::CDataBank*bankptr=&bank;memcpy(mainComponent.PTextBank,&bankptr,sizeof bankptr);world.POpinionReactionManager_Data=&manager;
 FixtureComponent<CTCHeroStats> stats(hero);FixtureComponent<CTCHero> title(hero);FixtureComponent<CTCOpinionSource> source(hero);FixtureComponent<CTCDrunkenness> drunk(creature);CTCWife wife(creature);FixtureTitle titleDef;CTCCreatureOpinionOfHero greeting(creature);
 initMap(hero);initMap(creature);addComponent(hero,TCI_HERO_STATS,stats);addComponent(hero,TCI_HERO,title);addComponent(hero,TCI_OPINION_SOURCE,source);addComponent(creature,TCI_DRUNKENNESS,drunk);addComponent(creature,TCI_WIFE,wife);
 const unsigned baseStrings=stringLive;unsigned cases=0;
 const char*suffix[]={"","RESPECT","AWE","DISDAIN","FEAR","FRIENDLINESS","WORSHIP","RIDICULOUS","OFFENSIVE","AGREEABLE","UGLY","ATTRACTED","LOVE","WIFE_LOVE","WIFE_LIKE","WIFE_NEUTRAL","WIFE_DISLIKE","WIFE_HATE"};
 for(unsigned scenario=0;scenario<16;++scenario)for(unsigned stage=0;stage<3;++stage)for(unsigned greet=0;greet<2;++greet)for(unsigned a=0;a<19;++a){
  activeHero=&hero;creature.ThingTypeFlags=8;propertyValue=scenario==0?3:0;greeting.GreetedFlag=greet!=0;stats.TrainingStatus=(EHeroTrainingStatus)stage;drunk.Drunk=scenario==1||scenario==11;married=scenario==2||scenario==3;wifeHasText=scenario==2;bankMissing=scenario==8;randomValue=(scenario==4||scenario==6||scenario==7||scenario==9||scenario==12||scenario==14)?0.0f:1.0f;opinionValue=scenario==12?0.0f:1.0f;selectedAttitude=OPINION_ATTITUDE_TYPE_FRIENDLINESS;
  fixtureSet(hero,TCI_HERO_STATS,scenario!=10&&scenario!=15);fixtureSet(hero,TCI_HERO,scenario!=6);fixtureSet(hero,TCI_OPINION_SOURCE,scenario==5);fixtureSet(creature,TCI_DRUNKENNESS,scenario!=11);fixtureSet(creature,TCI_WIFE,scenario!=13);
  title.PHeroTitleSubDef.Object=scenario==7?0:&titleDef;titleDef.IVRefCount=2;titleDef.HeroTitleGreetToHeroTag=scenario==9?-7:(scenario==14?0:700);
  if(scenario==10)activeHero=0;
  const unsigned effectiveA=a==18?5:a;const unsigned effectiveStage=(scenario==10||scenario==15)?0:stage;
  char expected[256];expected[0]=0;unsigned long expectedValue=0;
  if(propertyValue==3){strcpy(expected,greet?"TEXT_AI_GUARD_REGREET_TO_HERO":"TEXT_AI_GUARD_GREET_TO_HERO");if(effectiveStage)strcat(expected,effectiveStage==2?"_ASBOY":"_ASAPPRENTICE");}
  else if(drunk.Drunk&&scenario!=11){strcpy(expected,"TEXT_AI_OPINION_ACT_GREET_TO_HERO_DRUNK");}
  else if((effectiveA==3||effectiveA==4||effectiveA==8)&&married&&wifeHasText){strcpy(expected,"SPOUSE_GROUP");}
  else if(scenario==4&&effectiveA!=3&&effectiveA!=4&&effectiveA!=8){expectedValue=700;}
  else if(effectiveStage){strcpy(expected,greet?"TEXT_AI_OPINION_ACT_REGREET_TO_HERO":"TEXT_AI_OPINION_ACT_GREET_TO_HERO");strcat(expected,effectiveStage==2?"_ASBOY":"_ASAPPRENTICE");}
  else if(*suffix[effectiveA]){strcpy(expected,greet?"TEXT_AI_OPINION_ACT_REGREET_TO_HERO_":"TEXT_AI_OPINION_ACT_GREET_TO_HERO_");strcat(expected,suffix[effectiveA]);}
  if(*expected)expectedValue=bankMissing?0:hashText(expected);
  crcCalls=lookupCalls=randomCalls=attitudeCalls=blurCalls=opinionCalls=wifeCalls=0;lastText[0]=0;
  unsigned long got=greeting.GetGreetDialogueGroupIndex((EOpinionAttitudeType)a);
  if(got!=expectedValue){fprintf(stderr,"case %u %u %u %u got%lu expected%lu text%s\n",scenario,stage,greet,a,got,expectedValue,lastText);abort();}
  assert(lookupCalls==(*expected?1U:0U)&&crcCalls==lookupCalls);if(*expected)assert(strcmp(lastText,expected)==0);assert(stringLive==baseStrings);
  if(propertyValue==3||(drunk.Drunk&&scenario!=11))assert(randomCalls==0&&attitudeCalls==0&&opinionCalls==0);
  else{assert(attitudeCalls==(a==18?1U:0U));assert(blurCalls==(a==18&&scenario!=5?1U:0U));assert(randomCalls==((effectiveA==3||effectiveA==4||effectiveA==8||scenario==5)?0U:1U));}
  ++cases;
 }
 creature.ThingTypeFlags=0;activeHero=0;lookupCalls=0;assert(greeting.GetGreetDialogueGroupIndex(OPINION_ATTITUDE_TYPE_LAST)==0&&lookupCalls==0);++cases;
 title.PHeroTitleSubDef.Object=0;assert(cases==1825);assert(stringLive==baseStrings);puts("00683c10_TEST PASS");return 0;
}