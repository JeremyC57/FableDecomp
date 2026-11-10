# chunk 900 header overlay (rebuild/include/engine/...)

Every change is behind a new opt-in; with none defined each modified header is byte-identical in effect to the repo copy
(the diffs are pure `#ifdef` additions on top of the current repo files, post-fba8d6164).

| File | Kind | Opt-in | What | Used by |
|---|---|---|---|---|
| engine/CVectorMap.h | modified | `FABLE_RETAIL_CVECTORMAP_ASSIGN_OUTOFLINE` | declares `CVectorMap& operator=(const CVectorMap&)` (declaration only) so the implicit assignment of a class holding a CVectorMap calls the out-of-line COMDAT (0x006313A0 for CVectorMap<ulong,CSubDefInfo>) instead of expanding it. Note: contradicts the existing `FABLE_RETAIL_CVECTORMAP_ASSIGN` comment ("no out-of-line ??4CVectorMap") -- Fable.exe does hold one. | src/00f2a000, src/00f29ff0 (RELOCATION_MATCH) |
| engine/CTCWeapon.h | modified | `FABLE_RETAIL_CTCWEAPON_DAMAGE` (nested; includes engine/EWeaponClass.h) | `float GetDamage() const` (0x005DAAA0), `static bool __fastcall IsMeleeWeaponClass(EWeaponClass)` (0x005DA510) | differ/005c8459 |
| engine/CTCInventoryItem.h | modified | `FABLE_RETAIL_CTCINVENTORYITEM_ISAUGMENTATION` (nested) | `static bool __fastcall IsAugmentation(long)` (0x005D9870) | differ/005c8459 |
| engine/CCutsceneDef.h | NEW | requires `FABLE_RETAIL_DEF_CLASS_BASE`; turns on `FABLE_PDCB_TYPED_MEMBERS` (as NUISystem_CUIMiscThingsDef.h) | CCutsceneDef : CParentDefClassBase, eight `std::vector<CCharString>` +0x3c..+0x90, sizeof 0x9c asserted; implicit dtor / op= | src/00f2a000, src/00f29ff0 |
| engine/CTCInventoryWeapons.h | NEW | none (plain new header) | CTCInventoryWeapons : NInventory::CTCInventoryBase, NHeroInformationScreens::CBase; 19 Ego_r members at retail offsets (asserted 0x134/0x154/0x170/0x17C/0x198, sizeof 0x1A0); ConstructScreen(PC) slots 77/87, ConstructAugEntry(PC), ConstructWeaponItemEntries(PC) declarations; local `class CDefIndex { long Val; }`; forward-declares CEnginePrimitiveHandle | differ/005c8459 (ready for 0x005C9A95) |

Header debt found:
- engine/CEnginePrimitiveHandle.h does not compile in the CTCInventoryWeapons TU (forward declaration used; only the 12-byte vector is laid out).
- CDefIndex has no shared header (Ego_r.pdb `CDefIndex`, 4 bytes `long Val`); it is defined locally in CTCInventoryWeapons.h -- move it to engine/CDefIndex.h if another class needs it.
- The CVectorMap ASSIGN comment (no out-of-line ??4CVectorMap) is wrong for this TU family; see above.
