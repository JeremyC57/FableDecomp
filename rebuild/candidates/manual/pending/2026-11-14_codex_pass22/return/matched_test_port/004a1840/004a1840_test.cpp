#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <wchar.h>
#include "rebuild_abi.h"
#include "fable_compiler.h"
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
class CCharString {public:char Text[256];CCharString(){Text[0]=0;++CharAlive;}CCharString(const char*p,long n=-1){assert(n==-1);CopyC(Text,p);++CharAlive;}CCharString(const CCharString&p){CopyC(Text,p.Text);++CharAlive;}~CCharString(){assert(CharAlive>0);--CharAlive;}const CCharString&operator=(const CCharString&p){CopyC(Text,p.Text);return *this;}operator const char*()const{return Text;}void __cdecl Sprintf(const char*,...);};
class CWideString {public:wchar_t Text[256];CWideString(){Text[0]=0;++WideAlive;}CWideString(const wchar_t*p){CopyW(Text,p);++WideAlive;}CWideString(const CWideString&p){CopyW(Text,p.Text);++WideAlive;}~CWideString(){assert(WideAlive>0);--WideAlive;}CWideString&operator=(const CWideString&p){CopyW(Text,p.Text);return *this;}const wchar_t*GetAsCharArray()const{return Text;}CCharString ToCharString()const{Observe(ToChar);assert(wcscmp(Text,BaseName)==0);char name[256];int n=0;for(;Text[n];++n){assert(Text[n]>=0&&Text[n]<128);name[n]=(char)Text[n];}name[n]=0;return CCharString(name);}};
void CCharString::Sprintf(const char*format,...){assert(plan.Fail&&strcmp(format,"Unable to load world %s")==0);va_list args;va_start(args,format);const wchar_t*name=va_arg(args,const wchar_t*);assert(wcscmp(name,InputText)==0);va_end(args);CopyC(Text,"formatter result");Observe(::Sprintf);}
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CDISKFILEWIN32_DEFAULTCTOR
#include "engine/CDiskFileWin32.h"
#include "engine/CBBBFileException.h"
CBaseClassNonCopyable::CBaseClassNonCopyable(){++BaseNew;}CBaseClassNonCopyable::~CBaseClassNonCopyable(){++BaseDelete;}
CAFile::CAFile():TestID(++plan.NextFile){}
CBBBFileException::CBBBFileException(EFileExceptionType t):Type(t){}
CBBBFileException::CBBBFileException(CCharString,EFileExceptionType t):Type(t){}
void __fastcall CAFile::SplitPath(const CWideString&path,CWideString*drive,CWideString*dir,CWideString*name,CWideString*ext){assert(wcscmp(path.Text,InputText)==0&&!drive&&!dir&&name&&!ext);CopyW(name->Text,BaseName);Observe(::Split);}
bool __fastcall CAFile::PathExists(const CWideString&path){if(wcscmp(path.Text,QuestName)==0){Observe(PathQuest);return plan.Level;}assert(wcscmp(path.Text,L"Data\\Levels\\GlobalQuests.qst")==0);Observe(PathGlobal);return plan.Global;}
CDiskFileWin32::CDiskFileWin32(const CWideString&p,FileAccessType access,unsigned long flags):Pathname(p){Opened=false;Open(p,access,flags);}
void CDiskFileWin32::Open(const CWideString&p,FileAccessType access,unsigned long flags){assert(access==FILE_ACCESS_READ&&flags==2);Pathname=p;Position=Length=0;WinFileHandle=0;Observe(::Open,TestID);if(TestID==1){assert(wcscmp(p.Text,InputText)==0);if(plan.Fail)throw CBBBFileException(CBBBFileException::FILE_NOT_FOUND);}else assert(wcscmp(p.Text,QuestName)==0||wcscmp(p.Text,L"Data\\Levels\\GlobalQuests.qst")==0);Opened=true;}
void CDiskFileWin32::Close(){Observe(::Close,TestID);Opened=false;}
void CDiskFileWin32::Read(void*,long,CASuspendableProcess*){assert(false);}void CDiskFileWin32::Write(const void*,long,CASuspendableProcess*){assert(false);}void CDiskFileWin32::SetPosition(long){assert(false);}void CDiskFileWin32::MovePosition(long){assert(false);}void CDiskFileWin32::SetLength(long){assert(false);}bool CDiskFileWin32::IsOpen()const{return Opened;}CWideString CDiskFileWin32::GetPathname()const{return Pathname;}CAFile*CDiskFileWin32::Clone()const{assert(false);return 0;}CFileInformation CDiskFileWin32::GetFileInformation(){assert(false);return CFileInformation();}
class CLevelLoader {public:virtual~CLevelLoader(){++plan.LoaderDeleted;if(!plan.Teardown)Observe(DeleteLoader);}virtual void RunSuspendableProcess();bool IsBusy()const;};
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_SCOPED_PTR_RESET
#include "boost/scoped_ptr.h"
class CIEngine {public:virtual~CIEngine(){}virtual void SetStaticMapFileForUse(const CWideString&file){assert(wcscmp(file.Text,StaticName)==0);Observe(SetStatic);}};
class CDisplayEngine {CIEngine&Engine;public:CDisplayEngine(CIEngine&e):Engine(e){}CIEngine&Get3DEngine(){return Engine;}};
class CMainGameComponent {CDisplayEngine&Display;public:CMainGameComponent(CDisplayEngine&d):Display(d){}CDisplayEngine&GetDisplayEngine(){return Display;}};
static CIEngine*ActiveEngine;
class CWorldMap {public:CCountedPointer<CLevelLoader>PLevelLoader;CWorldMap():PLevelLoader(new CLevelLoader){}CCountedPointer<CLevelLoader>GetLevelLoader()const{++plan.LoaderGets;return PLevelLoader;}void SetAsUsingWAD(const CWideString&wad){if(wad.Text[0]){assert(plan.WAD&&wcscmp(wad.Text,WADName)==0);Observe(UseWAD);}else Observe(ResetWAD);}void CreateWorldLevelWad(const CWideString&world,const CWideString&wad){assert(plan.WAD&&wcscmp(world.Text,InputText)==0&&wcscmp(wad.Text,WADName)==0);Observe(CreateWAD);}void CreateStaticMaps(const CWideString&file,CIEngine&engine){assert(plan.Static&&&engine==ActiveEngine&&wcscmp(file.Text,StaticName)==0);Observe(CreateStatic);}};
static CWorldMap*ActiveMap;
bool CLevelLoader::IsBusy()const{assert(ActiveMap->PLevelLoader.Data==this&&ActiveMap->PLevelLoader.GetReferenceCount()==2);bool busy=plan.Remaining>0;Observe(::Busy,busy?1:0);if(!busy&&plan.Drop)ActiveMap->PLevelLoader.Reset(0);return busy;}
void CLevelLoader::RunSuspendableProcess(){assert(ActiveMap->PLevelLoader.Data==this&&ActiveMap->PLevelLoader.GetReferenceCount()==2&&plan.Remaining>0);Observe(::Run);--plan.Remaining;}
class CWorld {public:CMainGameComponent&Component;boost::scoped_ptr<CWorldMap>PWorldMap;CCharString CurrentWorldName;CWorld(CMainGameComponent&c):Component(c),PWorldMap(new CWorldMap){}virtual~CWorld(){}virtual CWorldMap&GetWorldMap(){Observe(GetMap);return *PWorldMap;}virtual void LoadWorldFromFile(CAFile&file,long version){assert(file.TestID==1&&file.IsOpen()&&version==-1);Observe(LoadWorld);}CWideString ConvertWorldToStaticMapPath(const CWideString&level)const{assert(wcscmp(level.Text,InputText)==0);Observe(ConvertStatic);++plan.StaticConversions;return CWideString(StaticName);}static CWideString FABLE_FASTCALL ConvertWorldToQuestPath(const CWideString&level){assert(wcscmp(level.Text,InputText)==0);Observe(ConvertQuest);return CWideString(QuestName);}CWideString ConvertWorldToLevelWADPath(const CWideString&level)const{assert(wcscmp(level.Text,InputText)==0);Observe(ConvertWAD);++plan.WadConversions;return CWideString(WADName);}void LoadQuestsFromFile(CAFile&file,bool level){assert(file.TestID>=2&&file.IsOpen());CWideString name=file.GetPathname();assert(wcscmp(name.Text,level?QuestName:L"Data\\Levels\\GlobalQuests.qst")==0);Observe(100,level?1:0);}void GenerateOfflineDataForWorld(const CWideString&);void Load(const CWideString&);};
class CSystemManager {public:void DisplayCriticalMessage(const char*message){assert(plan.Fail&&strcmp(message,"formatter result")==0);Observe(Critical);}};static CSystemManager System;
CSystemManager*__fastcall GFGetSystemManager(){assert(plan.Fail);++plan.SystemGets;return &System;}
bool GUseLevelWAD=false,GEnableStaticMapCreation=false;
static int Label(const char*text){if(strcmp(text,"Load Quests")==0)return 1;if(strcmp(text,"Startup Wad")==0)return 2;if(strcmp(text,"Generate Offline Data")==0)return 3;if(strcmp(text,"Set Static Map for Engine")==0)return 4;assert(false);return 0;}
namespace NProfileTimer{void FABLE_FASTCALL StartProfile(const CCharString&text,long id){assert(id==0);Observe(::StartProfile,Label(text));}void FABLE_FASTCALL EndProfile(long id){assert(id==0);Observe(::EndProfile);}}
namespace NProgressDisplay{void FABLE_FASTCALL DisplayProgress(const CCharString&text,float progress,bool a,bool b){assert(progress==-1.0f&&!a&&!b);int label=strcmp(text,"Startup WAD")==0?1:strcmp(text,"Generate Offline Data")==0?2:strcmp(text,"Set Static Map for Engine")==0?3:strcmp(text,"Set Static Map for Engine: End")==0?4:0;assert(label);Observe(::Progress,label);}}
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

