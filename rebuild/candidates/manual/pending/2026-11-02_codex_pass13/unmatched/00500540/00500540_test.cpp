#include <cassert>
#include <cstdio>
#include <string>
#include <vector>
#include <set>
static int def_refs=0,files_alive=0,objects_alive=0,pool_calls=0,run_calls=0;
static std::vector<std::string> events;
struct CWideString { std::string s; CWideString(const std::string& v=""):s(v){} };
struct CCharString {
 std::string s; CCharString(const char* p=""):s(p){} CCharString(const std::string& p):s(p){}
 CWideString ToWideString()const{return CWideString(s);} bool operator<(const CCharString& b)const{return s<b.s;}
};
static const CCharString MOUTH_DUMMY_OBJECT_NAME("mouth");
struct C3DVector { float x,y,z; C3DVector(float a=0,float b=0,float c=0):x(a),y(b),z(c){} };
static bool same(const C3DVector&a,const C3DVector&b){return a.x==b.x&&a.y==b.y&&a.z==b.z;}
template<class T> class CCountedPointer {
 struct Info { T* p; unsigned n; Info(T*v):p(v),n(1){} ~Info(){delete p;} }; Info* i;
 void release(){if(i&&!--i->n)delete i;i=0;}
public:
 CCountedPointer(T*p=0):i(p?new Info(p):0){} CCountedPointer(const CCountedPointer&b):i(b.i){if(i)++i->n;}
 ~CCountedPointer(){release();} CCountedPointer& operator=(const CCountedPointer&b){if(this!=&b){release();i=b.i;if(i)++i->n;}return *this;}
 void Reset(T*p){release();if(p)i=new Info(p);} T* Get()const{return i?i->p:0;} T* operator->()const{return Get();}
};
template<class T> struct CDefPointer {
 T*p; CDefPointer(T*v=0):p(v){if(p)++def_refs;} CDefPointer(const CDefPointer&v):p(v.p){if(p)++def_refs;}
 ~CDefPointer(){if(p)--def_refs;} T* Get()const{return p;} T*operator->()const{return p;}
};
template<class T> struct CIntelligentPointer {
 T*p; CIntelligentPointer(T*v=0):p(v){} CIntelligentPointer&operator=(T*v){p=v;return *this;}
 T*Get()const{return p;} T*operator->()const{return p;}
};
struct CRegionDef {
 long EnvironmentTheme,InternalEnvironmentTheme,InternalLightingChannelCount,AttachToCameraFX,HeroBreathParticle;
 bool EnableSpotlightShadowMode,HasSea,UseVillageTime;
};
struct CRegion {
 CRegionDef*def; CCharString name; std::vector<long> maps,vis;
 CDefPointer<const CRegionDef> GetPRegionDef()const{return CDefPointer<const CRegionDef>(def);}
 const CCharString&GetName()const{return name;} const std::vector<long>&GetMaps()const{return maps;} const std::vector<long>&GetVisMaps()const{return vis;}
};
struct CIEngine {
 std::vector<long> visible; int all,shadow_calls,sea_calls; bool shadow; CCharString sea;
 CIEngine():all(0),shadow_calls(0),sea_calls(0),shadow(false){}
 void SetMapVisibilityAllMaps(bool b){assert(!b);++all;visible.clear();}
 void SetMapVisibility(long n,bool b){assert(b);visible.push_back(n);}
 void SetSpotlightShadowMode(bool b){++shadow_calls;shadow=b;}
 void SetSeaBodyToRender(CCharString s){++sea_calls;sea=s;}
};
struct CDisplayEngine { CIEngine engine; CIEngine&Get3DEngine(){return engine;} };
struct CMainGameComponent {
 CDisplayEngine display; static CMainGameComponent*current; static CMainGameComponent*Get(){return current;}
 CDisplayEngine&GetDisplayEngine(){return display;}
}; CMainGameComponent*CMainGameComponent::current=0;
struct CEnvironment {
 long theme; int region_calls; std::vector<long> channels,internal;
 CEnvironment():theme(-99),region_calls(0){} void SetRegionTheme(long t){theme=t;++region_calls;}
 void SetTheme(long channel,long priority,long t){assert(priority==0);channels.push_back(channel);internal.push_back(t);}
};
struct CGameTimeManager { bool village; int calls; CGameTimeManager():village(false),calls(0){} void SetAsUsingVillageTime(bool b){village=b;++calls;} };
struct CCamera { C3DVector pos; const C3DVector&PeekWorldPos()const{return pos;} };
struct CAIGameCameraBase {
 CCamera camera; int calls; CAIGameCameraBase():calls(0){} CCamera GetRenderCamera(float t){assert(t==1.0f);++calls;return camera;}
};
struct CThing; struct CThingPlayerCreature;
struct CTCDParticleEmitter {
 CThing*owner; int camera_attach,thing_attach; CThingPlayerCreature*attached;
 CTCDParticleEmitter():owner(0),camera_attach(0),thing_attach(0),attached(0){}
 static CThing*Create(long,const C3DVector&,bool);
 void AttachToCamera(long n,float t){assert(n==8&&t==0);++camera_attach;}
 void AttachToThing(CThingPlayerCreature&h,long n,const CCharString&name,long p,float t){assert(n==10&&name.s=="mouth"&&p==0&&t==0);++thing_attach;attached=&h;}
};
enum ECutsceneBehaviour { CUTSCENE_BEHAVIOUR_NOT_PAUSED=2 };
struct CThing {
 bool dead,has_tc; int killed,pause; CTCDParticleEmitter emitter; C3DVector pos;
 CThing(bool d=false,bool t=true):dead(d),has_tc(t),killed(0),pause(0){emitter.owner=this;}
 bool IsDead()const{return dead;} void Kill(bool b){assert(!b);++killed;dead=true;}
 bool GetTC(CTCDParticleEmitter**out){if(has_tc)*out=&emitter;return has_tc;}
 void SetCutscenePauseBehaviour(ECutsceneBehaviour b){assert(b==CUTSCENE_BEHAVIOUR_NOT_PAUSED);++pause;}
 const C3DVector&GetPos()const{return pos;}
};
struct CThingPlayerCreature:CThing { CThingPlayerCreature(bool d=false):CThing(d,true){} };
struct Creation { long id; C3DVector pos; CThing*result; Creation(long i,const C3DVector&p,CThing*r):id(i),pos(p),result(r){} };
static int creation_mode=0;
static std::vector<Creation> creations;
static std::vector<CThing*> particles;
CThing*CTCDParticleEmitter::Create(long id,const C3DVector&p,bool b){assert(!b);CThing*t=0;if(creation_mode){t=new CThing(creation_mode==1,creation_mode==3);particles.push_back(t);}creations.push_back(Creation(id,p,t));return t;}
struct CPlayer { CThingPlayerCreature*hero; CThingPlayerCreature*GetPControlledCreature(){return hero;} };
struct CWorld {
 CEnvironment environment; CGameTimeManager time; CAIGameCameraBase camera; CPlayer player;
 CEnvironment&GetEnvironment(){return environment;} CGameTimeManager&GetGameTimeManager(){return time;}
 CAIGameCameraBase*GetGameCamera(){return &camera;} CPlayer*PeekMyPlayer(){return &player;}
};
enum EFileAccess { FILE_ACCESS_READ=0 };
struct CAFile {
 std::string name; int kind; CAFile(const CWideString&n,int k):name(n.s),kind(k){assert(def_refs==0);++files_alive;}
 virtual ~CAFile(){--files_alive;}
 static unsigned long GetFileModifiedTimeAsSomeWackyWin32BigNumber(const CWideString&n){unsigned long h=0;for(unsigned j=0;j<n.s.size();++j)h=(h*31+static_cast<unsigned char>(n.s[j]))%1000003;return h;}
};
struct CWADFile:CAFile { CWADFile(int&bank,const CWideString&n,EFileAccess a,long flags):CAFile(n,1){assert(bank==47&&a==FILE_ACCESS_READ&&flags==2);} };
struct CDiskFileWin32:CAFile { CDiskFileWin32(const CWideString&n,EFileAccess a,long flags):CAFile(n,0){assert(a==FILE_ACCESS_READ&&flags==2);} };
struct CLevelLoader {
 struct CObject {
  struct CLevelFiles { long LevelID; CCountedPointer<CAFile> PThingFile,PTopologyFile,PNavigationFile; };
  std::vector<CLevelFiles> RegionLevelFiles; std::set<CCharString> LoadableSections; long RegionIndex;
  CObject(){++objects_alive;} ~CObject(){--objects_alive;}
  void SetAsLoading(const std::vector<CLevelFiles>&f,const std::set<CCharString>&s,long r){RegionLevelFiles=f;LoadableSections=s;RegionIndex=r;}
 };
 CCountedPointer<CObject> saved; int busy,adds;
 CLevelLoader():busy(0),adds(0){} void Add(const CCountedPointer<CObject>&p){assert(def_refs==0);saved=p;++adds;events.push_back("add");}
 bool IsBusy()const{return busy!=0;} void RunSuspendableProcess(){assert(busy>0);--busy;++run_calls;events.push_back("run");}
};
struct CAsyncLoadingMemoryPool { static void ProcessLoading(){++pool_calls;events.push_back("pool");} };
struct CQuestManager {
 std::vector<std::string> names; static CQuestManager singleton; static CQuestManager&Get(){return singleton;}
 void OnRegionLoaded(const CCharString&n){names.push_back(n.s);events.push_back("quest");}
}; CQuestManager CQuestManager::singleton;
struct CMapInfo { CCharString LevelName; unsigned long LevStamp,TngStamp; CMapInfo():LevStamp(77),TngStamp(88){} };
struct CWorldMap {
 enum ELoadType { WAIT_FOR_LOAD=0,ASYNC=1 };
 CWorld&World; std::vector<CRegion> Regions; std::vector<CMapInfo> Maps; bool UseLevelWADBank; int*LevelWADBank;
 CIntelligentPointer<CThing> PCameraRegionParticle,PHeroBreathParticle; CLevelLoader*PLevelLoader; std::set<CCharString> sections;
 CWorldMap(CWorld&w):World(w){} std::set<CCharString>GetLoadableSections()const{return sections;}
 static CWideString GetThingFilenameFromLevelFilename(const CWideString&s){return CWideString(s.s+".things");}
 void LoadRegion(long,ELoadType,bool);
};
void CWorldMap::LoadRegion(long index,ELoadType load_type,bool load_navigation)
{
    CIEngine& engine=CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine();
    engine.SetMapVisibilityAllMaps(false);
    {
        CDefPointer<const CRegionDef> pregion_def=Regions[index].GetPRegionDef();
        if(pregion_def.Get()) {
            if(pregion_def->EnvironmentTheme>0) World.GetEnvironment().SetRegionTheme(pregion_def->EnvironmentTheme);
            if(pregion_def->InternalEnvironmentTheme>0) {
                for(long n=0;n<pregion_def->InternalLightingChannelCount;++n)
                    World.GetEnvironment().SetTheme(n+1,0,pregion_def->InternalEnvironmentTheme);
            }
            engine.SetSpotlightShadowMode(pregion_def->EnableSpotlightShadowMode);
            if(pregion_def->HasSea) engine.SetSeaBodyToRender(Regions[index].GetName());
            else engine.SetSeaBodyToRender("");
            if(PCameraRegionParticle.Get() && !PCameraRegionParticle->IsDead()) PCameraRegionParticle->Kill(false);
            if(pregion_def->AttachToCameraFX>0) {
                C3DVector camera_pos=World.GetGameCamera()->GetRenderCamera(1.0f).PeekWorldPos();
                PCameraRegionParticle=CTCDParticleEmitter::Create(pregion_def->AttachToCameraFX,camera_pos,false);
                if(PCameraRegionParticle.Get() && !PCameraRegionParticle->IsDead()) {
                    CTCDParticleEmitter* pparticle_driver;
                    if(PCameraRegionParticle->GetTC(&pparticle_driver)) {
                        pparticle_driver->AttachToCamera(8,0.0f);
                        PCameraRegionParticle->SetCutscenePauseBehaviour(CUTSCENE_BEHAVIOUR_NOT_PAUSED);
                    }
                }
            }
            if(PHeroBreathParticle.Get() && !PHeroBreathParticle->IsDead()) PHeroBreathParticle->Kill(false);
            if(pregion_def->HeroBreathParticle>0) {
                CThingPlayerCreature* phero=World.PeekMyPlayer()->GetPControlledCreature();
                if(phero && !phero->IsDead()) {
                    PHeroBreathParticle=CTCDParticleEmitter::Create(pregion_def->HeroBreathParticle,phero->GetPos(),false);
                    if(PHeroBreathParticle.Get() && !PHeroBreathParticle->IsDead()) {
                        CTCDParticleEmitter* pparticle_driver;
                        if(PHeroBreathParticle->GetTC(&pparticle_driver))
                            pparticle_driver->AttachToThing(*phero,10,MOUTH_DUMMY_OBJECT_NAME,0,0.0f);
                    }
                }
            }
            World.GetGameTimeManager().SetAsUsingVillageTime(pregion_def->UseVillageTime);
        } else World.GetGameTimeManager().SetAsUsingVillageTime(false);
    }
    std::vector<CLevelLoader::CObject::CLevelFiles> level_files;
    const std::vector<long>& region_maps=Regions[index].GetMaps();
    for(long c1=0;c1<region_maps.size();++c1) {
        engine.SetMapVisibility(region_maps[c1],true);
        CWideString thing_filename=GetThingFilenameFromLevelFilename(Maps[region_maps[c1]].LevelName.ToWideString());
        CCountedPointer<CAFile> pmap_file;
        CCountedPointer<CAFile> pthing_file;
        Maps[index].LevStamp=CAFile::GetFileModifiedTimeAsSomeWackyWin32BigNumber(Maps[region_maps[c1]].LevelName.ToWideString());
        Maps[index].TngStamp=CAFile::GetFileModifiedTimeAsSomeWackyWin32BigNumber(thing_filename);
        if(UseLevelWADBank) {
            pmap_file.Reset(new CWADFile(*LevelWADBank,Maps[region_maps[c1]].LevelName.ToWideString(),FILE_ACCESS_READ,2));
            pthing_file.Reset(new CWADFile(*LevelWADBank,thing_filename,FILE_ACCESS_READ,2));
        } else {
            pmap_file.Reset(new CDiskFileWin32(Maps[region_maps[c1]].LevelName.ToWideString(),FILE_ACCESS_READ,2));
            pthing_file.Reset(new CDiskFileWin32(thing_filename,FILE_ACCESS_READ,2));
        }
        level_files.push_back(CLevelLoader::CObject::CLevelFiles());
        level_files.back().LevelID=region_maps[c1];
        level_files.back().PThingFile=pthing_file;
        level_files.back().PTopologyFile=pmap_file;
        if(load_navigation) level_files.back().PNavigationFile=pmap_file;
    }
    const std::vector<long>& visible_maps=Regions[index].GetVisMaps();
    for(long c1=0;c1<visible_maps.size();++c1) engine.SetMapVisibility(visible_maps[c1],true);
    CCountedPointer<CLevelLoader::CObject> pload_object=new CLevelLoader::CObject;
    std::set<CCharString> section_names=GetLoadableSections();
    pload_object->SetAsLoading(level_files,section_names,index);
    PLevelLoader->Add(pload_object);
    if(load_type==WAIT_FOR_LOAD) {
        for(;;) {
            if(!PLevelLoader->IsBusy()) break;
            CAsyncLoadingMemoryPool::ProcessLoading();
            PLevelLoader->RunSuspendableProcess();
        }
    }
    CQuestManager::Get().OnRegionLoaded(Regions[index].GetName());
}

