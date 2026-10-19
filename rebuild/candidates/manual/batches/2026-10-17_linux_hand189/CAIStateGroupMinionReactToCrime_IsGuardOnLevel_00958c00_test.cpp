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


#include <cstdio>
#include <cstring>
struct CTCVillager {};
static CTCVillage* villageResult;
static int lookupCalls;
CRetailInterfaceMap::Entry* CRetailInterfaceMap::LowerBound(const long* key) {
 ++lookupCalls; Entry* p=First;while(p!=Last && p->Type<*key)++p;return p;
}
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager*) { return villageResult; }
CThingAICreature* CIntelligentPointerView::GetPItem() const { return Object; }
int main() {
 CAIBrain brain; CAIStateGroupBase state; CRetailThingView creature, first, second;
 std::memset(&brain,0,sizeof brain);std::memset(&state,0,sizeof state);
 std::memset(&creature,0,sizeof creature);std::memset(&first,0,sizeof first);std::memset(&second,0,sizeof second);
 brain.Creature=(CThingAICreature*)&creature;state.PBrain=&brain;
 CTCVillager villager; CRetailInterfaceMap::Entry entries[2];
 entries[0].Type=0x23;entries[0].Component=&villager; entries[1].Type=0x7fffffff;entries[1].Component=0;
 creature.Components.First=entries;creature.Components.Last=entries+1;
 CVillagePersonNode head,n1,n2; head.Next=&head;head.Previous=&head;
 CRetailVillagePeopleView village;village.VillagePeople.Head=&head;villageResult=(CTCVillage*)&village;
 if(IsGuardOnLevel(&state,0) || lookupCalls) return 1;
 creature.UsingComponents[1]=8; villageResult=0;
 if(IsGuardOnLevel(&state,0)) return 2;
 villageResult=(CTCVillage*)&village;
 if(IsGuardOnLevel(&state,0)) return 3;
 head.Next=&n1;n1.Next=&n2;n2.Next=&head;
 n1.Value.Object=(CThingAICreature*)&first;n2.Value.Object=(CThingAICreature*)&second;
 first.Dead=1;first.UsingComponents[5]=0x100000;
 if(IsGuardOnLevel(&state,0)) return 4;
 second.UsingComponents[5]=0x100000;
 if(!IsGuardOnLevel(&state,0)) return 5;
 second.Dead=1;if(IsGuardOnLevel(&state,0)) return 6;
 first.Dead=0;if(!IsGuardOnLevel(&state,0)) return 7;
 n1.Value.Object=0;if(IsGuardOnLevel(&state,0)) return 8;
 std::printf("00958c00_TEST PASS\n");return 0;
}
