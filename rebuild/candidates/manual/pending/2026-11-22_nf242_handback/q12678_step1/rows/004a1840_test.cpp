// NF242 (q12678 step 1): the CIEngine slot 8 / 11 / 12 / 13 overrides respelled from the retired VirtualNN_ placeholders to the PDB
// declarations engine/CIEngine.h now makes the default (Render, ConvertScreenPosToWorldPos, ...AtViewDistance, ConvertWorldPosToScreenPos).
// Self-contained fixture: actual shared objects and controlled external callees.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
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
enum EEnginePrimitiveStatIndex{};enum EPlayerMode{};enum ESaveGameLoadStatus{};enum ERegionLoadStatus{};
enum EPlayerSpawnStatus{};enum ESaveType{};
#endif
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_STLP_HOST_TAGS_PASS23
#define FABLE_RETAIL_CWORLD_CTOR_PASS23
#define FABLE_RETAIL_CLEVELLOADER_CTOR_PASS23
#define FABLE_RETAIL_CWORLDMAP_CTOR
#define FABLE_RETAIL_CGAME_CORE_PASS23
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR
#define FABLE_RETAIL_CDRAWMAPINFO_PASS23
#define private public
#define protected public
// 004a1840 CWorld::Load(const CWideString&) -- loads world/quests, startup WAD and static map state.
// Retail:0x004A1840..004A1CA9,1129bytes including its catch handler and normal-return cleanup.
// Flags: none
// Leaf: ?Load@CWorld@@QAEXABVCWideString@@@Z
#include "fable_compiler.h"
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_SPRINTF
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_TYPED_MEMBERS
#define FABLE_RETAIL_CWORLD_TYPED_SAVE
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CWORLD_LOAD_ORIGINAL
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CDISKFILEWIN32_DEFAULTCTOR
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CAFILE_SPLITPATH_WIDE
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_WORLDLOAD
#define FABLE_RETAIL_CLEVELLOADER
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CINITBASECLASS
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE
#define FABLE_RETAIL_CDISPLAYENGINE_GET3DENGINE
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CSYSTEMMANAGER_CRITICALDISPLAY
#include "fable_string.h"
#include "fable_profile.h"
#include "fable_world_options.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CDiskFileWin32.h"
#include "engine/CBBBFileException.h"
#include "engine/CLevelLoader.h"
#include "engine/CMainGameComponent.h"
#include "engine/CDisplayEngine.h"
#include "engine/CIEngine.h"
#include "engine/CSystemManager.h"
#include "engine/NProgressDisplay.h"

void CWorld::GenerateOfflineDataForWorld(const CWideString& world_name)
{
    if(GEnableStaticMapCreation){
        CWideString static_map_path=ConvertWorldToStaticMapPath(world_name);
        GetWorldMap().CreateStaticMaps(static_map_path,Component.GetDisplayEngine().Get3DEngine());
    }
}

void CWorld::Load(const CWideString& level)
{
    CWideString current_world_name;
    CAFile::SplitPath(level,0,0,&current_world_name,0);
    CurrentWorldName=current_world_name.ToCharString();
    CDiskFileWin32 file;
    try{
        file.Open(level,FILE_ACCESS_READ,2);
    }catch(CBBBFileException& exc){
        CCharString error;
        error.Sprintf("Unable to load world %s",level.GetAsCharArray());
        GFGetSystemManager()->DisplayCriticalMessage(error);
        return;
    }
    NProfileTimer::StartProfile("Load Quests",0);
    CWideString static_map_path=ConvertWorldToStaticMapPath(level);
    CWideString quest_path=ConvertWorldToQuestPath(level);
    if(CAFile::PathExists(quest_path)){
        CDiskFileWin32 quest_file(quest_path,FILE_ACCESS_READ,2);
        LoadQuestsFromFile(quest_file,true);
    }
    CWideString global_quests_path(L"Data\\Levels\\GlobalQuests.qst");
    if(CAFile::PathExists(global_quests_path)){
        CDiskFileWin32 global_quest_file(global_quests_path,FILE_ACCESS_READ,2);
        LoadQuestsFromFile(global_quest_file,false);
    }
    PWorldMap->SetAsUsingWAD(L"");
    NProfileTimer::StartProfile("Startup Wad",0);
    NProgressDisplay::DisplayProgress("Startup WAD",-1.0f,false,false);
    if(GUseLevelWAD){
        CWideString wad_path=ConvertWorldToLevelWADPath(level);
        PWorldMap->CreateWorldLevelWad(level,wad_path);
        PWorldMap->SetAsUsingWAD(ConvertWorldToLevelWADPath(level));
    }
    NProfileTimer::EndProfile(0);
    LoadWorldFromFile(file,-1);
    while(PWorldMap->GetLevelLoader()->IsBusy())
        PWorldMap->GetLevelLoader()->RunSuspendableProcess();
    NProfileTimer::StartProfile("Generate Offline Data",0);
    NProgressDisplay::DisplayProgress("Generate Offline Data",-1.0f,false,false);
    GenerateOfflineDataForWorld(level);
    NProfileTimer::StartProfile("Set Static Map for Engine",0);
    NProgressDisplay::DisplayProgress("Set Static Map for Engine",-1.0f,false,false);
    Component.GetDisplayEngine().Get3DEngine().SetStaticMapFileForUse(static_map_path);
    NProgressDisplay::DisplayProgress("Set Static Map for Engine: End",-1.0f,false,false);
    NProfileTimer::EndProfile(0);
}

