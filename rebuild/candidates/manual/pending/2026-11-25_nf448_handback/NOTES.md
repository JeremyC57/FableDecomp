# NF448 hand-back (2026-11-25): NF442 follow-ups

Repo state: branch FullDecomp. The work was done at 31be8b598d and re-verified at d3e3f622d3 (NF449 landed 76 rows meanwhile; no header,
no T3 change and no overlap with my rows). After NF449: the 76 changed rows were recompiled; the stale check, all dry runs, the censuses
and the drift checks were rerun; nothing changed in the deliverables. NF442 landed at acba5c708d; NF446 u4a/u4b (253dc2e46b) and a docs commit followed while I
worked. u4 changed two row sources (0x006D6570, 0x00899C90), reloc_bindings.tsv, self_symbols.tsv and T3_PORT_QUEUE.tsv. No header
changed. Every census, dry run and table check below was redone against the 31be8b598d table.

Disclosure: my first repo command ran `git log --oneline -3` and `git branch --show-current` (read-only). After that I ran no git.
HEAD was read from .git/refs and .git/logs.

## 1. STALE_TABLE_CHECKED.tsv (advisor condition): 50 of 52 lines verified, 2 dropped
- **Method** (`tools/refscan448.py`, `tools/symcache448.py`, `tools/stale_text448.py`, `tools/stale_final448.py`; symbol caches in
  `work/symcache_*.tsv`):
  - **Witness:** take the CURRENT table witness for each (symbol, target). All 52 still have exactly NF442's witness.
  - **Candidates:**
    - the witness;
    - every landed row whose retail bytes reference the target (rel32 E8/E9/0F8x or abs32): 1,198 rows;
    - every landed row whose source names all identifiers of the spelling;
    - the target row itself.
  - **The three censuses over 400 rows** were narrowed to the rows that can emit the spelling:
    - a local one-parameter `std::vector` model: 134 of 2,944;
    - a local three-parameter `std::map` model: 7 of 2,544;
    - all 421 CLandscapeBackgroundPatch rows.
  - **Compile:** each candidate is compiled with VC7.1 from its current repo source with its catalog flags, and the object's
    external symbols are read. 2,600+ landed objects in all.
  - **Keep rule:** a line is kept only when no compiled object has UND or DEF of the spelling.
- Narrowing check: no header under rebuild/include models a one-parameter std::vector or three-parameter std::map.
- **Dropped** (`evidence/STALE_TABLE_DROPPED.tsv`):
  - `??4?$CCountedPointer@UCScriptThingImp@@@@QAEAAV0@ABV0@@Z` / 00704580. It is UND/DEF in 13 landed objects (00d823a0 00da4e30
    00dd1af0 00def3b0 00e12f20 00e1cd40 00e2a900 00e3e720 00eb75f0 00ee2db0 00f01480 00f02210 00f0bb10). None of them references the
    target in retail; they are pick-any copies.
  - `??0CDefClassBase@@QAE@XZ` / 00430370. It is DEF in the target row 00430370's own object and in 00f29d00.
  - NF442's census missed both: it compiled only the witness and the libclang census rows.
- **apply_table.py --dry** on the current table: `{'delete': 50} misses 0`, multi-target before 1 after 1, new [].
- **Tracker** (naming_status.py on two shadow trees, HEAD v HEAD+50 deletes, `--dump-rec` full-record diff):
  - 45 records change, all at the file's targets; 0 worse.
  - **26 targets close** (split_fixable -> agree_pdb): `evidence/STALE_TARGETS_CLOSED.txt`.
  - **19 close partly** (fewer spellings): `evidence/STALE_TRACKER_DIFF.tsv`.
- **Finding:** these landed rows compile from the repo to a non-matching grade under nf.compile_one: 0072a680 DIFFER(75v54),
  00be7d70 DIFFER(1437v2983), 00be6f70 DIFFER(198v2654), 00be8920 DIFFER(328v365), 01214190 DIFFER(95v8).
  - 00c069d0 (LZO .asm, toolchain_library) gives no C++ object.
  - All the others produced objects, so the symbol census is valid for them. Their grades should be rechecked with the lander
    (possibly thin / option S / flag rows).

## 2. q442a (tplalt_hold -> batches/ta, 29 rows): HELD, check (c) fails for 2 rows
- **Token dump:**
  - Landed 0x006CA4E0 HEAD object: `.drectve` = `/alternatename:??$GetSubDef@VCHeroDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCHeroDef@@@@@Z=?GetSubDef@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCHeroDef@@@@@Z`,
    `/DEFAULTLIB:"LIBC"`, `/DEFAULTLIB:"OLDNAMES"`. Its one relocation names the ??$ side.
  - 0x00684420: HEAD has the 2 DEFAULTLIB tokens; new adds the one alternatename token.
  - Files: `evidence/drectve_*.txt`.
