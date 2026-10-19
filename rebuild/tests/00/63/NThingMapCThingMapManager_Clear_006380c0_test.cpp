#include <cstdio>
#ifndef _MSC_VER
#define FABLE_ENGINE_CTHINGMAPMANAGER_H
struct CWorld;
#endif
// NThingMap::CThingMapManager::Clear() -- delete thing maps, empty active lookups and reset map dimensions/counters
// Leaf: Clear
// Retail006380C0,188 bytes ending ret0063817B;4 following int3 bytes excluded.
// Propagated CShaderResource::CloseShaderBank owner wrong: native Init00638540 calls this,
// matching PDB NThingMap::CThingMapManager::Clear. Shared donor fields reconcile16-byte
// containers to retail12: ThingMaps8,ActiveThingMaps14,ActiveGameMaps20,NoThings2c,Dimensions30.
//8-byte CThingMapInfo=(PThingMap,RefCount), actual virtual dtor slot0; set Erase004A9CB0.
#include <string.h>
#include "engine/CThingMapManager.h"
namespace NThingMap {
struct CThingMap {virtual ~CThingMap();};
struct CThingMapInfo {CThingMap* PThingMap;long RefCount;};
template<class T> struct CThingMapArray {
    T* Start;T* Finish;T* End;
    unsigned long size()const {return Finish-Start;}
    T& operator[](unsigned long i) {return Start[i];}
};
struct CThingMapInfoArray : CThingMapArray<CThingMapInfo> {
    static CThingMapInfo* Copy(CThingMapInfo* first,CThingMapInfo* last,CThingMapInfo* result) {
        for(long n=last-first;n>0;--n,++first,++result) *result=*first;
        return result;
    }
    void Clear() {Finish=Copy(Finish,Finish,Start);}
};
struct CActiveThingMapsArray : CThingMapArray<CThingMapInfo*> {
    static CThingMapInfo** Copy(CThingMapInfo** first,CThingMapInfo** last,CThingMapInfo** result) {
        if(first==last) return result;
        long bytes=(char*)last-(char*)first;
        return (CThingMapInfo**)((char*)memmove(result,first,bytes)+bytes);
    }
    void Clear() {Finish=Copy(Finish,Finish,Start);}
};
struct CActiveGameMapsSet {
    struct Node {unsigned char Color;unsigned char Pad[3];Node* Parent;Node* Left;Node* Right;};
    Node* Head;unsigned long Count;unsigned char Comparator;
    void Erase(Node*); //004A9CB0
    void Clear() {
        if(Count) {Erase(Head->Parent);Head->Left=Head;Head->Parent=0;Head->Right=Head;Count=0;}
    }
};
struct C2DExtentsI {long X,Y;C2DExtentsI() {} C2DExtentsI(long x,long y):X(x),Y(y) {} void Set(long x,long y) {X=x;Y=y;}};
struct CThingMapManagerClearView {
    void* VTable;CWorld* PWorld;
    CThingMapInfoArray ThingMaps;
    CActiveThingMapsArray ActiveThingMaps;
    CActiveGameMapsSet ActiveGameMaps;
    long NoThings;C2DExtentsI Dimensions;
    void Clear();
};
void CThingMapManagerClearView::Clear() {
    for(unsigned long i=0;i<ThingMaps.size();++i) {
        if(ThingMaps[i].PThingMap) {delete ThingMaps[i].PThingMap;ThingMaps[i].PThingMap=0;}
    }
    ThingMaps.Clear();ActiveThingMaps.Clear();ActiveGameMaps.Clear();
    NoThings=0;Dimensions.Set(0,0);
}
}

