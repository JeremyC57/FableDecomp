#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define FABLE_FASTCALL
#else
#define FABLE_FASTCALL __fastcall
#endif
static int NodeAllocations,NodeFrees,TargetingQueries,TargetedQueries,TargetQueries,EnemyQueries,FilterQueries,ClipQueries,MapQueries,GetItemQueries;
namespace std {
template<class T>class allocator {public:T* allocate(unsigned int n){assert(n==1);void* p=std::malloc(sizeof(T));assert(p);new(p)T;++NodeAllocations;return static_cast<T*>(p);}void deallocate(T* p,unsigned int){++NodeFrees;std::free(p);}};
template<class T>class vector {public:typedef T* iterator;typedef const T* const_iterator;T* _M_start;T* _M_finish;T* _M_end_of_storage;vector():_M_start(0),_M_finish(0),_M_end_of_storage(0){}iterator begin(){return _M_start;}iterator end(){return _M_finish;}const_iterator begin()const{return _M_start;}const_iterator end()const{return _M_finish;}unsigned int size()const{return (unsigned int)(_M_finish-_M_start);}T& operator[](unsigned int i){return _M_start[i];}const T& operator[](unsigned int i)const{return _M_start[i];}void clear(){_M_finish=_M_start;}};
struct _List_node_base {_List_node_base* _M_next;_List_node_base* _M_prev;};
template<class T>struct _List_node:public _List_node_base {T _M_data;};
template<class T>class _List_proxy {public:_List_node<T>* _M_data;_List_proxy(const allocator<_List_node<T> >&, _List_node<T>* p):_M_data(p){}_List_node<T>* allocate(unsigned int n){return allocator<_List_node<T> >().allocate(n);}void deallocate(_List_node<T>* p,unsigned int n){allocator<_List_node<T> >().deallocate(p,n);}};
template<class _Tp,class _Alloc>class _List_base {public:_List_proxy<_Tp> _M_node;_List_base(const _Alloc&):_M_node(allocator<_List_node<_Tp> >(),0){_List_node<_Tp>* n=_M_node.allocate(1);n->_M_next=n;n->_M_prev=n;_M_node._M_data=n;}
 ~_List_base(){clear();_M_node.deallocate(_M_node._M_data,1);}
 void clear(){_List_node<_Tp>* cur=(_List_node<_Tp>*)_M_node._M_data->_M_next;while(cur!=_M_node._M_data){_List_node<_Tp>* tmp=cur;cur=(_List_node<_Tp>*)cur->_M_next;tmp->_M_data.~_Tp();_M_node.deallocate(tmp,1);}_M_node._M_data->_M_next=_M_node._M_data;_M_node._M_data->_M_prev=_M_node._M_data;}
};
template<class T,class A=allocator<T> >class list:public _List_base<T,A> {public:struct iterator {_List_node_base* Node;iterator(_List_node_base* p=0):Node(p){}T& operator*()const{return static_cast<_List_node<T>*>(Node)->_M_data;}iterator& operator++(){Node=Node->_M_next;return *this;}bool operator!=(const iterator& p)const{return Node!=p.Node;}};list():_List_base<T,A>(A()){}iterator begin(){return iterator(this->_M_node._M_data->_M_next);}iterator end(){return iterator(this->_M_node._M_data);}void push_back(const T& x){_List_node<T>* n=this->_M_node.allocate(1);n->_M_data=x;n->_M_next=this->_M_node._M_data;n->_M_prev=n->_M_next->_M_prev;n->_M_prev->_M_next=n;n->_M_next->_M_prev=n;}};
}
class C2DCoordI {public:long X,Y;C2DCoordI(long x,long y):X(x),Y(y){}};
class C2DBoxI {public:long TLX,TLY,BRX,BRY;C2DBoxI(){}C2DBoxI(const C2DCoordI& p,long r):TLX(p.X-r),TLY(p.Y-r),BRX(p.X+r),BRY(p.Y+r){}};
class C3DVector {public:float X,Y,Z;C3DVector(){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}C2DCoordI ToC2DCoordI()const{return C2DCoordI((long)X,(long)Y);}};
static C3DVector WantTargetPosition;static float WantRadius;
enum ETCInterfaceType {TCI_DRAIN_LIFE=151};
class CTCTargetingBase;class CTCTargeted;
class CThing {public:bool Dead,Enemy,InRange;unsigned long Flags;C3DVector Position;const CTCTargetingBase* Targeting;const CTCTargeted* Targeted;mutable int AliveChecks;int Removed;
 CThing():Dead(false),Enemy(false),InRange(true),Flags(15),Targeting(0),Targeted(0),AliveChecks(0),Removed(0){}
 bool IsDead()const{++AliveChecks;return Dead;}unsigned long GetTypeFlags()const{return Flags;}const C3DVector& GetPos()const{return Position;}
 bool PeekTC(const CTCTargetingBase** p)const{++TargetingQueries;if(Targeting){*p=Targeting;return true;}return false;}
 bool PeekTC(const CTCTargeted** p)const{++TargetedQueries;if(Targeted){*p=Targeted;return true;}return false;}
 void RemoveTC(ETCInterfaceType t){assert(t==TCI_DRAIN_LIFE);++Removed;}
};
class CThingCreatureBase:public CThing {public:static unsigned long StatGetTypeFlags(){return 15;}};
class CTCTargetingBase {public:CThing* Target;virtual CThing* GetPTargetedThing()const{++TargetQueries;return Target;}};
class CTCTargeted {public:bool Targetable;bool IsTargetable()const{return Targetable;}};
class CTCEnemy {public:static bool FABLE_FASTCALL AreThingsEnemies(const CThing& a,const CThing& b){assert(&a!=&b);++EnemyQueries;return b.Enemy;}};
template<class T>class CIntelligentPointer {public:T* Item;CIntelligentPointer():Item(0){}T* Get()const{++GetItemQueries;return Item;}T* operator->()const{return Get();}};
template<class T>bool FABLE_FASTCALL operator!=(const CIntelligentPointer<T>& p,int){return p.Get()!=0;}
template<class T>inline bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing){if(thing!=0){if(!thing->IsDead())return true;}return false;}
class CSpecialAbilitiesDrainLifeDef {public:std::vector<float> SpellRange;};
template<class T>class CDefPointer {public:T* Object;T* operator->()const{return Object;}};
class CTCMapwho {public:CThing* Parent;unsigned long Search;unsigned long GetSearchID()const{return Search;}void SetSearchID(unsigned long n){Search=n;}CThing& GetParentThing(){return *Parent;}};
namespace NThingMap {
struct CMapwhoNode {CTCMapwho* PMapwho;CMapwhoNode* PNext;};struct CThingMapCell {CMapwhoNode* PFirstNode;};struct CLayerInfo {long ScaleShift;};static long MaxLayers;static CLayerInfo LayerInfo[2];
class CThingMap {public:CThingMapCell Cells[2][256];CThingMap(){for(int k=0;k<2;++k)for(int c=0;c<256;++c)Cells[k][c].PFirstNode=0;}const CThingMapCell& PeekCell(unsigned char layer,unsigned long x,unsigned long y)const{unsigned long width=layer?8:16;assert(layer<2&&x<width&&y<width);return Cells[layer][width*y+x];}};
}
class CThingFilter_In2DRange {public:C3DVector Pos;float Range;CThingFilter_In2DRange(const C3DVector& p,float r):Pos(p),Range(r){}bool operator()(const CThing& t)const{++FilterQueries;assert(Pos.X==WantTargetPosition.X&&Pos.Y==WantTargetPosition.Y&&Pos.Z==WantTargetPosition.Z&&Range==WantRadius);return t.InRange;}};
class CThingSearchTools {public:std::vector<NThingMap::CThingMap*> AreaMaps;NThingMap::CThingMap* Backing[2];static unsigned long NoLookups,NoThingsScanned,SearchID;
 void GetAllThingMapsInArea(const C2DBoxI& a,std::vector<NThingMap::CThingMap*>& maps)const{++MapQueries;long x=(long)WantTargetPosition.X,y=(long)WantTargetPosition.Y,r=(long)WantRadius;assert(a.TLX==x-r&&a.TLY==y-r&&a.BRX==x+r&&a.BRY==y+r);maps._M_start=const_cast<NThingMap::CThingMap**>(Backing);maps._M_finish=maps._M_start+2;maps._M_end_of_storage=maps._M_finish;}
 C2DBoxI ClipAndConvertWorldMapAreaToLocalArea(const NThingMap::CThingMap&,const C2DBoxI& a)const{++ClipQueries;C2DBoxI r;r.TLX=a.TLX<0?0:a.TLX>16?16:a.TLX;r.TLY=a.TLY<0?0:a.TLY>16?16:a.TLY;r.BRX=a.BRX<0?0:a.BRX>16?16:a.BRX;r.BRY=a.BRY<0?0:a.BRY>16?16:a.BRY;return r;}
 template<class T,class Container,class Filter>void GetAllThingsInRoughArea(const C2DBoxI&,const Filter&,Container&)const;
};unsigned long CThingSearchTools::NoLookups;unsigned long CThingSearchTools::NoThingsScanned;unsigned long CThingSearchTools::SearchID;
class CTCBase {public:CThing* ParentThing;CThingSearchTools* SearchTools;CThing& GetParentThing(){return *ParentThing;}const CThingSearchTools& PeekThingSearchTools()const{return *SearchTools;}};
class CTCDrainLife:public CTCBase {public:CDefPointer<const CSpecialAbilitiesDrainLifeDef> PDef;long SpellLevel;std::vector<CIntelligentPointer<CThing> > Shots;bool JustReleased;long SpellDelayFrames;CThingCreatureBase* Drained[16];int DrainCount;void DoDrain(CThingCreatureBase* p){assert(DrainCount<16);Drained[DrainCount++]=p;}virtual void FrameUpdate();};

