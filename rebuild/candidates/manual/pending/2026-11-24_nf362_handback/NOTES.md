# NF362: size-pragma emitters of the reopened Reset<CComponent> / GetDef copies (Account A naming queue, 2026-11-24)

Scope: brief BRIEF.md (34 rows, NF358's census `pending/2026-11-24_nf358_handback/`). No repo write, no git.

## Step 1: the pragma state of each owner and each sub-group (measured BEFORE any fix)

Method:
- tools/pragscan362.py preprocesses a TU with clang -E (MSVC mode) and replays VC7.1's optimize semantics (`("", on)` = command line,
  no push / pop) in TU order. It prints each `#pragma optimize`, the state at marked lines (the CCPPointerInfo constructor, the
  CCountedPointer class template, the NF352 / NF349 specializations, the GetDef template body) and the state at the END of the TU, which
  is where VC7.1 takes a template instance's mode (GOTCHAS 2026-11-24, NF349). Output: evidence/pragscan/ (H_ = landed text, V_ = variants;
  SUMMARY_H.txt = all 34 rows).
- One-path VC7.1 compiles (tools/pass.py via tools/run362.sh, catalog flags), both checkers on every object (tools/pkrun352.py: pk_all
  `--tsv --row-length --leaf`, pickany_vs_retail `--leaf`), work/pk/s1/ and work/pk/own/. tools/pkdiff362.py summarises H v variant
  (evidence/step1_pkdiff.txt). tools/cmpcomdat.py compares the copy with the OWNER object's COMDAT (bytes + relocations).
