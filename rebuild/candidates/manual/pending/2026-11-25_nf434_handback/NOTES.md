# NF434 hand-back (2026-11-25): NScript script debt rows, the base slots first

No repo write, no git. Lane: VC7.1 RTM under Wine (`$S/wine/env.sh`). HEAD and new ran at one LANDVERIFY_WORK path per batch
(`tools/runpair432.sh`; HEAD root `work/rsroot`, new root `work/rs<b>` = repo symlinks + the batch's `hdr/engine` overlay).

## Re-measure on HEAD (task 1 step 1)
All 102 NF430 debt rows: 102 RELOCATION_MATCH PASS; vt432: agree 371, WRONG 463, unknown 245 (= NF432's after-g1 179+146+138).
WRONG by emitted symbol: 144 `?OnPersist@CScriptBase@@UAEXAAVCPersistContext@@@Z` (retail 0x00CBD4E0, self still the gapscan stand-in
`?Hook@T@@QAEXH@Z`), 98 RegisterMain / 98 Main / 61 Init (override gaps), the rest CScriptGameResourceObjectScriptedThingBase / CScriptThing
slots of 4 ctor rows. 78 rows also emit `??_7?$CLevelScriptBase<T>`, whose slot 4 is the same stand-in. FableWin `??_7<C>` for the 98 classes:
three override shapes, all public (UAE): RegisterMain+Main (36), +Init (27), +Init+OnPersist (35). Destructor: FableWin marks it
compiler-generated in every class checked; retail's `??_G<C>` sits after Alloc and calls ~CScriptBase 0x00CBD510 directly.

## Batch g1o (ACCEPT, 7 rows): the base default slots
- 0x00CBD4E0 `CScriptBase::OnPersist` (NF432 hold_g1o) and 0x00CDEBB0 / C0 / D0 / E0 `CEntityScriptBase::Init / OnPersist / OnPredicateFail /
  OnInterrupted`, respelt from the shared gapscan stand-ins `?Hook@T@@QAEXXZ` / `?Hook@T@@QAEXH@Z`. Sources: retail vftable slots (0x012C1648
  slot 4; 0x012C3224 slots 2/4/5/6, slots 1/3 `__purecall`) + FableWin.pdb `??_7CScriptBase@@6B@` / `??_7CEntityScriptBase@@6B@` same slots;
  access / order as the shared headers declare them; bodies `ret` / `ret 4`.
- The clash NF432 held on: compiled census of every row with a local CScriptBase / CEntityScriptBase carrying in-class default bodies
  (10 rows): only 0x00DC3EA0 and 0x00DCC000 emit any of the five names. Both are ported in this batch:
  - 0x00DCC000 `??_GCQ_OpeningGraveyardSecretPassageScript` and 0x00DC3EA0 `??_GCSecretPassageSkeleton@...` move onto
    engine/CQ_OpeningGraveyardSecretPassageScript.h (+ add-only opt-in `FABLE_RETAIL_CQ_OPENINGGRAVEYARDSECRETPASSAGESCRIPT_CTORS`: the two PDB
    ctor declarations, FableWin publics), FABLE_RETAIL_CLEVELSCRIPTBASE / FABLE_RETAIL_CCHARSTRING; the ctor is defined `inline` in the row and
    expanded by an anonymous-namespace scaffolding helper (0x00BB1740 pattern). Leaf sections byte+reloc equal to HEAD. The two existing size asserts are pinned (FABLE_STATIC_ASSERT_AT 230 / 231): unpinned, the inserted lines renamed their typedefs in the header's 15 other includers and equiv_check flagged them.
  - 0x00DCC000's ctor copy is byte+reloc EQ at retail 0x00DC2570, whose landed self is a stand-in spelling (NAME_FIXES.tsv).
  - 0x00DC3EA0 adds ??0 / ??_7 / ??_G CEntityScriptBase pick-any copies: OBJECT_EQ with landed 0x00DC3EE0's (R10); ??_G also EQ at 0x00CDEBF0.
- Gates (batches/g1o/CHECKS.tsv, evidence/): vland 7/7 grade kept, tests PASS, 0 LITERAL, warn kinds H {STANDIN, SHARED_HEADER_AVAILABLE} ->
  N {}; name_gate --test 7/7 PASS; vt432 HEAD 11 WRONG -> 0 (with the batch's own selves); fwchk 4/4 vftables = FableWin slot names; objsec
  5 ALL_EQ + 2 port NOT_EQ by design (leaf equal); pk 0 new NEQ, 0 reach flips; /FORCE link with 13 definers dups 6 -> 1 (the 1 left
  pre-existing, T3 0x00DC3DF0), LOST 0; apply_table n/a (no table change); reserved 0; hdrcheck SAME (add-only, 8 lines); mutation: 0x00DC3EA0
  layout mutant FAIL, 0x00DCC000 dtor-chain mutant FAIL (link), the five respells NF419 r1.
- Ruling asked (RULINGS_NEEDED R-NF434-1): 0x00DC3EA0 still emits an unreferenced ??0CSecretPassageSkeleton pick-any copy that differs from the landed strong row 0x00DC3DF0 (HEAD emitted it strong and also NEQ). g1o is ACCEPT if granted; otherwise the whole unit holds.
- Mutants ran with tools/tl.sh outside vland (evidence/mutant_*.txt + the mutant sources).
- Resolves: the nf432 held-rows line's 0x00CBD4E0 part, and CEntityScriptBase's base slots for task 2.

## Batch s48a (ACCEPT, 29 rows; lands AFTER g1o): the 0x48 Alloc shape with no header
- One generator (gen/gen434.py): new header engine/<C>.h per class (Ego_r.pdb size - 0x14 = 0x48, base CLevelScriptBase<T> with T from
  FableWin.pdb's LF_BCLASS (evidence/fw_base_dtor.tsv, which also confirms the compiler-generated destructor for all 29; 10 HEAD models
  used the wrong T, e.g. CTestNewNav1Script is CLevelScriptBase<CEmptyData>), inline user ctor, static Alloc, the FableWin overrides in ??_7 slot order, compiler-generated dtor,
  size assert); row = FABLE_RETAIL_NSCRIPT + FABLE_RETAIL_CLEVELSCRIPTBASE + include + Alloc; linked test (CScriptBase ctor / dtor via
  /alternatename stubs, operator new / delete recorded; checks size, Game / MasterScriptData, own overrides through the vptr, delete -> ~CScriptBase).
- Gates: vland 29/29 RELOCATION_MATCH kept, tests PASS, 0 LITERAL, no warn kinds; name_gate --test 29/29 PASS; fwchk 58/58 OK; vt432 plain
  58 WRONG (all slot 4 = the 0x00CBD4E0 stand-in g1o fixes) -> with g1o selves 0 WRONG (HEAD 121 WRONG); objsec leaf equal, NOT_EQ by design;
  every ??_G<C> / ??_G?$CLevelScriptBase<T> byte+reloc EQ at its retail slot 0 (58/58, slot0.tsv), ??0<C> / ??0?$CLevelScriptBase<T>
  unique EQ homes (homes.tsv); pk 0 new reached NEQ, 0 reach flips (the added unreached 5-byte ??1?$CLevelScriptBase<T> copy reads NEQ v the
  table's 0x00CBD510 binding: OBJECT_EQ with landed 0x00DCC000's copy (R10), EQ at the retail thunk set 0x00CDD100.. (T3)); /FORCE link with 68
  definers dups 58 -> 58, identical set, LOST 0; reserved 0; apply_table n/a; hdrcheck 29 NEW; mutation 29/29 FAIL. (Final rerun after the T fix: tools/gates434.sh, same results; name_gate rerun with an absolute table path: 29/29 PASS.)
- HEAD's ??_G<C> called an undefined ??1<C> (the local `virtual ~C();`); the new copies are the retail bodies.

## Slot names v landed rows
self_symbols.tsv lacks ~28,400 landed addresses (e.g. 0x00CDD5F0, 0x00CDDBA0), so plain vt432 reads their slots 'unknown'. Rechecked with
every landed row's `// Leaf:` where self_symbols has no line (work/leafmap_missing.tsv + g1o selves; evidence/vt_leafmap.txt): 0 WRONG in both
batches. Undefined references (llvm-nm U) not defined by any landed row or self: g1o 3 -> 2, s48a 33 -> 2 (HEAD's 32 never-defined ??1<C>
from the local `virtual ~C();` are gone; left: ??1CScriptBase (table-bound, not a row) and one unlanded override Main).

## equiv_check (copy v copy, g1o + s48a headers)
57,160 rows checked, 0 differ (after pinning the two asserts; the first run flagged 15 includers on __LINE__ typedef names). Planted control fires on both includers chosen. batches/*/evidence/equiv.txt.

## Not done (T3.txt): 73 debt rows by shape; task 2 rows 0x00EB2000 / 0x00DEF6A0 (base slots now fixed), CSP_UndeadTrigger; 0x0043B570.
## Not checked: naming_status.py tracker HEAD v new.
## Locks: ACCEPT rows locked NF434 (g1o 7 + s48a 29). No HOLD rows; nothing else locked.
