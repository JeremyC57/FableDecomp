#include <stdio.h>
#include <list>
#include <functional>
#include <vector>
#include <math.h>
class C2DCoordI{public:long X,Y;C2DCoordI(long x,long y):X(x),Y(y){}};
class C3DVector{public:float X,Y,Z;C3DVector(){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}C3DVector(const C3DVector& p):X(p.X),Y(p.Y),Z(p.Z){}C2DCoordI ToC2DCoordI()const{return C2DCoordI((long)X,(long)Y);}C3DVector operator-(const C3DVector& p)const{return C3DVector(X-p.X,Y-p.Y,Z-p.Z);}float GetSquaredMagnitude()const{return X*X+Y*Y+Z*Z;}};
class C2DBoxI{public:long TLX,TLY,BRX,BRY;C2DBoxI(){}C2DBoxI(const C2DCoordI& p,long radius):TLX(p.X-radius),TLY(p.Y-radius),BRX(p.X+radius),BRY(p.Y+radius){}};
class CTCPhysicsBase{public:C3DVector Centre;float Radius;void GetBoundingInfo(C3DVector& c,float& r)const{c=Centre;r=Radius;}};
class CThing{public:unsigned long Type,Flags;CTCPhysicsBase Physics;unsigned long GetType()const{return Type;}unsigned long GetTypeFlags()const{return Flags;}const CTCPhysicsBase* GetPhysicsTC()const{return &Physics;}};
class CThingBuilding:public CThing{public:static unsigned long StatGetTypeFlags(){return 0x47;}};
class CTCMapwho{public:CThing* Parent;unsigned long Stamp;unsigned long GetSearchID()const{return Stamp;}void SetSearchID(unsigned long id){Stamp=id;}CThing& GetParentThing(){return *Parent;}};
namespace NThingMap {
 class CMapwhoNode{public:CTCMapwho* PMapwho;CMapwhoNode* PNext;};class CThingMapCell{public:CMapwhoNode* PFirstNode;};
 class CMapwhoLevelInfo{public:struct ExtentsType{long X,Y;}Extents;};
 class CThingMap{public:std::vector<CMapwhoLevelInfo> MapwhoLevelInfo;std::vector<std::vector<CThingMapCell> > Cells;};
 static long MaxLayers=2;struct CLayerInfo{long ScaleShift;};static CLayerInfo LayerInfo[2];
}
class CThingSearchTools{public:static unsigned long NoLookups,NoThingsScanned,SearchID;std::vector<NThingMap::CThingMap*> AreaMaps;NThingMap::CThingMap* Map;
 void GetAllThingMapsInArea(const C2DBoxI& a,std::vector<NThingMap::CThingMap*>& maps)const{if(a.TLX<4&&a.BRX>0&&a.TLY<4&&a.BRY>0)maps.push_back(Map);}
 C2DBoxI ClipAndConvertWorldMapAreaToLocalArea(const NThingMap::CThingMap&,const C2DBoxI& a)const{C2DBoxI b=a;if(b.TLX<0)b.TLX=0;if(b.TLY<0)b.TLY=0;if(b.BRX>4)b.BRX=4;if(b.BRY>4)b.BRY=4;return b;}
    template<class T,class Container,class Filter> void GetAllThingsInRoughArea(const C2DBoxI& area,const Filter& filter,Container& result) const {
        std::vector<NThingMap::CThingMap*>& maps=const_cast<std::vector<NThingMap::CThingMap*>&>(AreaMaps);
        GetAllThingMapsInArea(area,maps);
        ++NoLookups;
        NoThingsScanned=0;
        ++SearchID;
        for(std::vector<NThingMap::CThingMap*>::iterator it=maps.begin();it!=maps.end();++it) {
            const NThingMap::CThingMap& map=**it;
            C2DBoxI local=ClipAndConvertWorldMapAreaToLocalArea(map,area);
            for(long layer=0;layer<NThingMap::MaxLayers;++layer) {
                long shift=NThingMap::LayerInfo[layer].ScaleShift;
                C2DBoxI cells;
                cells.TLX=local.TLX>>shift;
                cells.TLY=local.TLY>>shift;
                cells.BRX=((local.BRX-1)>>shift)+1;
                cells.BRY=((local.BRY-1)>>shift)+1;
                if(cells.BRX-cells.TLX==0 || cells.BRY-cells.TLY==0)
                    continue;
                for(long y=cells.TLY;y<cells.BRY;++y) {
                    const NThingMap::CThingMapCell* cell=&map.Cells[layer][y*map.MapwhoLevelInfo[layer].Extents.X+cells.TLX];
                    for(long x=cells.TLX;x<cells.BRX;++x,++cell) {
                        for(NThingMap::CMapwhoNode* node=cell->PFirstNode;node;node=node->PNext) {
                            CTCMapwho* who=node->PMapwho;
                            if(who->GetSearchID()<SearchID) {
                                who->SetSearchID(SearchID);
                                CThing& thing=who->GetParentThing();
                                unsigned long mask=T::StatGetTypeFlags();
                                if((thing.GetTypeFlags()&mask)==mask && filter(static_cast<const T&>(thing)))
                                    result.push_back(static_cast<T*>(&thing));
                            }
                        }
                    }
                }
            }
        }
        maps.clear();
    }
};
unsigned long CThingSearchTools::NoLookups=0,CThingSearchTools::NoThingsScanned=0,CThingSearchTools::SearchID=100;
class CWorld{public:CThingSearchTools* Tools;const CThingSearchTools& PeekThingSearchTools()const{return *Tools;}};
class CGameCameraManager{public:CGameCameraManager(const CWorld& w):World(w){}const CWorld& GetWorld()const{return World;}private:const CWorld& World;};
namespace NCameraModes{class CTools{public:CTools(const CGameCameraManager& m):CameraManager(m){}template<class T>void GetThingsOfType(std::list<const T*>& things,long thing_type,const C3DVector& origin,float radius)const;private:const CGameCameraManager& GetCameraManager()const{return CameraManager;}const CWorld& GetWorld()const{return GetCameraManager().GetWorld();}const CGameCameraManager& CameraManager;};}
class CThingFilter_IsOfType:public std::unary_function<CThing,bool>{public:CThingFilter_IsOfType(unsigned long type):Type(type){}bool operator()(const CThing& thing)const{return thing.GetType()==Type;}private:unsigned long Type;};
class CThingFilter_OverlappingSphere:public std::unary_function<CThing,bool>{private:C3DVector SpherePos;float Radius;public:CThingFilter_OverlappingSphere(const C3DVector& p,float r):SpherePos(p),Radius(r){}bool operator()(const CThing& test_thing)const{float radius;C3DVector test_pos;test_thing.GetPhysicsTC()->GetBoundingInfo(test_pos,radius);if((test_pos-SpherePos).GetSquaredMagnitude()<(radius+Radius)*(radius+Radius))return true;return false;}};
template<class A,class B>class CFilter_Bind2{protected:const A& F1;const B& F2;public:CFilter_Bind2(const A& a,const B& b):F1(a),F2(b){}template<class T>bool operator()(const T& t)const{return F1(t)&&F2(t);}};
template<class A,class B>CFilter_Bind2<A,B> GFFilter_Bind2(const A& a,const B& b){return CFilter_Bind2<A,B>(a,b);}
template<class T>
void NCameraModes::CTools::GetThingsOfType(std::list<const T*>& things,long thing_type,const C3DVector& origin,float radius)const
{
 C2DBoxI area(origin.ToC2DCoordI(),(long)radius);
 GetWorld().PeekThingSearchTools().GetAllThingsInRoughArea<T>(area,GFFilter_Bind2(CThingFilter_IsOfType(thing_type),CThingFilter_OverlappingSphere(origin,radius)),things);
}
template void NCameraModes::CTools::GetThingsOfType<CThingBuilding>(std::list<const CThingBuilding*>&,long,const C3DVector&,float)const;
int main(){
 CThingBuilding buildings[16],sentinel;CTCMapwho who[16];NThingMap::CMapwhoNode nodes[32];NThingMap::CThingMap map;map.MapwhoLevelInfo.resize(2);map.MapwhoLevelInfo[1].Extents.X=map.MapwhoLevelInfo[1].Extents.Y=2;map.MapwhoLevelInfo[0].Extents.X=map.MapwhoLevelInfo[0].Extents.Y=4;map.Cells.resize(2);map.Cells[1].resize(4);for(int clear=0;clear<4;++clear)map.Cells[1][clear].PFirstNode=0;map.Cells[0].resize(16);NThingMap::LayerInfo[0].ScaleShift=0;NThingMap::LayerInfo[1].ScaleShift=1;
 for(int i=0;i<16;++i){buildings[i].Type=i%3==0?3:4;buildings[i].Flags=i%5==0?7:0x47;buildings[i].Physics.Centre=C3DVector((float)(i%4)+0.5f,(float)(i/4)+0.5f,0);buildings[i].Physics.Radius=0.25f;who[i].Parent=&buildings[i];who[i].Stamp=0;nodes[2*i].PMapwho=nodes[2*i+1].PMapwho=&who[i];nodes[2*i].PNext=&nodes[2*i+1];nodes[2*i+1].PNext=0;map.Cells[0][i].PFirstNode=&nodes[2*i];}
 CThingSearchTools search;search.Map=&map;CWorld world;world.Tools=&search;CGameCameraManager manager(world);NCameraModes::CTools tools(manager);
 const float positions[]={-1.75f,0,0.75f,1.25f,2.25f,3.75f,4.75f};const float radii[]={0,0.5f,1,1.75f,3};const long types[]={3,4,777};int cases=0;
 for(int x=0;x<7;++x)for(int y=0;y<7;++y)for(int r=0;r<5;++r)for(int t=0;t<3;++t)for(int z=0;z<3;++z){
  C3DVector origin(positions[x],positions[y],(float)z);std::list<const CThingBuilding*> got,expected;got.push_back(&sentinel);expected.push_back(&sentinel);long cx=(long)origin.X,cy=(long)origin.Y,rr=(long)radii[r];
  for(int i=0;i<16;++i){int ix=i%4,iy=i/4;double dx=buildings[i].Physics.Centre.X-origin.X,dy=buildings[i].Physics.Centre.Y-origin.Y,dz=-origin.Z;double range=0.25+radii[r];if(ix>=cx-rr&&ix<cx+rr&&iy>=cy-rr&&iy<cy+rr&&(buildings[i].Flags&0x47)==0x47&&buildings[i].Type==(unsigned long)types[t]&&dx*dx+dy*dy+dz*dz<range*range)expected.push_back(&buildings[i]);}
  unsigned long lookups=CThingSearchTools::NoLookups,stamp=CThingSearchTools::SearchID;CThingSearchTools::NoThingsScanned=999;tools.GetThingsOfType<CThingBuilding>(got,types[t],origin,radii[r]);if(got!=expected||!search.AreaMaps.empty()||CThingSearchTools::NoLookups!=lookups+1||CThingSearchTools::SearchID!=stamp+1||CThingSearchTools::NoThingsScanned!=0)return 1;++cases;
 }
 if(cases!=2205)return 1;puts("00878390_TEST PASS");return 0;
}