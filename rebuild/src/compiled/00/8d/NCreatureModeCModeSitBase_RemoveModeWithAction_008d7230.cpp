// NCreatureMode::CModeSitBase::RemoveModeWithAction(const CCreatureActionBase& action) -- keep a clone of the action to perform when the
// sitting mode is removed (PRemoveAction, +0x24)
// Leaf: ?RemoveModeWithAction@CModeSitBase@NCreatureMode@@QAEXABVCCreatureActionBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D7230, 25 bytes, thiscall `ret 4`.
// Identity: Ego_r.pdb 0x008185A0 / FableWin.pdb 0x028423A0 ?RemoveModeWithAction@CModeSitBase@NCreatureMode@@QAEXABVCCreatureActionBase@@@Z;
//   PRemoveAction is Ego_r.pdb CModeSitBase +0x24 (CCountedPointer<CCreatureActionBase>); its caller NCreatureMode::GetUpFromSitting
//   0x0085FF10. The CModeSleepBed twin (PRemoveAction +0x2C) is 0x008D7250.
// Callees: the action's virtual Clone (slot 11, +0x2C), CCountedPointer<CCreatureActionBase>::Reset 0x00668D90.
// NF83 (round 7, q11020 binder + own leaf): ported onto engine/NCreatureMode_CModeSitFloor.h (NF83 overlay opt-in
//   NCREATUREMODE_CMODESITBASE_REMOVEMODEWITHACTION) and engine/CCreatureActionBase.h; the local CModeSitBase / CModeSitSub /
//   CCreatureActionBase models are gone, so Reset binds the template spelling and the own leaf is the PDB one.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_NCREATUREMODE_CMODESITBASE_REMOVEMODEWITHACTION
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CCreatureActionBase.h"
#include "engine/NCreatureMode_CModeSitFloor.h"

void NCreatureMode::CModeSitBase::RemoveModeWithAction(const CCreatureActionBase& action)
{
    PRemoveAction.Reset(action.Clone());
}
