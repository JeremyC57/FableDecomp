# NF103 notes (round 10, 2026-11-19): search tools and NThingMap

## Counts (from APPLIED.tsv / STATUS.tsv / recheck_final.log)
- Entries 5: done 2 (q11463, q11533); blocked: port 2 (q11280, q11283 = same claim); open 1 (q11281, not started); done-already 0, rejected 0, deferred 0.
- Rows in STATUS.tsv 98: ported 77 (rows/: 77 sources + rows/00435530_test.cpp); not-a-binder 21 (17 header users rechecked unchanged,
  0x0049C6F0 / 0x00436500 self rows already PDB, 0x0078B070 / 0x008D5790 already bind GetMissileLoadCarrySlot); blocked rows: see q11280 / q11281
  (their statics were ported; the 0x0049C6F0 / 0x0049C700 / 0x00436500 bindings are what stays blocked / open).
- recheck_final.log: one vland --recheck-landed run over 94 rows (77 changed + 17 header users): 94 MATCH / RELOCATION_MATCH + PASS (WINS: 94).
- Header opt-ins: 1 (FABLE_RETAIL_CTHINGSEARCHTOOLS_STATICS); plus the header claim (body respell + stand-in retirement), hdr/README.md.

## What changed
1. **q11463 (NThingMap::LayerInfo) done:** the 18 g_MapwhoLevels rows of NF100's liB_left.txt (NF100's li_b transform, folded into t/stat_l.py).
   An absolute-reference scan of 0x0137DAD8..0x0137DB28 (work/li_before.tsv) found no other landed binder on a stand-in; 0x006378E0 already
   spells NThingMap::LayerInfo (the nf101 split_gate line is stale). The stale LayerInfo_0137dad8 text is gone from both headers.
2. **CThingSearchTools statics 0x013B89A8 / AC / B0 (specifics 2):** every landed binder the exe scan finds (work/st_refs.tsv) now binds
   ?NoLookups / ?NoThingsScanned / ?SearchID@CThingSearchTools@@2KA:
   - 48 rows with a local `class CThingSearchTools` model (t/stat_l.py): the statics declared in place in that existing model (no new
     model), g_SearchStamps / SearchCount_013b89a8 externs gone. work/b1.log, b2.log.
   - 26 CThingSearchTools_Calls rows (t/stat_c.py): the statics from the shared header's generated branch (new opt-in STATICS). work/b3.log.
   - 0x004162B5 CMainGameComponent::UpdateRegular zeroes NoLookups (was ?g_frameCounter_013b89a8@@3JA).
   - The header body copies (ROUGHAREABODY / TCSINROUGHAREA_BODY, used by 15 caller rows and 0x0050A650) spell the PDB names.
   - The other 7 "hits" of the scan lie past their catalog row's length (unlanded gaps), not binders.
