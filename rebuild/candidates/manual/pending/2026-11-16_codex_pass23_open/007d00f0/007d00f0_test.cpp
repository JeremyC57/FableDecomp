// Self-contained C++03 behaviour test of the actual shared row.
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
enum ESaveGameLoadStatus {};enum ERegionLoadStatus {};enum EPlayerSpawnStatus {};enum ESaveType {};
enum ESex {};enum ECombatZone {};enum EControlledMovementType {};enum EMovementBandType {};
#endif
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_STLP_HOST_TAGS_PASS23
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE_CTOR_DECL
#include "engine/ETCInterfaceType.h"
#define private public
#define protected public
// 007d00f0 CTCTavernGameShoveHaPenny::FrameUpdate
// Grade: DIFFER(1016v1016) -- frame 0x28 instead of 0x2c; local-slot layout differs
// Flags: /Gr
// Inline-budget: 007D0186..1D6 expands GetAngleXY and its rounding helper. Plain, T116/160/174/512 and scoped depth16 fail the required split; force only these real helpers.
// Leaf: ?FrameUpdate@CTCTavernGameShoveHaPenny@@UAEXXZ
// Identity: both PDBs, donor 02739BB0, retail vtable 0126F2F4 slot 9.
#define FABLE_RETAIL_SHOVE_FRAME_PASS23
#define FABLE_RETAIL_SHOVE_STRING_CLEANUP_PASS23
#define FABLE_RETAIL_CTCTAVERNGAME_POSITIONMESH
#define FABLE_RETAIL_CTCTAVERNGAME_HIDEMESH
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_GETGAMESTATE
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_NORMALISE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_INPUTPROCESS_ZTARGET
#define FABLE_RETAIL_INPUTPROCESS_MOVEMENT
#define FABLE_RETAIL_INPUTPROCESS_EVENTINQUEUE
#define FABLE_RETAIL_CINPUTPROCESSTAVERNGAME_PRESSED_ACCESS
#define FABLE_RETAIL_CINPUTPROCESSTAVERNGAME_EVENTINQUEUE_ACCESS
#define FABLE_RETAIL_CPROCESSEDINPUT_PERFORMED
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameShoveHaPennyDef.h"
#include "engine/CDefinitionManager.h"
template <class T>
bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(T::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}

#include "engine/CTCTavernGame.h"
#include "engine/CTCTavernGameCoinBase.h"
#include "engine/CTCTavernGameShoveHaPenny.h"
#include "fable_game.h"
#include "engine/CThingObject.h"
#include "engine/EGameAction.h"
#include "engine/CInputEvent.h"
#include "engine/CProcessedInput.h"
#include "engine/CInputProcessGameBase.h"
#include "engine/CInputProcessTavernGame.h"

#ifdef _MSC_VER
#pragma intrinsic(sqrt,atan2,fmod)
#endif
// Full original string cleanup context: both PDBs and native0099E9B0.
void CCharString::UnassignString()
{
    if(PStringData) {
        CCharStringData* data=PStringData;
        data->DecRef();
        if(data->GetRefs()<=0) delete PStringData;
        PStringData=0;
    }
}
#ifdef _MSC_VER
// Native ctor0099EBF0 and dtor0099EAE0 remain calls. Plain context1088B expands them;
// this existing owner policy retains both exact native bodies61B/12B and cleanup87B.
#pragma auto_inline(off)
#endif
CCharString::~CCharString() { UnassignString(); --NoStrings; }
CCharString::CCharString(const char* text,long length) : PStringData(0)
{
    ++NoStrings;
    if(PStringData) UnassignString();
    if(text && *text) PStringData=AllocStringData(text,length);
}
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif

