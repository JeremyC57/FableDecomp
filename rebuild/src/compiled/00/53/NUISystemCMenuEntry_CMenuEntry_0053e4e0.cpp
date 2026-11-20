// NUISystem::CMenuEntry::~CMenuEntry() -- unregister from the UI manager, hand every UI action parameter and thing action parameter back to
//   the manager, delete the owned UI action parameters and both parameter lists, delete the six action lists, then the CClickable base
// Leaf: ??1CMenuEntry@NUISystem@@UAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0053E4E0, 606 bytes; ends in a tail jump to ~CClickable 0x0055B760. Callees: CManager::GetInstance
//   0x0041E5F2 and its virtual slots 5 RemoveObserver (+0x14), 25 RemoveThingActionParam (+0x64) and 28 RemoveUIActionParam (+0x70);
//   free 0x00BFEA14, operator delete 0x00BFE9BC, list<CCountedPointer<CAction> >::clear 0x005356A0, ~CClickable 0x0055B760.
// Identity (two sources): Ego_r.pdb / FableWin.pdb ??1CMenuEntry@NUISystem@@UAE@XZ (virtual; its only caller is the ??_G 0x0053E8E0, slot 0
//   of CMenuEntry's vtable 0x0124662C); the body stores the three CMenuEntry vptrs 0x0124662C / 0x01246604 / 0x012465FC and walks the
//   engine/NUISystem_CMenuEntry.h members (PUIActionParams +0x19C ... PActionsOnVisible +0x1B8).
// Was: the same body on local CPodList / CRetailList / CActionParam_ / CManager / CClickable (with local bases) models, whose non-virtual
//   declarations gave ??1CMenuEntry@NUISystem@@QAE@XZ and bound ~CClickable as ??1CClickable@NUISystem@@QAE@XZ
//   (rebuild/src/compiled/00/53/NUISystemCMenuEntry_CMenuEntry_0053e4e0.cpp).
// Now: on the shared headers (CManager's PDB slots RemoveObserver / RemoveThingActionParam / RemoveUIActionParam; CActionParam's
//   ActionParamType, UI_ACTION_PARAM_TYPE_COMPONENT = 1 is the not-owned kind; STLport list<T>* members, their destructors inline).
// Header: engine/NUISystem_CMenuEntry.h, engine/NUISystem_CManager.h, stlport/fable_containers.h (FABLE_RETAIL_STLP_LIST).
#define FABLE_RETAIL_STLP_LIST
#include "engine/NUISystem_CMenuEntry.h"
#include "engine/NUISystem_CManager.h"
namespace NUISystem {
CMenuEntry::~CMenuEntry()
{
    CManager::GetInstance()->RemoveObserver(this);
    if (PUIActionParams) {
        for (std::list<CActionParam*>::iterator it = PUIActionParams->begin(); it != PUIActionParams->end(); ++it)
            CManager::GetInstance()->RemoveUIActionParam(*it);
    }
    if (PThingActionParams) {
        for (std::list<long>::iterator it = PThingActionParams->begin(); it != PThingActionParams->end(); ++it)
            CManager::GetInstance()->RemoveThingActionParam(*it);
    }
    if (PUIActionParams) {
        for (std::list<CActionParam*>::iterator it = PUIActionParams->begin(); it != PUIActionParams->end(); ++it) {
            if ((*it)->ActionParamType != UI_ACTION_PARAM_TYPE_COMPONENT)
                delete *it;
        }
        delete PUIActionParams;
    }
    if (PThingActionParams) {
        delete PThingActionParams;
        PThingActionParams = 0;
    }
    delete PActions;
    delete PActionsOnBack;
    delete PPreActions;
    delete PActionsOnSelected;
    delete PActionsOnUnselected;
    delete PActionsOnVisible;
}
}
