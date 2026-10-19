// CAIStateGroup_EatAtTavern::IsStaffed() -- find a live village resident whose WorkBuilding is this tavern.
// Retail 0x0096D2D0, 175 bytes. GetBuilding virtual slot 14; LowerBound 0x004365B0;
// GFGetPVillageInterface 0x007EE540 and intelligent pointer GetPItem 0x00A01B50.
// Retail WorkBuilding is +0x170 (donor +0x198); shared engine headers retain the donor layout.
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCVillage.h"
struct CTCVillager;
struct CRetailInterfaceMap {
    struct Entry { long Type; void* Component; };
    Entry* First;
    Entry* Last;
    Entry* LowerBound(const long*); // 0x004365B0
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


struct CThingBuilding;
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
    CIntelligentPointerView WorkBuilding;
    __forceinline CIntelligentPointerView& GetWorkBuilding() { return WorkBuilding; }
};
bool __fastcall IsStaffed(CAIStateGroupBase* self, void*)
{
    CThingBuilding* building = ((IStateGroupBuildingView*)self)->GetBuilding();
    CTCVillager* villager;
    if (ThingView(self->PBrain->Creature)->GetInterface(&villager, 0x23)) {
        CTCVillage* owner = GFGetPVillageInterface(villager);
        if (owner) {
            CRetailVillagePeopleView* village = (CRetailVillagePeopleView*)owner;
            for (CVillagePersonNode* person = village->VillagePeople.Head->Next;
                 person != village->VillagePeople.Head; person = person->Next) {
                CIntelligentPointerView* p = &person->Value;
                if (p->GetPItem() && !ThingView(p->GetPItem())->Dead &&
                    (void*)((CRetailWorkerView*)p->GetPItem())->GetWorkBuilding().GetPItem() == building) return true;
            }
        }
    }
    return false;
}

void IStateGroupBuildingView::Slot00() {}
void IStateGroupBuildingView::Slot01() {}
void IStateGroupBuildingView::Slot02() {}
void IStateGroupBuildingView::Slot03() {}
void IStateGroupBuildingView::Slot04() {}
void IStateGroupBuildingView::Slot05() {}
void IStateGroupBuildingView::Slot06() {}
void IStateGroupBuildingView::Slot07() {}
void IStateGroupBuildingView::Slot08() {}
void IStateGroupBuildingView::Slot09() {}
void IStateGroupBuildingView::Slot10() {}
void IStateGroupBuildingView::Slot11() {}
void IStateGroupBuildingView::Slot12() {}
void IStateGroupBuildingView::Slot13() {}

#include <cstdio>
#include <cstring>
struct CTCVillager {};
static CTCVillage* villageResult;
static CThingBuilding* buildingResult;
CRetailInterfaceMap::Entry* CRetailInterfaceMap::LowerBound(const long* key) {
 Entry* p=First;while(p!=Last && p->Type<*key)++p;return p;
}
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager*) { return villageResult; }
CThingAICreature* CIntelligentPointerView::GetPItem() const { return Object; }
CThingBuilding* IStateGroupBuildingView::GetBuilding() { return buildingResult; }
struct CTestState : IStateGroupBuildingView { CAIBrain* Brain; };
int main() {
 CAIBrain brain; CTestState state; CRetailThingView creature;
 std::memset(&brain,0,sizeof brain);std::memset(&creature,0,sizeof creature);
 brain.Creature=(CThingAICreature*)&creature;state.Brain=&brain;
 CTCVillager villager; CRetailInterfaceMap::Entry entries[2];
 entries[0].Type=0x23;entries[0].Component=&villager;entries[1].Type=0x7fffffff;entries[1].Component=0;
 creature.Components.First=entries;creature.Components.Last=entries+1;
 CVillagePersonNode head,n1,n2;head.Next=&head;
 CRetailVillagePeopleView village;village.VillagePeople.Head=&head;villageResult=(CTCVillage*)&village;
 char tavernToken,otherToken;buildingResult=(CThingBuilding*)&tavernToken;
 if(IsStaffed((CAIStateGroupBase*)&state,0)) return 1;
 creature.UsingComponents[1]=8;villageResult=0;
 if(IsStaffed((CAIStateGroupBase*)&state,0)) return 2;
 villageResult=(CTCVillage*)&village;if(IsStaffed((CAIStateGroupBase*)&state,0)) return 3;
 CRetailWorkerView first,second;std::memset(&first,0,sizeof first);std::memset(&second,0,sizeof second);
 head.Next=&n1;n1.Next=&n2;n2.Next=&head;
 n1.Value.Object=(CThingAICreature*)&first;n2.Value.Object=(CThingAICreature*)&second;
 first.WorkBuilding.Object=(CThingAICreature*)&otherToken;
 second.WorkBuilding.Object=(CThingAICreature*)&tavernToken;
 ThingView((CThingAICreature*)&second)->Dead=1;
 if(IsStaffed((CAIStateGroupBase*)&state,0)) return 4;
 ThingView((CThingAICreature*)&second)->Dead=0;
 if(!IsStaffed((CAIStateGroupBase*)&state,0)) return 5;
 n2.Value.Object=0;if(IsStaffed((CAIStateGroupBase*)&state,0)) return 6;
 first.WorkBuilding.Object=(CThingAICreature*)&tavernToken;
 if(!IsStaffed((CAIStateGroupBase*)&state,0)) return 7;
 std::printf("0096d2d0_TEST PASS\n");return 0;
}