int main(){
 for(unsigned scenario=0;scenario<12288;++scenario){
  assert(def_refs==0&&files_alive==0&&objects_alive==0);pool_calls=run_calls=0;events.clear();creations.clear();CQuestManager::Get().names.clear();
  CMainGameComponent component;CMainGameComponent::current=&component;CWorld world;CLevelLoader loader;int bank=47;
  CThing old_camera((scenario/3)%3==1),old_hero((scenario/11)%3==1);CThingPlayerCreature hero((scenario/17)%3==1);hero.pos=C3DVector(7,8,9);world.player.hero=(scenario/17)%3==0?0:&hero;world.camera.camera.pos=C3DVector(1,2,3);
  CRegionDef def;def.EnvironmentTheme=static_cast<long>((scenario/5)%3)-1;def.InternalEnvironmentTheme=static_cast<long>((scenario/13)%3)-1;
  def.InternalLightingChannelCount=static_cast<long>((scenario/7)%5)-1;def.AttachToCameraFX=(scenario&1)?21:0;def.HeroBreathParticle=(scenario&2)?22:-1;
  def.EnableSpotlightShadowMode=(scenario&4)!=0;def.HasSea=(scenario&8)!=0;def.UseVillageTime=(scenario&16)!=0;
  const bool has_def=(scenario%7)!=0,wad=(scenario&32)!=0,nav=(scenario&64)!=0,wait=(scenario&128)!=0;
  creation_mode=(scenario/37)%4;loader.busy=(scenario/19)%4;const int initial_busy=loader.busy;
  {
   CWorldMap map(world);map.PLevelLoader=&loader;map.LevelWADBank=&bank;map.UseLevelWADBank=wad;map.Maps.resize(4);map.Regions.resize(2);
   map.Maps[0].LevelName="zero";map.Maps[1].LevelName="one";map.Maps[2].LevelName="two";map.Maps[3].LevelName="three";
   map.Regions[1].def=has_def?&def:0;map.Regions[1].name="region-one";
   const unsigned count=(scenario/23)%5;for(unsigned j=0;j<count;++j)map.Regions[1].maps.push_back(j%2?0:3);
   const unsigned visible=(scenario/29)%4;for(unsigned j=0;j<visible;++j)map.Regions[1].vis.push_back(j%2?3:2);
   map.sections.insert("alpha");if(scenario&256)map.sections.insert("beta");
   const bool camera_present=(scenario/3)%3!=0,hero_particle_present=(scenario/11)%3!=0;
   const bool camera_was_dead=old_camera.dead,hero_particle_was_dead=old_hero.dead;
   map.PCameraRegionParticle=camera_present?&old_camera:0;map.PHeroBreathParticle=hero_particle_present?&old_hero:0;
   map.LoadRegion(1,wait?CWorldMap::WAIT_FOR_LOAD:CWorldMap::ASYNC,nav);
   CIEngine&e=component.display.engine;assert(e.all==1&&e.visible.size()==count+visible);
   for(unsigned j=0;j<count;++j)assert(e.visible[j]==map.Regions[1].maps[j]);for(unsigned j=0;j<visible;++j)assert(e.visible[count+j]==map.Regions[1].vis[j]);
   assert(e.shadow_calls==(has_def?1:0)&&e.sea_calls==(has_def?1:0));if(has_def){assert(e.shadow==def.EnableSpotlightShadowMode);assert(e.sea.s==(def.HasSea?"region-one":""));}
   assert(world.environment.region_calls==(has_def&&def.EnvironmentTheme>0?1:0));if(world.environment.region_calls)assert(world.environment.theme==def.EnvironmentTheme);
   const unsigned channels=has_def&&def.InternalEnvironmentTheme>0&&def.InternalLightingChannelCount>0?def.InternalLightingChannelCount:0;
   assert(world.environment.channels.size()==channels);for(unsigned j=0;j<channels;++j){assert(world.environment.channels[j]==j+1);assert(world.environment.internal[j]==def.InternalEnvironmentTheme);}
   assert(world.time.calls==1&&world.time.village==(has_def&&def.UseVillageTime));
   assert(old_camera.killed==(has_def&&camera_present&&!camera_was_dead?1:0));assert(old_hero.killed==(has_def&&hero_particle_present&&!hero_particle_was_dead?1:0));
   const bool camera_create=has_def&&def.AttachToCameraFX>0,hero_create=has_def&&def.HeroBreathParticle>0&&world.player.hero&&!hero.dead;
   assert(creations.size()==static_cast<unsigned>(camera_create+hero_create));assert(world.camera.calls==(camera_create?1:0));
   unsigned ci=0;if(camera_create){const Creation&c=creations[ci++];assert(c.id==21&&same(c.pos,world.camera.camera.pos)&&map.PCameraRegionParticle.Get()==c.result);if(c.result){assert(c.result->emitter.camera_attach==(creation_mode==3?1:0));assert(c.result->pause==(creation_mode==3?1:0));}}
   else assert(map.PCameraRegionParticle.Get()==(camera_present?&old_camera:0));
   if(hero_create){const Creation&c=creations[ci++];assert(c.id==22&&same(c.pos,hero.pos)&&map.PHeroBreathParticle.Get()==c.result);if(c.result){assert(c.result->emitter.thing_attach==(creation_mode==3?1:0));assert(c.result->pause==0);if(creation_mode==3)assert(c.result->emitter.attached==&hero);}}
   else assert(map.PHeroBreathParticle.Get()==(hero_particle_present?&old_hero:0));
   assert(loader.adds==1&&loader.saved.Get()&&loader.saved->RegionIndex==1&&loader.saved->RegionLevelFiles.size()==count);assert(loader.saved->LoadableSections.size()==map.sections.size());
   assert(loader.saved->LoadableSections.find("alpha")!=loader.saved->LoadableSections.end());assert((loader.saved->LoadableSections.find("beta")!=loader.saved->LoadableSections.end())==((scenario&256)!=0));
   for(unsigned j=0;j<count;++j){const CLevelLoader::CObject::CLevelFiles&f=loader.saved->RegionLevelFiles[j];long id=map.Regions[1].maps[j];assert(f.LevelID==id&&f.PTopologyFile.Get()&&f.PThingFile.Get());assert(f.PTopologyFile->name==map.Maps[id].LevelName.s&&f.PThingFile->name==map.Maps[id].LevelName.s+".things");assert(f.PTopologyFile->kind==(wad?1:0)&&f.PThingFile->kind==(wad?1:0));assert(f.PNavigationFile.Get()==(nav?f.PTopologyFile.Get():0));}
   assert(files_alive==static_cast<int>(2*count)&&objects_alive==1);for(unsigned j=0;j<4;++j)if(j!=1||!count){assert(map.Maps[j].LevStamp==77&&map.Maps[j].TngStamp==88);}
   if(count){long last=map.Regions[1].maps.back();assert(map.Maps[1].LevStamp==CAFile::GetFileModifiedTimeAsSomeWackyWin32BigNumber(map.Maps[last].LevelName.ToWideString()));assert(map.Maps[1].TngStamp==CAFile::GetFileModifiedTimeAsSomeWackyWin32BigNumber(CWideString(map.Maps[last].LevelName.s+".things")));}
   assert(pool_calls==(wait?initial_busy:0)&&run_calls==pool_calls&&loader.busy==(wait?0:initial_busy));assert(events.size()==static_cast<unsigned>(2+pool_calls*2)&&events.front()=="add"&&events.back()=="quest");for(int j=0;j<pool_calls;++j){assert(events[1+j*2]=="pool"&&events[2+j*2]=="run");}
   assert(CQuestManager::Get().names.size()==1&&CQuestManager::Get().names[0]=="region-one");assert(def_refs==0);
  }
  loader.saved.Reset(0);assert(files_alive==0&&objects_alive==0);for(unsigned j=0;j<particles.size();++j)delete particles[j];particles.clear();
 }
 std::puts("00500540_TEST PASS");return 0;
}
