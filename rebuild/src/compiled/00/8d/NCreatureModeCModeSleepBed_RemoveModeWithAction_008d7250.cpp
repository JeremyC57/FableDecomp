// NCreatureMode::CModeSleepBed::RemoveModeWithAction(const CCreatureActionBase& action) -- keep a clone of the action to perform when the
// sleeping mode is removed (PRemoveAction, +0x2C)
// Leaf: ?RemoveModeWithAction@CModeSleepBed@NCreatureMode@@QAEXABVCCreatureActionBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D7250, 25 bytes, thiscall `ret 4`.
// Identity: Ego_r.pdb 0x008185C0 / FableWin.pdb 0x02842700 ?RemoveModeWithAction@CModeSleepBed@NCreatureMode@@QAEXABVCCreatureActionBase@@@Z
//   (right after CModeSitBase::RemoveModeWithAction 0x008185A0 = retail 0x008D7230 in the donor order); PRemoveAction is Ego_r.pdb
//   CModeSleepBed +0x2C (CCountedPointer<CCreatureActionBase>, after Filter +0x1C and SleepAnimName +0x28); its caller
//   NCreatureMode::GetUpFromSleeping 0x0085F6F0 already binds this spelling. RTTI .?AVCModeSleepBed@NCreatureMode@@ :
//   CTCCreatureModeManager::CModeBase.
// Callees: the action's virtual Clone (slot 11, +0x2C), CCountedPointer<CCreatureActionBase>::Reset 0x00668D90.
// NF83 (round 7, q11020 binder + own name): re-authored off the stand-ins (CModeSitBase / CModeList / a local CCreatureActionBase,
//   catalogued as a second CModeSitBase::RemoveModeWithAction). CModeSleepBed has no shared header: the minimal local model below
//   (Ego_r.pdb members up to PRemoveAction) derives from the shared CTCCreatureModeManager::CModeBase.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCountedPointer.h"
#include "engine/CTCCreatureModeManager.h"
namespace NCreatureMode {
class CModeSleepBed : public CTCCreatureModeManager::CModeBase {        // Ego_r.pdb 0x34 bytes
public:
    void RemoveModeWithAction(const CCreatureActionBase& action);       // 0x008D7250
private:
    unsigned char Filter[0xc];                                           // +0x1c CAnimFilterOfGroup
    CCharString SleepAnimName;                                           // +0x28
    CCountedPointer<CCreatureActionBase> PRemoveAction;                  // +0x2c
};
}

void NCreatureMode::CModeSleepBed::RemoveModeWithAction(const CCreatureActionBase& action)
{
    PRemoveAction.Reset(action.Clone());
}
