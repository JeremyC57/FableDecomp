# NF479 notes: Codex pass 29 name fixes (159 lines)

Line numbers are the data-line index of `CODEX_NAME_FIXES.tsv` (line 1 = the first data line), as `work/triage.tsv` and `STATUS.tsv` count them.

## What was done
- 5 batches (`batches/b1`..`b5`), 48 rows, 24 lines `done` (several partial; see STATUS.tsv), 17 `done-already`.
- Every row: objproof (HEAD repo object v new object, lander flags via cc.py): defined bytes and relocation offsets same; only the intended
  symbols renamed; `.drectve` same. `work/changes.tsv` lists every renamed self / relocation per row (no `other` or `drectve` lines).
- vland `--recheck-landed` HEAD payload v new payload (same LANDVERIFY_WORK path): every row MATCH / RELOCATION_MATCH PASS both sides.
- pickany_vs_retail + pk_all per row, HEAD v new: one change, 0x006D1DE0's IsMarriedToThePlayer pick-any copy: NOADDR (stand-in class)
  -> EQ at its owner 0x006CEC80 (16 bytes). The row's in-class inline now uses the landed `? true : false` text of 0x006CEC80 / 0x006D1E30;
  the self ReceivedConfirmation bytes are unchanged. `changes.tsv` shows that copy as `self_DIFF` (22 -> 16 bytes): it is the pick-any copy,
  not the self.
- Census of landed callers (Codex rows included: the census ran after the pass 29 landing, 541 + 171 HEAD objects compiled with cc.py into
  `work/csyms`, DEF/UND per external symbol): every landed binder of an old self spelling is in the same batch as the self; every table line
  whose witness moved is deleted, replaced, or rewitnessed onto a landed row that still references the old spelling.

## Table changes
- `TABLE_CHANGES.tsv` carries only lines whose witness column is a row of the run (land_scripted keeps only those). Rewitness lines (their new
  witness is a row outside the run that still binds the old spelling) are in `HAND_TABLE_CHANGES.tsv`, with b5's stale-line delete (line 94).
- Rewitness targets that are held rows (read-only use, RULINGS_NEEDED R4): 0x0099ED10 (T3 nf294), 0x00BDEB50 (T3 nf298), 0x0097EC00 (T3 nf309),
  0x00B54E30 (COORD_HOLD_NF465).
- Anonymous-namespace helper 0x0097E140: the binders' new UND carries a per-TU hash; the hash-stripped spelling already has a table line at
  0x0097E140 (witness 0x0097E380), so no line is added (EXTRA_RULES 2026-11-20). The old free spellings: `YIPAV...PAV` rewitnessed to
  0x0097EC00 (T3, stays), `YIPBV...PAV` deleted.
- 0x00886E80 CompactTextureSet: not a Codex line; 0x00887830's self respell into NEnvironmentInternal moves its call onto the landed
  0x00886E80 self spelling, so the old line (witness 0x00887830) is deleted.

## Header-driven binders left on old spellings (follow-up, add-only header opt-ins)
- CRC32TAB (line 6): `engine/CCRC.h` has a FABLE_RETAIL_CRC32TAB_PASS29 arm only in the ORIGINAL_TYPED_LOOP branch. 0x00409FD0, 0x006B9610,
  0x008DDAD0, 0x008E18D0 (PASS22_BODY), 0x009CEC60 (BANK_STATEMENTS), 0x00AF4900, 0x00B21A50 (CALC_BODY) need the same arm in their branch.
  0x0078ECD0 / 0x007A7D20 use the TYPED_LOOP branch (one `#define`) but are in Codex p28 / p27 RESERVED.tsv. 0x009AD410 is T3 nf303.
- g_SSEAvailable (line 131): shared headers NVectorMath.h / CMatrix4x4AlignedSSE.h declare it (header claim).
- CSurface::InitialiseAsDepthBuffer (line 7): 0x00B27EE0 / 0x00F37390 include the generated CSurface.h branch, which lacks the member
  (only the FABLE_RETAIL_CSURFACE branch declares it, under FABLE_RETAIL_SHADOW_BUFFERS_PASS27). Draft: work/drafts/00b27ee0.cpp.
