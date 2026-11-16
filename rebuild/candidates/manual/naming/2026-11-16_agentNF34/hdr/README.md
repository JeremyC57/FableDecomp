# NF34 header overlay (FABLE_HDR_OVERLAY=$S/w22/agentNF34/hdr)

Each file is a copy of the CURRENT repo header (2026-11-16) with `#ifdef` opt-ins only; the default path is unchanged.

| overlay file | copies | opt-in added | used by |
|---|---|---|---|
| engine/CTCInventoryItem.h | rebuild/include/engine/CTCInventoryItem.h | `FABLE_RETAIL_CTCINVENTORYITEM_ISSELLABLE` (inside the `_KINDS` block): `static bool __fastcall IsSellable(long)` = Ego_r 0x0055AB00 / FableWin 0x01E03B40 `?IsSellable@CTCInventoryItem@@SI_NJ@Z`, retail 0x005D9B30 (reads CInventoryItemDef::IsSellable +0x5C). Declaration only. | 0x005AF64E, 0x005B0AAC |
| engine/CAIStateGroupBase.h | rebuild/include/engine/CAIStateGroupBase.h | `FABLE_RETAIL_CAISTATEGROUPBASE_PEEKDEFINITIONMANAGER_PROTECTED`: a `protected:` label before `PeekDefinitionManager()` (the `_PEEKDEFINITIONMANAGER` block otherwise inherits the preceding label, public on the default path; Ego_r + FableWin spell it IBE). Access label only. | 0x00958840, 0x0097FEA0 |
| engine/CMeshDataBank.h | rebuild/include/engine/CMeshDataBank.h | `FABLE_RETAIL_CMESHDATABANK_SETTEXTUREBANK` (skipped when `_BASES` + `_CTOR` already declare it): `void SetTextureBank(const CGraphicDataBank*)` on the default layout, `?SetTextureBank@CMeshDataBank@@QAEXPBVCGraphicDataBank@@@Z`, retail 0x00A23B50. Declaration only. | 0x00A23B50 |

Coordinator notes:
- engine/CTCInventoryItem.h line ~96 comments `?IsBuyable@..@@SI_NJ@Z 0x005D9B30`; IsBuyable is 0x005D9B50 (+0x5D) and 0x005D9B30 is IsSellable. The declaration itself is right; only the address comment is wrong (left untouched: opt-ins only).
- engine/CAIStateGroupBase.h `_PEEK` block (PeekWorld / PeekThingSearchTools, commented "protected") has the same access-label problem; not in this queue.
