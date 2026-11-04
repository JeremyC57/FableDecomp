// CAIStateGroup_WorkInWarehouse::IsRunnable() -- a village member in working hours whose village has a crate-carrying task for it (remembered in PTask) is runnable
// Leaf: ?IsRunnable@CAIStateGroup_WorkInWarehouse@@UAE_NXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00943C20, 206 bytes; vtable slot 11 (Ego_r.pdb ?IsRunnable@CAIStateGroup_WorkInWarehouse@@UAE_NXZ).
// Callees: CAIStateGroup_WorkBase<CAIStateGroupBase>::IsInWorkHours() const 0x0091A6F0 (Ego_r.pdb, protected), CVectorMap LowerBound
//   0x0040F020 (inside CThing::GetTC<CTCVillageMember>: the miss path reads the never-written out pointer), CTCVillageMember::
//   GetPVillage() const 0x007EE540, CTCVillage::GetBestCrateTaskForThing(const CThing&) 0x00658EA0 (header opt-in), the converting
//   CIntelligentPointer<CVillageTaskCarryCrate> temporary (vtable 0x0128B0B0; CBaseIntelligentPointer ctor 0x00A01B10, SetPItem
//   0x00A01B90, GetPItem 0x00A01B50, dtor 0x00A01C10) assigned through the inline operator=.
// Layout: Ego_r.pdb CAIStateGroup_WorkInWarehouse (0x24): PTask (CIntelligentPointer<CVillageTaskCarryCrate>) +0x1C behind the
//   CAIStateGroup_WorkBase<CAIStateGroupBase> members (+0x14..+0x1B, not modelled).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_CRATETASK
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CBaseObject.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCVillage.h"
class CVillageTaskBase : public CBaseObject { };         // RTTI CVillageTaskCarryCrate : CVillageTaskBase : CBaseObject
class CVillageTaskCarryCrate : public CVillageTaskBase { };
template<class TBase> class CAIStateGroup_WorkBase : public TBase {
protected:
    bool IsInWorkHours() const;                         // 0x0091A6F0
    unsigned char _pad_0x14[0x1c - 0x14];               // +0x14 the work-base members (not modelled)
};
class CAIStateGroup_WorkInWarehouse : public CAIStateGroup_WorkBase<CAIStateGroupBase> {
public:
    virtual bool IsRunnable();                          // slot 11
private:
    CIntelligentPointer<CVillageTaskCarryCrate> PTask;  // +0x1c
};

bool CAIStateGroup_WorkInWarehouse::IsRunnable()
{
    if (PBrain->Creature->HasTC(TCI_VILLAGE_MEMBER) && IsInWorkHours()) {
        CTCVillageMember* member;
        PBrain->Creature->GetTC(&member);
        CTCVillage* village = member->GetPVillage();
        if (village != 0) {
            PTask = village->GetBestCrateTaskForThing(*PBrain->Creature);
            if (PTask.Get() != 0)
                return true;
        }
    }
    return false;
}