#include "engine/CWorldInit.h"
#include "engine/CGame.h"
#include "engine/CMainGameComponentInit.h"
#include "engine/CFileInformation.h"
#include "engine/CThingManager.h"
template<class T>class CDefPointer;
#include "engine/CGameDefinitionManager.h"
#include "engine/CPlayerManager.h"
#include "engine/CNavigatorManager.h"
#include "engine/CDrawMapInfo.h"
#include "engine/CCamera.h"
#include "engine/C2DExtentsI.h"
#include "engine/CPixelFormat.h"
#include "engine/CRGBFloatColour.h"

static int WideAlive=0,CharAlive=0,BaseNew=0,BaseDelete=0;
static void CopyW(wchar_t*out,const wchar_t*in){int n=0;do{assert(n<255);out[n]=in[n];}while(in[n++]);}
static void CopyC(char*out,const char*in){int n=0;do{assert(n<255);out[n]=in[n];}while(in[n++]);}
struct Event{int Kind,Arg;};static Event Expected[80];static int EventCount,NextEvent;
static void Expect(int kind,int arg=0){assert(EventCount<80);Expected[EventCount].Kind=kind;Expected[EventCount++].Arg=arg;}
static void Observe(int kind,int arg=0){assert(NextEvent<EventCount);Event&e=Expected[NextEvent++];assert(e.Kind==kind&&e.Arg==arg);}
enum{Split=1,ToChar,Open,Close,Sprintf,Critical,StartProfile,EndProfile,ConvertStatic,ConvertQuest,PathQuest,PathGlobal,ResetWAD,ConvertWAD,CreateWAD,UseWAD,Progress,LoadWorld,Busy,Run,DeleteLoader,GetMap,CreateStatic,SetStatic};
struct Plan {bool Fail,Level,Global,WAD,Static,Drop;int BusyCount,Remaining,CaseID,NextFile,LoaderDeleted,LoaderGets,StaticConversions,WadConversions,SystemGets;bool Teardown;};static Plan plan;
static const wchar_t*InputText;static wchar_t BaseName[256],StaticName[256],QuestName[256],WADName[256];
static void MakePaths(const wchar_t*text){InputText=text;const wchar_t*p=text;for(const wchar_t*i=text;*i;++i)if(*i==L'\\'||*i==L'/')p=i+1;CopyW(BaseName,p);wchar_t*dot=0;for(wchar_t*i=BaseName;*i;++i)if(*i==L'.')dot=i;if(dot)*dot=0;CopyW(StaticName,BaseName);CopyW(StaticName+wcslen(StaticName),L".stc");CopyW(QuestName,BaseName);CopyW(QuestName+wcslen(QuestName),L".qst");CopyW(WADName,BaseName);CopyW(WADName+wcslen(WADName),L".wad");}

static const wchar_t* Wide(const CWideString&x){return reinterpret_cast<const wchar_t*>(x.PStringData);}
static const char* Chars(const CCharString&x){return reinterpret_cast<const char*>(x.PStringData);}
static char* DupC(const char*x){size_t n=strlen(x)+1;char*p=(char*)malloc(n);assert(p);memcpy(p,x,n);return p;}
static wchar_t* DupW(const wchar_t*x){size_t n=(wcslen(x)+1)*sizeof(wchar_t);wchar_t*p=(wchar_t*)malloc(n);assert(p);memcpy(p,x,n);return p;}
CCharString::CCharString():PStringData(reinterpret_cast<CCharStringData*>(DupC(""))){++CharAlive;}
CCharString::CCharString(const char*p,long n):PStringData(reinterpret_cast<CCharStringData*>(DupC(p))){assert(n==-1);++CharAlive;}
CCharString::CCharString(const CCharString&p):PStringData(reinterpret_cast<CCharStringData*>(DupC(Chars(p)))){++CharAlive;}
CCharString::~CCharString(){assert(CharAlive>0);--CharAlive;free(PStringData);}
const CCharString& CCharString::operator=(const CCharString&p){if(this!=&p){free(PStringData);PStringData=reinterpret_cast<CCharStringData*>(DupC(Chars(p)));}return *this;}
CCharString::operator const char*()const{return Chars(*this);}

