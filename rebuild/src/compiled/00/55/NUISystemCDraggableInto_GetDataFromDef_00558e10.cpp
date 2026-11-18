// NUISystem::CDraggableInto::GetDataFromDef() -- read the component's UI def and, when it names an action, install it with AddAction
// Leaf: ?GetDataFromDef@CDraggableInto@NUISystem@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00558E10, 174 bytes, plain `ret`.
// Identity (two sources): Ego_r.pdb + FableWin.pdb ?GetDataFromDef@CDraggableInto@NUISystem@@QAEXXZ, in Ego_r donor order Initialise /
//   ProcessEvent / ExecuteActions / GetDataFromDef / ctor = retail 0x00558D80 / 0x00558D90 / 0x00558DE0 / 0x00558E10 / 0x00558EC0; the
//   body is the GetDataFromDef shape of the landed CTextSlider / CSlider rows (0x00549B20 / 0x0054B4B0: LoadAndReturnDef, then an action
//   per def action type). The catalogue's "Initialize" was a forge autoname (no such member in either PDB).
// Callees: LoadAndReturnDef (slot 108, +0x1B0), AddAction (slot 66, +0x108); operator new 0x00BFEA1A, CAction::CAction 0x0042BE50,
//   CCountedPointer<NUISystem::CAction>(CAction*) 0x0042AA29, operator delete 0x00BFE9BC (the counted pointer's release inlined).
// Headers: engine/NUISystem_CDraggableInto.h (+ my overlay opt-in NUISYSTEM_CDRAGGABLEINTO_GETDATAFROMDEF), engine/NUISystem_CUIDef.h
//   (Action +0xC4), engine/NUISystem_CAction.h, engine/CCountedPointer.h, engine/CDefPointer.h.
// NF79 (round 7, q10964): re-authored on the shared headers; was a local CDraggableInto / CAction / CDefPointer model inside namespace
//   NUISystem that included engine/CCountedPointer.h there, so 0x0042AA29 bound ??0?$CCountedPointer@VCAction@NUISystem@@@NUISystem@@..
//   (a NUISystem-scoped template); now the global template's ??0?$CCountedPointer@VCAction@NUISystem@@@@QAE@PAVCAction@NUISystem@@@Z.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_NUISYSTEM_CDRAGGABLEINTO_GETDATAFROMDEF
#define FABLE_RETAIL_CCLICKABLE_SHARED_ACTION
#include "engine/CDefPointer.h"
#include "engine/NUISystem_CUIDef.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_CDraggableInto.h"

namespace NUISystem {
void CDraggableInto::GetDataFromDef()
{
    CDefPointer<const CUIDef> pdefinition = LoadAndReturnDef();
    if (pdefinition->Action) {
        CCountedPointer<CAction> paction(new CAction);
        paction->ActionType = pdefinition->Action;
        AddAction(paction);
    }
}
}