void CTCTavernGameShoveHaPenny::FrameUpdate()
{
    CTCTavernGame::FrameUpdate();
    if (!IsBeingPlayed) return;
        CDefPointer<const CTavernGameShoveHaPennyDef> pdef;
        GetParentThing().PeekBaseDef().GetSubDef(&pdef);
        switch (GetGameState()) {
        case 15:
            HideMesh(Arrow);
            PositionMesh(Arrow1,BoardPos,BoardRHSet,C3DVector(Held->Pos.X-0.05f,Held->Pos.Y,0.03f),0,0.25f);
            PositionMesh(Arrow2,BoardPos,BoardRHSet,C3DVector(Held->Pos.X+0.05f,Held->Pos.Y,0.03f),0,0.75f);
            break;
        case 16:
            if (Held->Vel.GetMagnitude() > 0.0f) {
                PositionMesh(Arrow,BoardPos,BoardRHSet,
                    C3DVector(Held->Vel.X*pdef->TargetDistanceScale+Held->Pos.X,
                              Held->Vel.Y*pdef->TargetDistanceScale+Held->Pos.Y,0.001f),
                    0,Held->Vel.GetAngleXY()+0.5f);
            } else HideMesh(Arrow);
            HideMesh(Arrow1);
            HideMesh(Arrow2);
            break;
        default:
            HideMesh(Arrow);
            HideMesh(Arrow1);
            HideMesh(Arrow2);
            break;
        }
        if (GetGameState()==17) {
            float time_delta=1.0f/CMainGameComponent::GetConstantFPS();
            long physics_frames=5;
            for(long n=0;n<physics_frames;++n) UpdateCoins(time_delta/physics_frames);
            UpdateCoinSounds(false);
            long old_score=(long)Score;
            CalcScores();
            if(Score!=old_score) PlaySoundW("TAVERN_GAME_OBJECT_SELECTION_CHANGE");
            if(AtRest(pdef->RestSpeed)) {
                if(RoundsPlayed+1==pdef->Rounds) {
                    GameWon=TargetRound<=Score;
                    CDefPointer<const CTavernGameDef> base_def;
                    GetParentThing().PeekBaseDef().GetSubDef(&base_def);
                    PlaySoundW(GameWon?"TAVERN_GAME_WIN_GAME":"TAVERN_GAME_LOSE_GAME");
                    SetStateAfterOK(GetText(GameWon?base_def->WinPhrase:base_def->LosePhrase),GetText(base_def->Continue),(ETavernGameStates)6);
                } else {
                    ++RoundsPlayed;
                    AddNewCoin(C3DVector(BoardSize.X*0.5f,pdef->PositionCoinDepth,0.0f));
                    AverageJoystickPos=C2DVector(0.0f,0.0f);
                    MousePosEffect=C2DVector(0.0f,0.0f);
                    SetState((ETavernGameStates)15);
                }
            }
        }
        if(GetGameState()==45 && YesState==11) UpdateCoinSounds(true);
        UpdateMeshes();
}

#include "engine/CDefString.h"

