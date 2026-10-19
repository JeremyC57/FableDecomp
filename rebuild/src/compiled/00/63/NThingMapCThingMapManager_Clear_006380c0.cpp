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