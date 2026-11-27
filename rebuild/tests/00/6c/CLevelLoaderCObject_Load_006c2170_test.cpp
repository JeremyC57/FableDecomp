#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <set>
#include "engine/CCountedPointer.h"
#include "engine/EMessageEventType.h"
#include "engine/C3DVector.h"
static void check(bool x){if(!x)std::exit(2);}
class CCharString{
public:const char* text;
 CCharString(const char* s=""):text(s){} bool operator<(const CCharString& u)const{return std::strcmp(text,u.text)<0;}
};
class CThing{};class CAFile{public:explicit CAFile(int i):id(i){} int id;};
class CASuspendableProcess{};
static int trace[256],next_call,expected[256],total;
static void event(int kind,int id=0){check(next_call<256);trace[next_call++]=kind*1000+id;}
static void want(int k,int i=0){check(total<256);expected[total++]=k*1000+i;}
static CASuspendableProcess process;static bool editor;static unsigned st,count_loaded;
static CThing objects[64];static void* worldNameAddress;
static const char* const labels[]={"Loading topology","Post load topology","Post load topology end","Loading objects","Region Level Files: Post Load Initialise","Region Level Files: Activate Topology"};
static double GFGetTime(){event(1);return next_call*0.5;}
namespace NProgressDisplay{
 void DisplayProgress(const CCharString& name,float value,bool first,bool second){
  check(value==-1.f&&!first&&!second);
  unsigned k=0;while(k<6 && std::strcmp(labels[k],name.text))++k;check(k<6);event(2+k);
 }
}
class MapManager{public:void AddGameMap(long id){event(10,id);}};
class CWorldMap{
public:MapManager maps;MapManager& GetThingMapManager(){return maps;}
 void LoadTopologyFromFile(long id,CCountedPointer<CAFile> f,CASuspendableProcess& p){
  check(&p==&process && f->id==id && f.GetReferenceCount()==2);event(8,id);}
 void PostLoadTopology(long id){event(11,id);}
 void CreateNavMap(long id,const std::set<CCharString>& names,CAFile& f,CASuspendableProcess& p){
  check(names.size()==st && &p==&process && f.id==id+50);event(14,id);}
 void ActivateNavMap(long id){event(15,id);}
 void PostLoadInitialise(long id){event(17,id);}
 void ActivateTopology(long id){event(18,id);}
 void SetMapAsLoading(long id,bool loading){check(!loading);event(19,id);}
 void PostRegionLoad(long id){event(22,id);}
 void SetRegionAsLoaded(long id,const CCharString& str){check(std::strcmp(str.text,"Albion")==0 && worldNameAddress!=&str);event(23,id);}
};
class CThingManager{
public:void RuntimeLoadEntities(long id,const std::set<CCharString>& sections,std::vector<CThing*>& things){
 check(sections.size()==st && things.size()==count_loaded);things.push_back(&objects[count_loaded]);++count_loaded;event(12,id);}
 void LoadFromFile(CCountedPointer<CAFile> f,long id,std::set<CCharString> sections,std::vector<CThing*>& things,CASuspendableProcess* p,int mode){
  check(f->id==id+100 && f.GetReferenceCount()==2 && sections.size()==st && p==&process && mode==3);
  sections.clear();check(things.size()==count_loaded);things.push_back(&objects[count_loaded]);++count_loaded;event(13,id);}
 void ActivateThings(const std::vector<CThing*>& things){
  check(things.size()==count_loaded);for(unsigned i=0;i<count_loaded;++i)check(things[i]==&objects[i]);event(16);}
};
class CMainGameComponent{public:static CMainGameComponent* Get(){static CMainGameComponent c;return &c;}
 bool IsEditorActive()const{event(20);return editor;}};
