// CAIStateGroup_EatAtTavern::IsStaffed() -- find a live village resident whose WorkBuilding is this tavern.
// Retail 0x0096D2D0, 175 bytes. GetBuilding virtual slot 14; LowerBound 0x004365B0;
// GFGetPVillageInterface 0x007EE540 and intelligent pointer GetPItem 0x00A01B50.
// Retail WorkBuilding is +0x170 (donor +0x198); shared engine headers retain the donor layout.
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCVillage.h"
class CTCVillage;
class CTCVillageMember {
public:
    CTCVillage* GetPVillage() const;   // 0x007EE540
};
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
    const Entry* LowerBound(const ETCInterfaceType& key) const; // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CRetailInterfaceMap;
// Retail CThing prefix: PDB UsingComponents name, compact component map; Dead is +0x91 vs donor +0xa1.
struct CRetailThingView {
    unsigned char _prefix[0x20];
    unsigned long UsingComponents[9];
    CRetailInterfaceMap Components;
    unsigned char _beforeDead[0x91-0x4c];
    unsigned char Dead:1;
    template<class T> __forceinline bool GetInterface(T** out, long type) {
        if (UsingComponents[type >> 5] & (1UL << (type & 31))) {
            ETCInterfaceType key = (ETCInterfaceType)(type);
            CRetailInterfaceMap* map = &Components;
            const CRetailInterfaceMap::Entry* it = map->LowerBound(key);
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
inline CRetailThingView* ThingView(CThingAICreature* p) { return (CRetailThingView*)p; }


class CThingBuilding;
struct IStateGroupBuildingView {
    virtual void Slot00();
    virtual void Slot01();
    virtual void Slot02();
    virtual void Slot03();
    virtual void Slot04();
    virtual void Slot05();
    virtual void Slot06();
    virtual void Slot07();
    virtual void Slot08();
    virtual void Slot09();
    virtual void Slot10();
    virtual void Slot11();
    virtual void Slot12();
    virtual void Slot13();
    virtual CThingBuilding* GetBuilding();
};
struct CRetailWorkerView {
    unsigned char _beforeWorkBuilding[0x170];
    CIntelligentPointer<CThingAICreature> WorkBuilding;
    __forceinline CIntelligentPointer<CThingAICreature>& GetWorkBuilding() { return WorkBuilding; }
};
bool __fastcall IsStaffed(CAIStateGroupBase* self, void*)
{
    CThingBuilding* building = ((IStateGroupBuildingView*)self)->GetBuilding();
    CTCVillager* villager;
    if (ThingView(self->PBrain->Creature)->GetInterface(&villager, 0x23)) {
        CTCVillage* owner = ((CTCVillageMember*)(villager))->GetPVillage();
        if (owner) {
            CRetailVillagePeopleView* village = (CRetailVillagePeopleView*)owner;
            for (CVillagePersonNode* person = village->VillagePeople.Head->Next;
                 person != village->VillagePeople.Head; person = person->Next) {
                CIntelligentPointer<CThingAICreature>* p = &person->Value;
                if (p->GetPItem() && !ThingView(p->GetPItem())->Dead &&
                    (void*)((CRetailWorkerView*)p->GetPItem())->GetWorkBuilding().GetPItem() == building) return true;
            }
        }
    }
    return false;
}