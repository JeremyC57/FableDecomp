# Header overlay of chunk 1002b (agent1002b)

Grade every src/ and differ/ file with `FABLE_HDR_OVERLAY=$A/hdr` exported (gall.sh as shipped does not export it). Every change is
behind a new `FABLE_RETAIL_*` opt-in; the default paths of the changed repo headers are byte-identical to HEAD.

| File | Kind | Opt-in | What | Rows |
|---|---|---|---|---|
| engine/CAugmentationDef.h | changed | `FABLE_RETAIL_CAUGMENTATIONDEF` | retail class over CSubDefClassBase: inline StatGetTypeName ("CAugmentationDef"), Name +0x28, Type (EObjectAugmentationType) +0x2C, InventoryGraphic +0x30, the multipliers / regeneration members (Ego_r names) | 0x005C4F42, 0x005C574E, 0x005C5F11 |
| engine/EObjectAugmentationType.h | new | (none: enum) | Ego_r.pdb EObjectAugmentationType; the PDB has no enumerators, so the bit values carry descriptive names from the TEXT_OBJECT_AUGMENTATION_<X>_TITLE symbols the rows map them to | 0x005C4F42 |
| engine/NUISystem_CActionParamCCountedPointer.h | new | `FABLE_RETAIL_CACTIONPARAMCCOUNTEDPOINTER` (required, #error) | NUISystem::CActionParamCCountedPointer<T> (Ego_r ??0?$CActionParamCCountedPointer@VCComponent@NUISystem@@@NUISystem@@QAE@AAV?$CCountedPointer@...@Z, retail 0x004854DF, vtable 0x01236794), Value +8, sizeof 0x10; ctor declared only | 0x005C3E91, 0x005C47D5 |
| engine/NUISystem_CMenuEntry.h | changed | `FABLE_RETAIL_CMENUENTRY_ACTIONLISTS` (nested in `_ACTIONPARAMS`) | slots 152 AddActionOnUnselected (0x0053E2A0, appends to +0x1B4) / 153 AddActionOnVisible (0x0053E3C0, +0x1B8), Ego_r / FableWin spellings | 0x005C3E91, 0x005C47D5 |
| engine/CTCInventoryItem.h | changed | `FABLE_RETAIL_CTCINVENTORYITEM_STATICENGINEGRAPHIC` (needs `FABLE_RETAIL_CENGINEGRAPHIC_CTOR`) | static __fastcall ?GetEngineGraphic@CTCInventoryItem@@SI?AVCEngineGraphic@@J@Z (0x005D96A0, landed) | 0x005C3E91, 0x005C47D5 |
| engine/CEngineGraphic.h | changed | `FABLE_RETAIL_CENGINEGRAPHIC_CTOR` | Ego_r ??0CEngineGraphic@@QAE@XZ (zeroes the five members, as the landed 0x005D96A0 spells it): non-POD, so the by-value result's hidden pointer goes in ecx | 0x005C3E91, 0x005C47D5 |
| engine/CStockItemDef.h | changed | `FABLE_RETAIL_CSTOCKITEMDEF` | the CLOTHING_DEFS layout (DefaultPrice +0x28) plus FableWin ?StatGetTypeName@CStockItemDef@@SI?AVCCharString@@XZ | 0x005C6B26 |
| engine/CTCShop.h | changed | `FABLE_RETAIL_CTCSHOP_INTERFACETYPE` | the inline StatGetInterfaceType (TCI_SHOP, FableWin name set) without the _SHOPKEEPER body (which needs the complete CThingCreatureBase) | 0x005C6B26 |
