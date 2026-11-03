#include <vector>
#include <set>
#include <string>
#include <cassert>
#include <cstdio>
class CAFile;
static std::vector<int> events;
static std::string written;
static int writes;
class CCharString {public:std::string text;CCharString(){}CCharString(const char*s):text(s){}CCharString(const std::string&s):text(s){}bool operator<(const CCharString&s)const{return text<s.text;}bool operator!=(const char*s)const{return text!=s;}const CCharString&operator+=(const CCharString&s){text+=s.text;return*this;}const CCharString&operator+=(const char*s){text+=s;return*this;}void SaveToFile(CAFile&)const;};
static CCharString operator+(const char*a,const CCharString&b){return CCharString(std::string(a)+b.text);}
static CCharString operator+(const CCharString&a,const char*b){return CCharString(a.text+b);}
class C3DVector {public:float X,Y,Z;C3DVector():X(0),Y(0),Z(0){}};
class CThing;
class CTCDRegionExit {public:CCharString name;mutable int reads;CTCDRegionExit():reads(0){}static int StatGetInterfaceType(){return 100;}const CCharString&GetNameOfRegionConnectingTo()const{++reads;return name;}};
class CTCActionUseScriptedHook {public:CThing*entrance;mutable int reads;CTCActionUseScriptedHook():entrance(0),reads(0){}CThing*GetPEntranceConnectedTo()const{++reads;return entrance;}};
class CThing {public:long map;bool dead;C3DVector pos;CTCDRegionExit*exit;CTCActionUseScriptedHook*hook;mutable int map_reads,pos_reads,peek_calls,get_calls;CThing():map(0),dead(false),exit(0),hook(0),map_reads(0),pos_reads(0),peek_calls(0),get_calls(0){}long GetCurrentMapNumber()const{++map_reads;return map;}bool IsDead()const{return dead;}const C3DVector&GetPos()const{++pos_reads;return pos;}template<class T>bool PeekTC(T**out)const{++peek_calls;assert(exit);*out=exit;return true;}template<class T>bool GetTC(T**out){++get_calls;assert(hook);*out=hook;return true;}};
class CThingPlayerCreature:public CThing {public:bool limbo;CThingPlayerCreature():limbo(false){}void SetInLimbo(bool v){limbo=v;events.push_back(v?1:2);}};
class CPlayer {public:CThingPlayerCreature*hero;CPlayer():hero(0){}CThingPlayerCreature*GetPControlledCreature()const{return hero;}};
class CPlayerManager {public:CPlayer player;CPlayer&GetMainPlayer(){return player;}};
class CMainGameComponent {public:CPlayerManager manager;static CMainGameComponent*instance;static CMainGameComponent*Get(){return instance;}CPlayerManager&GetPlayerManager(){return manager;}};
CMainGameComponent*CMainGameComponent::instance=0;
class CThingFilter_HasTC {public:int type;explicit CThingFilter_HasTC(int v):type(v){assert(v==100);}bool operator()(CThing&t)const{return t.exit!=0;}};
template<class T>class CThingFilter_HasTCOfLeafType {public:bool operator()(CThing&t)const{return t.hook!=0;}};
class CThingSearchTools {public:long loaded;std::vector<CThing*>things[5];CThingSearchTools():loaded(0){}template<class T,class C,class F>void GetAllThings(const F&f,C&out)const{assert(out.empty());assert(loaded>0&&loaded<5);events.push_back(300+(int)loaded);for(unsigned i=0;i<things[loaded].size();++i)if(f(*things[loaded][i]))out.push_back(things[loaded][i]);}};
class CWorld {public:CThingSearchTools search;CThingSearchTools&GetThingSearchTools(){return search;}};
class CRegion {public:CCharString name;const CCharString&GetName()const{return name;}};
class CAFile {public:bool open;CAFile():open(false){}virtual~CAFile(){}};
enum FileAccessType {FILE_ACCESS_CREATE=4};
class CDiskFileWin32:public CAFile {public:CDiskFileWin32(const wchar_t*p,FileAccessType a,unsigned long f){assert(std::wstring(p)==L"RegionGraph.txt"&&a==FILE_ACCESS_CREATE&&f==2);assert(writes==0);open=true;events.push_back(400);}~CDiskFileWin32(){assert(open);open=false;events.push_back(500);}};
void CCharString::SaveToFile(CAFile&f)const{assert(f.open);written=text;++writes;events.push_back(401);}
class CWorldMap {public:enum ELoadType{WAIT_FOR_LOAD=0};CWorld&World;std::vector<CRegion>Regions;long LoadedRegion;bool kill_on_load;long old_region;mutable int map_at_calls;CWorldMap(CWorld&w):World(w),LoadedRegion(0),kill_on_load(false),old_region(0),map_at_calls(0){}void LoadRegion(long r,ELoadType e,bool restore){assert(e==WAIT_FOR_LOAD);if(restore){assert(r==old_region&&writes==1);events.push_back(600+(int)r);}else{assert(r>0&&r<(long)Regions.size());events.push_back(200+(int)r);}LoadedRegion=r;World.search.loaded=r;if(kill_on_load&&CMainGameComponent::Get()->manager.player.hero)CMainGameComponent::Get()->manager.player.hero->dead=true;}void UnloadRegion(long r,bool flag){assert(!flag&&r==LoadedRegion);events.push_back(100+(int)r);LoadedRegion=0;World.search.loaded=0;}long GetRegionNumberMapIsIn(long m)const{assert(m>=0&&m<500);return m/100;}virtual long GetMapNumberAt(const C3DVector&p)const{++map_at_calls;return(long)p.X;}void GenerateRegionConnectivityGraph();};

