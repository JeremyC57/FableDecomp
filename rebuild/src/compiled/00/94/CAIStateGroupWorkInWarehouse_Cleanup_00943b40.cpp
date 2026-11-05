// CAIStateGroup_WorkInWarehouse::Cleanup() -- if the group still holds its carry-crate task and the creature is a village member, remove the creature from the task through its village and clear PTask; then always run the move-goods base Cleanup
// Leaf: ?Cleanup@CAIStateGroup_WorkInWarehouse@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x00943B40, 215 bytes, thiscall, `ret`.
// Identity (proven): vtable 0x0128BC80 (RTTI CAIStateGroup_WorkInWarehouse) slot 9 = Cleanup; Ego_r.pdb
//   ?Cleanup@CAIStateGroup_WorkInWarehouse@@UAEXXZ (donor 0x00967930, between InitCreaturePosition and IsRunnable).
//   Ego_r.pdb member PTask (CIntelligentPointer<CVillageTaskCarryCrate>) at +0x1C after CAIStateGroup_MoveGoodsBase
//   (0x1C bytes: DroppingGoods +0x14, PickUpGoodsActionID +0x18); the base call 0x009162F0 is
//   CAIStateGroup_MoveGoodsBase::Cleanup (vtable 0x012898DC slot 9).
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50, CVectorMap<ETCInterfaceType, CTCBase*,
//   CKeyPairCompareLess<..> >::LowerBound 0x0040F020 (interface 0x23 = CTCVillageMember: bit 3 of +0x24),
//   CTCVillageMember::GetPVillage() const 0x007EE540, CBaseIntelligentPointer() 0x00A01B10 / SetPItem 0x00A01B90 /
//   ~CBaseIntelligentPointer 0x00A01C10, CTCVillage::RemoveThingFromTask(CIntelligentPointer<CVillageTaskBase>,
//   const CThing&) 0x00659EFC (Ego_r.pdb), CAIStateGroup_MoveGoodsBase::Cleanup 0x009162F0.
// Shape: the base Cleanup runs on every path (both early-outs jump to it); the HasTC result is a named bool (spilled
//   to a frame byte, `shr ecx,3 ; and cl,1 ; mov [esp+13h],cl`); the by-value CIntelligentPointer<CVillageTaskBase>
//   argument is built in its stack slot by the inline converting constructor (vtable 0x0125A8A4), `PTask = 0` goes
//   through a CIntelligentPointer<CVillageTaskCarryCrate> temporary (vtable 0x0128B0B0). The earlier parked attempt
//   called the base Cleanup only inside the guarded block (wrong early-out target, plus an ebx/ebp swap): with the call
//   at function scope `this` stays live to the end and takes ebp as in retail.
// Layout: CAIBrain through the engine/CAIBrain.h layout-rename pattern (Creature +0x18); the rest local partial views as the landed CAIStateGroup_VillageTaskTorch::Cleanup
//   0x0093EA00 / MoveGoodsBase::Cleanup (engine/CAIStateGroupBase.h flattens the vptr; engine/CTCVillage.h has no methods).
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                            // Ego_r: vptr + PData, 8 bytes
public:
    CBaseIntelligentPointer();                             // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                    // 0x00A01C10
    CBaseObject* GetPItem() const;                         // 0x00A01B50
    void SetPItem(CBaseObject* item);                      // 0x00A01B90
private:
    CBaseObjectPointer* PData;                             // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    template <class U> CIntelligentPointer(CIntelligentPointer<U>& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CTCBase;
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingAICreature : public CThing { };
class CVillageTaskBase { };
class CVillageTaskCarryCrate : public CVillageTaskBase { };
#define CTCVillage CTCVillageLayout                  // engine/CTCVillage.h: flat data layout without the methods
#include "engine/CTCVillage.h"
#undef CTCVillage
class CTCVillage {
public:
    void RemoveThingFromTask(CIntelligentPointer<CVillageTaskBase> task, const CThing& thing);   // 0x00659EFC
};
class CTCVillageMember {
public:
    CTCVillage* GetPVillage() const;                       // 0x007EE540
};
#define CAIBrain CAIBrainLayout                      // engine/CAIBrain.h: Creature +0x18 (flattened vptr; GetCreature() inline)
#include "engine/CAIBrain.h"
#undef CAIBrain
class CAIBrain {
public:
    unsigned char     _pad00[0x18];
    CThingAICreature* Creature;                            // +0x18
    CThingAICreature& GetCreature() const { return *Creature; }   // Ego_r.pdb ?GetCreature@CAIBrain@@QBEAAVCThingAICreature@@XZ (inline)
};
class CAIStateGroupBase {
public:
    virtual ~CAIStateGroupBase();
    CAIBrain*     PBrain;                                  // +0x04
    unsigned char _pad08[0x0C];
};
class CAIStateGroup_MoveGoodsBase : public CAIStateGroupBase {
public:
    bool DroppingGoods;                                    // +0x14
    long PickUpGoodsActionID;                              // +0x18
    virtual void Cleanup();                                // 0x009162F0
};
class CAIStateGroup_WorkInWarehouse : public CAIStateGroup_MoveGoodsBase {
public:
    CIntelligentPointer<CVillageTaskCarryCrate> PTask;     // +0x1C
    virtual void Cleanup();
};

void CAIStateGroup_WorkInWarehouse::Cleanup()
{
    if (PTask.GetPItem()) {
        bool isVillageMember = PBrain->GetCreature().HasTC(TCI_VILLAGE_MEMBER);
        if (isVillageMember) {
            CTCVillage* village = ((CTCVillageMember*)&PBrain->GetCreature().GetTC(TCI_VILLAGE_MEMBER))->GetPVillage();
            village->RemoveThingFromTask(PTask, PBrain->GetCreature());
            PTask = 0;
        }
    }
    CAIStateGroup_MoveGoodsBase::Cleanup();
}