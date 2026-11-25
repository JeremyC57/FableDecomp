# NF419 hand-back (2026-11-25, HEAD c3bec33437): the 00/5b-00/5c y-off audit and the C3DAnimFile / C3DMeshFile ??_G port

| batch | rows | verdict | one line |
|---|---|---|---|
| b1 | 44 | ACCEPT | y-off dropped; object section-identical without it (a0); + header hdr/engine/CAnimInit.h (byte-identical in all users) |
| b2, b3, b4 | 40 / 40 / 40 | ACCEPT | y-off dropped; leaf identical; every changed copy is EQ, a landed twin (tier 1) or gains a retail home (a1) |
| b5, b6 | 29 / 29 | ACCEPT | as a1, but each row also has 1-6 changed copies with no evidence either way (NEQ / unbound at HEAD and still) (a2) |
| b7 | 0x00A98F40, 0x00A9A730, 0x00A9AE60, 0x00A9D2A0 | ACCEPT (r2) | two Option S units; real ??_GC3DAnimFile / ??_GC3DMeshFile calling ??1CChunkedFile; private ctor respells |

Files at the root: AUDIT_5b5c.tsv (one line per audited row), OUTSIDE_YOFF.tsv (the 1,628 rows outside 00/5b-00/5c), T3.txt,
NAME_FIXES.tsv, RULINGS_NEEDED.tsv (r1 tests gate for b1-b6, r2 census pk for b7). Headers: hdr/engine/CAnimInit.h (b1),
hdr/engine/C3DAnimFile.h and hdr/engine/C3DMeshFile.h (b7), originals in orig/hdr_<Name>.h. src/compiled/ holds copies of the two b7
owners only so that the thin rows' `#include "../src/compiled/..."` resolves through FABLE_HDR_OVERLAY=hdr while grading: a grading
device, not a hand-back file. Helpers in tools/ (listed at the end).

