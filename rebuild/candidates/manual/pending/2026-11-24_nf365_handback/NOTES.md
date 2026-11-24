# NF365: the GetDef swap (row-local GetDef template -> GETDEF_GLOBAL_BODY + SETDYNAMICCAST_REREAD + NF349 SIZE_BODY), Account A naming queue, 2026-11-24

## Step 1: footprint check -- VERDICT: EQUAL, continue
Swapped objects: NF361 held/g1 texts of 005fc2d0 and 00868e50 (orig == current repo HEAD source for both), compiled by me at one path
(tools/chain361.sh -> pass.py, `timeout 900` each): H and G both RELOCATION_MATCH.
References (landed rows that define GLOBAL_BODY + REREAD + an NF349 SIZE_BODY, HEAD sources, compiled by me, all RELOCATION_MATCH):
- 0x006096E0 CTCInventoryMap::ConstructLogbookListPC (/O2 default flags; SIZE_BODY CThingObjectDef)
- 0x005E5F30 CTCQuestCompletionUI::ActivateReal (/GS; SIZE_BODY CUIMiscThingsDef + CThingObjectDef)
- 0x006D1250 CTCWife::CheckAppearanceChanges (/GS; SIZE_BODY CThingObjectDef; the only non-B landed reference for SetDynamicCast<CAppearanceModifierDef>)
- 0x005E4BE0 CTCQuestCompletionUI::GetQuestDescription (/d2inlT60 /d2inlS48; SIZE_BODY CUIMiscThingsDef + CThingObjectDef): the second
  reference for SetDynamicCast<CUIMiscThingsDef> (advisor: 11 rows depend on it)
- owners 0x0042B312 / 0x0042B040 / 0x00817B40 (read-only HEAD compiles, RELOCATION_MATCH).
Tool: tools/fpcheck365.py (every function COMDAT the swap changed or added, G v H, compared with the same-named COMDAT in each ref: bytes,
relocation list, COMDAT selection). Evidence: evidence/step1_fpcheck.txt, evidence/step1_typesub_armour.txt.
Results:
- 005fc2d0: changed/added = GetDef<CUIMiscThingsDef> (EQUAL 0x005E5F30, 0x005E4BE0 and owner 0x0042B312), SetDynamicCast<CUIMiscThingsDef> 52 B
  (EQUAL 0x005E5F30 and 0x005E4BE0), Get<CDefClassBase> 3 B (EQUAL 0x006096E0, 0x005E5F30, 0x006D1250). Every COMDAT G shares by name with the three
  rows is equal (25/25, 34/34, 20/20).