#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_THISEND
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
    template<class T,class Container,class Filter> void CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& area,const Filter& filter,Container& result) const {
        std::vector<NThingMap::CThingMap*>& maps=const_cast<std::vector<NThingMap::CThingMap*>&>(AreaMaps);
        GetAllThingMapsInArea(area,maps);
        ++NoLookups;
        NoThingsScanned=0;
        ++SearchID;
#ifdef FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_THISEND
        // Q18 opt-in (rows 0x006FE7C0 / 0x008780D0, the std::list instances): the loop's end is read from the member
        //   (0x00878115 `cmp esi,[ebp+10h]` = this->AreaMaps._M_finish, with `this` and &AreaMaps both kept: ebp / ebx).
        for(std::vector<NThingMap::CThingMap*>::iterator it=maps.begin();it!=AreaMaps.end();++it) {
#else
        for(std::vector<NThingMap::CThingMap*>::iterator it=maps.begin();it!=maps.end();++it) {
#endif
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
#if defined(FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST) || defined(FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE)
                    const NThingMap::CThingMapCell* cell=&map.PeekCell((unsigned char)layer,cells.TLX,y);
#else
                    const NThingMap::CThingMapCell* cell=&map.Cells[layer][y*map.MapwhoLevelInfo[layer].Extents.X+cells.TLX];
#endif
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

void CTCDrainLife::FrameUpdate()
{
    CThingCreatureBase& parent_creature=static_cast<CThingCreatureBase&>(GetParentThing());
    if(JustReleased) {
        JustReleased=false;
        const CTCTargetingBase* ptc_targeting;
        if(parent_creature.PeekTC(&ptc_targeting)) {
            float search_radius=PDef->SpellRange[SpellLevel];
            CThing* ptarget=ptc_targeting->GetPTargetedThing();
            if(IS_THING_ALIVE(ptarget)&&(ptarget->GetTypeFlags()&8)) {
                DoDrain(static_cast<CThingCreatureBase*>(ptarget));
                C3DVector targetpos=ptarget->GetPos();
                C2DBoxI search_area(targetpos.ToC2DCoordI(),(long)search_radius);
                std::list<CThingCreatureBase*> thingstosuck;
                PeekThingSearchTools().GetAllThingsInRoughArea<CThingCreatureBase>(search_area,CThingFilter_In2DRange(targetpos,search_radius),thingstosuck);
                for(std::list<CThingCreatureBase*>::iterator it=thingstosuck.begin(),endit=thingstosuck.end();it!=endit;++it) {
                    CThingCreatureBase* something=*it;
                    if(something==&parent_creature||something==ptarget)
                        continue;
                    if(!CTCEnemy::AreThingsEnemies(parent_creature,*something))
                        continue;
                    const CTCTargeted* ptc_targeted;
                    if(something->PeekTC(&ptc_targeted)&&!ptc_targeted->IsTargetable())
                        continue;
                    if(IS_THING_ALIVE(something))
                        DoDrain(something);
                }
            }
        }
    } else {
        --SpellDelayFrames;
        bool all_dead=true;
        for(long i=0;i<(long)Shots.size();++i) {
            if(IS_THING_ALIVE(Shots[i])) {
                all_dead=false;
                break;
            }
        }
        if(all_dead||SpellDelayFrames<=0)
            parent_creature.RemoveTC(TCI_DRAIN_LIFE);
    }
}

static void ResetCounts(){NodeAllocations=NodeFrees=TargetingQueries=TargetedQueries=TargetQueries=EnemyQueries=FilterQueries=ClipQueries=MapQueries=GetItemQueries=0;CThingSearchTools::NoLookups=100;CThingSearchTools::NoThingsScanned=99;CThingSearchTools::SearchID=42;}
int main()
{
 int cases=0;float radius_values[]={1.75f,3.25f,0.0f};NThingMap::LayerInfo[0].ScaleShift=0;NThingMap::LayerInfo[1].ScaleShift=1;
 for(int mode=0;mode<5;++mode)for(int radius=0;radius<3;++radius)for(int layers=0;layers<3;++layers)for(int flip=0;flip<2;++flip)for(int negative=0;negative<2;++negative){
  ResetCounts();NThingMap::MaxLayers=layers;
  CThingCreatureBase creatures[9];CThing noncreature;noncreature.Flags=7;CTCTargetingBase targeting;CTCTargeted untargetable,targetable;untargetable.Targetable=false;targetable.Targetable=flip==0;
  for(int i=0;i<9;++i){creatures[i].Position=C3DVector(8.75f,8.25f,4.5f);creatures[i].Enemy=true;}
  creatures[0].Enemy=false;creatures[2].Enemy=flip==0;creatures[3].Enemy=false;creatures[4].Targeted=&untargetable;creatures[5].Targeted=&targetable;creatures[6].Targeted=&targetable;creatures[6].Dead=true;creatures[7].InRange=false;
  creatures[1].Position=negative?C3DVector(-1.25f,8.75f,4.5f):C3DVector(8.75f,8.25f,4.5f);creatures[1].Dead=mode==2;
  targeting.Target=mode==1?0:mode==3?&noncreature:&creatures[1];if(mode!=0)creatures[0].Targeting=&targeting;
  CTCMapwho who[10];for(int i=0;i<10;++i){who[i].Parent=i==9?&noncreature:&creatures[i];who[i].Search=i==8?100:42;}
  NThingMap::CThingMap maps[2];NThingMap::CMapwhoNode nodes[2][2][10];
  for(int m=0;m<2;++m)for(int l=0;l<2;++l){for(int i=0;i<10;++i){nodes[m][l][i].PMapwho=&who[i];nodes[m][l][i].PNext=i==9?0:&nodes[m][l][i+1];}maps[m].Cells[l][l?4*8+4:8*16+8].PFirstNode=&nodes[m][l][0];}
  CThingSearchTools tools;tools.Backing[0]=&maps[0];tools.Backing[1]=&maps[1];CSpecialAbilitiesDrainLifeDef def;def.SpellRange._M_start=radius_values;def.SpellRange._M_finish=radius_values+3;def.SpellRange._M_end_of_storage=radius_values+3;
  CIntelligentPointer<CThing> shots[3];CTCDrainLife drain;drain.ParentThing=&creatures[0];drain.SearchTools=&tools;drain.PDef.Object=&def;drain.SpellLevel=radius;drain.JustReleased=true;drain.SpellDelayFrames=123;drain.DrainCount=0;drain.Shots._M_start=shots;drain.Shots._M_finish=shots;drain.Shots._M_end_of_storage=shots+3;
  WantTargetPosition=creatures[1].Position;WantRadius=radius_values[radius];drain.FrameUpdate();bool valid=mode==4,scanned=valid&&radius<2&&layers>0&&!negative;
  assert(!drain.JustReleased&&drain.SpellDelayFrames==123&&creatures[0].Removed==0);assert(TargetingQueries==1&&TargetQueries==(mode==0?0:1));assert(drain.DrainCount==(valid?(scanned&&!flip?3:1):0));if(valid){assert(drain.Drained[0]==&creatures[1]);if(scanned&&!flip){assert(drain.Drained[1]==&creatures[2]&&drain.Drained[2]==&creatures[5]);}}
  assert(MapQueries==(valid?1:0)&&ClipQueries==(valid?2:0));assert(CThingSearchTools::NoLookups==(unsigned long)(valid?101:100)&&CThingSearchTools::SearchID==(unsigned long)(valid?43:42)&&CThingSearchTools::NoThingsScanned==(unsigned long)(valid?0:99));
  assert(FilterQueries==(scanned?8:0)&&EnemyQueries==(scanned?5:0)&&TargetedQueries==(scanned?(flip?3:4):0));assert(NodeAllocations==(valid?(scanned?8:1):0)&&NodeFrees==NodeAllocations&&GetItemQueries==0);
  if(valid){assert(tools.AreaMaps.begin()==tools.AreaMaps.end());}assert(who[8].Search==100);for(int i=0;i<10;++i)if(i!=8)assert(who[i].Search==(unsigned long)(scanned?43:42));
  assert(creatures[1].AliveChecks==(mode==0||mode==1||mode==3?0:1));assert(noncreature.AliveChecks==(mode==3?1:0));assert(creatures[4].AliveChecks==0);assert(creatures[2].AliveChecks==(scanned&&!flip?1:0));assert(creatures[5].AliveChecks==(scanned&&!flip?1:0));assert(creatures[6].AliveChecks==(scanned&&!flip?1:0));++cases;
 }
 for(int count=0;count<4;++count){int permutations=1;for(int i=0;i<count;++i)permutations*=3;for(int encoding=0;encoding<permutations;++encoding)for(int delay=-1;delay<=2;++delay){
  ResetCounts();CThingCreatureBase parent;CThing shot_items[3];CIntelligentPointer<CThing> shots[3];int n=encoding,expected_gets=0;bool live=false;
  for(int i=0;i<count;++i){int state=n%3;n/=3;shot_items[i].Dead=state==1;shots[i].Item=state?&shot_items[i]:0;if(!live){expected_gets+=state?2:1;if(state==2)live=true;}}
  CTCDrainLife drain;drain.ParentThing=&parent;drain.SearchTools=0;drain.PDef.Object=0;drain.SpellLevel=0;drain.JustReleased=false;drain.SpellDelayFrames=delay;drain.DrainCount=0;drain.Shots._M_start=shots;drain.Shots._M_finish=shots+count;drain.Shots._M_end_of_storage=shots+3;
  drain.FrameUpdate();assert(!drain.JustReleased&&drain.SpellDelayFrames==delay-1&&drain.DrainCount==0);assert(parent.Removed==(!live||delay<=1?1:0));assert(GetItemQueries==expected_gets&&NodeAllocations==0&&NodeFrees==0&&TargetingQueries==0&&MapQueries==0);assert(CThingSearchTools::NoLookups==100&&CThingSearchTools::SearchID==42&&CThingSearchTools::NoThingsScanned==99);++cases;
 }}
 assert(cases==340);std::puts("007c36a0_TEST PASS");return 0;
}