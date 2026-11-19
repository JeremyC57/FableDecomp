// CCreatureAction_GetPlateFromCupboard::PerformAction() -- create the plate thing (ItemDefIndex) at the cupboard for the creature's
//   player, put it in the creature's carry slot named by the plate's carryable def, and add creature mode 5
// Leaf: ?PerformAction@CCreatureAction_GetPlateFromCupboard@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00848500, 275 bytes, __thiscall, plain `ret`. No retail vtable or call reaches it (no RTTI for the
//   class in Fable.exe); Ego_r.pdb 0x005235F0 / FableWin.pdb 0x01DD4D20 ?PerformAction@CCreatureAction_GetPlateFromCupboard@@UAEXXZ (virtual).
// Callees: CCharString(const char*, long) 0x0099EBF0 ("" 0x0122D70E) / ~CCharString 0x0099EAE0; CBaseIntelligentPointer::GetPItem
//   0x00A01B50 (PCreature +0x08); CThing::GetPos 0x004C73D0; CThingObject::Create 0x00703210 (static __fastcall);
//   CDefClassBase::GetSubDef<CCarryableDef> 0x005CC10E (out of line); CVectorMap<ETCInterfaceType,CTCBase*,..>::LowerBound 0x0040F020
//   (the expanded unchecked GetTC: TCI_CARRYING 70, TCI_ENTITY_MODE_MANAGER 49); CTCCarrying::PutThingInCarrySlot 0x006A1F80;
//   CTCCreatureModeManager::AddMode(unsigned long) 0x0073AE10; the carryable def's virtual delete (slot 1) on the last reference.
// Layout: retail CCreatureActionBase is 0xA8 bytes; the class's own members (Ego_r.pdb, each 0xC lower in retail) Cupboard +0xA8,
//   ItemDefIndex +0xAC, CupboardAnimIndex +0xB0. No shared header exists for this class, so it is declared here over the shared
//   engine/CCreatureActionBase.h (0x008D5300 form).
// Port (NF101, 2026-11-19, q11260): moved off its 12-class local model onto the shared headers (0x00848B70 define set); Create now
//   binds the PDB ?Create@CThingObject@@SIPAV1@JABVC3DVector@@JPBVCRightHandedSet@@KABVCCharString@@@Z (was the free stand-in CreateObject).
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
#define FABLE_RETAIL_CTHINGOBJECT_CREATE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE
#define FABLE_RETAIL_CTCCARRYING
#define FABLE_RETAIL_CTCCARRYING_PUTTHING
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/CDefPointer.h"
#include "engine/CCarryableDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingObject.h"
#include "engine/CThingBaseDef.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CCreatureActionBase.h"

class CCreatureAction_GetPlateFromCupboard : public CCreatureActionBase {
public:
    virtual void PerformAction();
protected:
    CThing* Cupboard;                                        // +0xA8
    long    ItemDefIndex;                                    // +0xAC
    long    CupboardAnimIndex;                               // +0xB0
};

void CCreatureAction_GetPlateFromCupboard::PerformAction()
{
    CThingObject* pplate = CThingObject::Create(ItemDefIndex, Cupboard->GetPos(), PCreature->GetPlayerNumber(), 0, 0, "");
    CDefPointer<const CCarryableDef> pcarry_def;
    pplate->PeekBaseDef().GetSubDef(&pcarry_def);
    CTCCarrying* ptc_carrying;
    PCreature->GetTC(TCI_CARRYING, &ptc_carrying);
    ptc_carrying->PutThingInCarrySlot(*pplate, pcarry_def->ActiveCarrySlot, true);
    CTCCreatureModeManager* ptc_modes;
    PCreature->GetTC(TCI_ENTITY_MODE_MANAGER, &ptc_modes);
    ptc_modes->AddMode(5);
}
