// NUISystem::CMenuEntry::CMenuEntry(long) -- CClickable base, CMenuEntry's three vtables, available, no action lists yet, then GetDataFromDef()
// Leaf: ??0CMenuEntry@NUISystem@@QAE@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0053DFE0, 103 bytes.
// Shape: CClickable(long) (0x0055B460); CMenuEntry's vtables for the three polymorphic subobjects
// (+0 0x0124662C, +4 0x01246604, +0x18 0x012465FC, rtti NUISystem::CMenuEntry); IsAvailable = true and
// the eight action-list pointers zeroed from one register (initialisers); GetDataFromDef() (0x0053DA30) runs in the body.
// Identity of the callee (two sources): Ego_r.pdb / FableWin.pdb ?GetDataFromDef@CMenuEntry@NUISystem@@QAEXXZ (declared in
//   engine/NUISystem_CMenuEntry.h); 0x0053DA30 reads the entry's def into its members (row 0x0053DA30, respelled with this one).
// Was: an all-local model (stand-in bases CClickableBase0/1/2 with a base list) that bound the vtables as
//   ??_7CMenuEntry@NUISystem@@6BCClickableBase0/1/2@1@@ and the callee as ?InitializeRumbleActions@CMenuEntry@NUISystem@@QAEXXZ.
// Now: on engine/NUISystem_CMenuEntry.h (+ NUISystem_CClickable.h): the compiler emits the vtables from the shared hierarchy as
//   ??_7CMenuEntry@NUISystem@@6BCUIInterface@1@@ / 6BCObserver@1@@ / 6BCActionParam@1@@, as the destructor row 0x0053E4E0 binds them.
// Header: engine/NUISystem_CMenuEntry.h (no opt-ins). Parameter name `definition` from FableWin.pdb (S_BPREL32 of ??0CMenuEntry@NUISystem@@QAE@J@Z).
#include "engine/NUISystem_CMenuEntry.h"
namespace NUISystem {
CMenuEntry::CMenuEntry(long definition)
    : CClickable(definition)
    , IsAvailable(true)
    , PUIActionParams(0)
    , PThingActionParams(0)
    , PActions(0)
    , PActionsOnSelected(0)
    , PActionsOnBack(0)
    , PPreActions(0)
    , PActionsOnUnselected(0)
    , PActionsOnVisible(0)
{
    GetDataFromDef();
}
}