CWideString::CWideString():PStringData(reinterpret_cast<CWideStringData*>(DupW(L""))){++WideAlive;}
CWideString::CWideString(const wchar_t*p):PStringData(reinterpret_cast<CWideStringData*>(DupW(p))){++WideAlive;}
CWideString::CWideString(const CWideString&p):PStringData(reinterpret_cast<CWideStringData*>(DupW(Wide(p)))){++WideAlive;}
CWideString::~CWideString(){assert(WideAlive>0);--WideAlive;free(PStringData);}
const CWideString& CWideString::operator=(const CWideString&p){if(this!=&p){free(PStringData);PStringData=reinterpret_cast<CWideStringData*>(DupW(Wide(p)));}return *this;}
const wchar_t* CWideString::GetAsCharArray()const{return Wide(*this);}
CCharString CWideString::ToCharString()const{Observe(ToChar);assert(wcscmp(Wide(*this),BaseName)==0);char name[256];int n=0;for(;Wide(*this)[n];++n){assert(Wide(*this)[n]>=0&&Wide(*this)[n]<128);name[n]=(char)Wide(*this)[n];}name[n]=0;return CCharString(name);}
void CCharString::Sprintf(const char*format,...){assert(plan.Fail&&strcmp(format,"Unable to load world %s")==0);va_list args;va_start(args,format);const wchar_t*name=va_arg(args,const wchar_t*);assert(wcscmp(name,InputText)==0);va_end(args);free(PStringData);PStringData=reinterpret_cast<CCharStringData*>(DupC("formatter result"));Observe(::Sprintf);}
CBaseClass::CBaseClass(){} CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){++BaseNew;}
CBaseClassNonCopyable::~CBaseClassNonCopyable(){++BaseDelete;}
CInitBaseClass::CInitBaseClass():Valid(false){} CInitBaseClass::~CInitBaseClass(){}
CIEngine::~CIEngine(){}
CGame::CGame():CurrentGameComponent(0),Quit(false){} CGame::~CGame(){}
CGameComponent::CGameComponent(CGame&g):Quit(false),Running(false),Game(g){} CGameComponent::~CGameComponent(){}
void CGameComponent::Init(){} void CGameComponent::SetQuit(){Quit=true;}
CMainGameComponent::CMainGameComponent(CGame&g,const CMainGameComponentInit&):CGameComponent(g){memset(PDisplayEngine,0,sizeof PDisplayEngine);}
CMainGameComponent::~CMainGameComponent(){}
CMainGameComponentInit::CMainGameComponentInit(){} CMainGameComponentInit::~CMainGameComponentInit(){}
CWorldInit::CWorldInit():Component(0),ScavengeWorld(0),PlayerManager(0),DefinitionManager(0),DisplayEngine(0){}
CFileInformation::CFileInformation():ReadOnly(false){}
CBBBFileException::CBBBFileException(EFileExceptionType t):Type(t){}
CBBBFileException::CBBBFileException(CCharString,EFileExceptionType t):Type(t){}
static const CDiskFileWin32* Files[4];static int FileIDs[4];
static int FileID(const CAFile&f){for(int i=0;i<4;++i)if(Files[i]==&f)return FileIDs[i];assert(false);return 0;}
void FABLE_FASTCALL CAFile::SplitPath(const CWideString&path,CWideString*drive,CWideString*dir,CWideString*name,CWideString*ext){assert(wcscmp(Wide(path),InputText)==0&&!drive&&!dir&&name&&!ext);*name=CWideString(BaseName);Observe(::Split);}
bool FABLE_FASTCALL CAFile::PathExists(const CWideString&path){if(wcscmp(Wide(path),QuestName)==0){Observe(PathQuest);return plan.Level;}assert(wcscmp(Wide(path),L"Data\\Levels\\GlobalQuests.qst")==0);Observe(PathGlobal);return plan.Global;}
bool CAFile::IsIOFinished()const{return true;}long CAFile::GetAlignment()const{return 1;}CFileInformation CAFile::GetFileInformation(){abort();}
CDiskFileWin32::CDiskFileWin32(const CWideString&p,FileAccessType access,unsigned long flags):Pathname(p){Opened=false;Open(p,access,flags);}
void CDiskFileWin32::Open(const CWideString&p,FileAccessType access,unsigned long flags){assert(access==FILE_ACCESS_READ&&flags==2);Pathname=p;Position=Length=0;WinFileHandle=0;int id=++plan.NextFile;bool recorded=false;for(int i=0;i<4;++i)if(!Files[i]){Files[i]=this;FileIDs[i]=id;recorded=true;break;}assert(recorded);Observe(::Open,id);if(id==1){assert(wcscmp(Wide(p),InputText)==0);
#if !defined(_MSC_VER) || defined(_CPPUNWIND)
if(plan.Fail)throw CBBBFileException(CBBBFileException::FILE_NOT_FOUND);
#else
assert(!plan.Fail);
#endif
}else assert(wcscmp(Wide(p),QuestName)==0||wcscmp(Wide(p),L"Data\\Levels\\GlobalQuests.qst")==0);Opened=true;}
void CDiskFileWin32::Close(){Observe(::Close,FileID(*this));for(int i=0;i<4;++i)if(Files[i]==this)Files[i]=0;Opened=false;}
void CDiskFileWin32::Read(void*,long,CASuspendableProcess*){abort();}void CDiskFileWin32::Write(const void*,long,CASuspendableProcess*){abort();}void CDiskFileWin32::SetPosition(long){abort();}void CDiskFileWin32::MovePosition(long){abort();}void CDiskFileWin32::SetLength(long){abort();}bool CDiskFileWin32::IsOpen()const{return Opened;}CWideString CDiskFileWin32::GetPathname()const{return Pathname;}CAFile*CDiskFileWin32::Clone()const{abort();}CFileInformation CDiskFileWin32::GetFileInformation(){abort();}
CASuspendableProcess::CASuspendableProcess(bool,long,float):Running(false),StopRunning(false),TerminateUsingExceptions(false){} CASuspendableProcess::~CASuspendableProcess(){}
CSuspendableProcessMicrothread::CSuspendableProcessMicrothread(bool a,long b,float c):CASuspendableProcess(a,b,c),PMicrothread(0){} CSuspendableProcessMicrothread::~CSuspendableProcessMicrothread(){}
CLevelLoader::CLevelLoader(CThingManager&t,CWorldMap&m,CWorld&w):CSuspendableProcessMicrothread(false,0,0),ThingManager(&t),WorldMap(&m),World(&w){}
CLevelLoader::CObject::CLevelFiles::~CLevelFiles(){}
class FixtureLoader:public CLevelLoader{public:FixtureLoader(CThingManager&t,CWorldMap&m,CWorld&w):CLevelLoader(t,m,w){}~FixtureLoader(){++plan.LoaderDeleted;if(!plan.Teardown)Observe(DeleteLoader);}};
namespace boost { template<> scoped_ptr<CEnvironment>::~scoped_ptr(){assert(!ptr);} }
namespace boost { template<> scoped_ptr<CGameTimeManager>::~scoped_ptr(){assert(!ptr);} }
namespace boost { template<> scoped_ptr<CThingSearchTools>::~scoped_ptr(){assert(!ptr);} }
namespace boost { template<> scoped_ptr<CAIGameCameraBase>::~scoped_ptr(){assert(!ptr);} }
namespace boost { template<> scoped_ptr<CGameScriptInterface>::~scoped_ptr(){assert(!ptr);} }
namespace boost { template<> scoped_ptr<CNavigatorManager>::~scoped_ptr(){assert(!ptr);} }
namespace boost { template<> scoped_ptr<CCombatManager>::~scoped_ptr(){assert(!ptr);} }
namespace boost { template<> scoped_ptr<CThingManager>::~scoped_ptr(){assert(!ptr);} }
namespace boost { template<> scoped_ptr<CFactionManager>::~scoped_ptr(){assert(!ptr);} }
namespace boost { template<> scoped_ptr<CBulletTimeManager>::~scoped_ptr(){assert(!ptr);} }
namespace boost { template<> scoped_ptr<CScriptConversationManager>::~scoped_ptr(){assert(!ptr);} }
namespace boost { template<> scoped_ptr<NThingMap::CThingMapManager>::~scoped_ptr(){assert(!ptr);} }
namespace boost { template<> scoped_ptr<CWorldMap>::~scoped_ptr(){delete ptr;} }
namespace std {template<> _Vector_base<CMapInfo,allocator<CMapInfo> >::~_Vector_base(){assert(!_M_start);} template<> vector<CMapInfo>::~vector(){assert(!_M_start&&!_M_finish);} }
namespace std {template<> _Vector_base<CRegion,allocator<CRegion> >::~_Vector_base(){assert(!_M_start);} template<> vector<CRegion>::~vector(){assert(!_M_start&&!_M_finish);} }
namespace std {template<> _Vector_base<CWorldMap::CPlayerPositionInfo,allocator<CWorldMap::CPlayerPositionInfo> >::~_Vector_base(){assert(!_M_start);} template<> vector<CWorldMap::CPlayerPositionInfo>::~vector(){assert(!_M_start&&!_M_finish);} }
namespace std {template<> _Vector_base<CEnginePrimitiveHandle,allocator<CEnginePrimitiveHandle> >::~_Vector_base(){assert(!_M_start);} template<> vector<CEnginePrimitiveHandle>::~vector(){assert(!_M_start&&!_M_finish);} }

