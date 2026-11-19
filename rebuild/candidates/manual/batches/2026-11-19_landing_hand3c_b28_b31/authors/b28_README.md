# b28 (hand3c chunk 1, Account B, 2026-11-19): 0x0063F4B0 CThingSearchTools::ForEachInRoughArea<CThingPhysical, CFilter_Bind2<CThingFilter_HasNavigationInformation, CThingFilter_OverlappingBoundingBox>, CThingFindNavigationLayerAt> -- READY

- Grade: RELOCATION_MATCH 769v769. Strict vland `RELOCATION_MATCH PASS` (LANDVERIFY_STRICT_HEADERS=1); one non-blocking
  `WARN STANDIN_LEAF` caused by the truncated catalog label (NAME_FIXES.tsv). 31 grades in ATTEMPTS (30 on the old
  FOREACHINROUGHAREA_BODY port, residue = post-node-loop reload order at 0x0063F6F4 / 0x0063F755; the port onto the egod body matched
  at its first grade). Flags: none. No overlay headers (repo headers only).

## What closed it
hand4d landed the Bind3 twin 0x0063F7C0 today (commit 7f2b3f082) on hand4c's shared egod body
(FABLE_RETAIL_CTHINGSEARCHTOOLS_FOREACHINROUGHAREA_EGOD + FOREACHINROUGHAREA + _CONSTPROC + ROUGHAREA_EGOD: Ego_d locals, FableWin
statement order, edge-compare empty test), which removed the same reload-order residue there. This row is that text with CFilter_Bind2
and without CThingFilter_IsNot: the same functor classes (Ego_r.pdb, Ego_d.pdb local names), the same opt-ins, explicit instantiation.

## Identity
Leaf = Ego_r.pdb / FableWin.pdb ??$ForEachInRoughArea@...CFilter_Bind2...@CThingSearchTools@@QBE_NABVC2DBoxI@@ABV...@ABVCThingFindNavigationLayerAt@@@Z.
Retail 0x0063F4B0..0x0063F7B0 (`ret 0Ch` at 0x0063F7AE, 769 bytes). Call targets in retail = the source header's Callees list
(0x0049C6F0, 0x0049C700, slots +0xA8 / +0xDC / +0x174, 0x006B0850, 0x00A12890, 0x00A127D0, 0x00A11D10, 0x00BFEAE6).

## Test (test/0063f4b0.cpp)
Links the real row object: the test TU sees the row's defines / includes / functors WITHOUT the egod body opt-in, so the template is only
declared and the call binds to the row object's instantiation. Harness adapted from hand4d's Bind3 test (map / layer / cell / node
images, fake thing and physics vtables, recording doubles). Checks: levels asked for exactly the passing things with navigation layers in
layer / row / cell / node order (1, 2, 4, 5, 18, 8, 13, 14), largest level kept, true/false result, bounding-box call count, map list
fetched once and cleared, each map clipped once, search statics (NoLookups, SearchID, NoThingsScanned), stamps; plus runs with all
levels <= 0, a level of 1, and no point inside. Prints `0063f4b0_TEST PASS`.
Mutation check (tools/mut.py; header mutants on a temporary overlay copy of the repo CThingSearchTools.h): 30 one-decision mutants
over the egod body, GetMapwhoArea and the functors: 27 KILLED, 1 CC_FAIL (a statement removal leaving an empty `if`; respelled in round 2
and KILLED), 2 SURVIVED, both equivalent: dropping `clipped_area.BRY==clipped_area.TLY` from the empty test (the cy loop then runs zero
times) and `GetLayer` (+1) (GetLayer is not emitted by the row object; the test's own inline copy is what runs).

## Header opt-ins
None new. Repo headers as they are today (the egod opt-ins already exist).

## NAME_FIXES
0x0063F4B0: catalog label truncated; correct symbol = the Leaf (NAME_FIXES.tsv).

## For the coordinator
- Same TU shape as the landed twin 0x0063F7C0; the old pending ports of this row (differ/ in older handoffs) are superseded.
