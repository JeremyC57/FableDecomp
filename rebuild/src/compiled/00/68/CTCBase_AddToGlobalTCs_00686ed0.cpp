// CTCBase::AddToGlobalTCs() -- register this thing component in the global-update TC list once (push_back(this), then set IsInGlobalUpdateTCs)
// Leaf: ?AddToGlobalTCs@CTCBase@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00686ED0, 56 bytes, thiscall, plain `ret`.
// Identity: the manifest's "std::list::push_back" is propagated; Ego_r.pdb names ?AddToGlobalTCs@CTCBase@@QAEXXZ next to
//   RemoveFromGlobalTCs 0x00686DF0 and the landed UpdateGlobalTCs 0x00686DC0, which walk the same one-word STLport list
//   at 0x013BA8AC. Twenty TC constructors call it (CTCBoastUI, CTCHeroOnlineScoreboard, CTCQuickAccessMenu, ...).
// Callees: malloc 0x00BFEA0E (the STLport list node allocator).
// Shape: `if (!IsInGlobalUpdateTCs) { GlobalUpdateTCs.push_back(this); IsInGlobalUpdateTCs = true; }` with STLport's
//   `push_back(x) { insert(end(), x); }` inline: end() (the sentinel) is read from the global BEFORE the node is
//   created, the placement new null-tests &node->_M_data, then the four link stores.
// Layout: Ego_r.pdb CTCBase (12 bytes): vptr, ParentThing +4, ToBeRemoved +8, IsInGlobalUpdateTCs +9, from the shared
//   engine/CTCBase.h (FABLE_RETAIL_CTCBASE + _GLOBALUPDATETCS give the protected static list, as in the landed UpdateGlobalTCs
//   0x00686DC0; _ADDTOGLOBALTCS declares the member) over the shared STLport list of stlport/fable_containers.h.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_GLOBALUPDATETCS
#define FABLE_RETAIL_CTCBASE_ADDTOGLOBALTCS
#include "stlport/fable_containers.h"
#include "engine/CTCBase.h"

void CTCBase::AddToGlobalTCs()
{
    if (!IsInGlobalUpdateTCs) {
        GlobalUpdateTCs.push_back(this);
        IsInGlobalUpdateTCs = true;
    }
}
