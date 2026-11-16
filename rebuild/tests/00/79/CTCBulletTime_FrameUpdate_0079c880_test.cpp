// Self-contained fixture: real shared classes, production function body, normal lifetimes and recording callee doubles.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
enum ESex {};enum EPlayerMode {};enum ESaveGameLoadStatus {};enum ERegionLoadStatus {};enum EPlayerSpawnStatus {};enum ESaveType {};
#endif
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_STLP_HOST_TAGS_PASS23
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE_CTOR_DECL
#define private public
#define protected public
// 0x0079C880 CTCBulletTime::FrameUpdate() -- refresh screen emitters while active, remove expired bullet time, and track/fade the hit-target blur.
// Leaf: ?FrameUpdate@CTCBulletTime@@UAEXXZ
// Retail: 0x0079C880, 933 bytes, thiscall.
// Flags: none
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_CPARTICLEEMITTER_KILL
#define FABLE_RETAIL_CPARTICLEEMITTER_INTERPOLATE
#define FABLE_RETAIL_CPARTICLEEMITTERMANAGER_CREATE2D
#define FABLE_RETAIL_CPARTICLEEMITTERMANAGER_BULLETTIME_PASS23
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_DEFAULT_IDENTITY
#define FABLE_RETAIL_CTCBULLETTIME
#define FABLE_RETAIL_CTCBULLETTIME_MEMBERS
#define FABLE_RETAIL_CTCBULLETTIME_PLAYEFFECT
#define FABLE_RETAIL_CTCBULLETTIME_FRAME_PASS23
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_SPELLEFFECTINTERPOLATOR
#define FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERPOLATOR_BULLETTIME_PASS23
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CSpecialAbilitiesBulletTimeDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CParticleEmitter.h"
#include "engine/CParticleEmitterManager.h"
#include "engine/CWorld.h"
#include "engine/CEnvironment.h"
#include "engine/CEnvironmentSpellEffectInterpolator.h"
#include "engine/CTCBulletTime.h"

float FABLE_FASTCALL GFVXToAX(float);
float FABLE_FASTCALL GFVYToAY(float);
extern double VIRTUAL_SCREEN_WIDTH,VIRTUAL_SCREEN_HEIGHT;


#include "engine/CCamera.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CDefString.h"
template<> bool CDefClassBase::GetSubDef<CSpecialEffectsDef>(CDefPointer<const CSpecialEffectsDef>*)const;
void CTCBulletTime::FrameUpdate()
{
    CThingCreatureBase* pcreature = GFDebugPDynamicCast(&GetParentThing(),(CThingCreatureBase*)0);
    if (!Paused) {
        --BulletTimeRemainingTicks;
        if (!m_tpLoopingEffect.Get() || m_tpLoopingEffect.Get()->IsDead()) {
            CDefPointer<const CSpecialEffectsDef> pspecial_effects;
            long effect = 0;
            if (PDef->GetSubDef(&pspecial_effects) && pspecial_effects->GetEffectFromString("SLOWTIME", effect)) {
                m_tpLoopingEffect = NParticleEngine::GetParticleEmitterManager().Create2DEmitter(effect,
                    C2DVector(GFVXToAX((float)VIRTUAL_SCREEN_WIDTH) * 0.5f, GFVYToAY((float)VIRTUAL_SCREEN_HEIGHT) * 0.5f), (EEngine2DLayer)0x52, true,
                    CRightHandedSet());
                if (m_tpLoopingEffect.Get() && !m_tpLoopingEffect.Get()->IsDead())
                    m_tpLoopingEffect.Get()->SetAlwaysUpdate(true);
            }
        }
        if (BulletTimeRemainingTicks == 0) {
            CDefPointer<const CSpecialEffectsDef> pspecial_effects;
            long effect = 0;
            if (PDef->GetSubDef(&pspecial_effects) && pspecial_effects->GetEffectFromString("END", effect)) {
                NParticleEngine::CParticleEmitter* pemitter = NParticleEngine::GetParticleEmitterManager().Create2DEmitter(effect,
                    C2DVector(GFVXToAX((float)VIRTUAL_SCREEN_WIDTH) * 0.5f, GFVYToAY((float)VIRTUAL_SCREEN_HEIGHT) * 0.5f), (EEngine2DLayer)0x52, true,
                    CRightHandedSet());
                if (pemitter && !pemitter->IsDead())
                    pemitter->SetAlwaysUpdate(true);
            }
            Pausable = false;
            pcreature->RemoveTC(TCI_BULLET_TIME);
        }
    }
    if (HitCreatureBlur.Get() && !HitCreatureBlur.Get()->IsDead()) {
        CEnvironmentSpellEffectInterpolator& sei = GetWorld().GetEnvironment().GetSpellEffectInterpolator();
        sei.RadialBlurSetCenterWorldPos(RadialBlurKey, HitCreatureBlur.Get()->GetPos());
        if (sei.GetRadialBlurParam(RadialBlurKey).Fade == 0.0f)
            HitCreatureBlur = CIntelligentPointer<CThing>(0);
    }
}