- CWorldMap::SetMapAsLoading (line 46): RULINGS_NEEDED R3. Drafts: work/drafts/006c20b0_newlocal_CWorldMap.cpp (+ test; vland
  RELOCATION_MATCH PASS with the test updated) and work/drafts/draft_004fcfe0_selfneedshdr.cpp.

## Rows taken out after the vland warning diff (no new local models of shared-header classes)
vland HEAD v new warning diff showed three rows adding SHARED_HEADER_AVAILABLE: 0x006C20B0 (CWorldMap), 0x00843840 (CThingAICreature /
CThingCreatureBase), 0x00A2E9A0 (C3DMeshFileConvexVolumeChunk). Naming a local stand-in after a shared-header class is a new local model
(brief rule), so they are parked (work/drafts) and their lines blocked. 0x00843840 with the shared header fails to compile (the row's local
CCharString / CSoundPair models collide): it needs a port.

## Stale table lines seen (not changed here)
`work/stale.tsv`: witness HEAD object no longer references the symbol. Line 94 (0x00B17F70 WriteFloat stand-in) is deleted in b5. The others sit
on held or Codex targets and are left: 60 (0x00643F13 / 0x00907670, COORD_HOLD_NF453), 97 (0x004D0F80 / 0x0043020A, COORD_HOLD_NF453), 129
(0x00C36430 at Codex row 0x00C461E0), 130 (0x00C0B380 / 0x00C25FF0, COORD_HOLD_NF453).

## Link note for b1
Codex row 0x004900F5 (landed with pass 29) defines a pick-any copy of `?WordWrapText@CSubtitleRenderer@@ABE...` (its
FABLE_RETAIL_SUBTITLE_WRAP_BODY_PASS29 inline body). After b1 the self row 0x0048DC8B defines the same symbol out of line (strong). pk_all on
the HEAD 0x004900F5 object already resolves that copy to 0x0048DC8B and grades it EQ (265 bytes, `batches/b1/evidence/pk_004900f5_head.txt`),
so b1 changes no pick-any verdict. The strong-plus-pick-any pair is the same shape HEAD already has for CTCWife::IsMarriedToThePlayer
(strong EQ in 0x006D1E30, pick-any copies in 0x006CFF10 / 0x006D0410).

## Units
`UNITS.tsv` unit column is a leader address (land_scripted groups rows by equal unit value and treats `unit != address` as a
binder): every single row is its own unit; b1 0x0048DC8B + 0x0048E012 + 0x0048F50C (the WordWrapText self, its binder and the shared
g_pWordWrapFont replace line), b2 0x006EA540 + 0x007E5B90 (self + only old-spelling binder), b3 0x009CC0B0 + 0x009D1320 + 0x009D4DB0 (they share the
g_crc32Table delete line). Every other TABLE_CHANGES delete / replace line has exactly one batch row binding its old spelling (its witness).
No landed source includes a batch row file, and no funclet names one as parent (grep of rebuild/src/compiled). The b4 catalog renames of
0x00687D10 / 0x00887830 rename those Source files at landing; nothing includes them.

## Tests
The HEAD tests are self-contained transcriptions (e.g. 0x008BFDA0's test re-declares and re-implements the class with the old pointer
signature; it does not link the row), so every row passed vland with its HEAD test. Updating those transcriptions to the new spellings is
cosmetic follow-up, not done.

## Tools
`tools/`: cc.py (lander flags), objdiff.py / objproof.sh, runpair.sh (vland pair), pk.sh + reachflip.py, census.py / census_syms.sh /
census_check.py, changes.py (per-row symbol change list), tablegen.py (table drafts), results.py (RESULTS + table split), mkrun479.py,
sandbox.sh (shadow repo, tracker dry run), write_status.py.