- 00868e50: GetDef<CThingObjectDef> 98 B (EQUAL 0x006096E0, 0x005E5F30, 0x006D1250; bytes + relocs equal to owner 0x0042B040, which is
  the strong sel-1 definition), SetDynamicCast<CThingObjectDef> 52 B (EQUAL x3), SetDynamicCast<CAppearanceModifierDef> 52 B (EQUAL
  0x006D1250), Get<CDefClassBase> (EQUAL x3), GetDef<CArmourDef> 108 B (bytes + relocs equal to owner 0x00817B40, strong sel 1 there).
  Shared-by-name COMDATs all equal (36/36, 25/25, 50/51: the one differing, GetSubDef<CAppearanceModifierDef>, is NOT changed by the swap,
  it is HEAD-identical in 00868e50 and is known NEQ debt 0x0057FF92).
  SetDynamicCast<CArmourDef> 52 B: no non-B landed object defines it (only 0x00839940, Account B's: stay off). Structural twin check
  (tools/typesub365.py): equal to SetDynamicCast<CThingObjectDef> of 0x006096E0 with the class name substituted (no relocations).
  B's committed landing log (batches/2026-11-24_landing_acctB_b24e/logs/00839940.pk_all.log, read, not compiled) lists no
  SetDynamicCast<CArmourDef> either, so no landed object anywhere carries a same-named reference. 00868e50 is in its own run dir (held/g2) so
  this residual cannot hold the other rows; it is HELD for ruling R1.
- Verdict: every helper COMDAT the swap changes or adds is byte-, relocation- and selection-equal to the same-named COMDAT of landed
  GLOBAL_BODY + REREAD + SIZE_BODY rows (twin-only for SetDynamicCast<CArmourDef>). The swap brings the shared body's normal footprint.
  Continue to Step 2.

## Step 2: the swap per row
Text: tools/mkvar365.py = NF361's mkvar361 kind G generator, wording unchanged (the form the ruling was made on: "Pick-any (NF361)" comment,
three defines above the first #include, the local template plus its directly-preceding GetDef comment replaced by one comment line). The
size-body type is chosen from the GetDef<T> DEF symbols of each row's HEAD object. It reproduces NF361's held/g1 texts byte for byte
(005fc2d0, 00868e50). Every rows/<a>.cpp is byte-identical to the compiled var/G/<a>.cpp (asserted by tools/mkbatch365.py).
Emptied `#pragma auto_inline(off)/(on)` pairs are kept (advisor: reproduces HEAD's pragma state).

Prechecks: every landed test before its lock (clang; 0047a9f0 / 005fc8a0 / ... fail only in VC7.1 headers, `_assert` or the static-assert
typedef redefinition, then `cl /Zs` PASS, D1; work/pre.log, work/pre_zs.log); every HEAD and variant source after its lock (clang PASS, all).
pragscan349 (GetDef block) C_OK on all 16 variants; pragscan352 (Reset block) C_OK on 006001e0 / 00600b60 / 00605570
(evidence/pragscan_getdef.txt, evidence/pragscan_reset.txt). Reverse includes: none of the 16 sources is #included anywhere
(evidence/reverse_includes.txt; 00868e50 / 0061b610 are named only in comments).

Per-row gates (H = repo source, g = swap; one path, pass.py, `timeout 900`):
- grade: pass.py RELOCATION_MATCH H and g for the 13 rows below; vland `--recheck-landed` HEAD payload and new payload all
  RELOCATION_MATCH PASS (logs in each run dir; the b1 vland payload covered the b1 rows plus 0060c900 / 0060eeb0, so held/g3's logs list all 12).
  The vland objects' GetDef copies equal my pass.py objects (116 at H, 98 / 108 new; evidence/vland_vs_pass_getdef.txt).
- leafrel (stable keys): leaf data- and relocation-equal on all 13.
- objsec352 H v g: exactly GetDef<T> (116 -> 98; 00868e50 also GetDef<CArmourDef> 116 -> 108), SetDynamicCast<T> 50 -> 52, Assign<T>
  dropped, IsNull<CDefClassBase> dropped, Get<CDefClassBase> added. Exception: 0060c900 / 0060eeb0 also change their strong same-TU
  CreateMapMarkers copy (0x005FF190; see held/g3).
- footprint (fpcheck365) per row v 0x006096E0 / 0x005E5F30 / 0x006D1250 / 0x005E4BE0: 0 DIFF on every row; the only changed/added
  COMDATs without a same-named reference are 00868e50's GetDef<CArmourDef> (owner-equal) and SetDynamicCast<CArmourDef> (twin), and
  held/g3's CreateMapMarkers.
- owner: every GetDef<T> copy byte+relocation EQUAL to its owner object (0x0042B312 x12, 0x0042B040, 0x00817B40; evidence/owner_cmp).
- pick-any gate (pkcmp346, both checkers HEAD v new): GATE PASS in b1 / g2 / g3, 0 turned NEQ, 0 newly NEQ, 14 copies FIXED; reversed
  control FAILs in each; reach flips 0 (both scans).
- drop test (agent_tools/nf357/droptest.py form, tools/droptest.py): 28 dropped symbols over 13 rows, all DEF of inline pick-any helpers
  (Assign<T> 48 B, IsNull<CDefClassBase> 10 B), no reloc_bindings line, no witness, NOADDR (no retail address, so no EQ copy with an
  address is lost). No UND symbol dropped or added. No landed object can be left undefined: they are inline members, so any TU that
  references one emits its own pick-any copy.

| Dir | Rows | Verdict |
|---|---|---|
| batches/b1 | 005fc2d0 0047a9f0 005fc8a0 005fcbe0 005fddd0 006001e0 00600b60 0061b610 0061e6d0 00622a00 | ACCEPT |
| held/g2 | 00868e50 | HOLD for ruling R1 (SetDynamicCast<CArmourDef> twin-only reference); passes every measured gate |
| held/g3 | 0060c900 0060eeb0 | HOLD for ruling R2 (an already-NEQ strong copy changes body); passes every measured gate |
| skipped | 005ff190 00605570 00607330 | the leaf moves (grade DIFFER) |

Skipped (measured; locks released):
- 005ff190: g DIFFER(4128v4176) (H RELOCATION_MATCH). leafrel: the leaf's inline GetDef expansion becomes a call (GetPDefFromGlobalIndex
  relocation at +0xD1D -> GetDef<CUIMiscThingsDef>).
- 00605570: g DIFFER(1841v1890): the leaf now calls GetDef<CUIMiscThingsDef> where retail expands it at 0x00605819..0x00605858
  (leafrel: GetPDefFromGlobalIndex relocation -> GetDef call).
- 00607330: g DIFFER(9084v9124): the same move (GetPDefFromGlobalIndex at +0x720 -> GetDef<CThingObjectDef> call; the row comment says
  the leaf expands it with the `!def.IsNull()` spelling). Measurements: evidence/leafmove_skipped.txt.

held/g3 detail: 0060c900 / 0060eeb0 each carry a STRONG same-TU copy of CTCInventoryMap::CreateMapMarkers (0x005FF190), leaf-called, NEQ at
HEAD (4476 v 4176) and NEQ after (4420 v 4176) under both checkers: its inline GetDef expansion becomes a call (leafrel on the copy:
GetPDefFromGlobalIndex at +0xEB0 -> GetDef<CUIMiscThingsDef>; evidence/cmm_<a>.txt), the move that makes 0x005FF190's own row DIFFER. It does not
turn NEQ and is not newly reached (gate PASS, 0 reach flips), and the nf203 rule makes an already-differing copy's body change debt,
not a hold. But the brief's objsec expectation was "the shared body's footprint only", so the two rows are HELD (not self-ruled).

Leaf-called NEQ copies left in the new objects are HEAD debt, unchanged (evidence/remaining_neq_new.tsv), e.g. 00868e50's
GetSubDef<CAppearanceModifierDef> 0x0057FF92 (182 v 162), 0047a9f0's map::operator[] 0x0047CB50 / ~CWideString 0x0099B510.

Stale comments (coordinator): in 005fc2d0 005fc8a0 005fcbe0 005fddd0 the "Same-TU body of the def lookup retail's TU holds ..." comment
above `#pragma auto_inline(off)` now describes a body that is gone (the generator drops only a comment directly above the template). Left as
is, because the held/g1 form was the ruled text; a comment-only edit would not change bytes.

## Rulings needed
- R1 (held/g2, 00868e50): SetDynamicCast<CArmourDef> 52 B has no same-named COMDAT in any landed object (B's 0x00839940 does not emit it
  either). It is equal to SetDynamicCast<CThingObjectDef> of the references with only the class name substituted, and has no relocations.
  Is the twin evidence enough for Step 1 on this row?
- R2 (held/g3, 0060c900 / 0060eeb0): their strong, leaf-called, already-NEQ CreateMapMarkers copy changes body (4476 -> 4420 v retail 4176).
  Land as debt (nf203), or hold?
- T3 lines: nf358 0x0042B312 / 0x0042B040: remove the b1 emitters on landing (g2 / g3 if ruled yes); the remaining 116 B emitters
  005ff190 00605570 00607330 need another device (their leaves expand GetDef).

## Process
- Locks: claimed with lock.sh, every one released at the end (work/locks.log; scan in evidence/locks_release.log).
- Owners 0x0042B312 / 0x0042B040 / 0x00817B40 and the reference rows were compiled read-only and released right after their builds.
- No repo write, no git. No retail bytes in the hand-back (the vland oracles stay in work/).
- Tools (tools/): NF361 copies retargeted to NF365 (nf.py, pass.py, chain361.sh, objsec352, leafrel, pkrun352, pkcmp346, cmpcomdat,
  comdatsel, reachflip_coordinator, pragscan349 / 352, pre_*.sh, zs_tests.sh, vland361.sh, gdclass361.py, mkvar361.py);
  droptest.py (nf357, retargeted + pk_all lane path); new: fpcheck365.py, typesub365.py, mkvar365.py, lane365.sh, eval365.sh,
  gate365.sh, mkbatch365.py.

## Stay-off check of the 13 changed rows (evidence/stayoff_changed_rows.txt)
- No row is in UNMATCHED_ROWS.tsv, none has a Codex pass-28 reservation, and every COORDINATION_CLAIMS line naming one is Account A's
  (NF357 / NF361 done, NF365 claimed). The Account B packages that mention them are the 2026-11-11..13 authoring packages of rows landed
  long ago, plus header comments in 2026-11-24_acctB_b24 (no open B claim): the NF357 Q5 / NF361 Q4 situation.
- 0x0060EEB0 has a T3_PORT_QUEUE line in column 2 (nf303, `port (census)`, naming entry 474: a stand-in binding for the address's own
  name). My brief named the row explicitly (the prefix wins). It is in held/g3 anyway, and the swap does not touch that binding (leaf and
  all leaf relocations unchanged). The coordinator should confirm this before landing g3.