#undef private
#undef protected

// Other virtual methods abort if unexpectedly called.
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
void CThingGameObject::OnHit(CHitParameters&) { abort(); }
void CThingCreatureBase::OnHit(CHitParameters&) { abort(); }
bool CThingCreatureBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) { abort(); }
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
const CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString&) const { abort(); }
bool CParentDefClassBase::IsParentDef() const { abort(); }
void CParentDefClassBase::InternalSaveBinary(CDataOutputStream&) { abort(); }
void CParentDefClassBase::InternalLoadBinary(CDataInputStream&) { abort(); }
CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> >* CParentDefClassBase::GetPSubDefInfoMap() { abort(); }
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

const float DELTA_NOTIONAL_ZERO=0.0001f;
double VIRTUAL_SCREEN_WIDTH=128.0,VIRTUAL_SCREEN_HEIGHT=64.0;
static CWorld* testWorld=0;
static NParticleEngine::CParticleEmitterManager manager;
static NParticleEngine::CParticleEmitter *slowResult=0,*endResult=0;
static CSpecialEffectsDef* effectDef=0;
static bool subdefFound=true,slowFound=true,endFound=true;
static float fade=1.0f;
static unsigned int stringLive=0,subdefs=0,lookups=0,creates=0,removes=0,blurSets=0,blurGets=0;
static C3DVector targetPos(2,4,8);
static void* expectedKey=0;
static char events[128];static unsigned int eventCount=0;
static void event(char c){assert(eventCount<127);events[eventCount++]=c;events[eventCount]=0;}
CBaseClass::CBaseClass(){}
CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){}
CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CBaseObject::CBaseObject():IntelligentPointer(0){}
CBaseObject::~CBaseObject(){}
CBaseIntelligentPointer::CBaseIntelligentPointer():PData(0){}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){}
CBaseObject* CBaseIntelligentPointer::GetPItem()const{return reinterpret_cast<CBaseObject*>(PData);}
void CBaseIntelligentPointer::SetPItem(CBaseObject* p){PData=reinterpret_cast<CBaseObjectPointer*>(p);}
CCharString::CCharString():PStringData(0){}
CCharString::CCharString(const char* text,long length):PStringData(reinterpret_cast<CCharStringData*>(const_cast<char*>(text))){assert(length==-1);++stringLive;}
CCharString::~CCharString(){if(PStringData){assert(stringLive);--stringLive;}}
CWideString::CWideString(){}
CWideString::~CWideString(){}
CIDrawWorld::~CIDrawWorld(){}
CWorld::~CWorld(){}
CTCBase::CTCBase(CThing& thing):ParentThing(&thing){}
CTCBase::~CTCBase(){}
CTCBulletTime::CTCBulletTime(CThing& thing):CTCBase(thing){}
CWorld& CTCBase::GetWorld()const{assert(testWorld);return *testWorld;}
CResource::CResource():ResourceList(0),PrevResource(0),NextResource(0),ResourceSize(0),LastUsedFrame(0){IVRefCount=1;}
CResource::~CResource(){}
CDefClassBase::CDefClassBase():PDefManager(0),GlobalIndex(0),Setup(false),Template(false),DefaultValsApplied(false){}
CParentDefClassBase::CParentDefClassBase(){}
CParentDefClassBase::~CParentDefClassBase(){}
CSubDefClassBase::CSubDefClassBase(){}
void CIVCountedPointeeBase::Delete()const{abort();}
class TestBulletDef:public CSpecialAbilitiesBulletTimeDef{public:void Transfer(CPersistContext&){}void Copy(const CDefClassBase*){}unsigned long GetSizeofClass(){return sizeof(*this);}};
class TestEffectDef:public CSpecialEffectsDef{public:void Transfer(CPersistContext&){}void Copy(const CDefClassBase*){}unsigned long GetSizeofClass(){return sizeof(*this);}};
class TestBullet:public CTCBulletTime{public:TestBullet(CThing& thing):CTCBulletTime(thing){}ETCInterfaceType GetInterfaceType()const{return TCI_BULLET_TIME;}CCharString GetClassTypeName()const{return CCharString();}};
template<> bool CDefClassBase::GetSubDef<CSpecialEffectsDef>(CDefPointer<const CSpecialEffectsDef>* out)const{
 event('D');++subdefs;assert(!out->Object);if(!subdefFound)return false;out->Object=effectDef;effectDef->AddRef();return true;
}
bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name,long& effect)const{
 ++lookups;assert(effect==0);const char* n=reinterpret_cast<const char*>(name.PStringData);
 if(strcmp(n,"SLOWTIME")==0){event('S');if(slowFound)effect=11;return slowFound;}
 assert(strcmp(n,"END")==0);event('E');if(endFound)effect=22;return endFound;
}
float __fastcall GFVXToAX(float x){assert(x==128.0f);return x*0.5f;}
float __fastcall GFVYToAY(float y){assert(y==64.0f);return y*0.5f;}
namespace NParticleEngine {
CParticleEmitter::CParticleEmitter(){Dead=false;AlwaysUpdate=false;}
CParticleEmitter::~CParticleEmitter(){}
CParticleEmitterManager& __fastcall GetParticleEmitterManager(){return manager;}
CParticleEmitter* CParticleEmitterManager::Create2DEmitter(long handle,const C2DVector& pos,EEngine2DLayer layer,bool start,const CRightHandedSet& axes){
 ++creates;assert(pos.X==32.0f&&pos.Y==16.0f&&(int)layer==0x52&&start);
 assert(axes.Up.X==0&&axes.Up.Y==0&&axes.Up.Z==1&&axes.Forward.X==0&&axes.Forward.Y==1&&axes.Forward.Z==0);
 if(handle==11){event('M');return slowResult;}assert(handle==22);event('N');return endResult;
}
}
void CThing::RemoveTC(ETCInterfaceType type){assert(type==TCI_BULLET_TIME);++removes;event('R');}
const C3DVector& CThing::GetPos()const{return targetPos;}
void CEnvironmentSpellEffectInterpolator::RadialBlurSetCenterWorldPos(void* key,const C3DVector& pos){assert(key==expectedKey&&&pos==&targetPos);++blurSets;event('C');}
CEnvironmentSpellEffectInterface::CRadialBlurParams CEnvironmentSpellEffectInterpolator::GetRadialBlurParam(void* key)const{assert(key==expectedKey);++blurGets;event('G');CEnvironmentSpellEffectInterface::CRadialBlurParams result;result.ThresholdColour=CRGBFloatColour(0,0,0,0);result.IntensityColour=CRGBFloatColour(0,0,0,0);result.Length=result.Range=result.Twist=0;result.Fade=fade;return result;}
static void reset(){subdefs=lookups=creates=removes=blurSets=blurGets=0;eventCount=0;events[0]=0;subdefFound=slowFound=endFound=true;fade=1;}
int main(){
 CWorld world;testWorld=&world;CEnvironment environment;CEnvironmentSpellEffectInterpolator sei;world.PEnvironment_ptr=&environment;environment.SpellEffectInterpolator_ptr=&sei;
 CThingCreatureBase parent,target;target.Dead=false;TestBulletDef bulletDef;TestEffectDef effects;effectDef=&effects;TestBullet bullet(parent);bullet.PDef.Object=&bulletDef;bulletDef.AddRef();
 NParticleEngine::CParticleEmitter live,dead,slow,end;dead.Dead=true;slowResult=&slow;endResult=&end;
 expectedKey=&world;bullet.RadialBlurKey=expectedKey;bullet.Pausable=true;
 reset();bullet.Paused=true;bullet.BulletTimeRemainingTicks=1;bullet.HitCreatureBlur.Set(&target);bullet.FrameUpdate();assert(bullet.BulletTimeRemainingTicks==1&&creates==0&&removes==0&&blurSets==1&&blurGets==1&&bullet.HitCreatureBlur.Get()==&target);
 reset();bullet.Paused=false;bullet.BulletTimeRemainingTicks=2;bullet.m_tpLoopingEffect.Set(&live);bullet.HitCreatureBlur.Set(0);bullet.FrameUpdate();assert(bullet.BulletTimeRemainingTicks==1&&subdefs==0&&creates==0&&removes==0);
 reset();bullet.BulletTimeRemainingTicks=2;bullet.m_tpLoopingEffect.Set(0);bullet.FrameUpdate();assert(creates==1&&bullet.m_tpLoopingEffect.Get()==&slow&&slow.AlwaysUpdate&&effects.IVRefCount==1);
 reset();bullet.BulletTimeRemainingTicks=2;bullet.m_tpLoopingEffect.Set(&dead);slow.AlwaysUpdate=false;slow.Dead=true;bullet.FrameUpdate();assert(creates==1&&!slow.AlwaysUpdate);slow.Dead=false;
 reset();bullet.BulletTimeRemainingTicks=2;bullet.m_tpLoopingEffect.Set(0);slowResult=0;bullet.FrameUpdate();assert(creates==1&&bullet.m_tpLoopingEffect.Get()==0);slowResult=&slow;
 reset();bullet.BulletTimeRemainingTicks=2;bullet.m_tpLoopingEffect.Set(0);subdefFound=false;bullet.FrameUpdate();assert(subdefs==1&&lookups==0&&creates==0);
 reset();bullet.BulletTimeRemainingTicks=2;bullet.m_tpLoopingEffect.Set(0);slowFound=false;bullet.FrameUpdate();assert(subdefs==1&&lookups==1&&creates==0&&effects.IVRefCount==1);
 reset();bullet.BulletTimeRemainingTicks=1;bullet.m_tpLoopingEffect.Set(&dead);bullet.HitCreatureBlur.Set(&target);bullet.Pausable=true;fade=0;bullet.FrameUpdate();assert(bullet.BulletTimeRemainingTicks==0&&!bullet.Pausable&&removes==1&&creates==2&&end.AlwaysUpdate&&bullet.HitCreatureBlur.Get()==0&&effects.IVRefCount==1);assert(strcmp(events,"DSMDENRCG")==0);
 reset();bullet.BulletTimeRemainingTicks=1;bullet.m_tpLoopingEffect.Set(&live);end.Dead=true;end.AlwaysUpdate=false;bullet.FrameUpdate();assert(removes==1&&creates==1&&!end.AlwaysUpdate);end.Dead=false;
 reset();bullet.BulletTimeRemainingTicks=1;bullet.m_tpLoopingEffect.Set(&live);endFound=false;bullet.FrameUpdate();assert(removes==1&&creates==0);
 reset();bullet.Paused=true;bullet.BulletTimeRemainingTicks=0;bullet.FrameUpdate();assert(removes==0&&creates==0&&bullet.BulletTimeRemainingTicks==0);
 reset();bullet.Paused=false;bullet.BulletTimeRemainingTicks=0;bullet.m_tpLoopingEffect.Set(&live);bullet.FrameUpdate();assert(bullet.BulletTimeRemainingTicks==-1&&removes==0);
 reset();bullet.Paused=true;bullet.HitCreatureBlur.Set(&target);target.Dead=true;bullet.FrameUpdate();assert(blurSets==0&&blurGets==0&&bullet.HitCreatureBlur.Get()==&target);target.Dead=false;
 reset();bullet.HitCreatureBlur.Set(&target);fade=0.5f;bullet.FrameUpdate();assert(blurSets==1&&blurGets==1&&bullet.HitCreatureBlur.Get()==&target);
 assert(stringLive==0&&effects.IVRefCount==1);puts("0079c880_TEST PASS");return 0;
}