class CMessageEventManager{public:
 void AddMessage(EMessageEventType type,unsigned long n,const C3DVector& v,CThing* p,CThing* q,float f,float g,long id){
  check(n==50&&v.X==0&&v.Y==0&&v.Z==0&&!p&&!q&&f==0&&g==0);
  check(type==MESSAGE_EVENT_LEVEL_LOADED||type==MESSAGE_EVENT_REGION_LOADED);event(type==MESSAGE_EVENT_LEVEL_LOADED?21:24,id);}
};
class CWorld{public:CCharString name;CMessageEventManager msg;CWorld():name("Albion"){}
 CMessageEventManager& GetMessageEventManager(){return msg;}
 const CCharString& GetCurrentWorldName()const{event(25);return name;}
};
class CLevelLoader{
public:CWorldMap* WorldMap;CThingManager* ThingManager;CWorld* World;
 class CObject{public:
 struct CLevelFiles{long LevelID;CCountedPointer<CAFile> PTopologyFile,PNavigationFile,PThingFile;};
 std::set<CCharString> LoadableSections;CLevelLoader* PLevelLoader;std::vector<CLevelFiles> RegionLevelFiles;long RegionIndex;
 void Load(CASuspendableProcess& suspendable_process);
 };
};
void CLevelLoader::CObject::Load(CASuspendableProcess& suspendable_process)
{
    const double start_time = GFGetTime();
    for (long c1 = 0; c1 < RegionLevelFiles.size(); ++c1) {
        const CCountedPointer<CAFile>& ptopology_file = RegionLevelFiles[c1].PTopologyFile;
        long level_id = RegionLevelFiles[c1].LevelID;
        if (!ptopology_file.IsNull()) {
            NProgressDisplay::DisplayProgress("Loading topology", -1.0f, false, false);
            PLevelLoader->WorldMap->LoadTopologyFromFile(level_id, ptopology_file, suspendable_process);
            PLevelLoader->WorldMap->GetThingMapManager().AddGameMap(level_id);
            NProgressDisplay::DisplayProgress("Post load topology", -1.0f, false, false);
            PLevelLoader->WorldMap->PostLoadTopology(level_id);
            NProgressDisplay::DisplayProgress("Post load topology end", -1.0f, false, false);
        }
    }
    std::vector<CThing*> loaded_thing_list;
    for (long c1 = 0; c1 < RegionLevelFiles.size(); ++c1) {
        const CCountedPointer<CAFile>& pthing_file = RegionLevelFiles[c1].PThingFile;
        if (!pthing_file.IsNull()) {
            NProgressDisplay::DisplayProgress("Loading objects", -1.0f, false, false);
            long level_id = RegionLevelFiles[c1].LevelID;
            PLevelLoader->ThingManager->RuntimeLoadEntities(level_id, LoadableSections, loaded_thing_list);
            PLevelLoader->ThingManager->LoadFromFile(pthing_file, level_id, LoadableSections, loaded_thing_list, &suspendable_process, 3);
        }
    }
    for (long c1 = 0; c1 < RegionLevelFiles.size(); ++c1) {
        const CCountedPointer<CAFile>& pnavigation_file = RegionLevelFiles[c1].PNavigationFile;
        long level_id = RegionLevelFiles[c1].LevelID;
        if (!pnavigation_file.IsNull())
            PLevelLoader->WorldMap->CreateNavMap(level_id, LoadableSections, *pnavigation_file, suspendable_process);
    }
    for (long c1 = 0; c1 < RegionLevelFiles.size(); ++c1) {
        if (!RegionLevelFiles[c1].PNavigationFile.IsNull())
            PLevelLoader->WorldMap->ActivateNavMap(RegionLevelFiles[c1].LevelID);
    }
    PLevelLoader->ThingManager->ActivateThings(loaded_thing_list);
    for (long c1 = 0; c1 < RegionLevelFiles.size(); ++c1) {
        NProgressDisplay::DisplayProgress("Region Level Files: Post Load Initialise", -1.0f, false, false);
        long level_id = RegionLevelFiles[c1].LevelID;
        PLevelLoader->WorldMap->PostLoadInitialise(level_id);
    }
    for (long c1 = 0; c1 < RegionLevelFiles.size(); ++c1) {
        NProgressDisplay::DisplayProgress("Region Level Files: Activate Topology", -1.0f, false, false);
        long level_id = RegionLevelFiles[c1].LevelID;
        if (!RegionLevelFiles[c1].PTopologyFile.IsNull())
            PLevelLoader->WorldMap->ActivateTopology(level_id);
        PLevelLoader->WorldMap->SetMapAsLoading(level_id, false);
        if (!CMainGameComponent::Get()->IsEditorActive())
            PLevelLoader->World->GetMessageEventManager().AddMessage(MESSAGE_EVENT_LEVEL_LOADED, 50, C3DVector(0, 0, 0), 0, 0, 0, 0, level_id);
    }
    if (RegionIndex > 0) {
        if (!CMainGameComponent::Get()->IsEditorActive())
            PLevelLoader->World->GetMessageEventManager().AddMessage(MESSAGE_EVENT_REGION_LOADED, 50, C3DVector(0, 0, 0), 0, 0, 0, 0, RegionIndex);
        PLevelLoader->WorldMap->PostRegionLoad(RegionIndex);
        CCharString world_name = PLevelLoader->World->GetCurrentWorldName();
        PLevelLoader->WorldMap->SetRegionAsLoaded(RegionIndex, world_name);
    }
    const double end_time = GFGetTime();
}
int main(){
 CWorldMap map;CThingManager things;CWorld world;CLevelLoader loader;
 loader.WorldMap=&map;loader.ThingManager=&things;loader.World=&world;worldNameAddress=&world.name;
 for(unsigned n=0;n<5;++n)for (unsigned mask=0;mask<64;++mask)
 for(int region=-1;region<=1;++region)for(unsigned ed=0;ed<2;++ed)for(st=0;st<3;++st){
  CLevelLoader::CObject o;o.PLevelLoader=&loader;o.RegionIndex=region*4;editor=ed!=0;
  if(st>0)o.LoadableSections.insert(CCharString("terrain"));if(st>1)o.LoadableSections.insert(CCharString("shops"));
  bool topo[4],nav[4],obj[4];o.RegionLevelFiles.resize(n);
  for(unsigned i=0;i<n;++i){unsigned shift=(i%2)*3;int id=9+i;
   topo[i]=(mask&(1U<<shift))!=0;nav[i]=(mask&(2U<<shift))!=0;obj[i]=(mask&(4U<<shift))!=0;
   CLevelLoader::CObject::CLevelFiles& e=o.RegionLevelFiles[i];e.LevelID=id;
   if(topo[i])e.PTopologyFile.Reset(new CAFile(id));
   if(nav[i])e.PNavigationFile.Reset(new CAFile(id+50));
   if(obj[i])e.PThingFile.Reset(new CAFile(id+100));
  }
  next_call=total=count_loaded=0;want(1);
  for(unsigned i=0;i<n;++i)if(topo[i]){int id=9+i;want(2);want(8,id);want(10,id);want(3);want(11,id);want(4);}
  for(unsigned i=0;i<n;++i)if(obj[i]){want(5);want(12,9+i);want(13,9+i);}
  for(unsigned i=0;i<n;++i)if(nav[i])want(14,9+i);
  for(unsigned i=0;i<n;++i)if(nav[i])want(15,9+i);
  want(16);for(unsigned i=0;i<n;++i){want(6);want(17,9+i);}
  for(unsigned i=0;i<n;++i){want(7);if(topo[i])want(18,9+i);want(19,9+i);want(20);if(!editor)want(21,9+i);}
  if(region>0){want(20);if(!editor)want(24,region*4);want(22,region*4);want(25);want(23,region*4);}
  want(1);o.Load(process);check(total==next_call && !std::memcmp(trace,expected,total*sizeof(int)));check(o.LoadableSections.size()==st);
  for(unsigned i=0;i<n;++i){const CLevelLoader::CObject::CLevelFiles& e=o.RegionLevelFiles[i];check(e.PTopologyFile.GetReferenceCount()==unsigned(topo[i])&&e.PNavigationFile.GetReferenceCount()==unsigned(nav[i])&&e.PThingFile.GetReferenceCount()==unsigned(obj[i]));}
 }
 std::puts("006c2170_TEST PASS");return 0;
}