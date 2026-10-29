// CAIStateGroup_AvoidRegionExit::IsRunnable() -- recover the live entrance position unless walk-to-marker disallows it.
// Grade: RELOCATION_MATCH -- compiler-generated vector assignment preserves the retail shared destination-address adjustment (207 bytes).
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCVillage.h"
struct CTCVillager;
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> Entry;
    Entry* First;
    Entry* Last;
    Entry* LowerBound(const ETCInterfaceType& key); // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CRetailInterfaceMap;
// Retail CThing prefix: PDB UsingComponents name, compact component map; Dead is +0x91 vs donor +0xa1.
struct CRetailThingView {
    unsigned char _prefix[0x20];
    unsigned long UsingComponents[9];
    CRetailInterfaceMap Components;
    unsigned char _beforeDead[0x91-0x4c];
    unsigned char Dead:1;
    bool HasTC(long type) const { if (UsingComponents[type>>5] & (1UL<<(type&31))) return true; return false; }
    void* GetTC(long type) {
      ETCInterfaceType key = (ETCInterfaceType)(type); CRetailInterfaceMap* map=&Components;
      CRetailInterfaceMap::Entry* it=map->LowerBound(key);
      if(it==map->Last || type<it->first) it=map->Last;
      return ((void*)it->second);
    }
    template<class T> __forceinline bool GetInterface(T** out, long type) {
        if (UsingComponents[type >> 5] & (1UL << (type & 31))) {
            ETCInterfaceType key = (ETCInterfaceType)(type);
            CRetailInterfaceMap* map = &Components;
            CRetailInterfaceMap::Entry* it = map->LowerBound(key);
            if (it == map->Last || type < it->first) it = map->Last;
            *out = (T*)((void*)it->second);
            return true;
        }
        return false;
    }
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
class CThingAICreature;
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
struct CVillagePersonNode {
    CVillagePersonNode* Next;
    CVillagePersonNode* Previous;
    CIntelligentPointer<CThingAICreature> Value;
};
// The engine village header retains donor 12-byte lists (+0x34 VillagePeople).
// Retail lists are one head pointer, so VillagePeople is +0x2c; no header is redefined.
struct CVillagePeopleList { CVillagePersonNode* Head; };
struct CRetailVillagePeopleView {
    unsigned char _beforePeople[0x2c];
    CVillagePeopleList VillagePeople;
};
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager*); // 0x007EE540
inline CRetailThingView* ThingView(CThingAICreature* p) { return (CRetailThingView*)p; }


#include "engine/CAIStateGroup_AvoidRegionExit.h"
class C3DVector { public:
 float X,Y,Z;
 C3DVector() {}
 C3DVector(const C3DVector& o):X(o.X),Y(o.Y),Z(o.Z) {}
};
// Retail CTCWalkToMarker bool fields; donor header is not available.
struct CTCWalkToMarkerView {
 unsigned char _before68[0x68]; bool Flag68;
 unsigned char _before7b[0x12]; bool Flag7b;
};
struct CTCAvoidRegionExitView {
 unsigned char _base[0xc]; CIntelligentPointer<CThingAICreature> EntranceMarker;
 C3DVector GetPosition(); // 0092BD80
};
struct CAvoidStateView : CAIStateGroupBase { C3DVector Position; bool IsRunnable(); };
bool CAvoidStateView::IsRunnable() {
 CRetailThingView* creature=ThingView(PBrain->Creature);
 if(creature->HasTC(0xaa)) {
  CTCWalkToMarkerView* walk=(CTCWalkToMarkerView*)creature->GetTC(0xaa);
  if(!walk->Flag7b || walk->Flag68) return false;
 }
 if(creature->HasTC(0x105)) {
  CTCAvoidRegionExitView* avoid=(CTCAvoidRegionExitView*)creature->GetTC(0x105);
  if(avoid->EntranceMarker.GetPItem() && !ThingView(avoid->EntranceMarker.GetPItem())->Dead) {
   Position=avoid->GetPosition();
   return true;
  }
 }
 return false;
}