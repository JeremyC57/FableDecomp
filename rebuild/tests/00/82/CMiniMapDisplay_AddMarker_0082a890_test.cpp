#include <cstdio>
#include <list>
#include <vector>
static int gConstructors,gDestructors,gGets,gSets;
class CBaseObject{public:int Refs;CBaseObject():Refs(0){}};class CThing:public CBaseObject{};
class CBaseIntelligentPointer{CBaseObject* PData;public:CBaseIntelligentPointer():PData(0){++gConstructors;}virtual ~CBaseIntelligentPointer(){++gDestructors;if(PData)--PData->Refs;}CBaseObject* GetPItem()const{++gGets;return PData;}void SetPItem(CBaseObject* item){++gSets;if(PData)--PData->Refs;PData=item;if(PData)++PData->Refs;}};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{public:CIntelligentPointer(T* item){SetPointer(item);}CIntelligentPointer(const CIntelligentPointer& item){SetPointer(item.GetPointer());}T* Get()const{return static_cast<T*>(GetPItem());}private:T* GetPointer()const{return static_cast<T*>(GetPItem());}void SetPointer(const T* item){SetPItem(const_cast<CBaseObject*>(static_cast<const CBaseObject*>(item)));}};
class CMiniMapDisplay{public:class CMarkerInfo{public:CIntelligentPointer<const CThing> PThing;long GraphicIndex;bool AutoMarker;CMarkerInfo(const CThing& thing,long graphic_index,bool auto_marker):PThing(&thing),GraphicIndex(graphic_index),AutoMarker(auto_marker){}const CThing* GetPThing()const{return PThing.Get();}long GetGraphicIndex()const{return GraphicIndex;}};std::list<CMarkerInfo> Markers;void AddMarker(const CThing&,long);};
struct Expected{int Thing;long Graphic;bool Auto;Expected(int t,long g,bool a):Thing(t),Graphic(g),Auto(a){}};
void CMiniMapDisplay::AddMarker(const CThing& thing,long graphic_index)
{
 if(graphic_index>0){
  for(std::list<CMarkerInfo>::const_iterator it=Markers.begin();it!=Markers.end();++it){
   const CMarkerInfo& marker_info=*it;
   if(marker_info.GetPThing()==&thing&&marker_info.GetGraphicIndex()==graphic_index)return;
  }
  Markers.push_back(CMarkerInfo(thing,graphic_index,false));
 }
}

int main(){int ok=1,cases=0;for(int seed=0;seed<12;++seed){CThing things[4];{CMiniMapDisplay display;std::vector<Expected> expected;for(int i=0;i<3;++i){display.Markers.push_back(CMiniMapDisplay::CMarkerInfo(things[i],i+1,true));expected.push_back(Expected(i,i+1,true));}for(int op=0;op<96;++op){int ti=(op+seed)%4;long graphic=(op*5+seed*7)%7-2;if(op==95)graphic=0x7fffffff;bool duplicate=false;int visited=0;if(graphic>0)for(unsigned int j=0;j<expected.size();++j){++visited;if(expected[j].Thing==ti&&expected[j].Graphic==graphic){duplicate=true;break;}}bool added=graphic>0&&!duplicate;gConstructors=gDestructors=gGets=gSets=0;display.AddMarker(things[ti],graphic);ok &= gConstructors==(added?2:0)&&gDestructors==(added?1:0)&&gSets==(added?2:0)&&gGets==visited+(added?1:0);if(added)expected.push_back(Expected(ti,graphic,false));ok &= display.Markers.size()==expected.size();unsigned int index=0;for(std::list<CMiniMapDisplay::CMarkerInfo>::const_iterator it=display.Markers.begin();it!=display.Markers.end();++it,++index)ok &= it->GetPThing()==&things[expected[index].Thing]&&it->GetGraphicIndex()==expected[index].Graphic&&it->AutoMarker==expected[index].Auto;for(int t=0;t<4;++t){int count=0;for(unsigned int j=0;j<expected.size();++j)if(expected[j].Thing==t)++count;ok &= things[t].Refs==count;}++cases;}}for(int t=0;t<4;++t)ok &= things[t].Refs==0;}
ok &= cases==1152;std::printf(ok?"0082a890_TEST PASS\n":"0082a890_TEST FAIL\n");return ok?0:1;}