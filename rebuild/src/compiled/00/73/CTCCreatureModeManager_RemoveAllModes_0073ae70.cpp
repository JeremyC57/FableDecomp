// CTCCreatureModeManager::RemoveAllModes() -- tell the front mode (when any) it is leaving (virtual slot 6), clear the mode list, then select mode 0
// Leaf: ?RemoveAllModes@CTCCreatureModeManager@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073AE70, 43 bytes, plain `ret`.
// Identity (NF120, PRIORITY q3944): Ego_r.pdb 0x00751960 / FableWin.pdb 0x024C2F70 ?RemoveAllModes@CTCCreatureModeManager@@QAEXXZ; Ego_r orders
//   AddMode(K) 0x00751900, RemoveAllModes 0x00751960, ~ 0x007519D0, ctor 0x00751A30 as retail has AddMode(K) 0x0073AE10, this row, the dtor
//   0x0073AEA0 and the ctor 0x0073AF00; the landed ResetModes 0x0073B0D0 expands this body inline and its emitted RemoveAllModes COMDAT grades
//   RELOCATION_MATCH against this address. Was ClearModes over local CRetailList_CountedPointer / SModeNode stand-ins.
// Callees: the front mode's virtual slot 6 CModeBase::OnDeactivate (vtable 0x012673CC), STLport
//   std::_List_base<CCountedPointer<CTCCreatureModeManager::CModeBase>, allocator<..> >::clear 0x0073E6A0 (out of line, _list.c), and
//   ?AddMode@CTCCreatureModeManager@@QAE_NK@Z 0x0073AE10.
// Header: engine/CTCCreatureModeManager.h (PModes, the STLport list, +0x0C) with the NF120 opt-in FABLE_RETAIL_CTCCREATUREMODEMANAGER_REMOVEALLMODES.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER_REMOVEALLMODES
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCCreatureModeManager.h"

void CTCCreatureModeManager::RemoveAllModes()
{
    if (!PModes.empty())
        PModes.front()->OnDeactivate();
    PModes.clear();
    AddMode(0);
}
