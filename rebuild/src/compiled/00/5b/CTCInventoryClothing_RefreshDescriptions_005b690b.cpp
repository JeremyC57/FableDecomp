// CTCInventoryClothing::RefreshDescriptions() -- re-texts the clothing menu: every "CLOTHING TITLE" text under ClothingList gets the title string, every "Clothing_Description" text the description of the item it shows (positive item indices only)
// Leaf: ?RefreshDescriptions@CTCInventoryClothing@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B690B, 290 bytes, thiscall `leave ; ret`, frame 0x14. Ego_r.pdb / FableWin.pdb
//   ?RefreshDescriptions@CTCInventoryClothing@@QAEXXZ (Ego_r donor 0x005609A0).
// Shape: one list collects both passes (FindAllChildren appends; it is not cleared in between); the description is the
//   CTCInventoryBase slot-78 virtual GetItemDescription (Clothing override 0x005B5CF8); the text is CText::SetValue(const CWideString&,
//   bool, bool), slot 144. The description pass keys the map with a named CComponent* local and STLport's operator[] body is
//   visible (FABLE_RETAIL_STLP_MAP; still called, 0x0042D1B7): VC7.1 then keeps the component in ebx and stores the key copy right
//   before the call (0x005B69D3); a CText* key converted at the call swaps edi/ebx, a `*it` key passes the node's address.
// Header: engine/NInventory_CTCInventoryBase.h FABLE_RETAIL_CTCINVENTORYBASE_VTABLE (slot 78), engine/CTCInventoryClothing.h
//   FABLE_RETAIL_CTCINVENTORYCLOTHING_SCREEN (ClothingItemMap / ClothingList / ConstructTitleString).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDOWNED
#define FABLE_RETAIL_CTCINVENTORYCLOTHING_CALCULATE
#define FABLE_RETAIL_CTCINVENTORYCLOTHING_SCREEN
#define FABLE_RETAIL_CTCINVENTORYCLOTHING_REFRESH
#include "engine/CTCInventoryClothing.h"
#include "engine/NUISystem_CText.h"

void CTCInventoryClothing::RefreshDescriptions()
{
    if (ClothingList) {
        std::list<NUISystem::CComponent*> components;
        std::list<NUISystem::CComponent*>::iterator it;
        std::list<NUISystem::CComponent*>::iterator end;
        ClothingList->FindAllChildren(CCharString("CLOTHING TITLE"), components);
        for (it = components.begin(), end = components.end(); it != end; ++it) {
            NUISystem::CText* text = static_cast<NUISystem::CText*>(*it);
            CWideString title;
            ConstructTitleString(title);
            text->SetValue(title, true, true);
        }
        ClothingList->FindAllChildren(CCharString("Clothing_Description"), components);
        for (it = components.begin(), end = components.end(); it != end; ++it) {
            NUISystem::CComponent* component = *it;
            long itemDefIndex = ClothingItemMap[component];
            if (itemDefIndex > 0) {
                CWideString description;
                GetItemDescription(description, itemDefIndex, false);
                static_cast<NUISystem::CText*>(component)->SetValue(description, true, true);
            }
        }
    }
}