static unsigned wideLive=0,baseCalls=0,lookupCalls=0,stepCalls=0,meshCalls=0,restCalls=0,soundCalls=0,scoreCalls=0,addCalls=0,stateCalls=0,okCalls=0;
static bool restAnswer=false;static float newScore=0.0f;static bool soundArgs[4];static unsigned plays=0;static char played[3][64];static unsigned textCalls=0;static unsigned long textIds[2];
static CTCTavernGameShoveHaPenny* activeGame;static CThingBaseDef* parentDef;static CTavernGameShoveHaPennyDef* shoveDef;static CTavernGameDef* baseDef;static CDefinitionManager* manager;
static CDefClassBase::CSubDefInfo shoveInfo,baseInfo;static unsigned hides=0,positions=0;static CThing* hidden[3];static CThing* positioned[3];static C3DVector offsets[3],added;static float ups[3];
// External allocator doubles observe real C++ array/member destruction order.
struct TextAllocation { void* Object; void* Buffer; bool BufferReleased; };
static TextAllocation textAllocations[8];
static unsigned textAllocated=0,textArraysDeleted=0,textObjectsDeleted=0;
void __cdecl operator delete[](void* pointer) throw()
{
    for(unsigned i=0;i<8;++i) if(textAllocations[i].Buffer==pointer && pointer) {
        assert(!textAllocations[i].BufferReleased);
        textAllocations[i].BufferReleased=true;
        ++textArraysDeleted;
        break;
    }
    free(pointer);
}
void __cdecl operator delete(void* pointer) throw()
{
    for(unsigned i=0;i<8;++i) if(textAllocations[i].Object==pointer && pointer) {
        assert(textAllocations[i].BufferReleased);
        textAllocations[i].Object=0;
        textAllocations[i].Buffer=0;
        ++textObjectsDeleted;
        break;
    }
    free(pointer);
}
static void TrackTextAllocation(CCharStringData* object,void* buffer)
{
    for(unsigned i=0;i<8;++i) if(!textAllocations[i].Object) {
        textAllocations[i].Object=object;
        textAllocations[i].Buffer=buffer;
        textAllocations[i].BufferReleased=false;
        ++textAllocated;
        return;
    }
    assert(false);
}
static CCharStringData* makeText(const char*s,long n){if(n<0)n=(long)strlen(s);if(!n)return 0;CCharStringData*d=new CCharStringData;d->Data.PData=new char[n+1];assert(d->Data.PData);memcpy(d->Data.PData,s,n);d->Data.PData[n]=0;d->Data.StringLength=(unsigned long)n;d->Data.DataLength=(unsigned long)n+1;d->Data.UseFastExtend=false;d->NoRefs=1;TrackTextAllocation(d,d->Data.PData);return d;}
const char CCharString::NullChar=0;
long CCharString::NoStrings=0;
CCharString::CCharString():PStringData(0){++NoStrings;}
CCharStringData* CCharString::AllocStringData(const char*s,long n){return makeText(s,n);}
CCharString::CCharString(const CCharString&s):PStringData(s.PStringData){++NoStrings;if(PStringData)++PStringData->NoRefs;}

