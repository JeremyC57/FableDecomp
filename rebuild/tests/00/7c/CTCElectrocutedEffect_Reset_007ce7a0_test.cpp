// Self-contained full-body test: real shared class models and controlled external operations.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __int64 long long
#define __declspec(x)
#define FABLE_PORTABLE
#include <strings.h>
#define _stricmp strcasecmp
enum ESex{};enum EAIStateGroupType{};enum EScriptAIPriority{};enum EReactionSpeechType{FixtureReactionZero};enum ESaveGameLoadStatus{};enum ERegionLoadStatus{};enum EPlayerSpawnStatus{};enum ESaveType{};enum EEnginePrimitiveStatIndex{};
#endif
#include "engine/EPlayerMode.h"
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_STLP_HOST_TAGS_PASS23
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#define FABLE_RETAIL_DEF_CLASS_BASE_CTOR_DECL
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR
#define FABLE_RETAIL_CDEFPOINTER_POINTER_CTOR_ADOPT
#define FABLE_RETAIL_CDEFPOINTER_RESET
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CTCBOASTINGCROWDCONTROL
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CGAME_CORE_PASS23
#define FABLE_RETAIL_CINITBASECLASS
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define private public
#define protected public

#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#include "engine/EMessageEventType.h"
#define FABLE_RETAIL_ELECTROCUTED_LIFETIMES_PASS23
#include "engine/fable_ai_action_enums.h"
// 007ce7a0 CTCElectrocutedEffect::Reset
// Flags: none
// Leaf: ?Reset@CTCElectrocutedEffect@@QAEXXZ
#define FABLE_RETAIL_ELECTROCUTED_ORIGINAL_PASS23
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CMainGameComponent.h"
#include "engine/CTCElectrocutedEffect.h"
#include "engine/_quarantine/CSpecialAbilitiesElectrocutedEffectDef.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/ThingAlive.h"
void CTCElectrocutedEffect::Reset()
{
 CThing& parent=GetParentThing();
 parent.SetAsRenderedThisFrame(1);
 parent.SetAsFrameUpdated();
 CurTime=0;
 FadeInTime=(long)(PDef->FadeInTimeSecs*CMainGameComponent::GetConstantFPS());
 FadeOutTimeStart=(long)(PDef->FadeOutTimeStartSecs*CMainGameComponent::GetConstantFPS());
 FadeOutTime=(long)(PDef->FadeOutTimeSecs*CMainGameComponent::GetConstantFPS());
 CDefPointer<const CSpecialEffectsDef> pspecial_effects;
 long effect=-1;
 if(!IS_THING_ALIVE(PEffect)) {
  bool use_dummy_center=false;
  C3DVector pos=parent.GetPos();
  CCharString effect_name("FRAZZLED_EFFECT_MEDIUM");
  C3DVector bottompos=pos;
  C3DVector toppos=bottompos+C3DVector(0.0f,0.0f,PDef->HeightMedium);
  const NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance=parent.GetTCGraphicAppearance();
  if(ptc_graphic_appearance) {
   toppos=ptc_graphic_appearance->GetFocalPos(1.0f);
   CRightHandedSet rhs;
   use_dummy_center=ptc_graphic_appearance->GetDummyObject("DUMMY_CENTRE",0,0,pos,rhs,1.0f);
  }
  CDefPointer<const CSpecialEffectsDef> pcreature_special_fx;
  GetParentThing().GetPBaseDef()->GetSubDef(&pcreature_special_fx);
  if(!(pcreature_special_fx.Get()&&pcreature_special_fx->GetEffectFromString("ELECTROCUTED_EFFECT",effect))) {
   float height=toppos.Z-bottompos.Z;
   if(use_dummy_center)effect_name="FRAZZLED_EFFECT_POINT";
   else if(height<PDef->HeightSmall)effect_name="FRAZZLED_EFFECT_SMALL";
   else if(height>PDef->HeightMedium)effect_name="FRAZZLED_EFFECT_LARGE";
   if(PDef->GetSubDef(&pspecial_effects))pspecial_effects->GetEffectFromString(effect_name,effect);
  }
  if(effect!=-1) {
   PEffect=CIntelligentPointer<CThing>(CTCDParticleEmitter::Create(effect,pos,false));
   if(IS_THING_ALIVE(PEffect)) {
    CTCDParticleEmitter* ptc_emitter=0;
    PEffect->GetTC(&ptc_emitter);
    if(ptc_emitter) {
     if(use_dummy_center)ptc_emitter->AttachToThing(parent,10,"DUMMY_CENTRE",0,0.0f);
     else ptc_emitter->AttachToThing(parent,8,"",0,0.0f);
    }
   }
  }
 }
}

