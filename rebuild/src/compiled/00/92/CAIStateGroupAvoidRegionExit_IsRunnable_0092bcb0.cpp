// CAIStateGroup_AvoidRegionExit::IsRunnable() -- recover the live entrance position unless walk-to-marker disallows it.
// Plan phase 5 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's pointer members First/Last are the header CArray base's begin() / end() (STLport vector start +0 / finish +4) re-padded by-value maps +8.
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Grade: RELOCATION_MATCH -- compiler-generated vector assignment preserves the retail shared destination-address adjustment (207 bytes).
#include "fable_compiler.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCVillage.h"
struct CTCVillager;
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CRetailInterfaceMap;
// Retail CThing prefix: PDB UsingComponents name, compact component map; Dead is +0x91 vs donor +0xa1.
struct CRetailThingView {
    unsigned char _prefix[0x20];
    unsigned long UsingComponents[9];
    CRetailInterfaceMap Components;
    unsigned char _beforeDead[0x3d];
    unsigned char Dead:1;
    bool HasTC(long type) const { if (UsingComponents[type>>5] & (1UL<<(type&31))) return true; return false; }
    void* GetTC(long type) {
      ETCInterfaceType key = (ETCInterfaceType)(type); CRetailInterfaceMap* map=&Components;
      CRetailInterfaceMap::value_type* it=map->LowerBound(key);
      if(it==map->end() || type<it->first) it=map->end();
      return ((void*)it->second);
    }
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** out, long type) {
        if (UsingComponents[type >> 5] & (1UL << (type & 31))) {
            ETCInterfaceType key = (ETCInterfaceType)(type);
            CRetailInterfaceMap* map = &Components;
            CRetailInterfaceMap::value_type* it = map->LowerBound(key);
            if (it == map->end() || type < it->first) it = map->end();
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
#include "engine/C3DVector.h"
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