## Task 1: T3 nf416 0x005B0000-0x005CFFFF audit
- **Population.** 222 catalog rows in rebuild/src/compiled/00/5b and 00/5c carry a line-anchored `#pragma optimize("y", off)` (both
  spellings). T3 said 221; the difference is not reconciled (NF416's count method is not recorded). No row is included by another row.
  0x005C2C65 has prose in its CompilerFlags (T3 catalog line); the lane tools take only its `/` tokens.
- **Method.** Per row, at ONE compile path (tools/meas.py): H = landed text, N = every y-off line removed, R = landed + row-end
  `#pragma optimize("", on)`. Compared by function symbol (tools/fdiff.py, classify.py), pk_all + pickany_vs_retail on H and N with the
  exact graded leaf (tools/pkrun.sh, pkflip2.py keyed on symbol + address), each changed copy also against (i) a reloc-masked retail scan
  for a function-boundary home and (ii) the same-named COMDAT in HEAD objects of the other 532 00/5b-00/5c rows (tools/tier1.py).
- **Result: every row is (a).**
  - The leaf (graded symbol) is byte- and relocation-identical H v N in all 222 rows (evidence/leafsym.tsv).
  - Every retail leaf starts with `push ebp ; mov ebp,esp`: the size mode that every row also sets keeps the frame, so y-off never
    contributed to a leaf.
  - What y-off did change is end-of-TU template instances and compiler-generated members. They take the TU's final optimize state,
    and y-off framed them (3 bytes longer: push/mov/pop ebp).
  - 44 objects are section-identical (a0, b1).
  - In the other 178 objects, 954 copies change:
    - 136 copy verdicts go NEQ/NOADDR -> EQ;
    - 528 unbound copies gain a retail home at a function boundary (no framed body had one);
    - 166 stay NEQ, 124 stay unbound with no home.
  - Of those last 290, 164 equal a landed twin (12 of them also have a framed twin), and none equals only a framed twin. The rest
    (no landed definer, or twins matching neither) are listed per row (a2, 58 rows, b5/b6).
  - pk / pvr HEAD v new: 0 REGRESS, 0 NEW_NEQ, 0 REACHFLIP, 0 new pvr NEQ. pk gives IMPROVED 63 rows / PVR_IMPROVED 62 rows.
  - CHANGED (6 lines) are NOADDR copies whose reach went y -> n. GONE/NEW_EQ (2) are NOADDR -> EQ at an address.
- **Retail's own framed copies.** These TUs' bound GetDef / GetSubDef / erase homes ARE framed (0x0042AF3C, 0x005BADE1,
  0x005BB102). They are size-mode bodies, and our copies of them are different (shorter) stubs either way, NEQ HEAD and new. So
  "retail is frameless" is not claimed: the row comment says only that the pragma framed end-of-TU copies, each checked here.
- **R-NF409-2.** Not needed (no (b) row). Measured: the row-end reset moves the leaf in 52 of 222 rows (cause not established; template
  leaves are among them), so it is not a general device.
- **Row text.** The y-off line is replaced by a dated two-line NF419 comment. For a0 rows it says "object byte-identical"; for the
  others, "leaf identical, only copies framed". Comment spellings of the pragma pair become `optimize("s", on)`: single-line spellings by regex,
  then the 28 remaining lines (multi-line spellings) by a second, reviewed pass. Every remaining `"y"` mention was reviewed. No code changes. The vland new objects' functions are
  identical to the measured N objects (objsec.txt `measured_variant=SAME`).
- **Header opt-ins citing these rows.** Only one block cites an audited row as its size-pragma owner:
  - FABLE_RETAIL_CANIMINIT_CTOR_SIZE_BODY (engine/CAnimInit.h, owner 0x005B33F4, b1).
  - Dropping the block's own y-off changes no COMDAT in any of its three users (0x007502D0, 0x00753630, 0x00666310) or the owner
    (evidence/hdr_CAnimInit.txt).
  - So condition 2 stays literal: the owner and the block both set ("s", on). This ships as hdr/engine/CAnimInit.h with b1. b1 lands
    without it too (byte-identical either way).
  - FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RESET_SIZE_BODY_YOFF_TU hard-restores y-off, but no audited row uses it. 0x005CDD86 uses
    the NF352 sibling with its include before the row pragmas, and its Reset copy stays EQ 0x00429C6F.
- **Gates (each batch, CHECKS.tsv).**
  - vland --recheck-landed HEAD and new at one path: 222 rows, grades unchanged, tests PASS, warning sets identical, 0 LITERAL_DIFF.
  - objsec: only .text copy COMDATs differ, the leaf never, no section added or removed.
  - /FORCE census link (the 222 rows + the 532 other 00/5b-00/5c HEAD objects): duplicates 49 -> 49 (same lines), unresolved
    668 -> 668 (same set).
  - apply_table --dry: 0 misses. reserved_check: 0.
- **Tests (RULINGS_NEEDED r1).**
  - The tests are the HEAD tests, unchanged.
  - 217 of the 222 are self-contained models: they link with no row object (work/selfcont.log), so no mutation of the row can reach
    them.
  - The 5 that link the row: 4 FAIL on a mechanical mutation (tools/mutate.py). 0x005B1FF4 has no in-row site: it is an explicit
    instantiation of a header template.
  - Booked as T3 (nf419 tests).

## Task 2: T3 nf416 0x00A9A730, 0x00A9D2A0 port
- **Names (two sources each).**
  - NF416's line had the FableWin publics swapped: FableWin has ??_EC3DAnimFile at 0x03204C70 and ??_GC3DMeshFile at 0x033D54E0.
    Ego_r folds both bodies at 0x00C0E0D0 under ??_GC3DMeshFile.
  - Both PDBs name the proc `X::`scalar deleting destructor'` for each class. The body tests bit 0 only, and vftable 0x0129E034 /
    0x0129E0D8 slot 0 is the row. So the names are ??_GC3DAnimFile@@UAEPAXI@Z and ??_GC3DMeshFile@@UAEPAXI@Z.
  - The root-chunk constructors are PRIVATE in both PDBs (AAE). The landed QAE spellings are stand-ins (NAME_FIXES).
- **Destructors.**
  - Retail has no ~C3DAnimFile / ~C3DMeshFile body. A raw E8/E9 scan finds only 0x00A9A733 / 0x00A9D2A3 reaching 0x00AA7DF0.
  - VC7.1 emits the retail 30-byte ??_G (call ~CChunkedFile, no vtable store) only for an implicit destructor. A user-written empty
    one stores the vtable: 36 B (probe/p1, p2). So: no destructor rows.
  - C3DMeshFile.h's destructor is already implicit. C3DAnimFile.h gets the add-only opt-in FABLE_RETAIL_C3DANIMFILE_IMPLICIT_DESTRUCTOR.
- **Vtable emission.** No scaffold emits ??_7 / ??_G without a constructor body:
  - explicit qualified destructor calls and an out-of-class inline definition fail (probe/p1-p4);
  - the implicit copy constructor works (p5-p7), but adds a homeless 25-byte COMDAT and an unresolved ??0CChunkedFile copy ctor.
  So each ??_G joins its retail TU-mate constructor as an Option S unit:
  - lib_3d_anim_file.obj: owner 0x00A98F40, thin row 0x00A9A730;
  - lib_3d_mesh_file.obj: owner 0x00A9AE60, thin row 0x00A9D2A0.
  Both PDBs list both members of each unit. Each owner is the lowest member address.
  - Both owners match on the real headers at once: RELOCATION_MATCH, ctor bytes and relocations identical to HEAD.
  - C3DMeshFile.h gets the add-only FABLE_RETAIL_C3DMESHFILE_ROOT_CTOR (the private ctor declaration).
  - The ??_G copies are EQ at 0x00A9A730 / 0x00A9D2A0 and call ??1CChunkedFile@@UAE@XZ (0x00AA7DF0, NF416 b3).
- **Gates (b7/CHECKS.tsv).**
  - vland HEAD/new RELOCATION_MATCH PASS x4 (new also strict leaf).
  - Real tests that link the row object through /alternatename stubs; 4 mutants FAIL.
  - objsec: the rename, the vtable slot and the added ??_G only.
  - pk: ??_G NEW_EQ. The NOADDR CCountedPointer copies are tier 1 (identical to HEAD 0x00A98F40 / 0x00AA7DB0).
  - s_unit_check OK.
  - /FORCE census link (209 landed objects naming C3DAnimFile / C3DMeshFile / CChunkedFile, the 0x00AA7DB0 unit once): duplicates
    11 -> 11, 4 newly resolved (the stand-in callees and ?v00 slots), 0 newly unresolved. With the 0x00AA7DB0 unit alone:
    duplicates 0 HEAD and new, ??0 / ??1CChunkedFile resolved (evidence/link_t2unit.txt). ??_7 slot 0 = ??_E (-> ??_G) as retail's
    one-slot vftables (evidence/vftables.txt). No row includes either owner file.
  - Headers proven add-only (macro undefined = HEAD text; no user).
- **Exposed debt (RULINGS_NEEDED r2).**
  - With the selves named ??_G, census pk now checks the pick-any ??_G copies that six landed rows already emit: the default ctors
    0x00A98E60 / 0x00A9AD80 and the four loaders.
  - Those copies call ??1C3DxFile, not ~CChunkedFile. In the loaders pk infers NEQ for their 5-byte ??1C3DxFile thunk at
    0x00AA7DF0 (reach n).
  - The copies and their callee are unchanged from HEAD. T3 lines for porting those rows.

## Locks
- Kept NF419 (all ACCEPT): the 222 audited rows and 00a98f40, 00a9a730, 00a9ae60, 00a9d2a0.
- Released: 005b211f. It was taken by mistake during a first probe and is unchanged.
- Read-only HEAD compiles of other rows (census, tier-1 twins, link) took no locks.

## Deviations / notes
- Nothing was written to the repo, git was not run, and wineserver was never killed.
- After the container restart, a census run stalled behind a defunct cl.exe on the first wine start (cause not established). I
  killed only my own python PIDs and reran. Nothing half-written was kept: every census object has its log line.
- A first /FORCE link of 754 objects failed silently (command line too long) and reported 0/0. link.py now uses a response file and
  flags a silent failure.
- A second runpair with the same tag nested its work dir into the first. The t2 pair was rerun clean; vland_t2n_strict.log is from
  the strict first run.
- Tools (tools/):
  - Measurement: common.py, meas.py, fdiff.py, classify.py, neqclose.py (scored closeness; not used for verdicts), tier1.py,
    pkrun.sh, pkflip2.py.
  - Probes: pc.py, hdrprobe.py.
  - Tests and gates: mutate.py, mut2.py, selfcont.py, census.py, link.py, objsec.py, vcmp.py, runpair.sh.
  - Packaging: mkrows.py, pack.py.
  - Copied from NF416: rdis.py, immscan.py, rawref.py, pkflip.py, pe.py.