#include "engine/CDefString.h"
#include "engine/CInitBaseClass.h"

struct FixtureToken{CBaseObject*Owner;explicit FixtureToken(CBaseObject*p):Owner(p){}};
static int tokens,strings,rendered,updated,focalCalls,dummyCalls,baseLookups,defaultLookups,baseEffects,defaultEffects,createCalls,attachCalls,deletions;
static int baseMode,defaultMode,graphicMode,createMode;static CThing*parentThing,*createdThing;static CTCDParticleEmitter*emitter;
static CThingBaseDef*baseDef;static CSpecialAbilitiesElectrocutedEffectDef*effectDef;static CSpecialEffectsDef*baseSub,*defaultSub;static CDefinitionManager*manager;static C3DVector parentPos,dummyPos;static float bodyHeight;static const char*expectedEffect;
static CDefClassBase::CSubDefInfo baseInfo,defaultInfo;
CBaseClass::CBaseClass(){}CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){}CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CInitBaseClass::CInitBaseClass():Valid(false){}CInitBaseClass::~CInitBaseClass(){}
CBaseObject::CBaseObject():IntelligentPointer(0){}CBaseObject::~CBaseObject(){}
CBaseIntelligentPointer::CBaseIntelligentPointer():PData(0){}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){SetPItem(0);}
CBaseObject*CBaseIntelligentPointer::GetPItem()const{return PData?reinterpret_cast<FixtureToken*>(PData)->Owner:0;}
void CBaseIntelligentPointer::SetPItem(CBaseObject*p){if(PData){delete reinterpret_cast<FixtureToken*>(PData);PData=0;--tokens;}if(p){PData=reinterpret_cast<CBaseObjectPointer*>(new FixtureToken(p));++tokens;}}
CCharString::CCharString():PStringData(0){}
CCharString::CCharString(const char*s,long n){assert(n==-1);PStringData=new CCharStringData;PStringData->Data.PData=const_cast<char*>(s);PStringData->Data.StringLength=(unsigned long)strlen(s);PStringData->NoRefs=1;++strings;}
CCharString::CCharString(const CCharString&s):PStringData(s.PStringData){if(PStringData){++PStringData->NoRefs;++strings;}}
CCharString::~CCharString(){if(PStringData){--strings;assert(strings>=0);if(--PStringData->NoRefs==0)delete PStringData;}}
const CCharString&CCharString::operator=(const char*s){if(PStringData){--strings;if(--PStringData->NoRefs==0)delete PStringData;}PStringData=new CCharStringData;PStringData->Data.PData=const_cast<char*>(s);PStringData->Data.StringLength=(unsigned long)strlen(s);PStringData->NoRefs=1;++strings;return*this;}
const char CCharString::NullChar=0;
CWideString::CWideString():PStringData(0){}CWideString::~CWideString(){assert(!PStringData);}
CTCBase::CTCBase(CThing&t):ParentThing(&t){}CTCBase::~CTCBase(){}
CTCElectrocutedEffect::CTCElectrocutedEffect(CThing&t):CTCBase(t){}CTCElectrocutedEffect::~CTCElectrocutedEffect(){}
CTCDriverBase::CTCDriverBase(CThing&t):CTCBase(t){}CTCDriverBase::~CTCDriverBase(){}
CTCDParticleEmitter::CTCDParticleEmitter(CThing&t):CTCDriverBase(t){}CTCDParticleEmitter::~CTCDParticleEmitter(){}
NGraphicAppearance::CTCGraphicAppearance::CTCGraphicAppearance(CThing&t):CTCBase(t){}NGraphicAppearance::CTCGraphicAppearance::~CTCGraphicAppearance(){}
CResource::CResource():ResourceList(0),PrevResource(0),NextResource(0),ResourceSize(0),LastUsedFrame(0){IVRefCount=1;}CResource::~CResource(){}
CDefClassBase::CDefClassBase():PDefManager(0),GlobalIndex(0),Setup(false),Template(false),DefaultValsApplied(false){}
CParentDefClassBase::CParentDefClassBase(){}CParentDefClassBase::~CParentDefClassBase(){}
CSubDefClassBase::CSubDefClassBase(){}
CSpecialAbilitiesElectrocutedEffectDef::CSpecialAbilitiesElectrocutedEffectDef(){}CSpecialAbilitiesElectrocutedEffectDef::~CSpecialAbilitiesElectrocutedEffectDef(){}
CDefinitionManager::~CDefinitionManager(){}CFailedAllocationHandler::~CFailedAllocationHandler(){}CResourceList::~CResourceList(){}
void CIVCountedPointeeBase::Delete()const{assert(this==baseSub||this==defaultSub);++deletions;}
long CMainGameComponent::ConstantFPS=4;
void CThing::SetAsRenderedThisFrame(unsigned long n){assert(this==parentThing&&n==1&&updated==0);++rendered;}
void CThing::SetAsFrameUpdated(){assert(this==parentThing&&rendered==1);++updated;}
const C3DVector&CThing::GetPos()const{assert(this==parentThing);return parentPos;}
C3DVector NGraphicAppearance::CTCGraphicAppearance::GetFocalPos(float f)const{assert(f==1&&graphicMode);++focalCalls;return C3DVector(33,44,parentPos.Z+bodyHeight);}
bool NGraphicAppearance::CTCGraphicAppearance::GetDummyObject(const CCharString&name,long i,unsigned long flags,C3DVector&pos,CRightHandedSet&,float f)const{assert(graphicMode&&strcmp(name.GetAsCharArray(),"DUMMY_CENTRE")==0&&i==0&&flags==0&&f==1);++dummyCalls;if(graphicMode==2){pos=dummyPos;return true;}return false;}
const CDefClassBase::CSubDefInfo*CParentDefClassBase::GetPSubDefInfo(const CCharString&name)const{assert(strcmp(name.GetAsCharArray(),"CSpecialEffectsDef")==0);if(this==baseDef){++baseLookups;return baseMode?&baseInfo:0;}assert(this==effectDef);++defaultLookups;return defaultMode?&defaultInfo:0;}
CDefPointer<CDefClassBase>CDefinitionManager::GetPDefFromGlobalIndex(long index){assert(this==manager&&(index==1||index==2));CDefPointer<CDefClassBase>r;r.Object=index==1?baseSub:defaultSub;r.Object->AddRef();return r;}
bool CSpecialEffectsDef::GetEffectFromString(const CCharString&name,long&effect)const{if(this==baseSub){++baseEffects;assert(strcmp(name.GetAsCharArray(),"ELECTROCUTED_EFFECT")==0);if(baseMode==2){effect=71;return true;}return false;}assert(this==defaultSub);++defaultEffects;assert(strcmp(name.GetAsCharArray(),expectedEffect)==0);if(defaultMode==2){effect=83;return true;}return false;}
CThing*FABLE_FASTCALL CTCDParticleEmitter::Create(long id,const C3DVector&pos,bool flag){assert(id==(baseMode==2?71:83)&&!flag);++createCalls;const C3DVector&want=graphicMode==2?dummyPos:parentPos;assert(pos.X==want.X&&pos.Y==want.Y&&pos.Z==want.Z);return createMode?createdThing:0;}
void CTCDParticleEmitter::AttachToThing(CThing&parent,long mode,const CCharString&name,long index,float offset){assert(this==emitter&&&parent==parentThing&&mode==(graphicMode==2?10:8)&&index==0&&offset==0);assert(strcmp(name.GetAsCharArray(),graphicMode==2?"DUMMY_CENTRE":"")==0);++attachCalls;}
class TestEffect:public CTCElectrocutedEffect{public:TestEffect(CThing&t):CTCElectrocutedEffect(t){}ETCInterfaceType GetInterfaceType() const{abort();}CCharString GetClassTypeName() const{abort();}};
class TestEmitter:public CTCDParticleEmitter{public:TestEmitter(CThing&t):CTCDParticleEmitter(t){}ETCInterfaceType GetInterfaceType() const{abort();}CCharString GetClassTypeName() const{abort();}};
class TestGraphic:public NGraphicAppearance::CTCGraphicAppearance{public:TestGraphic(CThing&t):CTCGraphicAppearance(t){}ETCInterfaceType GetInterfaceType() const{abort();}CCharString GetClassTypeName() const{abort();}};
class TestBaseDef:public CThingBaseDef{public:void Transfer(CPersistContext&) {abort();}void Copy(const CDefClassBase*) {abort();}unsigned long GetSizeofClass() {abort();}};
class TestEffectDef:public CSpecialAbilitiesElectrocutedEffectDef{public:void Transfer(CPersistContext&) {abort();}void Copy(const CDefClassBase*) {abort();}unsigned long GetSizeofClass() {abort();}};
class TestSub:public CSpecialEffectsDef{public:void Transfer(CPersistContext&) {abort();}void Copy(const CDefClassBase*) {abort();}unsigned long GetSizeofClass() {abort();}};

