// CAIStateGroup_MinionReactToCrime::IsGuardOnLevel() -- find a live guard in the creature's village.
// Retail 0x00958C00, 163 bytes. LowerBound 0x0040F020, GFGetPVillageInterface 0x007EE540,
// intelligent pointer GetPItem 0x00A01B50. Interface 0x23 is villager; guard is bit 20 at +0x34.
// PDB names plus landed GetBuilding 0x0096D260 establish the villager/village lookup.
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCVillage.h"
struct CTCVillager;
struct CRetailInterfaceMap {
    struct Entry { long Type; void* Component; };
    Entry* First;
    Entry* Last;
    Entry* LowerBound(const long*); // 0x0040F020
};
// Retail CThing prefix: PDB UsingComponents name, compact component map; Dead is +0x91 vs donor +0xa1.
struct CRetailThingView {
    unsigned char _prefix[0x20];
    unsigned long UsingComponents[9];
    CRetailInterfaceMap Components;
    unsigned char _beforeDead[0x91-0x4c];
    unsigned char Dead:1;
    template<class T> __forceinline bool GetInterface(T** out, long type) {
        if (UsingComponents[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CRetailInterfaceMap* map = &Components;
            CRetailInterfaceMap::Entry* it = map->LowerBound(&key);
            if (it == map->Last || type < it->Type) it = map->Last;
            *out = (T*)it->Component;
            return true;
        }
        return false;
    }
};
struct CIntelligentPointerView {
    void* __vftable;
    CThingAICreature* Object;
    CThingAICreature* GetPItem() const; // 0x00A01B50
};
struct CVillagePersonNode {
    CVillagePersonNode* Next;
    CVillagePersonNode* Previous;
    CIntelligentPointerView Value;
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

bool __fastcall IsGuardOnLevel(CAIStateGroupBase* self, void*)
{
    CTCVillager* villager;
    if (ThingView(self->PBrain->Creature)->GetInterface(&villager, 0x23)) {
        if (GFGetPVillageInterface(villager)) {
            CRetailVillagePeopleView* village = (CRetailVillagePeopleView*)GFGetPVillageInterface(villager);
            CVillagePeopleList* people = &village->VillagePeople;
            for (CVillagePersonNode* person = people->Head->Next;
                 person != people->Head; person = person->Next) {
                CIntelligentPointerView* p = &person->Value;
                if (p->GetPItem() && !ThingView(p->GetPItem())->Dead &&
                    (ThingView(p->GetPItem())->UsingComponents[5] & 0x100000)) return true;
            }
        }
    }
    return false;
}