// CBrainOutput::SetAsPerformAction(const CCreatureActionBase& action) -- mark the brain output as performing an action (Types bit 8) and
// keep a clone of the action (PAction, +0x20)
// Leaf: ?SetAsPerformAction@CBrainOutput@@QAEXABVCCreatureActionBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00860130, 32 bytes, thiscall `ret 4`; its caller is 0x0094B253 (a stack CBrainOutput, the action
//   by address).
// Identity (NF83, round 7, q11020 binder + own name): Ego_d.pdb carved function CBrainOutput::SetAsPerformAction
//   (creature_mode_helpers.obj, debug length 51; ghidra_out/egod_carved_functions.tsv), whose parameter `action` has the same type
//   index (0x2304c, read with egod_locals.py) as NCreatureMode::CModeSleepBed::RemoveModeWithAction's, i.e. the Ego_r.pdb
//   `const CCreatureActionBase&`; the body is the brain-output form landed rows inline as SetAction (Types |= 8; PAction.Reset(Clone())).
//   Ego_r.pdb CBrainOutput: Types +0, LookFlags +4, NavigationInput +8, PAction +0x20 (CCountedPointer<CCreatureActionBase>).
//   The name is Ego_d-only (neither Ego_r nor FableWin keeps it): one PDB source plus the body and layout. Was catalogued as a
//   third NCreatureMode::CModeSitBase::RemoveModeWithAction (a free __fastcall stand-in over CModeSitBase / CSubObject models).
// Callees: the action's virtual Clone (slot 11, +0x2C), CCountedPointer<CCreatureActionBase>::Reset 0x00668D90.
// CBrainOutput has no shared header: the minimal local model below has the Ego_r.pdb members up to PAction.
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
class CBrainOutput {                                                     // Ego_r.pdb 0x48 bytes
public:
    void SetAsPerformAction(const CCreatureActionBase& action);         // 0x00860130
    unsigned long Types;                                                 // +0x00
    long LookFlags;                                                      // +0x04
    unsigned char NavigationInput[0x18];                                 // +0x08 CNavigationInput
    CCountedPointer<CCreatureActionBase> PAction;                        // +0x20
};

void CBrainOutput::SetAsPerformAction(const CCreatureActionBase& action)
{
    Types |= 8;
    PAction.Reset(action.Clone());
}
