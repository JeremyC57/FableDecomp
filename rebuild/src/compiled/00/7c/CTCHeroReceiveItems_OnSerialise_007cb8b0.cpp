// CTCHeroReceiveItems::OnSerialise(CPersistContext& context, bool in_game) -- save or restore the hero's picked-up item list by definition name: saving turns every picked-up def index (> 0) into its def name and transfers the names as "PickedUpItems"; loading reads the names back and appends each one's def index
// Leaf: ?OnSerialise@CTCHeroReceiveItems@@MAEXAAVCPersistContext@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007CB8B0, 508 bytes (+4 int3), thiscall `ret 8`, no EH frame. The catalog row (608 bytes)
//   runs on over two more functions, 0x007CBAB0 and 0x007CBAE0 (SPLITS.tsv; coverage.tsv size 508).
// Identity: vtable 0x0126EBFC (RTTI CTCHeroReceiveItems) slot 1 = 0x007CB8B0, CTCBase's two-argument OnSerialise slot; Ego_r.pdb
//   ?OnSerialise@CTCHeroReceiveItems@@MAEXAAVCPersistContext@@_N@Z (protected virtual, `ret 8`); the literal 0x0126ECEC is
//   "PickedUpItems", the member it transfers. The catalog's "Save_TransferPickedUpItems" is a placeholder.
// Callees: vector<CCharString>::reserve 0x004BA7A0, CTCBase::GetDefinitionManager 0x006869D0,
//   CDefinitionManager::GetDefNameFromGlobalIndex 0x009ACCC0, CDefStringTable::GetString 0x009D49B0 on GDefStringTable 0x013CA828
//   (the inline CDefString -> CCharString conversion), CCharString copy 0x0099EC30 / ~CCharString 0x0099EAE0,
//   vector<CCharString>::_M_insert_overflow 0x00433530, CPersistContext::TransferVector<CCharString> 0x0049B8D0, free 0x00BFEA14,
//   vector<long>::reserve 0x00660684, CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410, vector<long>::_M_insert_overflow
//   0x00440990.
// Shape: no base-class call; IsSaving() is the inline Mode test (1 or 3); each branch owns its own name vector (one frame slot).
// Headers: FABLE_HDR_OVERLAY=$A/hdr (CTCHeroReceiveItems.h opt-in FABLE_RETAIL_CTCHERORECEIVEITEMS_PICKEDUPITEMS).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CTCHERORECEIVEITEMS_PICKEDUPITEMS
#include "fable_string.h"
#include "engine/CTCHeroReceiveItems.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CDefString.h"
#include "engine/CPersistContext.h"

void CTCHeroReceiveItems::OnSerialise(CPersistContext& context, bool in_game)
{
    if (context.IsSaving()) {
        std::vector<CCharString> picked_up_item_names;
        picked_up_item_names.reserve(PickedUpItems.size());
        for (unsigned long i = 0; i < PickedUpItems.size(); ++i) {
            if (PickedUpItems[i] > 0)
                picked_up_item_names.push_back(GetDefinitionManager().GetDefNameFromGlobalIndex(PickedUpItems[i]));
        }
        context.TransferVector("PickedUpItems", picked_up_item_names);
    } else {
        std::vector<CCharString> picked_up_item_names;
        context.TransferVector("PickedUpItems", picked_up_item_names);
        PickedUpItems.reserve(picked_up_item_names.size());
        for (unsigned long i = 0; i < picked_up_item_names.size(); ++i)
            PickedUpItems.push_back(GetDefinitionManager().GetDefGlobalIndexFromName(picked_up_item_names[i]));
    }
}