namespace std {template<> basic_string<unsigned short>::basic_string():_Base(allocator_type(),2){*_M_start=0;}}
CWideString::CWideString():PStringData(0){}
CWideString::CWideString(const CWideString&s):PStringData(s.PStringData){if(PStringData)++PStringData->NoRefs;}
CWideString::~CWideString(){if(PStringData&&--PStringData->NoRefs==0){delete PStringData;--wideLive;}}
CBaseClass::CBaseClass(){} CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){} CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CBaseObject::CBaseObject():IntelligentPointer(0){} CBaseObject::~CBaseObject(){}
CBaseIntelligentPointer::CBaseIntelligentPointer():PData(0){} CBaseIntelligentPointer::~CBaseIntelligentPointer(){assert(!PData);}
CTCBase::CTCBase(CThing&t):ParentThing(&t),ToBeRemoved(false),IsInGlobalUpdateTCs(false){} CTCBase::~CTCBase(){}
CTCTavernGame::CTCTavernGame(CThing&t):CTCBase(t),IsBeingPlayed(false),GameWon(false),Score(0),TargetRound(0),YesState(ETGS_NONE),TavernGameState(ETGS_NONE){}
CTCTavernGame::~CTCTavernGame(){}
CTCTavernGameCoinBase::CTCTavernGameCoinBase(CThing&t):CTCTavernGame(t),Arrow(0),Arrow1(0),Arrow2(0),Held(0){}
CTCTavernGameShoveHaPenny::CTCTavernGameShoveHaPenny(CThing&t):CTCTavernGameCoinBase(t),RoundsPlayed(0){}
Mass::Mass():Pos(0,0),Vel(0,0),IMass(0),APos(0),AVel(0),AIMass(0){} Mass::~Mass(){}
Coin::Coin(float x,float radius,CThing*,bool owned):Owned(owned),Radius(radius),Owner(0),Sound(0){Pos.X=x;}
CResource::CResource():ResourceList(0),PrevResource(0),NextResource(0),ResourceSize(0),LastUsedFrame(0){IVRefCount=1;}CResource::~CResource(){}
CDefClassBase::CDefClassBase():PDefManager(0),GlobalIndex(0),Setup(false),Template(false),DefaultValsApplied(false){}
CSubDefClassBase::CSubDefClassBase(){}
CParentDefClassBase::CParentDefClassBase(){}CParentDefClassBase::~CParentDefClassBase(){}
CResourceList::~CResourceList(){} CDefinitionManager::~CDefinitionManager(){}
void CIVCountedPointeeBase::Delete()const{abort();}
CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString&){abort();return 0;}
const CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString&name)const{assert(this==parentDef);++lookupCalls;if(!strcmp(name.GetAsCharArray(),"CTavernGameShoveHaPennyDef"))return &shoveInfo;assert(!strcmp(name.GetAsCharArray(),"CTavernGameDef"));return &baseInfo;}
CDefPointer<CDefClassBase> CDefinitionManager::GetPDefFromGlobalIndex(long i){assert(this==manager);CDefPointer<CDefClassBase> result;assert(i==1||i==2);result.Object=i==1?(CDefClassBase*)shoveDef:(CDefClassBase*)baseDef;result.Object->AddRef();return result;}
long CMainGameComponent::ConstantFPS=4;
void CTCTavernGame::FrameUpdate(){assert(this==activeGame);++baseCalls;}
void FABLE_FASTCALL CTCTavernGame::HideMesh(CThing*t){assert(hides<3);hidden[hides++]=t;}
void FABLE_FASTCALL CTCTavernGame::PositionMesh(CThing*t,const C3DVector&p,const CRightHandedSet&rh,const C3DVector&o,float forward,float up){assert(activeGame&&&p==&activeGame->BoardPos&&&rh==&activeGame->BoardRHSet&&forward==0&&positions<3);positioned[positions]=t;offsets[positions]=o;ups[positions]=up;++positions;}
void CTCTavernGameCoinBase::UpdateCoins(float delta){assert(this==activeGame&&delta==0.25f/5.0f);++stepCalls;}
void CTCTavernGameCoinBase::UpdateCoinSounds(bool b){assert(this==activeGame&&soundCalls<4);soundArgs[soundCalls++]=b;}
void CTCTavernGameShoveHaPenny::CalcScores(){assert(this==activeGame);++scoreCalls;Score=newScore;}
bool CTCTavernGameCoinBase::AtRest(float v){assert(this==activeGame&&v==shoveDef->RestSpeed);++restCalls;return restAnswer;}
void CTCTavernGameCoinBase::UpdateMeshes(){assert(this==activeGame);++meshCalls;}
void CTCTavernGameCoinBase::AddNewCoin(const C3DVector&v){assert(this==activeGame);++addCalls;added=v;}
void CTCTavernGame::SetState(ETavernGameStates s){assert(this==activeGame&&s==15);++stateCalls;TavernGameState=s;}
unsigned long CTCTavernGame::PlaySoundW(const CCharString&s){assert(this==activeGame&&plays<3);strcpy(played[plays++],s.GetAsCharArray());return 0;}
CWideString CTCTavernGame::GetText(unsigned long id){assert(this==activeGame&&textCalls<2);textIds[textCalls++]=id;CWideString result;result.PStringData=new CWideStringData;result.PStringData->NoRefs=1;result.PStringData->Data._M_start[0]=(unsigned short)id;result.PStringData->Data._M_start[1]=0;result.PStringData->Data._M_finish=result.PStringData->Data._M_start+1;++wideLive;return result;}
void CTCTavernGame::SetStateAfterOK(CWideString title,CWideString cont,ETavernGameStates s){assert(this==activeGame&&s==6);assert(title.PStringData&&cont.PStringData);assert(title.PStringData->Data._M_start[0]==(GameWon?baseDef->WinPhrase:baseDef->LosePhrase));assert(cont.PStringData->Data._M_start[0]==baseDef->Continue);++okCalls;}
// A concrete test subclass implements only the original abstract serialization surface.
class FixtureThingDef:public CThingBaseDef{public:void Transfer(CPersistContext&){abort();}void Copy(const CDefClassBase*){abort();}unsigned long GetSizeofClass(){abort();return 0;}};