void CBaseObject::Validate() const { abort(); }
void CClass::Save(CCharString&) { abort(); }
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
bool CResource::UnloadResource() { abort(); }
unsigned long CResource::GetUnloadableMemory() const { abort(); }
bool CResource::IsResourceLoaded() const { abort(); }
void CResource::OnResourceLoaded() { abort(); }
bool CResource::CanAbortResourceLoad(long) const { abort(); }
void CResource::OnResourceLoadAborted() { abort(); }
unsigned long CResource::GetDebugMemoryInUse() const { abort(); }
void CResource::ResetDebugMemoryInUse() { abort(); }
bool CResourceList::HandleFailedAllocation(unsigned long) { abort(); }
bool CDefClassBase::UnloadResource() { abort(); }
CDefString CDefClassBase::GetInstantiationName() const { abort(); }
void CDefClassBase::SetInstantiationName(CDefString) { abort(); }
void CDefClassBase::AddSubDef(const CCharString&, long, long) { abort(); }
void CDefClassBase::RemoveSubDef(const CCharString&) { abort(); }
CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&) { abort(); }
const CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&) const { abort(); }
bool CDefClassBase::IsParentDef() const { abort(); }
void CDefClassBase::PostTransfer() { abort(); }
void CDefClassBase::InternalSaveBinary(CDataOutputStream&) { abort(); }
void CDefClassBase::InternalLoadBinary(CDataInputStream&) { abort(); }
void CDefClassBase::Validate(const CValidateDefinitionInitBase&, const CCharString&) const { abort(); }
CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> >* CDefClassBase::GetPSubDefInfoMap() { abort(); }
CDefString CParentDefClassBase::GetInstantiationName() const { abort(); }
void CParentDefClassBase::SetInstantiationName(CDefString) { abort(); }
void CParentDefClassBase::AddSubDef(const CCharString&, long, long) { abort(); }
void CParentDefClassBase::RemoveSubDef(const CCharString&) { abort(); }
CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString&) { abort(); }
bool CParentDefClassBase::IsParentDef() const { abort(); }
void CParentDefClassBase::InternalSaveBinary(CDataOutputStream&) { abort(); }
void CParentDefClassBase::InternalLoadBinary(CDataInputStream&) { abort(); }
CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> >* CParentDefClassBase::GetPSubDefInfoMap() { abort(); }
bool CTCDriverBase::IsDriver()const { abort(); }
bool CTCDriverBase::IsDriverType(long)const { abort(); }
void CTCDriverBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&) { abort(); }
void CTCDriverBase::CreateAdditionalTCs() { abort(); }
ETCInterfaceType CTCDParticleEmitter::GetInterfaceType() const { abort(); }
CCharString CTCDParticleEmitter::GetClassTypeName() const { abort(); }
bool CTCDParticleEmitter::IsDriverType(long p0) const { abort(); }
void CTCDParticleEmitter::FrameUpdate() { abort(); }
bool CTCDParticleEmitter::HasFrameUpdate() const { abort(); }
void CTCDParticleEmitter::OnCreate() { abort(); }
void CTCDParticleEmitter::OnKill() { abort(); }
void CTCDParticleEmitter::OnDie() { abort(); }
void CTCDParticleEmitter::CreateAdditionalTCs() { abort(); }
void CTCDParticleEmitter::OnSerialise(CPersistContext & p0, bool p1) { abort(); }
bool CTCDParticleEmitter::IsComponentSerialisable() const { abort(); }
void CTCDParticleEmitter::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo & p0) { abort(); }
void CTCDParticleEmitter::OnSetInLimbo(bool p0) { abort(); }
ETCInterfaceType CTCElectrocutedEffect::GetInterfaceType()const { abort(); }
CCharString CTCElectrocutedEffect::GetClassTypeName()const { abort(); }
bool CTCElectrocutedEffect::HasFrameUpdate()const { abort(); }
void CTCElectrocutedEffect::FrameUpdate() { abort(); }
void CTCElectrocutedEffect::OnKill() { abort(); }
void CTCElectrocutedEffect::OnRemove() { abort(); }
void CTCElectrocutedEffect::OnInitialActivate() { abort(); }
void CSpecialAbilitiesElectrocutedEffectDef::Copy(const CDefClassBase*) { abort(); }
unsigned long CSpecialAbilitiesElectrocutedEffectDef::GetSizeofClass() { abort(); }
void CSpecialAbilitiesElectrocutedEffectDef::Transfer(CPersistContext&) { abort(); }