CBaseIntelligentPointer::CBaseIntelligentPointer():PData(0){} CBaseIntelligentPointer::~CBaseIntelligentPointer(){assert(!PData);}
CIDrawWorld::~CIDrawWorld(){} CIDrawWorldMap::~CIDrawWorldMap(){}
CWorld::CWorld(const CWorldInit&i):Component(*i.Component){} CWorld::~CWorld(){}
CWorldMap::CWorldMap(const CMainGameComponent&c,CWorld&w,const C2DBoxI&a,unsigned long):World(w),Component(c),Granularity(1),GranularityShift(0),Area(a){} CWorldMap::~CWorldMap(){}
CDefinitionManager::~CDefinitionManager(){} CFailedAllocationHandler::~CFailedAllocationHandler(){} CResourceList::~CResourceList(){}
CResource::CResource():ResourceList(0),PrevResource(0),NextResource(0),ResourceSize(0),LastUsedFrame(0){} CResource::~CResource(){}
CClassFactory::CClassFactory(unsigned char){} CClassFactory::~CClassFactory(){}
CGameDefinitionManager::CGameDefinitionManager(){} CGameDefinitionManager::~CGameDefinitionManager(){}
CThingManager::CThingManager(const CMainGameComponent&c,const CGameDefinitionManager&d,CWorld&w,CPlayerManager&p,CNavigatorManager&n,unsigned long*seed,unsigned long,unsigned long,unsigned long):CClassFactory(0),Component(c),DefinitionManager(d),World(w),WorldMap(*w.PWorldMap),WorldSeed(seed),PlayerManager(p),NavigationManager(n){} CThingManager::~CThingManager(){}

class FixtureEngine:public CIEngine {public:
void SetCamera(const CCamera*)  { abort(); }
void SetWindow(const C2DBoxF*)  { abort(); }
void Render(const CEngineInterpolationInfo&, unsigned long)  { abort(); }
bool ConvertScreenPosToWorldPos(const C2DVector&, C3DVector&) const  { abort(); }
bool ConvertScreenPosToWorldPosAtViewDistance(float, const C2DVector&, C3DVector&) const  { abort(); }
bool ConvertWorldPosToScreenPos(const C3DVector&, C2DVector&) const  { abort(); }
bool PreChangeResolution()  { abort(); }
bool PostChangeResolution(unsigned long, unsigned long, unsigned long)  { abort(); }
void SetPersistentPrimitive(CEnginePrimitiveHandle&, const CEnginePrimitive&, unsigned long)  { abort(); }
};