int main(){static const wchar_t*inputs[]={L"Data\\Levels\\Oakvale.wld",L"levels/longer_map.wld",L"Bowerstone.wld",L"X:\\levels\\A.wld"};unsigned cases=0;int coverage[8]={0,0,0,0,0,0,0,0};
 for(int seed=0;seed<1024;++seed){assert(WideAlive==0&&CharAlive==0);memset(&plan,0,sizeof(plan));plan.Fail=(seed&1)!=0;plan.Level=(seed&2)!=0;plan.Global=(seed&4)!=0;plan.WAD=(seed&8)!=0;plan.Static=(seed&16)!=0;plan.BusyCount=plan.Remaining=seed/32%4;plan.Drop=(seed&128)!=0;plan.CaseID=seed/256%4;GUseLevelWAD=plan.WAD;GEnableStaticMapCreation=plan.Static;MakePaths(inputs[plan.CaseID]);BaseNew=BaseDelete=EventCount=NextEvent=0;
  Expect(Split);Expect(ToChar);Expect(Open,1);
  if(plan.Fail){Expect(Sprintf);Expect(Critical);Expect(Close,1);++coverage[0];}
  else{Expect(StartProfile,1);Expect(ConvertStatic);Expect(ConvertQuest);Expect(PathQuest);int next=2;if(plan.Level){Expect(Open,next);Expect(100,1);Expect(Close,next++);++coverage[1];}Expect(PathGlobal);if(plan.Global){Expect(Open,next);Expect(100,0);Expect(Close,next);++coverage[2];}Expect(ResetWAD);Expect(StartProfile,2);Expect(Progress,1);if(plan.WAD){Expect(ConvertWAD);Expect(CreateWAD);Expect(ConvertWAD);Expect(UseWAD);++coverage[3];}Expect(EndProfile);Expect(LoadWorld);for(int n=0;n<plan.BusyCount;++n){Expect(Busy,1);Expect(Run);}Expect(Busy,0);if(plan.Drop){Expect(DeleteLoader);++coverage[4];}Expect(StartProfile,3);Expect(Progress,2);if(plan.Static){Expect(ConvertStatic);Expect(GetMap);Expect(CreateStatic);++coverage[5];}Expect(StartProfile,4);Expect(Progress,3);Expect(SetStatic);Expect(Progress,4);Expect(EndProfile);Expect(Close,1);if(plan.BusyCount)++coverage[6];else ++coverage[7];}
  {CIEngine engine;ActiveEngine=&engine;CDisplayEngine display(engine);CMainGameComponent component(display);CWorld world(component);ActiveMap=world.PWorldMap.get();CWideString level(InputText);world.Load(level);assert(NextEvent==EventCount);char expected[256];int n=0;for(;BaseName[n];++n)expected[n]=(char)BaseName[n];expected[n]=0;assert(strcmp(world.CurrentWorldName.Text,expected)==0);assert(BaseNew==BaseDelete);if(plan.Fail){assert(plan.StaticConversions==0&&plan.WadConversions==0&&plan.LoaderGets==0&&plan.SystemGets==1);}else{assert(plan.StaticConversions==1+(int)plan.Static&&plan.WadConversions==(plan.WAD?2:0)&&plan.LoaderGets==2*plan.BusyCount+1&&plan.Remaining==0&&!plan.SystemGets);assert(plan.LoaderDeleted==(plan.Drop?1:0));if(!plan.Drop)assert(ActiveMap->PLevelLoader.GetReferenceCount()==1);}plan.Teardown=true;}
  assert(WideAlive==0&&CharAlive==0&&BaseNew==BaseDelete&&plan.LoaderDeleted==1);++cases;
 }
 assert(cases==1024u);for(int n=0;n<8;++n)assert(coverage[n]>0);puts("004a1840_TEST PASS");return 0;}