int main(){unsigned cases=0;
for(int alive=0;alive<3;++alive)for(int graphic=0;graphic<3;++graphic)for(int height=0;height<5;++height)for(int bm=0;bm<3;++bm)for(int dm=0;dm<3;++dm)for(int cm=0;cm<5;++cm)for(int last=0;last<2;++last){
 {tokens=strings=rendered=updated=focalCalls=dummyCalls=baseLookups=defaultLookups=baseEffects=defaultEffects=createCalls=attachCalls=deletions=0;baseMode=bm;defaultMode=dm;graphicMode=graphic;createMode=cm;
  CThing parent,oldEffect,newEffect;TestEffect object(parent);TestEmitter particle(newEffect);TestGraphic appearance(parent);TestBaseDef base;TestEffectDef def;TestSub creatureSub,generalSub;CDefinitionManager definitions;
  parentThing=&parent;createdThing=&newEffect;emitter=&particle;baseDef=&base;effectDef=&def;baseSub=&creatureSub;defaultSub=&generalSub;manager=&definitions;
  parent.PTCGraphicAppearance=graphic?&appearance:0;parent.PBaseDef.Object=&base;base.AddRef();base.PDefManager=&definitions;def.PDefManager=&definitions;baseInfo.DefIndex=1;defaultInfo.DefIndex=2;
  object.PDef.Object=&def;def.AddRef();object.CurTime=99;def.FadeInTimeSecs=0.5f;def.FadeOutTimeStartSecs=2.0f;def.FadeOutTimeSecs=1.5f;def.HeightSmall=4.0f;def.HeightMedium=8.0f;bodyHeight=(float)(height*4);parentPos=C3DVector(8,16,32);dummyPos=C3DVector(64,128,256);
  creatureSub.IVRefCount=generalSub.IVRefCount=last?0:1;oldEffect.Dead=alive==2;newEffect.Dead=cm==1;if(alive)object.PEffect.Set(&oldEffect);
  memset(&newEffect.UsingComponents,0,sizeof(newEffect.UsingComponents));newEffect.Components.Dirty=false;
  if(cm>=3){unsigned n=(unsigned)TCI_PARTICLE_EMITTER;newEffect.UsingComponents._M_w[n/(8*sizeof(unsigned long))]|=1UL<<(n%(8*sizeof(unsigned long)));}
  typedef std::pair<ETCInterfaceType,CTCBase*> Entry;Entry entries[1];entries[0].first=TCI_PARTICLE_EMITTER;entries[0].second=cm==3?&particle:0;
  newEffect.Components._M_start=entries;newEffect.Components._M_finish=entries+1;newEffect.Components._M_end_of_storage._M_data=entries+1;
  expectedEffect=graphic==2?"FRAZZLED_EFFECT_POINT":graphic&&bodyHeight<4?"FRAZZLED_EFFECT_SMALL":graphic&&bodyHeight>8?"FRAZZLED_EFFECT_LARGE":"FRAZZLED_EFFECT_MEDIUM";
  object.Reset();assert(rendered==1&&updated==1&&object.CurTime==0&&object.FadeInTime==2&&object.FadeOutTimeStart==8&&object.FadeOutTime==6&&strings==0);
  if(alive==1){assert(!baseLookups&&!defaultLookups&&!createCalls&&!attachCalls&&!dummyCalls&&object.PEffect.Get()==&oldEffect);}
  else{assert(baseLookups==1&&baseEffects==(bm!=0)&&focalCalls==(graphic!=0)&&dummyCalls==(graphic!=0));assert(defaultLookups==(bm!=2)&&defaultEffects==(bm!=2&&dm!=0));bool found=bm==2||dm==2;assert(createCalls==(int)found&&attachCalls==(int)(found&&cm==3));assert(object.PEffect.Get()==(found?(cm?&newEffect:0):(alive?&oldEffect:0)));}
  assert(base.IVRefCount==2&&def.IVRefCount==2&&creatureSub.IVRefCount==(last?0:1)&&generalSub.IVRefCount==(last?0:1)&&deletions==(last&&alive!=1?((bm!=0)+(bm!=2&&dm!=0)):0));
  newEffect.Components._M_start=newEffect.Components._M_finish=newEffect.Components._M_end_of_storage._M_data=0;object.PEffect.Set(0);object.PDef.Object=0;def.Release();parent.PBaseDef.Object=0;base.Release();++cases;
 }
 assert(tokens==0&&strings==0);
}
assert(cases==4050);puts("007ce7a0_TEST PASS");return 0;}