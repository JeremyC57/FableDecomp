// 004feb70 behavior fixture. Target body pasted unchanged.
// Real constructed objects; reduced unused interfaces and fixture-only constructors.
// Vector backing and external engine/navigation/map methods are checked boundaries.
// This fixture does not assert native layout, dispatch offsets, or helper internals.
#include <stdio.h>
#include <stdlib.h>
static int failures=0,liveVectors=0;static void Check(bool x){if(!x)++failures;}
class C2DBoxF{public:float TLX,TLY,BRX,BRY;C2DBoxF(float a,float b,float c,float d):TLX(a),TLY(b),BRX(c),BRY(d){}};
class C2DBoxI{public:long TLX,TLY,BRX,BRY;C2DBoxI():TLX(0),TLY(0),BRX(0),BRY(0){}C2DBoxI(long a,long b,long c,long d):TLX(a),TLY(b),BRX(c),BRY(d){}C2DBoxI(const C2DBoxI&o):TLX(o.TLX),TLY(o.TLY),BRX(o.BRX),BRY(o.BRY){}C2DBoxF ToC2DBoxF()const{return C2DBoxF((float)TLX,(float)TLY,(float)BRX,(float)BRY);}};
static bool Same(const C2DBoxI&a,const C2DBoxI&b){return a.TLX==b.TLX&&a.TLY==b.TLY&&a.BRX==b.BRX&&a.BRY==b.BRY;}
static bool SameF(const C2DBoxF&a,const C2DBoxI&b){return a.TLX==(float)b.TLX&&a.TLY==(float)b.TLY&&a.BRX==(float)b.BRX&&a.BRY==(float)b.BRY;}
namespace std{template<class T>class vector{T*first;T*last;public:vector():first(0),last(0){++liveVectors;}~vector(){delete[]first;--liveVectors;}void Set(const T*p,unsigned int n){delete[]first;first=new T[n+1];last=first+n;for(unsigned int i=0;i<n;++i)first[i]=p[i];}T*begin(){return first;}T*end(){return last;}T&operator[](unsigned int i){return first[i];}};}
static C2DBoxI expectedArea,expectedNav;static unsigned long order[4];static int expectedCount,stage,clipIndex,mapCallbacks,navIndex;static unsigned int mask;
static C2DBoxI Local(long id,const C2DBoxI&b){return C2DBoxI(b.TLX-id*8,b.TLY-id*4,b.BRX-id*8,b.BRY-id*4);}
class CMap{public:long id;CMap():id(0){}void HeightAreaChanged(const C2DBoxI*p){Check(stage==2&&clipIndex>0);Check(id==(long)order[clipIndex-1]);Check(p&&Same(*p,Local(id,expectedArea)));++mapCallbacks;}};
template<class T>class CCountedPointer{public:T*Data;CCountedPointer():Data(0){}};
class CMapInfo{public:CCountedPointer<CMap>PMap;};
class CNavigatorManager{public:void UpdateNavigationMapArea(long id,const C2DBoxF&b){Check(stage==3&&navIndex<expectedCount);Check(id==(long)order[navIndex]);Check(SameF(b,expectedNav));++navIndex;}};
class CWorld{CNavigatorManager&nav;public:int gets;CWorld(CNavigatorManager&n):nav(n),gets(0){}CNavigatorManager&GetNavigatorManager(){++gets;return nav;}};
class CEngineManager{public:void AddChangedArea(const C2DBoxI&b){Check(stage++==0&&Same(b,expectedArea));}};
static CEngineManager manager;
template<class T>class CTBaseSingleton{public:static T&Get(){return manager;}};
class CWorldMap{public:CWorld&World;std::vector<CMapInfo>Maps;C2DBoxI Dimensions;C2DBoxI*external;bool mutate;int dimensionsRead;
 CWorldMap(CWorld&w):World(w),Dimensions(-8,-4,16,12),external(0),mutate(false),dimensionsRead(0){}
 const C2DBoxI&GetDimensions()const{++const_cast<CWorldMap*>(this)->dimensionsRead;return Dimensions;}
 void GetAllMapsInArea(const C2DBoxI&b,std::vector<unsigned long>&v)const{Check(stage++==1&&Same(b,expectedArea));v.Set(order,expectedCount);}
 C2DBoxI ClipAndConvertWorldMapAreaToLocalArea(long id,const C2DBoxI&b)const{Check(stage==2&&clipIndex<expectedCount);Check(id==(long)order[clipIndex]&&Same(b,expectedArea));++clipIndex;return Local(id,b);}
 void UpdateAllEdges(){Check(stage++==2&&clipIndex==expectedCount);if(mutate){Dimensions.TLX+=2;Dimensions.BRX+=2;if(external){external->TLY+=2;external->BRY+=2;}}}
 void HeightAreaChanged(const C2DBoxI*area);
};
void CWorldMap::HeightAreaChanged(const C2DBoxI*area){
 C2DBoxI dimensions=GetDimensions();std::vector<unsigned long>maps;
 if(!area)area=&dimensions;
 CTBaseSingleton<CEngineManager>::Get().AddChangedArea(*area);GetAllMapsInArea(*area,maps);
 for(unsigned long*it=maps.begin();it<maps.end();++it){long map=*it;C2DBoxI local=ClipAndConvertWorldMapAreaToLocalArea(map,*area);CMap*pmap=Maps[map].PMap.Data;if(pmap)pmap->HeightAreaChanged(&local);}
 UpdateAllEdges();
 for(unsigned long*it=maps.begin();it<maps.end();++it)World.GetNavigatorManager().UpdateNavigationMapArea(*it,area->ToC2DBoxF());
}

int main(){unsigned int cases=0;
 for(int count=0;count<=4;++count)for(unsigned int present=0;present<16;++present)for(int permutation=0;permutation<3;++permutation)for(int nullArea=0;nullArea<2;++nullArea)for(int mutate=0;mutate<2;++mutate){
  Check(liveVectors==0);{
   CNavigatorManager nav;CWorld world(nav);CWorldMap worldMap(world);CMap maps[4];CMapInfo info[4];for(int j=0;j<4;++j){maps[j].id=j;info[j].PMap.Data=(present&(1u<<j))?&maps[j]:0;order[j]=permutation==0?j:permutation==1?3-j:(j+1)%4;}
   worldMap.Maps.Set(info,4);C2DBoxI supplied(-3,2,9,14);worldMap.external=nullArea?0:&supplied;worldMap.mutate=mutate!=0;
   expectedArea=nullArea?worldMap.Dimensions:supplied;expectedNav=expectedArea;if(mutate&&!nullArea){expectedNav.TLY+=2;expectedNav.BRY+=2;}
   expectedCount=count;stage=clipIndex=mapCallbacks=navIndex=0;mask=present;
   worldMap.HeightAreaChanged(nullArea?0:&supplied);
   int wantedCallbacks=0;for(int j=0;j<count;++j)if(present&(1u<<order[j]))++wantedCallbacks;
   Check(stage==3&&clipIndex==count&&mapCallbacks==wantedCallbacks&&navIndex==count);Check(world.gets==count&&worldMap.dimensionsRead==1);Check(liveVectors==1);++cases;
  }Check(liveVectors==0);
 }
 Check(cases==960);if(failures){fprintf(stderr,"failures=%d cases=%u\n",failures,cases);return 1;}puts("004feb70_TEST PASS");return 0;
}
