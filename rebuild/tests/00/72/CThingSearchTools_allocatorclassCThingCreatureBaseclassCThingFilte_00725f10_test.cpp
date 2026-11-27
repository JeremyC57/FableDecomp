#include <vector>
#include <stdio.h>
#include <stdlib.h>
struct C2DBoxI {long TLX,TLY,BRX,BRY;long GetWidth()const{return BRX-TLX;}long GetHeight()const{return BRY-TLY;}};
struct CThing {unsigned long f;unsigned long GetTypeFlags()const{return f;}};
struct CThingCreatureBase: CThing {static unsigned long StatGetTypeFlags(){return 15;}};
struct CThingFilter_IsNot {const CThing* x;bool operator()(const CThing& t)const{return &t!=x;}};
struct CTCMapwho {CThing *p;unsigned long stamp;unsigned long GetSearchID()const{return stamp;}void SetSearchID(unsigned long v){stamp=v;}CThing& GetParentThing(){return *p;}};
namespace NThingMap{
struct CMapwhoNode{CTCMapwho* PMapwho;CMapwhoNode* PNext;};
struct CThingMapCell{CMapwhoNode* PFirstNode;};
struct CLayerInfo{long ScaleShift;};
CLayerInfo LayerInfo[2]={{0},{1}};long MaxLayers;
struct CThingMap {CThingMapCell cells[2][4][4];const CThingMapCell& PeekCell(unsigned char l,long x,long y)const{return cells[l][y][x];}};
}
struct CThingSearchTools{
mutable std::vector<NThingMap::CThingMap*> AreaMaps;
std::vector<NThingMap::CThingMap*> fixture;
static unsigned long NoLookups,NoThingsScanned,SearchID;
void GetAllThingMapsInArea(const C2DBoxI&,std::vector<NThingMap::CThingMap*>& out)const{out=fixture;}
C2DBoxI ClipAndConvertWorldMapAreaToLocalArea(const NThingMap::CThingMap&,const C2DBoxI& a)const{return a;}
    C2DBoxI GetMapwhoArea(const NThingMap::CThingMap& map,long layer,const C2DBoxI& area) const {
        const NThingMap::CLayerInfo& layer_info=NThingMap::LayerInfo[layer];
        C2DBoxI clipped_area;
        clipped_area.TLX=area.TLX>>layer_info.ScaleShift;
        clipped_area.TLY=area.TLY>>layer_info.ScaleShift;
        clipped_area.BRX=((area.BRX-1)>>layer_info.ScaleShift)+1;
        clipped_area.BRY=((area.BRY-1)>>layer_info.ScaleShift)+1;
        return clipped_area;
    }
public:
    template<class T,class Container,class Filter>
    void GetAllThingsInRoughArea(const C2DBoxI& area,const Filter& filter_func,Container& things) const {
        unsigned long flags=T::StatGetTypeFlags();
        GetAllThingMapsInArea(area,const_cast<std::vector<NThingMap::CThingMap*>&>(AreaMaps));
        ++NoLookups;
        NoThingsScanned=0;
        ++SearchID;
        for(std::vector<NThingMap::CThingMap*>::const_iterator it=AreaMaps.begin();it!=AreaMaps.end();++it) {
            NThingMap::CThingMap& thing_map=**it;
            C2DBoxI local_area=ClipAndConvertWorldMapAreaToLocalArea(thing_map,area);
            for(long c1=0;c1<NThingMap::MaxLayers;++c1) {
                C2DBoxI clipped_area=GetMapwhoArea(thing_map,c1,local_area);
                if(clipped_area.GetWidth()==0 || clipped_area.GetHeight()==0)
                    continue;
                for(long cy=clipped_area.TLY;cy<clipped_area.BRY;++cy) {
                    const NThingMap::CThingMapCell* pcell=&thing_map.PeekCell((unsigned char)c1,clipped_area.TLX,cy);
                    for(long cx=clipped_area.TLX;cx<clipped_area.BRX;++cx,++pcell) {
                        for(const NThingMap::CMapwhoNode* ploop_node=pcell->PFirstNode;ploop_node;ploop_node=ploop_node->PNext) {
                            CTCMapwho* pmapwho=ploop_node->PMapwho;
                            if(pmapwho->GetSearchID()<SearchID) {
                                pmapwho->SetSearchID(SearchID);
                                CThing* loop_thing=&pmapwho->GetParentThing();
                                if((loop_thing->GetTypeFlags()&flags)==flags) {
                                    T* tthing=static_cast<T*>(loop_thing);
                                    if(filter_func(*tthing))
                                        things.push_back(tthing);
                                }
                            }
                        }
                    }
                }
            }
        }
        const_cast<std::vector<NThingMap::CThingMap*>&>(AreaMaps).clear();
    }
};
unsigned long CThingSearchTools::NoLookups,CThingSearchTools::NoThingsScanned,CThingSearchTools::SearchID;
int main(){
 using namespace NThingMap;
 for(int maps=0;maps<=2;++maps)for(int layers=0;layers<=2;++layers)for(int width=0;width<=4;++width)for(int height=0;height<=4;++height)for(int excluded=0;excluded<4;++excluded){
 CThingSearchTools tools;CThingMap m[2]={0};CThingCreatureBase things[4];CTCMapwho entries[4];CMapwhoNode nodes[4];
 for(int i=0;i<4;++i){things[i].f=i==2?7:31;entries[i].p=things+i;entries[i].stamp=0;nodes[i].PMapwho=entries+i;nodes[i].PNext=0;}
 for(int map=0;map<maps;++map){tools.fixture.push_back(&m[map]);for(int l=0;l<layers;++l)for(int y=0;y<4;++y)for(int x=0;x<4;++x)m[map].cells[l][y][x].PFirstNode=nodes+((map+l+x+2*y)%4);}
 MaxLayers=layers;CThingFilter_IsNot filter={things+excluded};C2DBoxI area={0,0,width,height};std::vector<CThingCreatureBase*> result;
 for(int run=0;run<2;++run){CThingSearchTools::NoThingsScanned=44;unsigned long old=CThingSearchTools::SearchID,n=CThingSearchTools::NoLookups;size_t prev=result.size();tools.GetAllThingsInRoughArea<CThingCreatureBase,std::vector<CThingCreatureBase*>,CThingFilter_IsNot>(area,filter,result);
 if(!tools.AreaMaps.empty()||CThingSearchTools::SearchID!=old+1||CThingSearchTools::NoLookups!=n+1||CThingSearchTools::NoThingsScanned)return 1;
 bool seen[4]={0};for(int map=0;map<maps;++map)for(int l=0;l<layers;++l)for(int y=0;y<(height+(1<<l)-1)/(1<<l);++y)for(int x=0;x<(width+(1<<l)-1)/(1<<l);++x){int idx=(map+l+x+y*2)%4;if(seen[idx])continue;seen[idx]=true;if(idx!=excluded&&idx!=2){if(prev>=result.size()||result[prev++]!=things+idx)return 2;}}
 if(prev!=result.size())return 3;for(int i=0;i<4;++i)if((entries[i].stamp==old+1)!=seen[i])return 4;}
 }puts("00725f10_TEST PASS");return 0;}