void CWorldMap::GenerateRegionConnectivityGraph()
{
    CThingPlayerCreature* phero=CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    if(phero && !phero->IsDead()) phero->SetInLimbo(true);
    long current_region=LoadedRegion;
    UnloadRegion(current_region,false);
    CCharString graph_data;
    for(long region_index=1;region_index<Regions.size();++region_index) {
        LoadRegion(region_index,WAIT_FOR_LOAD,false);
        std::vector<CThing*> region_exits;
        World.GetThingSearchTools().GetAllThings<CThing>(CThingFilter_HasTC(CTCDRegionExit::StatGetInterfaceType()),region_exits);
        std::set<CCharString> regions_connecting_to;
        for(long c1=0;c1<region_exits.size();++c1) {
            if(GetRegionNumberMapIsIn(region_exits[c1]->GetCurrentMapNumber())==region_index) {
                const CTCDRegionExit* ptc_region_exit=0;
                region_exits[c1]->PeekTC(&ptc_region_exit);
                const CCharString& region_connecting_to_name=ptc_region_exit->GetNameOfRegionConnectingTo();
                if(region_connecting_to_name!="") regions_connecting_to.insert(region_connecting_to_name);
            }
        }
        region_exits.clear();
        World.GetThingSearchTools().GetAllThings<CThing>(CThingFilter_HasTCOfLeafType<CTCActionUseScriptedHook>(),region_exits);
        for(long c1=0;c1<region_exits.size();++c1) {
            if(GetRegionNumberMapIsIn(region_exits[c1]->GetCurrentMapNumber())==region_index) {
                const CTCActionUseScriptedHook* ptc_scripted_hook;
                region_exits[c1]->GetTC(&ptc_scripted_hook);
                CThing* pentrance=ptc_scripted_hook->GetPEntranceConnectedTo();
                if(pentrance && !pentrance->IsDead())
                    regions_connecting_to.insert(Regions[GetRegionNumberMapIsIn(GetMapNumberAt(pentrance->GetPos()))].GetName());
            }
        }
        if(!regions_connecting_to.empty()) {
            graph_data+="\""+Regions[region_index].GetName()+"\": ";
            std::set<CCharString>::iterator last_entry=regions_connecting_to.end();
            --last_entry;
            for(std::set<CCharString>::iterator it=regions_connecting_to.begin();it!=regions_connecting_to.end();++it) {
                graph_data+="\""+*it+"\"";
                if(it!=last_entry) graph_data+=", ";
            }
            graph_data+=";\r\n";
        }
        UnloadRegion(region_index,false);
    }
    CDiskFileWin32 graph_file(L"RegionGraph.txt",FILE_ACCESS_CREATE,2);
    graph_data.SaveToFile(graph_file);
    LoadRegion(current_region,WAIT_FOR_LOAD,true);
    if(phero && !phero->IsDead()) phero->SetInLimbo(false);
}

