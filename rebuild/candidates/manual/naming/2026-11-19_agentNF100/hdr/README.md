# NF100 header overlay (round 9, 2026-11-19)

`#ifdef` opt-ins only. Without the new macros every header is token-identical to its repo copy (orig/ holds the base copies; checked
with diff: the only differences are the blocks listed here).

## engine/NInventory_CTCInventoryBase.h (base: rebuild/include/engine/NInventory_CTCInventoryBase.h, copy in orig/engine/)
- New opt-in `FABLE_RETAIL_NINVENTORY_CITEM_ISSAMEITEMTYPEASDEFINDEX` inside NInventory::CItem (after NF96's `_REMOVEITEM`): the declaration
  `bool IsSameItemTypeAsDefIndex(long defIndex) const; // 0x005BC450` (Ego_r.pdb 0x0061F2D6 + FableWin.pdb 0x0235F53C,
  ?IsSameItemTypeAsDefIndex@CItem@NInventory@@QBE_NJ@Z). Declaration only, no layout change.
- Used by rows/005c150b.cpp (which also carries the same-TU strong definition). Also by the blocked draft work/005bdec1_blocked.cpp.
- pin_asserts.py: `pinned 0` (no FABLE_STATIC_ASSERT moved).
- Landed includers: 85 rows include this header (grep -rlE 'engine/NInventory_CTCInventoryBase.h' rebuild/src/compiled). None defines the new
  macro (grep of rebuild/: 0 hits), so the opt-in changes none of them.
- No sibling agent (NF99P2, NF101) overlays this header.

## engine/NThingMap_CThingMap.h (base: rebuild/include/engine/NThingMap_CThingMap.h, copy in orig/engine/)
- New opt-in `FABLE_RETAIL_NTHINGMAP_LAYERINFO_ONLY` (q11463 / q11282). Line 1 gains a leading `#ifndef FABLE_RETAIL_NTHINGMAP_LAYERINFO_ONLY`
  (the existing comment stays on that line as a trailing comment, so no line moves), and the end of the file gains the matching `#endif`
  plus an `#ifdef FABLE_RETAIL_NTHINGMAP_LAYERINFO_ONLY` block that declares ONLY NThingMap::CLayerInfo (Ego_r.pdb record, 16 bytes) and
  `extern const CLayerInfo LayerInfo[]; // 0x0137DAD8` (Ego_d.pdb ?LayerInfo@NThingMap@@3QBVCLayerInfo@1@B), the same text as the
  existing FABLE_RETAIL_CTHINGMAP_LAYERINFO block, with engine/CRGBColour.h. It lets the 56 rough-area rows that model the rest of NThingMap
  locally (pre-existing) take the table from the shared header instead of a global stand-in struct.
- Default path: token-identical (gcc cpp -P of the repo header and the overlay header without the macro: cmp equal).
- pin_asserts.py: `pinned 0` (no line before the two FABLE_STATIC_ASSERTs moved).
- Landed includers: 144 rows include this header (list: work/includers_NThingMap_CThingMap.h.txt) (grep -rlE 'engine/NThingMap_CThingMap.h' rebuild/src/compiled); none defines the new macro
  (grep of rebuild/: 0 hits), so the opt-in changes none of them.
- Needs header claim (not applied): the default branch of this header still declares the stand-in `struct CLayerInfo` /
  `LayerInfo_0137dad8` (lines 181-182), and engine/CThingSearchTools.h body blocks still read `LayerInfo_0137dad8` (lines 348, 484, 525).
  No landed row binds the stand-in through those blocks (cpp -P of all 99 referencing rows), so this is dead text to retire.

## engine/C2DBoxI.h: dropped from the overlay
- The q11400 draft added a `FABLE_RETAIL_C2DBOXI_VALIDATEORIENTATIONANDFLIP` declaration. No finished row uses it, so the copy moved to
  work/dropped_overlay/ (C2DBoxI.h + C2DBoxI.orig.h), the blocked draft work/0063e950_blocked.cpp still names it.

## engine/CAIStateGroup_HoverHornetQueen.h (base: rebuild/include/engine/CAIStateGroup_HoverHornetQueen.h, copy in orig/engine/)
- New opt-in `FABLE_RETAIL_CAISTATEGROUP_HOVERHORNETQUEEN_ISROUTETOPOSBLOCKEDBYTARGET` (just before the DIVEBOMB block, in the protected
  section): `bool IsRouteToPosBlockedByTarget(long n); // 0x0095A490` (Ego_r.pdb 0x0097ED40 + FableWin.pdb 0x02C11620,
  ?IsRouteToPosBlockedByTarget@CAIStateGroup_HoverHornetQueen@@IAE_NJ@Z). Declaration only, no layout change. Used by rows/0095a6c0.cpp.
- pin_asserts.py: `pinned 0` (the class's assert is FABLE_STATIC_ASSERT_AT(41, ...), already pinned). Landed includers: 11; none defines the
  macro, so none changes.

## Checks (the three overlaid headers)
- orig/ is byte-equal to the repo header (cmp) for all three.
- Default path token-identical: gcc `cpp -P` of the orig and the overlay copy without any new macro: equal for all three.
- No sibling overlay (NF99P2, NF101) touches any of these three headers (their hdr/ directories at hand-back time).
- Every FABLE_RETAIL_* macro used in rows/ resolves in rebuild/include or hdr/ (46 macros checked).
- Landed includer lists (the brief's header-users check): work/includers_<header>.txt, one row source per line.