3. **q11533 done:** 0x005DA920 ported onto engine/CTCWeapon.h (NF101's GETMISSILELOADCARRYSLOT) + CWeaponDef.h (MissileLoadCarrySlot +0x94)
   with the 0x005DA6E0 define set, MATCH first form; self rename (RENAMES s1; file slug moves CTCWeapon_GetWeaponDefLongAt94 ->
   CTCWeapon_GetMissileLoadCarrySlot). 0x00789FF0 respelled in place in its existing local CTCWeapon model (GetAmmoCarrySlot QAE ->
   GetMissileLoadCarrySlot QBE). Tests of both are self-contained models and unchanged (keep their PassPatterns).
4. **q11280 / q11283 (0x0049C6F0 + ClipAndConvert 0x0049C700): blocked: port, 1 row ported.** 0x00518900 ported as an explicit instantiation
   of the shared TCDSINROUGHAREA_EGOD body (the define set of its twin 0x005185F0, T = CTCDRegionExit), RELOCATION_MATCH first form; its self
   leaf is now the PDB template spelling its caller 0x0050A650 already binds (RENAMES s1; file slug CThingSearchToolsCalls_ -> CThingSearchTools_),
   and 0x0049C370 moves onto the std spelling 0x005185F0 / 0x006F3570 already bind (an existing split narrowed).
   **Rows left (56):** 006b2720 006ce300 007197c0 007313b0 0074d710 007512e0 0077adf0 00785dd0 0078954d 0078971d 0078db20 00792e50 007930b0 0079a050 0079a280 0079bf30 007a5190 007a63d0 007a9300 007a9580 007c3c20 007f4500 007f5d20 007f70d0 007f7340 007f9c00 007fcef0 007fd110 007fdb10 007fe730 00809a70 0080a550 0080e670 0080e8e0 0080ecc0 00851490 008786e0 008ad710 008e4100 008e60d0 00905d10 009147e0 0091a750 0091adb0 0091ba40 00937080 00937420 00937690 0093cdb0 00942390 009595c0 0095db90 00969fb0 0096f260 009793d0 00987430 
   Why not in place: the PDB spelling needs std::vector<NThingMap::CThingMap*> and NThingMap::CThingMap; the landed rows that bind it in place
   (0x00786140, 0x008AD950, ...) carry a local namespace-std vector and a local NThingMap::CThingMap, which the round-9 rules count as NEW local
   models if copied, and a cast to an opaque type was rejected (advisor). Each row needs a port onto the shared header: the explicit
   instantiation with its filter / T headers and the matching body variant (EGOD, or ORIGINAL_ROUGHAREA with THISEND / CELL_BYTEOFFSET as in
   0x007F4020; BESTROUGH_BODY / FIRSTROUGH_BODY for GetPBest / GetPFirst). The 56 rows were NOT attempted (context), not failed: resume them. Next batch: the 25 Calls rows (0x0091BA40 GetAllTCsInRoughArea on
   0x00518340's define set first), then the 31 local rows.
5. **q11281 (GetMapwhoArea 0x00436500): open (not started, 0 grades).** 0x0078954D / 0x0078971D call GetMapwhoArea out of line under the
   CThingSearchTools_Calls stand-in; the header's GetMapwhoArea is inline only. Needs first: a declared-only GetMapwhoArea opt-in (header) and,
   for 0x0078954D, NF101's out-of-line helper list (STATUS of NF101; PDB spellings of 0x00436830 / 0x00436A80 / 0x004AD520 unsettled).

## Self renames, funclets
- RENAMES s1: 0x00518900, 0x005DA920 (catalog module / name change: their files move). No EH funclet in rebuild/src/compiled/01/21 names either
  file or any changed row (grep of the 78 addresses).
- RENAMES s2: 245 caller respellings (statics x3 per row, LayerInfo for the 18, 0x00518900's three callees, 0x004162B5, 0x00789FF0).

## Locks
- Claimed 78 rows (work/claims.txt): the 75 family rows of work/famrows.txt + 0x0078B070 / 0x008D5790 + 0x00435530; 21 new mkdir, 53
  overwritten `released` owners (no live owner), 4 already NF103. No row of UNMATCHED_ROWS / COORDINATION_CLAIMS held by Account B; 0x0063F4B0 untouched (its cpp -P is identical).

## Descriptive names
- None added. The local test-model names in untouched tests (CTCWeapon_Retail::GetWeaponDefLongAt94 in 0x005DA920's test) are left as they
  were: renaming a test stand-in to the real class name would be a new local model.

## Rule slips
- b3 (the Calls payload) was running while I edited the overlay headers (the body respell); every row was regraded in recheck_final.log.
- Several early read-only orientation calls ran without the env line; one used `ls` with a too-long glob (exit 1, harmless) and one
  q11533 orientation call exited 1 (a loop's last `[ -n ]` test). No grade was affected.
- The test-only change rows/00435530_test.cpp touches a row outside the slice (claimed first; header-user fallout of the header claim).
- RENAMES s2 old symbols for the Calls rows' statics come from the landed sources, not from a before-object dump; 0x00518900's old leaf in
  RENAMES s1 was derived from its landed source (no object of the old form was kept).
- q11281 was not started (0 grades); recorded open, not blocked.

## For the coordinator: drafts that still name the retired stand-ins
The five stand-ins this round retired from the shared headers (g_NoMapwhoLevels, LayerInfo_0137dad8, SearchCount_013b89a8, SearchWork_013b89ac,
SearchID_013b89b0) or the two body macros (ROUGHAREABODY / TCSINROUGHAREA_BODY) appear in 74 files of 40 old pending/ records and in 13
Account B workspaces (read-only grep; work/acctb_hits.txt: acctb/0042ec7c 0049f4c0 004a21f0 004a6e30 00745120 007a0d90 00821a80 00a7af60,
acctb_ws/ai game mesh open render). A draft that includes NThingMap_CThingMap.h / CThingSearchTools.h and names a stand-in through the header
must respell (NThingMap::MaxLayers / NThingMap::LayerInfo / CThingSearchTools::NoLookups / NoThingsScanned / SearchID): worth a
COORDINATION_LOG note to B before this header change lands.
- Tests: work/ppgate_tests.tsv: 41 other landed tests name the three headers; all preprocess identically (0 differ).