static void run_case(unsigned mask,int region_count,int hero_mode,int current,int reverse_order)
{
 events.clear();written="old";writes=0;
 CMainGameComponent component;CMainGameComponent::instance=&component;
 CThingPlayerCreature hero;hero.dead=hero_mode==1;component.manager.player.hero=hero_mode==0?0:&hero;
 CWorld world;CWorldMap map(world);map.Regions.resize(region_count);const char*names[5]={"Zero","Zulu","Alpha","Alpha","Name with spaces"};
 for(int r=0;r<region_count;++r)map.Regions[r].name=CCharString(names[r]);
 map.LoadedRegion=current;map.old_region=current;world.search.loaded=current;map.kill_on_load=hero_mode==3;
 CTCDRegionExit exits[5][5];CTCActionUseScriptedHook hooks[5][4];CThing things[5][10],entrances[5][4];
 std::vector<std::string> expected_lines(region_count);int expected_positions=0;
 for(int r=1;r<region_count;++r){
  std::set<std::string> expected;
  const char*dest[5]={"Beta","Alpha","Beta","","Ignored"};
  for(int i=0;i<5;++i){exits[r][i].name=CCharString(dest[i]);things[r][i].map=(i==4?0:r)*100+7;things[r][i].exit=&exits[r][i];if(mask&(1u<<i)){world.search.things[r].push_back(&things[r][i]);if(i<4&&*dest[i])expected.insert(dest[i]);}}
  for(int j=0;j<4;++j){int index=5+j;things[r][index].map=r*100+9;things[r][index].hook=&hooks[r][j];entrances[r][j].pos.X=(float)((region_count>1?(j%(region_count-1))+1:0)*100+3);entrances[r][j].dead=j==2;hooks[r][j].entrance=j==1?0:&entrances[r][j];if(mask&(1u<<(5+j))){world.search.things[r].push_back(&things[r][index]);if(j!=1&&j!=2){expected.insert(names[(int)entrances[r][j].pos.X/100]);++expected_positions;}}}
  // A matching hook whose owning thing belongs to another region must be ignored.
  things[r][9].map=0;things[r][9].hook=&hooks[r][0];world.search.things[r].push_back(&things[r][9]);
  if(reverse_order){std::vector<CThing*>&v=world.search.things[r];for(unsigned i=0;i<v.size()/2;++i){CThing*t=v[i];v[i]=v[v.size()-1-i];v[v.size()-1-i]=t;}}
  if(!expected.empty()){std::string line="\""+std::string(names[r])+"\": ";for(std::set<std::string>::const_iterator it=expected.begin();it!=expected.end();++it){if(it!=expected.begin())line+=", ";line+="\""+*it+"\"";}expected_lines[r]=line+";\r\n";}
 }
 map.GenerateRegionConnectivityGraph();
 std::string expected_text;for(int r=1;r<region_count;++r)expected_text+=expected_lines[r];assert(written==expected_text&&writes==1&&map.LoadedRegion==current&&world.search.loaded==current&&map.map_at_calls==expected_positions);
 std::vector<int> expected_events;if(hero_mode>=2)expected_events.push_back(1);expected_events.push_back(100+current);for(int r=1;r<region_count;++r){expected_events.push_back(200+r);expected_events.push_back(300+r);expected_events.push_back(300+r);expected_events.push_back(100+r);}expected_events.push_back(400);expected_events.push_back(401);expected_events.push_back(600+current);if(hero_mode==2)expected_events.push_back(2);expected_events.push_back(500);assert(events==expected_events);assert(hero.limbo==(hero_mode==3));
 for(int r=1;r<region_count;++r){assert(map.Regions[r].name.text==names[r]);for(int i=0;i<5;++i){bool selected=(mask&(1u<<i))!=0;assert(things[r][i].map_reads==(selected?1:0));assert(things[r][i].peek_calls==(selected&&i<4?1:0));assert(exits[r][i].reads==(selected&&i<4?1:0));}for(int j=0;j<4;++j){bool selected=(mask&(1u<<(5+j)))!=0;assert(hooks[r][j].reads==(selected?1:0));assert(entrances[r][j].pos_reads==(selected&&j!=1&&j!=2?1:0));assert(things[r][5+j].get_calls==(selected?1:0));}assert(things[r][9].get_calls==0);}
}
int main(){int cases=0;for(unsigned mask=0;mask<512;++mask)for(int h=0;h<4;++h)for(int order=0;order<2;++order){run_case(mask,5,h,(int)(mask%5),order);++cases;}for(int regions=1;regions<5;++regions)for(int h=0;h<4;++h){run_case(511,regions,h,0,0);++cases;}assert(cases==4112);puts("00501450_TEST PASS");}