static CIEngine* ActiveEngine;static CWorldMap*ActiveMap;
void CIEngine::SetStaticMapFileForUse(const CWideString&file){assert(wcscmp(Wide(file),StaticName)==0);Observe(SetStatic);}
void CWorldMap::SetAsUsingWAD(const CWideString&wad){if(Wide(wad)[0]){assert(plan.WAD&&wcscmp(Wide(wad),WADName)==0);Observe(UseWAD);}else Observe(ResetWAD);}
void CWorldMap::CreateWorldLevelWad(const CWideString&world,const CWideString&wad){assert(plan.WAD&&wcscmp(Wide(world),InputText)==0&&wcscmp(Wide(wad),WADName)==0);Observe(CreateWAD);}
void CWorldMap::CreateStaticMaps(const CWideString&file,CIEngine&engine){assert(plan.Static&&&engine==ActiveEngine&&wcscmp(Wide(file),StaticName)==0);Observe(CreateStatic);}
bool CLevelLoader::IsBusy()const{++plan.LoaderGets;assert(ActiveMap->PLevelLoader.Data==this&&ActiveMap->PLevelLoader.GetReferenceCount()==2);bool busy=plan.Remaining>0;Observe(::Busy,busy?1:0);if(!busy&&plan.Drop){CCountedPointer<CLevelLoader> empty;ActiveMap->PLevelLoader=empty;}return busy;}
void CSuspendableProcessMicrothread::RunSuspendableProcess(){++plan.LoaderGets;assert(ActiveMap->PLevelLoader.Data==this&&ActiveMap->PLevelLoader.GetReferenceCount()==2&&plan.Remaining>0);Observe(::Run);--plan.Remaining;}
CWorldMap& CWorld::GetWorldMap(){Observe(GetMap);return *PWorldMap;}
void CWorld::LoadWorldFromFile(CAFile&file,long version){assert(FileID(file)==1&&file.IsOpen()&&version==-1);Observe(LoadWorld);}
CWideString CWorld::ConvertWorldToStaticMapPath(const CWideString&level)const{assert(wcscmp(Wide(level),InputText)==0);Observe(ConvertStatic);++plan.StaticConversions;return CWideString(StaticName);}
CWideString FABLE_FASTCALL CWorld::ConvertWorldToQuestPath(const CWideString&level){assert(wcscmp(Wide(level),InputText)==0);Observe(ConvertQuest);return CWideString(QuestName);}
CWideString CWorld::ConvertWorldToLevelWADPath(const CWideString&level)const{assert(wcscmp(Wide(level),InputText)==0);Observe(ConvertWAD);++plan.WadConversions;return CWideString(WADName);}
void CWorld::LoadQuestsFromFile(CAFile&file,bool level){assert(FileID(file)>=2&&file.IsOpen());CWideString name=file.GetPathname();assert(wcscmp(Wide(name),level?QuestName:L"Data\\Levels\\GlobalQuests.qst")==0);Observe(100,level?1:0);}
void CSystemManager::DisplayCriticalMessage(const char*message){assert(plan.Fail&&strcmp(message,"formatter result")==0);Observe(Critical);}
static CSystemManager System;
CSystemManager* FABLE_FASTCALL GFGetSystemManager(){assert(plan.Fail);++plan.SystemGets;return &System;}
bool GUseLevelWAD=false,GEnableStaticMapCreation=false;
static int Label(const char*text){if(strcmp(text,"Load Quests")==0)return 1;if(strcmp(text,"Startup Wad")==0)return 2;if(strcmp(text,"Generate Offline Data")==0)return 3;if(strcmp(text,"Set Static Map for Engine")==0)return 4;assert(false);return 0;}
namespace NProfileTimer{void FABLE_FASTCALL StartProfile(const CCharString&text,long id){assert(id==0);Observe(::StartProfile,Label(text));}void FABLE_FASTCALL EndProfile(long id){assert(id==0);Observe(::EndProfile);}}
namespace NProgressDisplay{void FABLE_FASTCALL DisplayProgress(const CCharString&text,float progress,bool a,bool b){assert(progress==-1.0f&&!a&&!b);int label=strcmp(text,"Startup WAD")==0?1:strcmp(text,"Generate Offline Data")==0?2:strcmp(text,"Set Static Map for Engine")==0?3:strcmp(text,"Set Static Map for Engine: End")==0?4:0;assert(label);Observe(::Progress,label);}}