- **Checks** (`tools/tplalt448.py`, `batches/ta/evidence/tplalt_checks.tsv`):
  - Inputs: re-gated on 31be8b598d, with vland HEAD/new at one path, objgate, pk + reachflip, link, name_gate, xrcheck and the table checks.
  - **(a)** passes in all 29. Every relocation names the target's ??$ self name; none names the alias; the alias is neither UND nor DEF in the new object.
  - **(b)** passes in all 29. The ??$ name is DEF in each target's own HEAD object (work/g/<t>_D).
  - **(d)** passes in all 29. `.drectve` gains exactly one `/alternatename:<??$ new>=<alias>` per rebound callee and loses nothing; every other section is equal (ALL_EQ_TPLALT).
  - **(c)** passes for 27 and fails for 2:
    - **00921630** fails on both readings. The alias `?GetPFirstThing@CThingSearchTools@@QBE_NABVCThingFilter_HasScriptName@@PAPAVCThing@@@Z`
      stays bound at 0056d050 after the batch (CROSSBATCH rewitness to 00963640). It is also DEF as a local stand-in body in
      landed objects 00963640 and 0097b900 (q442b rows).
    - **008b74d0** fails on object evidence. The alias `?GetSubDef@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCSpecialEffectsDef@@@@@Z`
      is DEF in 007f0b10, 0080f270, 0080f5e0, 0080f870 and 0080fad0 (local stand-in bodies). It is in neither reloc_bindings nor self_symbols.
    - 6 rows' aliases (00731e20, 00747820, 007aefd0, 007dfb20, 007e5f00, 0080fd90) are bound in the table today as the rows' own old
      stand-in lines. The batch deletes those lines (no other binder in 2,687 objects), so after the batch they are unbound.
- **Other gates:**
  - vland: 29/29 RELOCATION_MATCH PASS both sides. The only new WARN is +2 SHARED_HEADER_AVAILABLE CAnimationEntry (r3 debt, as in NF442).
  - objgate: ALL_EQ_TPLALT 29/29.
  - pk: BAD 0; reachflip: 0 NEQ reach flips.
  - link: PASS (duplicates 1 -> 1, NEW_UNRESOLVED 0, 23/23 new names defined, control 26 -> 0).
  - name_gate: NO_WORSE 29/29.
  - xrcheck: 29/29.
  - apply_table --dry, TC: add 15, delete 23. TC+CROSSBATCH: also rewitness 2. 0 misses; no new multi-target.
  - reserved_check: 0.
  - Head payload v repo: 0 drift. orig/ v repo: 0 drift (rows and tests).
  - Object census of the 23 deletes: no binder outside the batch. Both rewitness targets are still bound (00750050/00750540; 00963640/0097b900).
- **Verdict:** all 29 HOLD (task rule: any failed check holds the batch).
  - The 27 other rows pass all four checks and every gate, so a split is one coordinator decision.
  - T3: every row is named in the reason text of a T3 line (nf303 / nf306 / nf315), and each line's col 2 is the row's target.
    land_scripted holds only col 2 / col 3 addresses, so it holds none of the 29 (measured), and no --release-t3 is needed.
    Those T3 lines can be closed when the rows land.
  - Split note: neither failing row witnesses a TABLE_CHANGES line. A 27-row split drops only the 0056d050 rewitness, which is
    00921630's (CROSSBATCH / HAND_TABLE_CHANGES); 008b74d0's target 00669580 has no table line in the batch.
  - CHECKS.tsv was regenerated from the NF448 run.
  - SELF_SYMBOLS.tsv is rewritten to four columns (address, symbol, status, leaf_given) and equals the committed lines.
  - Locks released.

## 3. CThingSearchTools.h PeekClassList opt-in + callers
- **Header** (`hdr/engine/CThingSearchTools.h`, base `orig/hdr_CThingSearchTools.h`). Add-only `FABLE_RETAIL_CTHINGSEARCHTOOLS_PEEKCLASSLIST`
  in the flattened (generated) branch, which all 51 residue rows use:
  - forward declarations of CClass, `std::allocator<T>` and `std::list<T,A>` (no defaults, the fable_nscript_forward.h style);
  - `protected: const std::list<CClass*,std::allocator<CClass*> >& PeekClassList() const; public:` at the end of the class.
  - No layout change. The new UND is exactly `?PeekClassList@CThingSearchTools@@IBEABV?$list@PAVCClass@@V?$allocator@PAVCClass@@@std@@@std@@XZ`.
- **equiv_check copy against copy:**
  - 0 differing of 2,034 rows: every row or test that includes CThingSearchTools.h, plus the `../src/compiled` includers. No header includes it.
  - Planted control (opt-in forced on) flags 50 of the first 200: `evidence/EQUIV_CHECK.tsv`, `EQUIV_CONTROL.tsv`.