namespace NThingMap {
static CThingMapManagerClearView* active;
static CThingMapInfo* replacement;
static int bad,destroyed,baseDestroyed,erased,moveStorage;
static CThingMap* expectedMaps[2];
CThingMap::~CThingMap() {++baseDestroyed;}
struct Map : CThingMap {
    int ID;
    Map(int id):ID(id) {}
    ~Map() {
        if(ID!=destroyed || active->NoThings!=17 || active->Dimensions.X!=23 || active->Dimensions.Y!=29 ||
           erased || active->ThingMaps.Finish-active->ThingMaps.Start!=3) bad=1;
        ++destroyed;
        if(ID==0 && moveStorage) {
            for(int i=0;i<3;++i) replacement[i]=active->ThingMaps.Start[i];
            active->ThingMaps.Start=replacement;active->ThingMaps.Finish=replacement+3;active->ThingMaps.End=replacement+3;
        }
    }
};
void CActiveGameMapsSet::Erase(Node* root) {
    if(this!=&active->ActiveGameMaps || root!=Head->Parent || active->ThingMaps.Finish!=active->ThingMaps.Start ||
       active->ActiveThingMaps.Finish!=active->ActiveThingMaps.Start || active->NoThings!=17 || active->Dimensions.X!=23) bad=1;
    ++erased;
}
static int run(int empty,int nonemptyTree,int relocate) {
    CThingMapManagerClearView manager;active=&manager;
    CThingMapInfo original[3],moved[3];replacement=moved;
    Map* a=empty?0:new Map(0);Map* b=empty?0:new Map(1);expectedMaps[0]=a;expectedMaps[1]=b;
    original[0].PThingMap=a;original[0].RefCount=31;original[1].PThingMap=0;original[1].RefCount=37;original[2].PThingMap=b;original[2].RefCount=41;
    manager.ThingMaps.Start=original;manager.ThingMaps.Finish=original+(empty?0:3);manager.ThingMaps.End=original+3;
    CThingMapInfo* actives[3]={original,original+1,original+2};
    manager.ActiveThingMaps.Start=actives;manager.ActiveThingMaps.Finish=actives+(empty?0:3);manager.ActiveThingMaps.End=actives+3;
    CActiveGameMapsSet::Node head,root;
    manager.ActiveGameMaps.Head=&head;manager.ActiveGameMaps.Count=nonemptyTree?2:0;manager.ActiveGameMaps.Comparator=47;
    head.Parent=nonemptyTree?&root:0;head.Left=nonemptyTree?&root:&head;head.Right=nonemptyTree?&root:&head;
    manager.NoThings=17;manager.Dimensions.X=23;manager.Dimensions.Y=29;manager.PWorld=(CWorld*)&root;
    bad=destroyed=baseDestroyed=erased=0;moveStorage=relocate;
    manager.Clear();
    CThingMapInfo* data=(!empty && relocate)?moved:original;
    if(bad || destroyed!=(empty?0:2) || baseDestroyed!=destroyed || erased!=nonemptyTree ||
       manager.ThingMaps.Start!=data || manager.ThingMaps.Finish!=data || manager.ThingMaps.End!=data+3 ||
       manager.ActiveThingMaps.Start!=actives || manager.ActiveThingMaps.Finish!=actives || manager.ActiveThingMaps.End!=actives+3 ||
       manager.ActiveGameMaps.Count || manager.ActiveGameMaps.Head!=&head || manager.ActiveGameMaps.Comparator!=47 ||
       head.Parent || head.Left!=&head || head.Right!=&head || manager.NoThings || manager.Dimensions.X || manager.Dimensions.Y ||
       manager.PWorld!=(CWorld*)&root) return 0;
    if(!empty && (data[0].PThingMap || data[1].PThingMap || data[2].PThingMap || data[0].RefCount!=31 || data[1].RefCount!=37 || data[2].RefCount!=41)) return 0;
    return true;
}
}
int main() {
    using namespace NThingMap;
    if(!run(1,0,0) || !run(1,1,0) || !run(0,0,0) || !run(0,1,1)) {printf("006380c0_TEST FAIL\n");return 1;}
    printf("006380c0_TEST PASS\n");return 0;
}