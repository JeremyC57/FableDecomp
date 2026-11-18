# egod4a NOTES

## 0x00BE2CD0 CEngineLandscapeMeshBuilder::BuildSharedIndexBufferGenerationTables (/GS)
- Source src/00be2cd0.cpp unchanged from egod3a: q.py RELOCATION_MATCH (/GS). Header opt-in FABLE_RETAIL_CENGINELANDSCAPEMESHBUILDER_GENERATIONTABLES
  is already in rebuild/include (no overlay needed).
- Test test/00be2cd0.cpp: links the row object; stubs only vector<unsigned short>::_M_fill_insert (malloc, since the row's deallocate frees).
  Cases: overflow guard (negative, >0x7fffffff, 64-bit), zero-area grid, hand-worked 1x1, 10 grid sizes x {caller strip + list,
  caller strip no list, temp strip + list} vs an independent model with per-arm counters.
- Unreachable / equivalent: the six-index flip (count > 3) and the `count < 3` start (parity argument, model-checked 1..16 x 1..16);
  the two continuation compares are always equal (exhaustively checked), so either alone is an equivalent mutant.
- Mutants checked: `vert < 0` -> `vert <= 0` FAILS the test (good); dropping the poly_verts[1] compare passes (equivalent, see above).

## 0x0059FEE3 CTCInventoryTrade::AddBuyItemToList
- Ported off the local `class CThingPlayerCreature : public CThing { };` onto `#include "engine/CThingPlayerCreature.h"`.
  The two opt-ins the reviewer named (FABLE_RETAIL_CTHINGPLAYERCREATURE + FABLE_RETAIL_CTHINGCREATUREBASE) alone do NOT reach
  PeekTC (C2039: the CThingCreatureBase -> CThingGameObject -> CThingPhysical chain falls back to the opaque default classes).
  Adding the existing FABLE_RETAIL_CTHINGGAMEOBJECT + FABLE_RETAIL_CTHINGPHYSICAL completes the chain to CThing (same set as the landed
  sibling 0x005A04F7 ConstructNormalShopList). q.py: RELOCATION_MATCH unchanged. No header edits / overlay.
- Test test/0059fee3.cpp links the row object; fake vtables for the entry (CComponent slots 0/67/69/92, CMenuEntry 151/152/153) and
  the list (152/153); stubs carry the exact decorated names (note CreateMenuEntryComponent's `const CCharString` by-value text param:
  the retail mangling spells the last param as V5@, not a back-reference). 9 cases: every condition both ways (affordable, room,
  special case, quantity 0 / -1, already have special, inventory component missing -> space check on null, static / dynamic add);
  checks template, MakeUnavailable, return, ItemsToBuy record, the 7 actions (type / SLONG + string params / AddParam targets),
  AddUIActionParam, list slot, call order, final release, string temporaries balance.
- Mutants checked (all FAIL the test): quantity >= 0, IsSpecialCase dropped, !already_have_special dropped, dynamic inverted,
  IsItemAffordable dropped.

## Read-only landing check (vland, LANDVERIFY_STRICT_HEADERS=1)
- 00be2cd0 RELOCATION_MATCH[/GS] PASS; 0059fee3 RELOCATION_MATCH PASS. No RELOC_IDENTITY / header warnings. lv/ deleted.

## 0x00BE2CD0 mutant evidence (added after vland; src/test unchanged since vland)
- Killed: drop `overflow_check < 0` (FAIL negative size), drop `> 0x7fffffff` (crash), drop `if (tri_list_indicies)` (crash),
  `poly_within_quad == 1` -> `== 0` (FAIL 1x1 tri list), drop `tri_striped_indicies == 0` (crash), `vert < 0` -> `<= 0` (FAIL).
- No killing input exists (documented exceptions): the six-index flip arm (`count > 3` true side) and the `count < 3` arm
  (parity: after the first polygon count&1 != poly_within_quad always); the `count > 2 &&` conjunct (always true when reached:
  count >= 4 after the first polygon); either half of the continuation compare (the two compares are equal on every reachable
  polygon, exhaustively checked over 1..16 x 1..16; the drop-poly_verts[1] mutant survives).
