# NF361: the /O2 emitters of the reopened Reset<CComponent> and GetDef copies (Account A naming queue, 2026-11-24)

Measured on repo HEAD (this session's working tree). No repo write, no git, no header change. All 27 NF361 locks (20 brief rows, owner 0x004291DE, 6 Part-2 rows) are released; a scan of
nflocks/*/owner finds 0 NF361 owners (evidence/locks_release.log). work/ (vland oracles included) and the scratch variant folders were
removed with clean.py; the variant texts are kept in evidence/variants/.

## Result
| Dir | Rows | Verdict |
|---|---|---|
| batches/b1 | 005fb7e0 005fe090 006001e0 00600b60 | ACCEPT: Reset<NUISystem::CComponent> 0x00429C6F and the ReleaseData 0x004291DE copy it calls, both made EQ with the EXISTING NF352 opt-ins |
| held/g1 | 005fc2d0 00868e50 | HOLD for ruling Q1: the GetDef copies are fixed only by deleting the row-local GetDef template |
| (none) | 15 other row/copy pairs | skipped with measurement (STATUS.tsv) |

## Part 1a: Reset (FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RELEASEDATA_SIZE_BODY + _RESET_SIZE_BODY)
Text: four lines (two comment lines, two defines) above the row's first `#include`, the NF352 form of 0x00605570 / 0x00626110
(var/R/). pragscan352 (preprocessed TU, stub loop added): every Reset row C_OK, spec 2/2, no optimize pragma before the block.

Three compiles per row at one path (tools/chain361.sh -> pass.py, each `timeout 900` on its own line): H, RESET alone (`r1`, NF352
Q2 isolation), RESET + RELEASEDATA (`N`).

**ACCEPT (b1): 005fb7e0 005fe090 006001e0 00600b60.** For each row:
- Grade: RELOCATION_MATCH in H, r1 and N (pass.py). vland `--recheck-landed` on the HEAD payload and on the new payload: 4/4
  RELOCATION_MATCH PASS both times (logs/recheck_{head,new}.log). The vland objects carry 103/53 B (H) and 59/44 B (N) copies, equal to
  the pass.py objects (evidence/lvsize.txt).
- objsec352 H v N: `VERDICT DIFF n=2 symdelta=0 leaf=same`; the two changed sections are the Reset and ReleaseData COMDATs. The r1
  run changes only the Reset COMDAT (n=1), which isolates the first opt-in (NF352 Q2).
- leafrel: leaf data-equal and relocation-equal.
- Owner comparison (cmpcomdat, bytes + relocation list): the Reset copy EQUALs the COMDAT in the owner object of 0x00429C6F, and the
  ReleaseData copy EQUALs the one in the 0x004291DE object, which I built myself. Both are pick-any sel 2, align 1, as in the owners.
  - The 0x00429C6F / 0x0042B312 / 0x0042B040 locks were held by NF362 all session (retried once at the end). The 0x00429C6F owner
    object is therefore NF362's HEAD build (agentNF362/work/obj/H_00429c6f.obj, copied read-only; evidence/owner_ref_PROVENANCE.txt).
  - Second reference, built by me: the landed nf352_r1 row 0x00605570's HEAD copies (EQ 59 / 44). All four rows' copies equal them
    (b1/evidence/owner_cmp_ref2.txt).
- Pick-any gate (pkcmp346, pk_all `--leaf --row-length` + pickany_vs_retail `--leaf`): GATE PASS. FIXED is Reset (and ReleaseData) in
  every row; 0 turned NEQ, 0 newly NEQ. The reversed control FAILs.
- Reach flips: 0 onto a NEQ copy. ReleaseData goes reach n -> y, and it lands on the EQ copy.
- Drop test: symdelta 0; nothing dropped or added.
- Leaf-called NEQ copies left as HEAD debt:
  - 005fb7e0: its own strong local GetDef<CUIMiscThingsDef> body (51 B), out of scope per the brief.
  - 006001e0 / 00600b60: GetDef<CUIMiscThingsDef>, 116 v 98 (Part 1b).

**Skipped (measured):**
- **The leaf moves.** The leaf expands Reset / ReleaseData inline at HEAD, and the noinline specializations turn those expansions into
  calls. Measured with tools/leafcalls361.py (evidence/leafcalls.txt), as leaf relocations to Reset / ReleaseData, H v RESET-alone v
  both opt-ins:
  - 005ff190: 7/0 v 9/0 v 9/3
  - 00603000: 3/0 v 13/0 v 13/2
  - 00607330: 0/0 v 1/0 v 1/33
  - 0061b610: 0/0 v 3/0 v 3/17
  - 0061e6d0: 0/0 v 3/0 v 3/14

  Grades:

  | Row | Both opt-ins (R) | RESET alone (r1) |
  |---|---|---|
  | 005ff190 | DIFFER(3896v4176) | DIFFER(4000v4176) |
  | 00603000 | DIFFER(8520v9571) | DIFFER(8645v9571) |
  | 00607330 | DIFFER(7804v9124) | DIFFER(9036v9124) |
  | 0061b610 | DIFFER(11311v12341) | DIFFER(12063v12341) |
  | 0061e6d0 | DIFFER(11476v12356) | DIFFER(12100v12356) |

- **006096e0, 0060c900, 0060eeb0:**
  - R moves the leaf: DIFFER(11828v12832), DIFFER(8826v9635), DIFFER(9660v10254). The leaf's ReleaseData relocations go 0 -> 25 / 18 / 15
    (evidence/leafcalls.txt).
  - RESET alone keeps RELOCATION_MATCH and makes Reset EQ. But the already-NEQ ReleaseData copy's reach flips n -> y, which is the NF352
    hold (evidence/gate/r1/reachflip.txt).
- **00622a00:**
  - R grades RELOCATION_MATCH, and its Reset / ReleaseData / ~CCountedPointer copies turn EQ.
  - But this TU carries a pick-any copy of CTCInventoryQuests::ConstructQuestList (0x0061B610), from a copy of that row's text (its own
    comment, L27; not an #include). That copy is leaf-called and turns EQ -> NEQ
    (12341 -> 11311 B, the same leaf move as 0061b610's own row) under both checkers: gate FAIL (evidence/gate/N_all5/).
  - RESET alone gives the same TURNED_NEQ.
- No form is left inside the brief for these nine rows. Any fix needs another device (an inline-visible size body, or the ctor / dtor
  forms NF352 already found to move leaves).

## Part 1b: GetDef<CUIMiscThingsDef> 0x0042B312 / GetDef<CThingObjectDef> 0x0042B040
- **Finding.** All 16 GetDef copies in scope come from a ROW-LOCAL primary template
  `template<class T> bool CDefinitionManager::GetDef(long, CDefPointer<const T>*) const` (tools/gdclass361.py; evidence/gdclass.txt).
  None of them comes from GETDEF_GLOBAL_BODY. NF358's "shared-body" label for these 116 B copies is wrong for these rows. Only 006096e0
  defines GLOBAL_BODY, and its GetDef<CThingObjectDef> is already EQ (NF349).
- **No define-only form exists.** The NF349 per-type blocks sit inside `#ifdef FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY` and
  `#error` without REREAD. Defining the three macros with the local template left in fails.
  - Control (var/C/005fc2d0.cpp): clang reports "redefinition of 'GetDef'", and VC7.1 `cl /Zs` gives C2995 "template function has
    already been defined" (evidence/pre/pre_zs.log, ZC_005fc2d0).
  - Per the brief ("existing opt-ins only, no header block"), every GetDef row is skipped with that measurement.
- **Swap probe (held/g1; ruling Q1).** The probe deletes the local template and adds GLOBAL_BODY + REREAD + the type's SIZE_BODY. It was
  run on 005fc2d0 (simple case) and 00868e50 (/Gr; three GetDef types).
  - Grade: RELOCATION_MATCH before and after, both in pass.py and in vland `--recheck-landed` H / N.
  - Leaf: leafrel equal.
  - Target copy: 116 -> 98 B, EQ under both checkers, and byte+relocation EQUAL to the owner object's COMDAT (NF362's HEAD objects of
    0x0042B312 / 0x0042B040).
  - Gate: PASS, reversed control FAIL, 0 reach flips. pragscan C_OK.
  - 00868e50 also turns GetDef<CArmourDef> 0x00817B40 from 116 NEQ to 108 EQ.
  - **Not target-only.** REREAD is TU-wide, and the header body differs from the local one:
    - each SetDynamicCast<T> helper goes 50 -> 52 B;
    - the Assign<T> helpers and IsNull<CDefClassBase> are dropped;
    - `?Get@?$CDefPointer@$$CBVCDefClassBase@@@@QBEPBVCDefClassBase@@XZ` (3 B) is added.
  - All of these are NOADDR, unreached, and have no reloc_bindings line, so the drop test is clean. This is the NF349 pattern ("Dropped
    Assign helpers").
  - The brief gate "objsec equal except the fixed copies" is therefore not met literally, and the change deletes row text. Hence held.
  - Probe text left as is: in 005fc2d0 the now-empty `#pragma auto_inline(off)/(on)` pair and the comment above it are untouched.
  - Not extended to the other rows (advisor: only if both probes pass every brief gate).
  - Candidates if Q1 is yes:
    - /O2 rows with the template under auto_inline(off): 0047a9f0 005fc8a0 005fcbe0 005fddd0 006001e0 00600b60 0060c900 0060eeb0
      0061b610 0061e6d0 00622a00.
    - Likely to move the leaf, so measure them separately: 005ff190 and 00605570 (template not under auto_inline(off); 00605570's
      comment says retail expands GetDef in its leaf), and 00607330 (its comment says the leaf needs the `!def.IsNull()` spelling).

## Part 2: acctB_b24B6 0x0066C340 GetDef<CMaterialDef> (read-only census, NF358 form): STAYS CLOSED
- **Retail leg** (tools/p2callers361.py; evidence/p2/callers.tsv). 11 E8 sites, 0 E9, 0 abs32:
  - 0066b370 x1, 00703150 x1, 007f0300 x1, 0082d5c0 x1, 00839940 x3, 008f7950 x1.
  - Three sites are in unlanded, uncatalogued functions:
    - 0x00734E0D and 0x00734FC9 in CTCCombat::SetStruck 0x00734C60 (named in landed comments of 0x00734880 / 0x00868A40);
    - 0x007A57BF in a function that starts near 0x007A56B0, right after CTCForcePushPower::PlayCastSound. No name was found.
- **Source leg** (tools/p2srcgrep361.py). Folded includes are followed. It matches `GetDef<[const ]CMaterialDef`, or
  `CDefPointer<const CMaterialDef>` together with a `GetDef(` call (for T deduced), or GLOBAL_BODY together with CMaterialDef. It finds
  the same six rows plus the owner.
  - A wider grep (also `CDefPointer<CMaterialDef const>`) adds only 0x0063F3A0, a comment mention in the GetDef<CEngineThemeDef> row.
  - No shared header has an inline GetDef<CMaterialDef> user.
- **HEAD compiles.** Test precheck before the lock (2 tests passed by `cl /Zs`), source precheck after it, one-path pass.py (`timeout
  900` each).
  - All five non-B emitters are RELOCATION_MATCH. Each defines the copy pick-any (sel 2). The copy is EQ 108/108 under pk_all and under
    pickany_vs_retail, and it is leaf-referenced (evidence/p2/census_H.tsv, evidence/p2/pk/).
  - The owner 0x0066C340 grades RELOCATION_MATCH.
- **00839940** (NOT compiled by NF361) is Account B's row (the newest COORDINATION_CLAIMS line is B `claimed`), so it was not compiled (stay-off). Its copy is
  EQ by that landing's own checker log: batches/2026-11-24_landing_acctB_b24e/logs/00839940.pk_all.log line 197, `EQ EQ 0066c340 a,b pickany
  108 leaf=y reach=y`. The README says the same.
- **Object, closure and fold legs** (tools/p2legs361.py over all 27 of my HEAD objects (20 Part-1 rows, the 0x004291DE owner, the 6 Part-2 rows)):
  - Only the six P2 objects (five emitters plus the owner) define the symbol. None of the 20 Part-1 objects defines or references it.
  - The only intermediate in each emitter is its own leaf.
  - No landed row folds an emitter's source.
- **Emitter list:** 0066b370 00703150 007f0300 0082d5c0 008f7950 (EQ, measured), 00839940 (EQ, B's landing record).
- **Verdict:** STAYS CLOSED. The nf347 closure holds; it lacked a census, and this is it.

## Extra checks before the hand-back
- **Reverse includes** (evidence/reverse_includes.txt). The six changed sources are named by no other source, test or header, apart
  from two comment mentions of 00868e50's file in the 0x007097E0 / 0x007097F0 rows. No includer needs gating.
- **Every NEQ copy left in the new b1 objects** (b1/evidence/remaining_neq.txt). Each is identical to HEAD (objsec: only the two target
  COMDATs change).
  - 005fb7e0: GetDef<CUIMiscThingsDef> strong 51/98, leaf-called; ~CCountedPointer<CComponent> 0x00429C53 53/5, unreached;
    DeleteData 0x00428B10 8/11, reach y at HEAD already.
  - 005fe090: ~CCountedPointer 0x00429C53 53/5 and the (T*) ctor 0x00429C15 78/62, both unreached.
  - 006001e0 / 00600b60:
    - leaf-called: GetDef<CUIMiscThingsDef> 116/98, GetAllLeafThings<CThingBuilding> 0x00617A90 135/144, map<ulong,CUIState>::operator[]
      0x0042D5B1 137/83;
    - unreached: IsIncludedInThingScans 0x0048BC40, ~CCountedPointer 0x00429C53, pair ctor 0x0042AA96, map::insert 0x0042D4D3,
      _Vector_base<long> ctor 0x005D82F3.
- **Stay-off, precise** (evidence/stayoff_changed_rows.txt). No changed row has an UNMATCHED status, an open B claim, a Codex
  reservation or a T3 line.
  - 005fe090 and 00600b60 have stale 2026-11-11 Account B authoring packages: pending/2026-11-11_acctB_c2029, _c2025.
  - Each package's row text equals the landed source except the final newline, so each is the row's original authoring package and
    long since landed.
  - Same situation as the NF357 Q5 B pair (stale packages, no open claim).

## Rulings needed
- **Q1 (GetDef swap).** May a row drop its row-local GetDef template and take GLOBAL_BODY + REREAD + the NF349 SIZE_BODY (held/g1)?
  - The target copies become owner-equal.
  - The NOADDR helper COMDATs change (SetDynamicCast 50->52; Assign / IsNull dropped; Get<CDefClassBase> added), and the drop test is
    clean.
  - If yes: extend to the 11 auto_inline(off) rows listed above, and measure 005ff190 / 00605570 / 00607330 separately.
  - If no: the GetDef lines stay open with these emitters, and they need another device.
- **Q3 (census reach).** May an A census agent compile a B-claimed, B-landed row read-only (no lock on B's side, no change)? Here
  0x00839940 was left uncompiled, and its EQ verdict rests on B's landing log.
- **Q4 (stale B packages).** b1 rows 005fe090 / 00600b60 have only the 2026-11-11 authoring packages (the NF357 Q5 situation). OK to
  land?
- **Q2 (Reset rows whose leaf moves).** Nine rows cannot take the NF352 noinline specializations, because their leaves expand Reset /
  ReleaseData inline (005ff190 00603000 00607330 0061b610 0061e6d0 006096e0 0060c900 0060eeb0), or because a folded copy turns NEQ
  (00622a00).
  - Queue them as debt on T3 nf358 0x00429C6F (and 0x004291DE for the reach-flip three)?

## T3 suggestions
- nf358 0x00429C6F: remove 005fb7e0 005fe090 006001e0 00600b60 from the 103 B list (b1).
  - Note: "NF361: 103 B leaf-move rows 005ff190 00603000 006096e0 0060c900 0060eeb0; unreached 00607330 0061b610 0061e6d0 00622a00
    (00622a00's fix turns its folded ConstructQuestList 0x0061B610 copy NEQ)".
- nf358 0x0042B312 / 0x0042B040: note "NF361: the 116 B copies come from row-local templates, not GETDEF_GLOBAL_BODY. Define-only
  opt-in impossible (C2995). The swap is held (ruling Q1, held/g1)".
- acctB_b24B6 0x0066C340: closure confirmed by census (NF361 evidence/p2).

## Process notes
- Every grade ran through chain361.sh: one pass.py per variant, each with its own `timeout 900`, in background lanes. The checks were
  `kill -0` / log reads; there were no sleeps.
- Prechecks:
  - Tests were prechecked before their locks: 12 passed clang (8 Part 1 incl. 4 owner tests, 4 Part 2); the other 18 (16 Part 1 + 2 Part 2) failed clang only on VC7.1 headers /
    `functional`, on static-assert typedef redefinitions, or on "explicit specialization after instantiation" in the unchanged test, and
    passed `cl /Zs` (D1).
  - Every compiled source (HEAD and variants) passed clang after the lock. The control variant failed by design, was not graded, and
    was used only as the measurement.
- The owner locks 0x00429C6F / 0x0042B312 / 0x0042B040 were NF362's all session. I never claimed them and never compiled them.
- Tools (tools/):
  - Retargeted copies: NF357 nf.py / pass.py / objsec352 / leafrel / pkrun352 / pkcmp346 / cmpcomdat / comdatsel /
    reachflip_coordinator; pragscan352 / pragscan349, with a stub-header loop added so cpp does not stop on `<new>`.
  - New: scanrows361.py, gdclass361.py, mkvar361.py, chain361.sh, pre_tests361(_p2).sh, pre_srcs361.sh, zs_tests.sh, eval361.sh,
    gate361.sh, vland361.sh, mkbatch361.py (no-header run dir), p2callers361.py, p2fnstart361.py, p2srcgrep361.py, p2legs361.py.
  - The p2_* files are NF358 copies, used for reference only.
