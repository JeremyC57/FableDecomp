// CCreatureAction_PickUpThingAtTable::PerformAction() -- when the thing and the creature are alive: put the thing in the creature's carry slot named by its carryable def, then add the action's creature mode (if any)
// Leaf: ?PerformAction@CCreatureAction_PickUpThingAtTable@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00849F70, 302 bytes, virtual PerformAction. Ego_r.pdb ?PerformAction@CCreatureAction_PickUpThingAtTable@@UAEXXZ.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap::LowerBound 0x0040F020 (CThing::GetTC<CTCCarrying> TCI 0x46,
//   GetTC<CTCCreatureModeManager> TCI 0x31), CDefClassBase::GetSubDef<CCarryableDef> 0x005CC10E (body in the TU),
//   CTCCarrying::PutThingInCarrySlot 0x006A1F80, CTCCreatureModeManager::AddMode(unsigned long) 0x0073AE10; the def's release (slot 1).
// Layout: Ego_r.pdb PThingToPickUp / Mode at retail +0xA8 / +0xB0 (retail CCreatureActionBase 0xA8); CCarryableDef ActiveCarrySlot +0x28.
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CTCCARRYING
#define FABLE_RETAIL_CTCCARRYING_PUTTHING
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/NCreatureMode.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CDefClassBase.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CThingBaseDef.h"

class CCarryableDef : public CSubDefClassBase { public:        // RTTI .?AVCCarryableDef@@ : CSubDefClassBase
    static CCharString __fastcall StatGetTypeName() { return CCharString("CCarryableDef", -1); }
    long ActiveCarrySlot;                                       // +0x28
    long SecondaryActiveCarrySlot;                              // +0x2C
    long PassiveCarrySlot;                                      // +0x30
};


template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const   // <CCarryableDef> 0x005CC10E
{
    const CSubDefInfo* info = GetPSubDefInfo(T::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}

class CCreatureAction_PickUpThingAtTable : public CCreatureActionBase { public:
    virtual void PerformAction();
    CIntelligentPointer<CThing> PThingToPickUp;                 // +0xA8
    NCreatureMode::EMode Mode;                                  // +0xB0
};

void CCreatureAction_PickUpThingAtTable::PerformAction()
{
    if (PThingToPickUp.GetPItem() && !PThingToPickUp->IsDead()) {
        if (PCreature.GetPItem() && !PCreature->IsDead()) {
            CTCCarrying* carrying;
            PCreature->GetTC(&carrying);
            CDefPointer<const CCarryableDef> carryableDef;
            PThingToPickUp->PeekBaseDef().GetSubDef(&carryableDef);
            carrying->PutThingInCarrySlot(*PThingToPickUp.Get(), carryableDef->ActiveCarrySlot, true);
            if (Mode) {
                CTCCreatureModeManager* modeManager;
                PCreature->GetTC(&modeManager);
                modeManager->AddMode(Mode);
            }
        }
    }
}