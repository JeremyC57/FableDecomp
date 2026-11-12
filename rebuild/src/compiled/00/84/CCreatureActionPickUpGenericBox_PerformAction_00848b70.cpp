// CCreatureAction_PickUpGenericBox::PerformAction() -- the creature picks the box up: when the box is gone, dead or already carried by
// someone the action finishes; otherwise the creature's primary carry slot is emptied (a weapon taken out of a type-1 creature's hand is
// killed), the box goes into the carry slot its CCarryableDef names, and creature mode 0x22 is added.
// Leaf: ?PerformAction@CCreatureAction_PickUpGenericBox@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00848B70, 402 bytes; vtable 0x01274BD4 slot 3 (Ego_r.pdb 0x00523D20 and FableWin.pdb carry
// the same mangled name).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PThingToPickUp +0xa8, PCreature +0x08), CVectorMap::LowerBound 0x004365B0
//   (const; inlined PeekTC TCI_CARRYABLE 0x47) / 0x0040F020 (inlined GetTC TCI_CARRYING 0x46 and TCI_ENTITY_MODE_MANAGER 0x31),
//   CTCCarrying::RemoveThingFromPrimarySlot(bool) 0x006A1E00, CThing::Kill(bool) 0x004C9B80, CDefClassBase::GetSubDef<CCarryableDef>
//   0x005CC10E (COMDAT, body visible to the TU and called out of line), CTCCarrying::PutThingInCarrySlot(CThing&, long, bool) 0x006A1F80,
//   CTCCreatureModeManager::AddMode(unsigned long) 0x0073AE10, CCreatureActionBase::FinishAction (virtual slot 4, `call [reg+10h]`).
// Members: CTCCarryable::PCarryingTC +0x10, CClass::Type +0x10 (GetType() == 1), CThing::Dead +0x91 bit 0, CThing::PBaseDef +0x70,
//   CCarryableDef::ActiveCarrySlot +0x28, held->HasTC(TCI_WEAPON) = bit 72 of UsingComponents (`test ah,1` on the word at +0x28).
// Shape: IS_THING_ALIVE is inlined (two GetPItem calls); the two FinishAction calls are the ELSE arms of nested ifs, which is why retail
//   places them after the main path (carried arm, then dead arm) with their own shrink-wrapped epilogues -- early returns put them in
//   line (390 v 402 bytes). The three component lookups ignore their result (the miss path reads the out variable's frame word). The
//   CDefPointer local is released at the end with the null test folded after the dereference.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCARRYABLEDEF
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE
#define FABLE_RETAIL_CCREATUREACTION_GENERICBOX
#define FABLE_RETAIL_CTCCARRYING
#define FABLE_RETAIL_CTCCARRYING_PUTTHING
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#include "fable_string.h"
#include "engine/CDefPointer.h"
#include "engine/CCarryableDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingBaseDef.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCCarryable.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CCreatureAction_PickUpGenericBox.h"
#include "engine/ThingAlive.h"

void CCreatureAction_PickUpGenericBox::PerformAction()
{
    if (IS_THING_ALIVE(PThingToPickUp)) {
        const CTCCarryable* ptc_carryable;
        PThingToPickUp->PeekTC(TCI_CARRYABLE, &ptc_carryable);
        if (ptc_carryable->PCarryingTC == 0) {
            CTCCarrying* ptc_carrying;
            PCreature->GetTC(TCI_CARRYING, &ptc_carrying);
            CThing* held = ptc_carrying->RemoveThingFromPrimarySlot(false);
            if (held && !held->IsDead() && PCreature->GetType() == 1 && held->HasTC(TCI_WEAPON))
                held->Kill(true);
            CDefPointer<const CCarryableDef> def;
            PThingToPickUp->PeekBaseDef().GetSubDef(&def);
            ptc_carrying->PutThingInCarrySlot(*PThingToPickUp, def->ActiveCarrySlot, true);
            CTCCreatureModeManager* ptc_modes;
            PCreature->GetTC(TCI_ENTITY_MODE_MANAGER, &ptc_modes);
            ptc_modes->AddMode(0x22);
        } else {
            FinishAction();
        }
    } else {
        FinishAction();
    }
}