# NF58 header overlay (opt-ins only; the default path of every header is token-identical to the repo copy)

Each file copies the CURRENT repo header (re-diffed before hand-back: `diff repo overlay | grep '^<'` is empty for all four) and adds
`#ifdef` blocks only.

| Overlay file | Copies | Added opt-in | Used by |
|---|---|---|---|
| `stlport/fable_map.h` | `rebuild/include/stlport/fable_map.h` | `FABLE_RETAIL_STLP_TREE_INC_OUTOFLINE`: `_Rb_tree_iterator::operator++` with the same STLport body under `__declspec(noinline)` (MSVC only), the twin of the existing `FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE`. Retail calls each instance's operator++ as a thiscall COMDAT (0x008010D0 / 0x00801200, 62 bytes, `_M_increment` inlined). | 0x00801EC0, 0x00801CB0 (the /O2 hinted insert_unique of map<EIdleStateGroup, long> / map<EIdleStateGroup, CAIStateGroupBase*>). |
| `stlport/fable_map.h` | (same file) | `FABLE_RETAIL_STLP_TREE_CREATE_NODE_NOINLINE`: `_Rb_tree::_M_create_node` with the same body under `__declspec(noinline)`. Retail calls that instance's `_M_create_node` (0x00783DD0) from both arms of an /O2 `_M_insert`, where the /O2 siblings expand it. | 0x00783F00 (map<unsigned __int64, CIntelligentPointer<CThing> >). |
| `engine/CWeaponDef.h` | `rebuild/include/engine/CWeaponDef.h` | `FABLE_RETAIL_CWEAPONTRAILGRAPHICSET_CLASS`: spells `CWeaponTrailGraphicSet` as `class` with public members (same layout). Both PDB name sets have `VCWeaponTrailGraphicSet@@` only; the default `struct` mangles `U` and splits the binding with the landed caller 0x004E9156. The coordinator may prefer to make `class` the default after checking the other includers (0x00839100 binds through the opt-in FABLE_RETAIL_CWEAPONDEF_WEAPONTRAILS; NF58 regraded it unchanged, see below). | 0x004E9299, 0x004E9213. |
| `engine/CTCInventoryExperience.h` | `rebuild/include/engine/CTCInventoryExperience.h` | `FABLE_RETAIL_SEXPSPENDUNDO_ONLY` (only when `FABLE_RETAIL_CTCINVENTORYEXPERIENCE` is not defined): a block before the header's own `#ifdef FABLE_RETAIL_CTCINVENTORYEXPERIENCE` that defines only the global `SExpSpendUndo` record (the same text as the full model, behind its own guard `FABLE_ENGINE_SEXPSPENDUNDO_ONLY_H`, with `stlport/fable_containers.h` and a forward `class CTCHeroExperience;`). The full model needs the CTCInventoryBase chain (`NInventory_CTCInventoryBase.h` static asserts fail without it: 2 failed grades). | 0x00579FD8, 0x0057A124, 0x0057A060, 0x0057A1AA (map<EHeroTrainableStatType / EHeroAbility, SExpSpendUndo>). |
| `engine/CDefString.h` | `rebuild/include/engine/CDefString.h` | `FABLE_RETAIL_CDEFSTRING_COMPARE_FRIEND`: `template<class T> friend class CCompareDefString;` before the private `TablePos`. Both PDBs name the comparator `??R?$CCompareDefString@J@@QBE_NABVCDefString@@0@Z`; retail inlines it as a signed compare of the two TablePos words. CCompareDefString has no shared header; the two rows define it (empty class + operator()). | 0x004E5BA7, 0x004E5B21 (map<CDefString, long, CCompareDefString<long> >). |

Unrelated-includer check (`work/recheck_unrelated.log`, through this overlay, none of them defines an NF58 macro): 0x0042CB08 and
0x004ABC70 (fable_map.h, ORIGINAL_INSERT_BODY rows), 0x00839100 (CWeaponDef.h), 0x00576C0B (CTCInventoryExperience.h), 0x0045CFAD
(CDefString.h): all five RELOCATION_MATCH, PASS. The 123 rows of `recheck_final.log` all compile through the overlay too.

Overlay-path caution (NF41): `stlport/fable_map.h` is reached directly (`#include "stlport/fable_map.h"` from the row) by every row that
uses an NF58 fable_map.h opt-in, and before any root header, so the overlay copy wins.