- **Generator** (`tools/peek448.py`; NF442's rebind442 rejects a protected member of a header class). The callers are members of
  CThingSearchToolsMethods, which derives from CThingSearchTools, so the call stays implicit-this. Per row:
  - add the `#define` before the include;
  - remove the stand-in declaration;
  - rewrite `PeekAllThings()` as `const_cast<CThingList*>(reinterpret_cast<const CThingList*>(&PeekClassList()))`;
  - respell the `// Callees:` comments.
  - Tests are self-contained models: none includes the row or the header, so they are unchanged. Leaf bytes are equal, so NF419 r1 applies.
- **Screens:**
  - 3 of the 51 rows (006cd3b0, 00914b20, 0093f8a0) are left out, conservatively, because the reason text of T3 nf378
    (0x00445AE0 helper port) names them. land_scripted would not hold them, since they are not in col 2 / col 3, so they can join
    a later batch.
  - The other 48 rows have no lock, claim, reserved or other-agent conflicts.
- **Batch:** `batches/h1_hdr` holds the header and all 48 caller rows, 48 ACCEPT.
  - Why one batch: all 48 rows rebind the same stand-in, so units438 makes them one unit, and a unit must not cross a batch.
    A pilot (004a9da0 alone) and a 47-row split were gated first and are kept in work/superseded/.
  - **Gates** (FABLE_HDR_OVERLAY on the new side only; runpair / prefam / namegate patched):
    - vland HEAD/new at one path: 48/48 RELOCATION_MATCH PASS both sides, 0 LITERAL. No new WARN kind; RELOC_NAME warns go 61 -> 13.
    - objgate: ALL_EQ 48/48. The UND delta is exactly old -> new; DEF sets and `.drectve` are equal.
    - pk: BAD 0; reachflip: 0 NEQ reach flips.
    - link /FORCE with the definer 0049c770: PASS (duplicates 0 -> 0, NEW_UNRESOLVED 0, new name defined, control 1 -> 0).
    - name_gate --test: NO_WORSE 48/48.
    - xrcheck: 48/48.
    - reserved_check: 0.
    - clang precheck with the overlay: 0 FAIL.
    - Head payload, orig/ and tests v repo: 0 drift; tests unchanged.
  - **apply_table --dry:** TABLE_CHANGES is empty. TABLE_CHANGES + CROSSBATCH is `{'rewitness': 1}`, 0 misses, no new multi-target.
  - **Hand line:** the one rewitness (stand-in at 0049c770 -> witness 0048d400, outside the batch) is in HAND_TABLE_CHANGES.tsv
    (same as CROSSBATCH.tsv). land_scripted filters it, so apply it by hand after landing; otherwise the table keeps witness
    004a9da0 for a spelling that row no longer binds.
  - **Closes:** h1_hdr closes no target. It moves 48 of 56 binders of one stand-in; 16 objects still bind the two stand-ins at 0049c770.
  - **Combined dry run** (stale 50 + ta TC/CROSSBATCH + h1_hdr hand line, on d3e3f622d3): delete 73, add 15, rewitness 3, 0 misses, no new multi-target.
  - **Includers:** no landed source or test `#include`s any of the 48 row files.
  - **Self lines:** SELF_SYMBOLS.tsv has four columns and holds the committed lines unchanged.
  - **T3:** T3_RESOLVES.tsv records the nf303 0x0049C770 target line as partly resolved. No row is named in a T3 line.
- **Table:**
  - The stand-in `?PeekAllThings@CThingSearchToolsMethods@@QAEPAUCThingList@@XZ` at 0049c770 stays bound in 8 landed objects
    outside the 48 (0048d400 004acf90 0052b270 005ed860 006c1dd0 006cd3b0 00914b20 0093f8a0). So its line is a rewitness
    (CROSSBATCH), not a delete.
  - The PDB line at 0049c770 already exists (witness 00914330); there is no add.
  - The T3 line nf303 0x0049C770 (target) is partly resolved.

## Locks
None held. The 29 ta rows and the 48 h1_hdr rows were locked NF448 while I worked; all are released.

## Tools
All of these are in tools/. Copied from NF442 and repointed: nf.py, mkb438.py, runb442.sh, runpair.sh / prefam.sh / namegate438.py (header overlay
on the new side only), gate438.py, pkgate438.py, linkgate438.py, xrcheck438.py, units438.py, payfilter438.py, screen438.py.
New: refscan448, symcache448 (+ run_symcache.sh), symload448, stale_text448, stale_final448, tc_text448, drectve448, tplalt448, peek448
(my own generator, used instead of genpre442.sh / rebind442), final448 (finalize442 with the object census), equiv_run.sh.
