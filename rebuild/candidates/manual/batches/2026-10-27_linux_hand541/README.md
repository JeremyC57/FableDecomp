# Retry chunk 541 (agent541, 2026-10-28): 1 of 8 parked rows lands (RELOCATION_MATCH), 7 re-parked

0x00663C40 is `CThingCreatureBase::DrawPreparePrimitivesForRendering` (PDB protected virtual, vtable 0x0125AC0C slot 36;
the manifest's CThingPhysical attribution is wrong), 577v577.

Re-parked (pending/2026-10-27_linux_hand541/differ): 0x00622A00 (880v877, frame slots), 0x00627400 (750v742, dead
`_List_base` null store), 0x0063AB90 (x87 scheduling), 0x0063D0A0 / 0x0063D4A0 (1017v1017, scheduling; 0x0063D4A0 is
FindThemeCollisionPointWithinCell), 0x0063E6F0 CWorldMap::IsLineOfSight (now real STLport list, 607v599), 0x0065CB6B
(frame 0x1c v 0x18). Lane gap: retail drops STLport `_STLP_alloc_proxy`'s dead null store.

NAME_FIXES.tsv: 44 rows, deferred to the naming pass (0x0041D122 `?OnOpenMenus@CManager@NUISystem@@SIXXZ`; 0x00487DC0
GetPControlledCreature; 0x00623560 STLport `_M_erase` of `map<CThing*,CComponent*>`; PointIsWithinCell TR/BL/TL/BR permuted).