void CIEngine::Validate() const { abort(); }
bool CIEngine::Initialise(const CEngineInit&) { abort(); }
void CIEngine::Release() { abort(); }
void CIEngine::SetEnvironmentalEffectsAlwaysUpdate(bool) { abort(); }
void CIEngine::SetWorld(const CIDrawWorld*) { abort(); }
void CIEngine::WorldFrameUpdate(bool) { abort(); }
void CIEngine::PausedWorldFrameUpdate() { abort(); }
bool CIEngine::SetVideoOptions(const CEngineVideoOptions&) { abort(); }
void CIEngine::Refresh() { abort(); }
bool CIEngine::CheckPrimitiveVisibility(unsigned long long) { abort(); }
float CIEngine::GetPrimitiveDrawDistanceFactor() const { abort(); }
void CIEngine::AddPrimitive(const CEnginePrimitive&) { abort(); }
bool CIEngine::ClipSphere(const C3DBoundingSphere&) const { abort(); }
bool CIEngine::ClipBox(const C3DBoundingBox&) const { abort(); }
void CIEngine::SetPrimitiveAnimation(CEnginePrimitiveHandle&, const CEnginePrimitiveAnimation&) { abort(); }
void CIEngine::SetPrimitivePosition(CEnginePrimitiveHandle&, const CEnginePrimitivePosition&) { abort(); }
unsigned long CIEngine::AddChildPrimitive(CEnginePrimitiveHandle&, const CEnginePrimitive&, unsigned long) { abort(); }
void CIEngine::SetChildPrimitive(CEnginePrimitiveHandle&, unsigned long, const CEnginePrimitive&, unsigned long) { abort(); }
void CIEngine::SetChildPrimitive(CEnginePrimitiveHandle&, unsigned long, CEnginePrimitiveHandle&, const CEnginePrimitive&, unsigned long) { abort(); }
void CIEngine::GetChildPrimitiveHandle(CEnginePrimitiveHandle&, unsigned long, CEnginePrimitiveHandle&) { abort(); }
void CIEngine::ClearChildPrimitive(CEnginePrimitiveHandle&, unsigned long) { abort(); }
void CIEngine::ClearAllChildPrimitives(CEnginePrimitiveHandle&) { abort(); }
void CIEngine::SetAttachedPrimitive(CEnginePrimitiveHandle&, const CEnginePrimitive&, const CEnginePrimitiveAttachmentInfo&, const CEnginePrimitiveHandle&, long, unsigned long) { abort(); }
void CIEngine::CollectPrimitiveStatsForArea(std::map<unsigned long, unsigned long, std::less<unsigned long>, std::allocator<std::pair<const unsigned long, unsigned long> > >&, const C2DBoxF&, EEnginePrimitiveStatIndex, unsigned long) const { abort(); }
void CIEngine::ProcessPrimitiveStats(const std::map<unsigned long, unsigned long, std::less<unsigned long>, std::allocator<std::pair<const unsigned long, unsigned long> > >&, EEnginePrimitiveStatIndex, unsigned long&, CArray<CCharString>&) const { abort(); }
const C3DAnimationInfo* CIEngine::GetEditorAnimation(const CWideString&) const { abort(); }
unsigned long CIEngine::AddCustomMorphTexture(CTexture&, const CCharString&) const { abort(); }
long CIEngine::GetStaticMapUpdatePassCount() { abort(); }
void CIEngine::StartStaticMapUpdatePass(const CWideString&, long, const CStaticMapGenerationVisibiilityInfo&, const CArray<CCharString>&) { abort(); }
bool CIEngine::IsStaticMapNeedingUpdate(long, const CCharString&, unsigned long) const { abort(); }
void CIEngine::UpdateStaticMap(long, const CCharString&, unsigned long, long) { abort(); }
void CIEngine::CloseStaticMapFile() { abort(); }
void CIEngine::AddMap(long, long) { abort(); }
void CIEngine::RemoveMap(long) { abort(); }
void CIEngine::SetMapAsLoaded(long) { abort(); }
void CIEngine::SetMapAsUnloaded(long) { abort(); }
void CIEngine::SetMapVisibilityAllMaps(bool) { abort(); }
void CIEngine::SetMapVisibility(long, bool) { abort(); }
void CIEngine::AreaChanged(const C2DBoxI&) { abort(); }
void CIEngine::OnActivate() { abort(); }
void CIEngine::OnDeactivate() { abort(); }
bool CIEngine::IsActive() const { abort(); }
void CIEngine::SetLandscapeDebugTintColour(long, CRGBColour) { abort(); }
void CIEngine::ReloadLocalDetailDefs() { abort(); }
void CIEngine::StartNewScene(unsigned long, float) { abort(); }
void CIEngine::CutscenePreload(bool, bool) { abort(); }
void CIEngine::CutsceneAddCameraAngle(const CCamera&) { abort(); }
void CIEngine::ChangeCameraAngle() { abort(); }
bool CIEngine::IsPreloaded() const { abort(); }
void CIEngine::UpdateTextureMorphCache(const CArray<CEngineTextureMorph>&, bool, float, const std::set<long, std::less<long>, std::allocator<long> >*) { abort(); }
void CIEngine::SetScreenColourFilter(const CEngineColourFilterSettings&) { abort(); }
void CIEngine::SetSpotlightShadowMode(bool) { abort(); }
void CIEngine::SetSeaBodyToRender(CCharString) { abort(); }
void CIEngine::UpdateDebugOverlay(long, const CRGBColour*, const C2DBoxI&) { abort(); }
void CIEngine::EnableDebugOverlay(long, bool, bool) { abort(); }
void CIEngine::SetGlobalLightingParameters(const C3DVector&, const CRGBFloatColour&, const CRGBFloatColour&, const CRGBFloatColour&) { abort(); }
void CIEngine::Set2DPrimitiveLightingParameters(const C3DVector&, const CRGBFloatColour&, const CRGBFloatColour&, const CRGBFloatColour&) { abort(); }
void CIEngine::SetRenderTargetTexture(const CTexture*) { abort(); }
CTexture* CIEngine::AllocateTextureFromEngineResourcePools(unsigned long, const C2DExtentsI&, long, const CPixelFormat&) { abort(); }
void CIEngine::FreeTextureFromEngineResourcePools(CTexture*) { abort(); }
CMovableResourceMemoryPool* CIEngine::GetLoadingMemoryPool() { abort(); }
CMovableResourceMemoryPool* CIEngine::GetAnimationMemoryPool() { abort(); }
CGPUScratchBufferTextureManager* CIEngine::GetScratchBufferTextureManager() { abort(); }
bool CIEngine::DeallocateShadowBuffer() { abort(); }
void CIEngine::ReallocateShadowBuffer() { abort(); }
void CIEngine::Enable3DSceneRendering(bool) { abort(); }
bool CIEngine::Is3DSceneRenderingEnabled() { abort(); }
void CIEngine::Enable2DSceneRendering(bool) { abort(); }
bool CIEngine::Is2DSceneRenderingEnabled() { abort(); }
void CIEngine::OverrideRain(float) { abort(); }
void CIEngine::OverrideSnow(float) { abort(); }
void CIEngine::RenderColorFilteredQuad(float, float, const C4DVector&, float, float, CTexture*) { abort(); }
const CRGBFloatColour CIEngine::PeekGlobalDiffuseColour() const { abort(); }
const CRGBFloatColour CIEngine::PeekGlobalAmbientColour() const { abort(); }
void CWorld::PostLoadInit() { abort(); }
CThingManager& CWorld::GetThingManager() { abort(); }
const CThingManager& CWorld::PeekThingManager() const { abort(); }
CCamera CWorld::GetCurrentRenderCamera() const { abort(); }
CCamera CWorld::GetCurrentRenderCamera(const CInterpolationInfo& info) const { abort(); }
void CWorld::PostInit(CDisplayEngine& displayEngine) { abort(); }
C3DVector CWorld::GetGameLODCentre() const { abort(); }
void CWorld::Init() { abort(); }
void CWorld::Uninitialise() { abort(); }
void CWorld::PreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo& info) { abort(); }
const CWorldMap& CWorld::PeekWorldMap() const { abort(); }
const CProceduralTexture* CWorld::DrawGetProceduralTexture() const { abort(); }
const CIDrawWorldMap* CWorld::DrawGetWorldMap() const { abort(); }
const CIDrawEnvironment* CWorld::DrawGetEnvironment() const { abort(); }
void CWorldMap::DeleteAllLevels() { abort(); }
void CWorldMap::DeleteAllRegions() { abort(); }
void CWorldMap::LoadFromFile(CAFile&, long) { abort(); }
void CWorldMap::SaveToFile(CAFile&) { abort(); }
void CWorldMap::RemoveLevel(long) { abort(); }
void CWorldMap::LoadTopologyFromFile(long, CCountedPointer<CAFile>, CASuspendableProcess&) { abort(); }
void CWorldMap::PostLoadTopology(long) { abort(); }
void CWorldMap::ForceLoad(long) { abort(); }
void CWorldMap::ForceUnload(long) { abort(); }
bool CWorldMap::IsMapLoaded(long) const { abort(); }
bool CWorldMap::IsMapUsed(long) const { abort(); }
long CWorldMap::GetGranularity() const { abort(); }
const C2DBoxI& CWorldMap::GetDimensions() const { abort(); }
CMap& CWorldMap::GetMapAt(const C2DCoordI&) { abort(); }
const CMap& CWorldMap::PeekMapAt(const C2DCoordI&) const { abort(); }
long CWorldMap::GetMapNumberAt(const C2DCoordI&) const { abort(); }
long CWorldMap::GetMapNumberAt(const C3DVector&) const { abort(); }
CMap& CWorldMap::GetMap(long) { abort(); }
const CMap& CWorldMap::PeekMap(long) const { abort(); }
void CWorldMap::GetAllMapsInArea(const C2DBoxI&, std::vector<unsigned long, std::allocator<unsigned long> >&) const { abort(); }
void CWorldMap::GetAllLoadedMapsInArea(const C2DBoxI&, std::vector<unsigned long, std::allocator<unsigned long> >&) const { abort(); }
void CWorldMap::PostRegionLoad(long) { abort(); }
C2DCoordI CWorldMap::GetMapPos(long) const { abort(); }
bool CWorldMap::IsMapChangeable(long) const { abort(); }
bool CWorldMap::IsPosChangeable(const C2DCoordI&) const { abort(); }
std::set<CCharString, std::less<CCharString>, std::allocator<CCharString> > CWorldMap::GetLoadableSections() const { abort(); }
void CWorldMap::Update() { abort(); }
void CWorldMap::HeightAreaChanged(const C2DBoxI*) { abort(); }
long CWorldMap::DrawGetFrame() const { abort(); }
long CWorldMap::DrawGetNoMaps() const { abort(); }
CDrawMapInfo CWorldMap::DrawGetMapInfo(long) const { abort(); }
C2DBoxI CWorldMap::DrawGetDimensions() const { abort(); }
long CWorldMap::DrawGetGranularity() const { abort(); }
bool CWorldMap::DrawIsPosInMap(const C3DVector&) const { abort(); }
float CWorldMap::DrawGetSizeZAt(const C3DVector&, float) const { abort(); }
C3DVector CWorldMap::DrawConvertWorldMapCoordsToLocalCoords(long, const C3DVector&) const { abort(); }
long CWorldMap::DrawGetMyPlayerMapNumber() const { abort(); }
bool CWorldMap::DrawFindLastUnblockedGroundPositionAndReportCollision(const C3DVector&, const C3DVector&, C3DVector&) const { abort(); }
bool CWorldMap::DrawFindLastUnblockedThemePositionAndReportCollision(const C3DVector&, const C3DVector&, C3DVector&) const { abort(); }
void CWorldMap::LoadScriptBrushesFromFile(long, CCountedPointer<CAFile>) { abort(); }
long CWorldMap::AllocateNewMap() { abort(); }
bool CMainGameComponent::Run(CGameComponent**) { abort(); }
void CMainGameComponent::ChangeTextureColourDepth(long) { abort(); }
void CMainGameComponent::OnPreDeviceReset() { abort(); }
bool CMainGameComponent::OnPostDeviceReset() { abort(); }
void CASuspendableProcess::SuspendableProcess() { abort(); }
bool CSuspendableProcessMicrothread::SuspendSuspendableProcess() { abort(); }
void CSuspendableProcessMicrothread::Restart() { abort(); }
void CClassFactory::AddNewClassType(const CClassTypeInit&) { abort(); }
void CClassFactory::OnCreateClass(CClass&) { abort(); }
void CClassFactory::OnDeleteClass(CClass&) { abort(); }
void CThingManager::OnCreateClass(CClass&) { abort(); }
void CThingManager::OnDeleteClass(CClass&) { abort(); }
void CGameDefinitionManager::InitAndCompile() { abort(); }
bool CResource::UnloadResource() { abort(); }
unsigned long CResource::GetUnloadableMemory() const { abort(); }
bool CResource::IsResourceLoaded() const { abort(); }
void CResource::OnResourceLoaded() { abort(); }
bool CResource::CanAbortResourceLoad(long) const { abort(); }
void CResource::OnResourceLoadAborted() { abort(); }
unsigned long CResource::GetDebugMemoryInUse() const { abort(); }
void CResource::ResetDebugMemoryInUse() { abort(); }
bool CResourceList::HandleFailedAllocation(unsigned long) { abort(); }
void CIVCountedPointeeBase::Delete() const { abort(); }
int main(){static const wchar_t*inputs[]={L"Data\\Levels\\Oakvale.wld",L"levels/longer_map.wld",L"Bowerstone.wld",L"X:\\levels\\A.wld"};unsigned cases=0;int wideBaseline=WideAlive,charBaseline=CharAlive;int coverage[8]={0,0,0,0,0,0,0,0};
 for(int seed=0;seed<1024;++seed){assert(WideAlive==wideBaseline&&CharAlive==charBaseline);memset(&plan,0,sizeof(plan));plan.Fail=(seed&1)!=0;
#if defined(_MSC_VER) && !defined(_CPPUNWIND)
if(plan.Fail)continue;
#endif
plan.Level=(seed&2)!=0;plan.Global=(seed&4)!=0;plan.WAD=(seed&8)!=0;plan.Static=(seed&16)!=0;plan.BusyCount=plan.Remaining=seed/32%4;plan.Drop=(seed&128)!=0;plan.CaseID=seed/256%4;GUseLevelWAD=plan.WAD;GEnableStaticMapCreation=plan.Static;MakePaths(inputs[plan.CaseID]);BaseNew=BaseDelete=EventCount=NextEvent=0;
  Expect(Split);Expect(ToChar);Expect(Open,1);
  if(plan.Fail){Expect(Sprintf);Expect(Critical);Expect(Close,1);++coverage[0];}
  else{Expect(StartProfile,1);Expect(ConvertStatic);Expect(ConvertQuest);Expect(PathQuest);int next=2;if(plan.Level){Expect(Open,next);Expect(100,1);Expect(Close,next++);++coverage[1];}Expect(PathGlobal);if(plan.Global){Expect(Open,next);Expect(100,0);Expect(Close,next);++coverage[2];}Expect(ResetWAD);Expect(StartProfile,2);Expect(Progress,1);if(plan.WAD){Expect(ConvertWAD);Expect(CreateWAD);Expect(ConvertWAD);Expect(UseWAD);++coverage[3];}Expect(EndProfile);Expect(LoadWorld);for(int n=0;n<plan.BusyCount;++n){Expect(Busy,1);Expect(Run);}Expect(Busy,0);if(plan.Drop){Expect(DeleteLoader);++coverage[4];}Expect(StartProfile,3);Expect(Progress,2);if(plan.Static){Expect(ConvertStatic);Expect(GetMap);Expect(CreateStatic);++coverage[5];}Expect(StartProfile,4);Expect(Progress,3);Expect(SetStatic);Expect(Progress,4);Expect(EndProfile);Expect(Close,1);if(plan.BusyCount)++coverage[6];else ++coverage[7];}
  {FixtureEngine engine;ActiveEngine=&engine;CDisplayEngine display;display.Engine3D=&engine;CGame game;CMainGameComponentInit componentInit;CMainGameComponent component(game,componentInit);CDisplayEngine* pd=&display;memcpy(component.PDisplayEngine,&pd,sizeof pd);CWorldInit init;init.Component=&component;CWorld world(init);world.PWorldMap.ptr=new CWorldMap(component,world,C2DBoxI(),0);ActiveMap=world.PWorldMap.get();CGameDefinitionManager definitions;CPlayerManager players;CNavigatorManager navigation;unsigned long seedValue=0;CThingManager things(component,definitions,world,players,navigation,&seedValue,0,0,0);ActiveMap->PLevelLoader.Reset(new FixtureLoader(things,*ActiveMap,world));CWideString level(InputText);int baseBalance=BaseNew-BaseDelete;world.Load(level);assert(NextEvent==EventCount);char expected[256];int n=0;for(;BaseName[n];++n)expected[n]=(char)BaseName[n];expected[n]=0;assert(strcmp(Chars(world.CurrentWorldName),expected)==0);assert(BaseNew-BaseDelete==baseBalance-(!plan.Fail&&plan.Drop?1:0));if(plan.Fail){assert(plan.StaticConversions==0&&plan.WadConversions==0&&plan.LoaderGets==0&&plan.SystemGets==1);}else{assert(plan.StaticConversions==1+(int)plan.Static&&plan.WadConversions==(plan.WAD?2:0)&&plan.LoaderGets==2*plan.BusyCount+1&&plan.Remaining==0&&!plan.SystemGets);assert(plan.LoaderDeleted==(plan.Drop?1:0));if(!plan.Drop)assert(ActiveMap->PLevelLoader.GetReferenceCount()==1);}plan.Teardown=true;}
  assert(WideAlive==wideBaseline&&CharAlive==charBaseline&&BaseNew==BaseDelete&&plan.LoaderDeleted==1);++cases;
 }
 
#if defined(_MSC_VER) && !defined(_CPPUNWIND)
assert(cases==512u);for(int n=1;n<8;++n)assert(coverage[n]>0);
#else
assert(cases==1024u);for(int n=0;n<8;++n)assert(coverage[n]>0);
#endif
puts("004a1840_TEST PASS");return 0;}