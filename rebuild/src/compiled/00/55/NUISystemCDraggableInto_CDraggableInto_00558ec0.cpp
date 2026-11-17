// NUISystem::CDraggableInto::CDraggableInto(long id) -- construct the drop target: hoverable base, no action list yet, then GetDataFromDef()
// Leaf: ??0CDraggableInto@NUISystem@@QAE@J@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00558EC0, 56 bytes (`ret 4`): CHoverable(long) 0x0055BA20, the three vtables 0x0124B2D4 / 0x0124B2B0 /
//   0x0124B2A8 (.?AVCDraggableInto@NUISystem@@ at +0 / +4 / +0x18), PActions (+0x16C) = 0, then GetDataFromDef() 0x00558E10.
// Identity: Ego_r.pdb + FableWin.pdb ??0CDraggableInto@NUISystem@@QAE@J@Z (engine/NUISystem_CDraggableInto.h declares it for 0x00558EC0);
//   its callee 0x00558E10 is ?GetDataFromDef@CDraggableInto@NUISystem@@QAEXXZ (Ego_r.pdb + FableWin.pdb, donor order; NF79 re-authored that row).
// NF79 (round 7, q10964 follow-up): ported onto the shared header (+ my overlay opt-in NUISYSTEM_CDRAGGABLEINTO_GETDATAFROMDEF); was a local
//   CHoverable / interface / CDraggableInto model calling 0x00558E10 as the made-up Initialize, with made-up vtable base names.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_NUISYSTEM_CDRAGGABLEINTO_GETDATAFROMDEF
#include "engine/NUISystem_CDraggableInto.h"

namespace NUISystem {
CDraggableInto::CDraggableInto(long id)
    : CHoverable(id), PActions(0)
{
    GetDataFromDef();
}
}