CFailedAllocationHandler::~CFailedAllocationHandler(){}
CTavernGameDef::CTavernGameDef(){}
namespace std {template<> void _String_base<unsigned short,allocator<unsigned short> >::_M_throw_length_error()const{abort();}}

void CBaseObject::Validate() const { abort(); }
void CClass::Save(CCharString&) { abort(); }
void CThingGameObject::OnHit(CHitParameters&) { abort(); }
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
void CTCTavernGame::OnSerialise(CPersistContext&, bool) { abort(); }
void CTCTavernGame::OnInitialActivate() { abort(); }
bool CTCTavernGame::HasFrameUpdate() const { abort(); }
void CTCTavernGame::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&) { abort(); }
ETCInterfaceType CTCTavernGame::GetInterfaceType() const { abort(); }
void CTCTavernGame::ProcessInput(const CInputEvent&, CProcessedInput&, CInputProcessTavernGame*) { abort(); }
void CTCTavernGame::EnterGame() { abort(); }
void CTCTavernGame::LeaveGame() { abort(); }
void CTCTavernGame::NewGame() { abort(); }
void CTCTavernGame::NewRound() { abort(); }
void CTCTavernGame::EndRound() { abort(); }
void CTCTavernGame::QuitMidGame() { abort(); }
void CTCTavernGame::Greeting() { abort(); }
void CTCTavernGame::Options() { abort(); }
void CTCTavernGame::Instructions() { abort(); }
void CTCTavernGame::Betting() { abort(); }
void CTCTavernGame::ReactionAndPayout() { abort(); }
bool CTCTavernGame::UpdateValues() { abort(); }
void CTCTavernGame::Farewell() { abort(); }
CWideString CTCTavernGame::GetWinPhrase() { abort(); }
CWideString CTCTavernGame::GetAdditionalInfo() { abort(); }
CWideString CTCTavernGame::GetBet() { abort(); }
CWideString CTCTavernGame::GetPlayersMoney() { abort(); }
CWideString CTCTavernGame::GetTotalWinnings() { abort(); }
void CTCTavernGame::DrawBetAndMoney(const CEnginePrimitivePrepareInfo&, float, float, float, float, float, float) { abort(); }
void CTCTavernGameCoinBase::OnInitialActivate() { abort(); }
ETCInterfaceType CTCTavernGameCoinBase::GetInterfaceType() const { abort(); }
void CTCTavernGameCoinBase::EnterGame() { abort(); }
void CTCTavernGameCoinBase::LeaveGame() { abort(); }
void CTCTavernGameCoinBase::NewGame() { abort(); }
void CTCTavernGameCoinBase::NewRound() { abort(); }
void CTCTavernGameCoinBase::EndRound() { abort(); }
void CTCTavernGameShoveHaPenny::OnInitialActivate() { abort(); }
CCharString CTCTavernGameShoveHaPenny::GetClassTypeName() const { abort(); }
void CTCTavernGameShoveHaPenny::ProcessInput(const CInputEvent&, CProcessedInput&, CInputProcessTavernGame*) { abort(); }
void CTCTavernGameShoveHaPenny::EnterGame() { abort(); }
void CTCTavernGameShoveHaPenny::LeaveGame() { abort(); }
void CTCTavernGameShoveHaPenny::NewGame() { abort(); }
void CTCTavernGameShoveHaPenny::NewRound() { abort(); }
void CTCTavernGameShoveHaPenny::EndRound() { abort(); }
void CTCTavernGameShoveHaPenny::QuitMidGame() { abort(); }
bool CTCTavernGameShoveHaPenny::UpdateValues() { abort(); }
CWideString CTCTavernGameShoveHaPenny::GetBestScore() { abort(); }
CWideString CTCTavernGameShoveHaPenny::GetCurrentScore() { abort(); }
CWideString CTCTavernGameShoveHaPenny::GetRequiredScore() { abort(); }
CWideString CTCTavernGameShoveHaPenny::GetAdditionalInfo() { abort(); }
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
bool CParentDefClassBase::IsParentDef() const { abort(); }
void CParentDefClassBase::InternalSaveBinary(CDataOutputStream&) { abort(); }
void CParentDefClassBase::InternalLoadBinary(CDataInputStream&) { abort(); }
CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> >* CParentDefClassBase::GetPSubDefInfoMap() { abort(); }
void CTavernGameDef::Transfer(CPersistContext&) { abort(); }
void CTavernGameDef::Copy(const CDefClassBase*) { abort(); }
unsigned long CTavernGameDef::GetSizeofClass() { abort(); }
bool CResourceList::HandleFailedAllocation(unsigned long) { abort(); }
int main(){
 // Isolate the nonzero-reference branch as well as final array/member deletion.
 const char* cleanupTexts[]={"","x","cleanup","TAVERN_GAME_WIN_GAME"};
 const long originalStrings=CCharString::NoStrings;
 unsigned cleanupCases=0;
 for(unsigned text=0;text<4;++text) for(unsigned lengthMode=0;lengthMode<3;++lengthMode) {
  const long n=lengthMode==0?-1:(long)strlen(cleanupTexts[text])/(lengthMode==1?1:2);
  const unsigned beforeObjects=textObjectsDeleted,beforeArrays=textArraysDeleted;
  {
   CCharString value(cleanupTexts[text],n);
   const unsigned allocated=textAllocated;
   if(value.PStringData) assert(value.PStringData->GetRefs()==1);
   {
    CCharString shared(value);
    if(value.PStringData) assert(value.PStringData->GetRefs()==2);
    assert(!strcmp(shared.GetAsCharArray(),value.GetAsCharArray()));
   }
   assert(textObjectsDeleted==beforeObjects && textArraysDeleted==beforeArrays && textAllocated==allocated);
   if(value.PStringData) assert(value.PStringData->GetRefs()==1);
  }
  assert(CCharString::NoStrings==originalStrings && textAllocated==textObjectsDeleted && textAllocated==textArraysDeleted);
  ++cleanupCases;
 }
 { CCharString nullText((const char*)0,-1);assert(!nullText.PStringData); }
 assert(cleanupCases==12 && CCharString::NoStrings==originalStrings);

 CThing host;CThingObject arrow,arrow1,arrow2;FixtureThingDef pd;CTavernGameShoveHaPennyDef sd;CTavernGameDef bd;CDefinitionManager dm;Coin coin(1,0.5f,0,false);CTCTavernGameShoveHaPenny game(host);
 activeGame=&game;parentDef=&pd;shoveDef=&sd;baseDef=&bd;manager=&dm;pd.PDefManager=&dm;shoveInfo.DefIndex=1;baseInfo.DefIndex=2;host.PBaseDef.Object=&pd;pd.AddRef();
 sd.TargetDistanceScale=2.0f;sd.PositionCoinDepth=0.5f;sd.RestSpeed=0.25f;sd.Rounds=3;bd.WinPhrase=11;bd.LosePhrase=12;bd.Continue=13;
 game.Arrow=&arrow;game.Arrow1=&arrow1;game.Arrow2=&arrow2;game.Held=&coin;game.BoardPos=C3DVector(1,2,3);game.BoardSize=C3DVector(4,8,0);coin.Pos=C2DVector(1,2);
 unsigned cases=0;const long states[]={0,15,16,17,45,46};
 for(unsigned active=0;active<2;++active)for(unsigned st=0;st<6;++st)for(unsigned moving=0;moving<5;++moving)for(unsigned rest=0;rest<2;++rest)for(unsigned finalRound=0;finalRound<2;++finalRound)for(unsigned win=0;win<2;++win)for(unsigned changed=0;changed<2;++changed)for(unsigned confirm=0;confirm<2;++confirm){
  game.IsBeingPlayed=active!=0;game.TavernGameState=(CTCTavernGame::ETavernGameStates)states[st];game.YesState=(CTCTavernGame::ETavernGameStates)(confirm?11:10);game.GameWon=false;game.RoundsPlayed=finalRound?2:0;game.Score=changed?2.0f:4.0f;newScore=4.0f;game.TargetRound=win?4:8;restAnswer=rest!=0;game.AverageJoystickPos=C2DVector(3,4);game.MousePosEffect=C2DVector(5,6);
  coin.Vel=C2DVector(moving==1?1.0f:(moving==3?-1.0f:0.0f),moving==2?1.0f:(moving==4?-1.0f:0.0f));
  baseCalls=lookupCalls=stepCalls=meshCalls=restCalls=soundCalls=scoreCalls=addCalls=stateCalls=okCalls=plays=textCalls=hides=positions=0;
  const long chars=CCharString::NoStrings;const unsigned wide=wideLive;const unsigned long refs=sd.IVRefCount,brefs=bd.IVRefCount,prefs=pd.IVRefCount;
  game.FrameUpdate();assert(textAllocated==textObjectsDeleted && textAllocated==textArraysDeleted);assert(baseCalls==1&&CCharString::NoStrings==chars&&wideLive==wide&&sd.IVRefCount==refs&&bd.IVRefCount==brefs&&pd.IVRefCount==prefs);
  if(!active){assert(!lookupCalls&&!meshCalls&&!stepCalls&&!hides&&!positions&&!soundCalls&&!plays);++cases;continue;}
  assert(meshCalls==1&&lookupCalls==(states[st]==17&&rest&&finalRound?2U:1U));
  if(states[st]==15){assert(hides==1&&hidden[0]==&arrow&&positions==2&&positioned[0]==&arrow1&&positioned[1]==&arrow2);assert(offsets[0].X==1.0f-0.05f&&offsets[1].X==1.0f+0.05f&&offsets[0].Y==2.0f&&offsets[1].Y==2.0f&&offsets[0].Z==0.03f&&offsets[1].Z==0.03f&&ups[0]==0.25f&&ups[1]==0.75f);}
  else if(states[st]==16&&moving){assert(hides==2&&hidden[0]==&arrow1&&hidden[1]==&arrow2&&positions==1&&positioned[0]==&arrow);assert(offsets[0].X==coin.Vel.X*2.0f+1.0f&&offsets[0].Y==coin.Vel.Y*2.0f+2.0f&&offsets[0].Z==0.001f);const float expected[]={0,0.75f,0.5f,1.25f,1.0f};assert(fabs(ups[0]-expected[moving])<0.000001);}
  else assert(hides==3&&positions==0&&hidden[0]==&arrow&&hidden[1]==&arrow1&&hidden[2]==&arrow2);
  if(states[st]==17){assert(stepCalls==5&&scoreCalls==1&&restCalls==1&&soundCalls==1&&!soundArgs[0]);unsigned n=0;if(changed)assert(!strcmp(played[n++],"TAVERN_GAME_OBJECT_SELECTION_CHANGE"));if(rest&&finalRound){assert(game.GameWon==(win!=0)&&okCalls==1&&textCalls==2&&!addCalls&&!stateCalls);assert(textIds[0]==13&&textIds[1]==(win?11U:12U));assert(!strcmp(played[n++],win?"TAVERN_GAME_WIN_GAME":"TAVERN_GAME_LOSE_GAME"));}else if(rest){assert(addCalls==1&&stateCalls==1&&game.RoundsPlayed==1&&added.X==2.0f&&added.Y==0.5f&&added.Z==0&&game.AverageJoystickPos.X==0&&game.AverageJoystickPos.Y==0&&game.MousePosEffect.X==0&&game.MousePosEffect.Y==0&&!okCalls);}else{assert(!okCalls&&!addCalls&&!stateCalls&&game.RoundsPlayed==(finalRound?2:0));}assert(plays==n);}
  else{assert(!stepCalls&&!scoreCalls&&!restCalls&&!plays&&!okCalls&&!addCalls&&!stateCalls);assert(soundCalls==((states[st]==45&&confirm)?1U:0U));if(soundCalls)assert(soundArgs[0]);}
  ++cases;
 }
 host.PBaseDef.Release();assert(cases==1920&&sd.IVRefCount==1&&bd.IVRefCount==1&&pd.IVRefCount==1);puts("007d00f0_TEST PASS");return 0;
}
