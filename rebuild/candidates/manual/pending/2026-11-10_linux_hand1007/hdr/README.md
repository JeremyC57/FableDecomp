# chunk 1007 header overlay (`FABLE_HDR_OVERLAY=$A/hdr`)

Every grade in this workspace ran with `FABLE_HDR_OVERLAY` pointing here. Each file is a copy of `rebuild/include/engine/<file>`
with new **nested opt-ins only**: the default path (no new macro defined) is byte-identical to the shared header. All are
declarations only, so there is no layout change.

| header | new opt-in | adds | used by |
|---|---|---|---|
| `engine/CTCInventory.h` | `FABLE_RETAIL_CTCINVENTORY_ITEMSELECTED` (needs `FABLE_RETAIL_CTCINVENTORYBASE_VTABLE`) | `virtual void OnItemSelected(long itemDefIndex, bool& handled)`, the slot-33 override, `?OnItemSelected@CTCInventory@@UAEXJAA_N@Z` 0x00583E26 | src/00583e26 |
| `engine/CTCInventory.h` | `FABLE_RETAIL_CTCINVENTORY_OPENPC` | `OpenPCInventory()` 0x0058F649, `OpenPCPhotojournal()` 0x0058F9BC, `bool HasScreenshots()` 0x00581B33 (Ego_r.pdb / FableWin.pdb) | differ/0058f649 |
| `engine/CTCInventory.h` | `FABLE_RETAIL_CTCINVENTORY_ITEMENTRY` (needs `_QUICKACCESS`) | `ConstructItemEntry` 0x0058A0C7 / `ConstructItemEntryPC` 0x0058E208 `(long, long, NUISystem::CList*, NUISystem::CComponent*, CCountedPointer<NUISystem::CComponent>&)` (PDB `QAEXJJPAVCList@NUISystem@@PAVCComponent@3@AAV?$CCountedPointer@...`), plus a `template <class T> class CCountedPointer;` forward declaration at namespace scope | src/0058a0c7, src/0058e208 |
| `engine/CTCInventory.h` | `FABLE_RETAIL_CTCINVENTORY_CONSOLEMENUS` (needs `_QUICKACCESS`) | `GetItemsListUIComponent(CComponent*)` 0x00590235, `ConstructItemCategoryList(...)` 0x0058F016 (FableWin / Ego_r spellings) | probe/00590235_shared_lastgraded.cpp only (the differs kept are local-model priors) |
| `engine/CTCInventoryItem.h` | `FABLE_RETAIL_CTCINVENTORYITEM_USEBUTTONACTION` (needs `FABLE_RETAIL_CTCINVENTORYITEM`) | `enum EInventoryActionType;` (namespace scope) and `static EInventoryActionType __fastcall GetUseButtonActionType(long defIndex)` 0x005D9710 (FableWin.pdb `W4EInventoryActionType`; body CacheDef + def+0x4C UseButtonAction) | src/00583e26, src/0058a0c7, src/0058e208 |
| `engine/CTCInventoryMap.h` | `FABLE_RETAIL_CTCINVENTORYMAP_PCMAPLIST` | `NUISystem::CComponent* ConstructPCMapList(bool, bool*)` 0x0060EEB0 (Ego_r.pdb / FableWin.pdb) | differ/00589a30 |
| `engine/NInventory_CTCInventoryBase.h` | `FABLE_RETAIL_CTCINVENTORYBASE_HIGHLIGHTEDITEM` | protected `long GetHighlightedItem() const` 0x005BCAE3 | src/00583e26 |
| `engine/NInventory_CTCInventoryBase.h` | `FABLE_RETAIL_CTCINVENTORYBASE_CAPTUREBACKBUFFER` | `void CaptureBackBufferToTexture()` 0x005BC6ED (Ego_r.pdb; tail-calls CUserProfileManager::CaptureBackBufferForUIBackground 0x00406CC0; landed rows spell it `PrepareScreen`, see NAME_FIXES.tsv) | differ/0058f649 |

To land: merge each block into the shared header under the same macro name. No existing opt-in was changed.