- Variants (tools/mkvar362.py): `rs` = NF352 RESET_SIZE_BODY define in place; `rd` = NF352 RELEASEDATA_SIZE_BODY in place; `hc` = include
  move (the row's CCOUNTEDPOINTER defines + `#include "engine/CCountedPointer.h"` placed directly above the row's first `#pragma optimize`,
  the row's own include line of it removed, pragmas and all other lines untouched); `rr` = `#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD`
  in place; `go` = NF349 GETDEF_CTHINGOBJECTDEF_SIZE_BODY in place.

### Owners
| Copy | Owner text | Owner state where the copy is compiled |
|---|---|---|
| Reset<CComponent> 0x00429C6F (59 B) | row-head `optimize("s", on)` before the include, explicit instantiation, /O2 /Oy /W3 | end of TU: **s on, y on** (no frame pointer) |
| NF352 block (as used by 0x00605570 / 0x00626110) | /O2 TUs with no row pragma; the specialization sits inside CCountedPointer.h after `optimize("s", on)` | at the specialization: **s on, y on**; then `optimize("", on)` |
| GetDef<CUIMiscThingsDef> 0x0042B312 / GetDef<CThingObjectDef> 0x0042B040 (98 B) | row-head `optimize("y", off)` + `("s", on)`, local models | **s on, y off** |
| NF349 block | specialization between its own `("y", off)` + `("s", on)` and `("", on)` | **s on, y off**, then command line |

### Reset<CComponent>, the 20 `("y", off)` + `("s", on)` TUs: the state differs (y)
- Every one of the 20 rows sets `optimize("y", off)` + `("s", on)` at row head, ABOVE its (direct or transitive) include of
  CCountedPointer.h; 19 of them also have the CCPPointerInfo constructor and the class template in the y-off region.
  The exception is **0x005CDD86**: it includes CCountedPointer.h ABOVE its pragmas (the nf189 form).
- The Reset instance is compiled at the end-of-TU state **s on, y off**; the owner's is **s on, y on**. That is the 62 v 59 B
  difference (frame pointer). Measured on 00427216 / 00582ace / 005a29f4: H copy 62 B NEQ, reach y (= NF358).
- **NF352 opt-in in place (`rs`) does not give an EQ copy in these TUs.** The specialization is defined inside CCountedPointer.h, i.e.
  after the row's `("y", off)`, so it is compiled at s on, y off and stays **62 B NEQ**; and the block's `optimize("", on)` returns the
  rest of the TU (the row's own leaf) to /O2: **leaf DIFFER** (00427216 3569v2995, 00582ace 2233v1356, 005a29f4 2215v1709), with
  ReleaseData 0x004291DE and the destructor 0x00429C53 turning NEQ. `rs+rd`: Reset becomes 59 B EQ (it now follows the RELEASEDATA block's
  reset) but the leaf is still DIFFER and 3-12 EQ copies turn NEQ. So the opt-in is excluded here by its own condition (nf189: no row
  optimize pragma before the include).
- **The include move is NOT byte-neutral (control `hc`).** Moving CCountedPointer.h above the pragmas keeps the leaf (RELOCATION_MATCH,
  identical) and every addressed copy, but changes one COMDAT: `??0CCPPointerInfo@@QAE@KP6IXPAX@Z0@Z` (NOADDR, no table line) 26 B -> 25 B.
  CCPPointerInfo's constructor is an ordinary (non-template) inline function in CCountedPointer.h, so it takes the mode at its DEFINITION:
  y off + s on in HEAD, the command line after the move. The moved copy (25 B) also differs from the owner object's own copy (25 B, other
  bytes: compiled at s on, y on). Same result on all three rows.
- `hc+rs` (move + NF352 opt-in): Reset 62 -> **59 B EQ**, byte- and relocation-equal to the owner object's COMDAT; leaf identical;
  the only other change is the same CCPPointerInfo constructor. So the opt-in WOULD fix the copy, but only with a move that is not byte-neutral.
- **Verdict:** the 19 rows other than 0x005CDD86 need a device this brief does not allow (a header block that restores the TU's
  y-off / s-on state instead of the command line, or a ruling that accepts the CCPPointerInfo change). STATUS skipped, proposal below.
  0x005CDD86 already has the nf189 shape and is measured separately in Step 2.

### GetDef<CUIMiscThingsDef> / GetDef<CThingObjectDef>: the state is NOT the cause; the body is
- Every row of both sub-groups except 0x00584B86 defines the shared-header member template `CDefinitionManager::GetDef(long,
  CDefPointer<const T>*) const` IN THE ROW (a "same-TU body" under `#pragma auto_inline(off)`), so the copy is that template's
  instance at the end-of-TU state: **s on, y off** in the y-off rows (= the owner's state) and **s on, y on** in the `("s", on)`-alone
  rows. 0x00584B86 takes the header's GETDEF_GLOBAL_BODY template (state s on, y off).
- The y bit makes no difference to this function: our copy keeps `push ebp; mov ebp,esp` in the `("s", on)`-alone TU too (0058578b:
  the body takes the address of a parameter slot, so VC7.1 keeps the frame pointer either way).
- The 101 v 98 B difference is the default CDefPointer::SetDynamicCast (keeps the pointer in a register); retail re-reads it
  (NF341 / NF346: FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD, which the NF349 block itself requires).
- **REREAD alone (`rr`) gives the EQ copy in every measured row**, both sub-groups and both types: 005a29f4 / 005a8a5d (y-off,
  UIMisc), 0058578b (s-alone, UIMisc), 005b3aef (s-alone, ThingObj), 00584b86 (both, ThingObj, header body): 101 -> 98 B, EQ under both
  checkers, byte- and relocation-equal to the owner object's COMDAT (0x0042B312 / 0x0042B040); leaf RELOCATION_MATCH and unchanged.
  Only NOADDR `?Assign@?$CDefPointer@...` helper COMDATs drop (the NF346 / NF349 class). 0058578b's GetDef<CThingBaseDef> 0x0042AF3C copy
  also turns 101 -> 98 B EQ.
- **The NF349 block in place (`rr+go`, 00584b86) moves the leaf** (DIFFER 1314v854): its `optimize("", on)` resets the row's y-off /
  s-on for everything after it, and 12 EQ copies turn NEQ. The specialization itself is EQ. In the in-row-body rows the NF349 block
  also sits inside `#ifdef FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY`, whose template body would duplicate the row's own body
  (read from the guard, NOT compiled; the measured reset reason above is sufficient on its own).
- **The group heading is loose for GetDef:** 17 of the 18 GetDef rows carry the GetDef template BODY in the row text (same-TU body of
  the shared-header member template, under the row's own `auto_inline(off)`); only 0x00584B86 takes the shared-header body. The fix does
  not touch that text: the REREAD define changes only the shared CDefPointer::SetDynamicCast it calls.
- **Verdict:** fix = the REREAD define alone (the NF349 block's own prerequisite; NF341 r2 / NF349 b1 0x0074F760 landed it alone).

## Step 2: the fix, per sub-group (one-path compiles, both checkers, objsec, owner-object compare, drop test, vland)

Order: test precheck (clang; else `cl /Zs`, D1: work/pre_zs.log) -> lock NF362 -> source precheck of HEAD and new (clang, nowchar for
0058578b / 0041e6d3; 005cacb9 by `cl /Zs` on both texts, the same fable_map.h clang error fires on HEAD: NF305 Q2) -> compile (tools/gate362.sh:
pass.py H and N at one path, then pkrun352 into evidence/pk_gate/ as H_<a> / N_<a>).

### Batch b1: GetDef rows, `#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD` (one added line per row, with a `// NF362:` comment)
17 rows ACCEPT: 0058578b 005a04f7 005a29f4 005a8a5d 005a9cbe 005aaf28 005ad571 005c6b26 005c7452 005c8459 005c90fc 005c9a95 005cacb9
(GetDef<CUIMiscThingsDef>) and 00584b86 005b3aef 005b76d1 005b8743 (GetDef<CThingObjectDef>).
- Grade: scratch RELOCATION_MATCH H and N for all 17 (work/pass_gate.log); vland `--recheck-landed` HEAD v new: see logs/.
- Leaf identical in all 17 (objsec352 leaf=same; evidence/objsec/<a>.txt). tools/classify362.py classifies EVERY differing objsec line:
  only (a) the target GetDef COMDATs, (b) CHANGED `??$SetDynamicCast@` COMDATs that pk_all lists NOADDR in H and N (the REREAD body itself),
  (c) DROPPED `?Assign@?$CDefPointer@` COMDATs (+ their SYM- DEF) that pk_all lists NOADDR in H. 17/17 OK, other=0 (evidence/classify.txt).
- Copies: every target copy 101 -> 98 B, NEQ -> EQ under both checkers (pkcmp346: GATE PASS, 0 TURNED_NEQ / 0 NEWLY_NEQ in either checker,
  27 FIXED each; the reversed control FAILs: evidence/pk_gate.txt, pk_gate_control_reversed.txt). Reach flips: 0 over 19 H/N pairs
  (reachflip.py; evidence/reachflip.txt; pair count = 19 rows including 0041e6d3 / 005cdd86, so the pairing worked).
- Owner-object compare (tools/cmpcomdat.py v the owners' OWN objects H_0042b312 / H_0042b040): all 13 + 4 fixed copies EQUAL in bytes AND
  relocations (evidence/owner_cmp_getdef.txt).
- Bonus copies: GetDef<CThingBaseDef> 0x0042AF3C turns 101 -> 98 B NEQ -> EQ (both checkers) in 9 rows (0058578b 005a9cbe 005aaf28 005ad571
  005b76d1 005b8743 005c8459 005c9a95 005cacb9). No owner-object compare: 0x0042AF3C is NF363's row (stay off). The 9 copies are equal to
  one another (evidence/bonus_0042af3c_cmp.txt).
- Drop test: 11 distinct NOADDR `?Assign@?$CDefPointer@...` helpers drop (work/dropped_syms.txt). reloc_bindings.tsv names none (0 lines);
  tools/helperscan356.py over the 77,096 objects under the scratchpad (1 unreadable): UND 0 for all 11; the 20 references found (12 to
  the CCreatureDef helper, all in 00667d30.t.obj; 8 to the CThingBaseDef helper, all in 006c14d0.t.obj) sit in objects that define the
  helper themselves, so no object is left undefined (evidence/helperscan.txt). tools/droptest.py itself was NOT run; what stands in for
  it: no UND delta in any row (objsec symdelta = SYM- DEF lines only), 0 reloc_bindings lines, UND 0 over the corpus. None has a
  retail address, so the "EQ definer elsewhere" condition does not arise. This is the NF346 / NF349 / nf341_r2 class.
- Mixed rows 005a29f4 005a9cbe 005aaf28 005ad571: GetDef fixed; their Reset copy stays 62 B NEQ, reach y (already-NEQ debt, unchanged H v N).
- REREAD alone is not literally "the NF349 block" the brief names. It is the block's own prerequisite (the block `#error`s without it), an
  existing define with no header or pragma change, landed alone before (nf341_r2; NF349 b1 0x0074F760). Ruling asked (Q1).

### Batch b2: 0x005CDD86, NF352 opt-in in place (`#define FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RESET_SIZE_BODY`)
- 005cdd86 includes CCountedPointer.h ABOVE its `("y", off)` / `("s", on)` (the nf189 form). pragscan of the NEW TU
  (evidence/pragscan/N_005cdd86.txt): no optimize pragma before the block; the specialization is compiled at s on, y on (the owner's
  state); the block's `("", on)` comes before the row's own pragmas; the leaf and the end of TU stay at s on, y off.
- Exactly one COMDAT changes (objsec: Reset 62 -> 59 B; leaf same; no symbol delta). ReleaseData 0x004291DE stays EQ 44 B. Reset is now EQ
  under both checkers and EQUAL (bytes + relocations) to the owner object's COMDAT (evidence/owner_cmp_005cdd86.txt). pvr reads it
  not-leaf-called, pk_all leaf-called; either way FIXED, 0 TURNED_NEQ. ACCEPT.

### Skipped
- **Reset<CComponent>, 19 rows** (00427216 00582ace 0058315b 0058a0c7 0058e208 005907ab 0059cb90 0059d270 0059ed42 0059f51e 0059fee3 005a29f4
  005a8c66 005a9cbe 005aaf28 005ad571 005d64dd 005d6748 005d7081): the row's `("y", off)` + `("s", on)` precede CCountedPointer.h. Measured
  (Step 1, 3 rows): NF352 opt-in in place = Reset still 62 B and leaf DIFFER; include move = not byte-neutral (??0CCPPointerInfo 26 -> 25 B,
  a third form, equal to neither HEAD nor the owner object). Needs a header device (proposal P1) or a ruling (P2). Pragscan of all 19
  (evidence/pragscan/SUMMARY_H.txt) shows the same layout as the 3 measured rows.
- **0x0041E6D3** (GetDef<CThingObjectDef>, /GS, inline_depth regions): REREAD makes the copy 105 -> 98 B but it is still NEQ under both
  checkers and differs from the owner object in bytes (the failure branch is placed differently); its GetDef<CThingBaseDef> 0x0042AF3C and
  GetDef<CDefClassBase> 0x0042B144 copies also go 105 -> 98 B and stay NEQ. Nothing turns EQ, so the row is not changed.

## Proposals / rulings needed
- **Q1 (GetDef):** accept REREAD alone as the existing opt-in for b1 (above). The NF349 block cannot be used in these TUs (its reset moves
  the leaf), and is not needed (the y bit does not change GetDef's frame here).
- **Q2 (GetDef):** b1's "objsec equal except the fixed copies" is read through the NF340 copy-repair drop-test scope: besides the target
  GetDef COMDATs, the REREAD define also rewrites NOADDR `??$SetDynamicCast@` helper bodies and drops 11 NOADDR `?Assign@?$CDefPointer@`
  helpers (the NF346 / NF349 / nf341_r2 classes, all classified by script, other=0). Confirm that this counts as passing.
- **P1 (Reset, preferred), measured as a scratch probe only, not handed back as a header change:** a sibling of the NF352 Reset block for
  TUs whose row head sets `optimize("y", off)` + `("s", on)` above the include: `optimize("y", on)` + `("s", on)`, the same
  specialization, then RESTORE `optimize("y", off)` + `("s", on)` instead of `("", on)`. Probe (work/probe/tree, a full copy of
  rebuild/include with one appended `#ifdef NF362_PROBE_RESET_YOFF_TU` block; rows 00427216 / 00582ace with the probe macro defined in
  place): scratch RELOCATION_MATCH, objsec exactly ONE COMDAT changes (Reset 62 -> 59 B), leaf same, no symbol delta, ReleaseData stays EQ,
  Reset EQUAL to the owner object's COMDAT (evidence/probe_*). It is valid only where the TU's state at the include is exactly y off + s on
  (a pragscan precondition, like nf189's), so it needs its own opt-in name and an equiv_check / planted control at landing. It would fix the
  19 rows with one define each and no include move.
- **P2 (Reset, alternative):** rule that the include move (`hc`) is acceptable although `??0CCPPointerInfo@@QAE@KP6IXPAX@Z0@Z` changes
  26 -> 25 B: that COMDAT is NOADDR (no table line) and no relocation in the three measured H objects references it (objdump -r: 0
  references; the owner object references it only from .debug$F), so the linker can drop it. Weaker than P1: the moved copy matches
  neither HEAD nor the owner object's copy, and the move reorders every non-template inline of CCountedPointer.h into /O2 speed mode.

## Run dirs (land_scripted form; no header, test or table change)
- batches/b1 (17 rows, all ACCEPT): rows/, rows.txt, RESULTS.tsv, head/payload.json (HEADPAY), head/HEAD_FROM_REPO.txt (bare addresses),
  TABLE_CHANGES.tsv (header only), orig/rows + orig/tests (asserted equal to the repo at build time), logs/recheck_head.log /
  recheck_new.log (vland `--recheck-landed`: 17/17 RELOCATION_MATCH PASS HEAD and new, WARN sets identical), evidence/ (pk_gate H/N per row,
  pk_gate.txt GATE PASS, reversed control GATE FAIL, reachflip 17 pairs 0 flips, classify 17/17 OK, objsec per row, owner_cmp_getdef,
  bonus_0042af3c_cmp, dropped_syms + helperscan).
- batches/b2 (005cdd86, ACCEPT): same layout; vland 1/1 RELOCATION_MATCH PASS HEAD and new, WARN sets identical; pragscan H / N,
  owner_cmp_005cdd86.
- Landing notes for the coordinator: (1) 0x00584B86 is a T3_PORT_QUEUE column-2 address (nf307 binder port, unrelated to this copy fix;
  that T3 line must stay); land_scripted will hold it as T3_PORT_QUEUE unless overridden. (2) COORDINATION_CLAIMS carries the open NF362
  claim line naming every address; land_scripted holds rows named by an open claim until that line is closed. (3) T3 lines to update:
  nf358 0x00429C6F (005cdd86 fixed; 19 emitters wait for P1 / P2), nf358 0x0042B312 (13 emitters fixed), nf358 0x0042B040 (4 fixed,
  0041e6d3 left with the measurement above).

## Locks and process
- 24 locks taken: 21 group rows that were compiled (all 34 except the 13 Reset-only rows that were never compiled; 00427216 / 00582ace were
  the Step 1 Reset samples) plus the 3 owners 0x00429C6F / 0x0042B312 / 0x0042B040 for the owner-object compares. All are released (evidence/locks_release.log: 24 RELEASED; a scan of
  $S/w22/nflocks/*/owner finds no NF362).
- Owner rows were compiled read-only (one HEAD compile each), never changed.
- Only batches/b1/rows and batches/b2/rows are candidates. The top-level rows/ holds the same final texts; rows/var/ holds Step 1 variants
  and the measured-but-skipped 0041e6d3 text (rows/var/0041e6d3_rr_final_skipped.cpp; its HEAD copies are in orig/skipped/).
- NF363 overlap: b1 turns the GetDef<CThingBaseDef> 0x0042AF3C copy EQ in 9 rows (0058578b 005a9cbe 005aaf28 005ad571 005b76d1 005b8743
  005c8459 005c9a95 005cacb9). 0x0042AF3C is on NF363's list; none of these 9 rows is. The coordinator should reconcile NF363's results and
  0x0042AF3C's T3 line with these 9 emitters.
- pk_all printed SRC_A_MISSING only for the 7 Step 1 variants whose leaf DIFFERs (rs / rs_rd x3, rr_go), never for a gate object: all 38
  evidence/pk_gate headers read source_a=yes, and no .pvr.txt has a Traceback.
- Account B screen: no B claim line and no UNMATCHED status for any changed row; the pending/*acctB* hits are old (2026-11-11..13)
  authoring packages of these since-landed rows and comments inside B's 2026-11-24_acctB_b24 header overlays, not staged rows.
- Reading list: BRIEF, the standing brief, NF358's hand-back, COORDINATION.md and ASM_RULES.md were read in full; EXTRA_RULES.md was read in
  its relevant sections (top rules, inline-budget, include order, pick-any / size-pragma / relocation rulings 2026-11-20..24) and GOTCHAS.md
  was grepped (optimize / pragma entries), not read in full.
- One background `cl /Zs` launch (zs2) failed because the script did not source the lane env (cl.exe path); fixed in tools/zs_tests.sh and
  relaunched once (zs3, all PASS). One command contained a no-op `sleep 0`; no wait was made.
- Cleaned with clean.py: work/one, work/probe (the probe tree; its block is evidence/probe_block.diff), the vland work dirs and every
  oracle file (retail bytes). No byte dumps in any hand-back file.

## Tools (tools/)
pragscan362.py (TU optimize-state replay), mkvar362.py (variants / final row texts), run362.sh (Step 1 one-path runs), gate362.sh (H / N
gate compiles + both checkers), pkdiff362.py (H v variant copy summary), classify362.py (objsec line classifier), mkbatch362.py (run dirs),
zs_tests.sh / zs_srcs.sh (D1 `cl /Zs`), vlandchain362.sh; copies: nf.py / pass.py / pkrun352.py (NF358), objsec352.py / leafrel.py /
cmpcomdat.py / droptest.py (NF352), pkcmp346.py / helperscan356.py (NF356).
