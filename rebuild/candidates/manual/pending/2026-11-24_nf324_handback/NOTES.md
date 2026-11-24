# NF324 (2026-11-24): flatten v4 over the flattened virtual-member pool (1,744 entries)

Brief: BRIEF.md (this folder) + rebuild/candidates/manual/naming/briefs/BRIEF_NF_BATCH.md, plus the coordinator's 2026-11-24 correction
of step 3 (identity claims split three ways by FULL mangled symbol) and the note on the NF321 / NF322 / NF323 header landings. The
deliverable is the tool: `tools/flatten_pilot.py` v4 (usage docstring at its top), which the coordinator moves into the repo over v3.
Helpers are listed in tools/README.txt. Nothing was written under the repo and no git was run.

## Result

STATUS.tsv, one line per ENTRIES address (1,744):

| status | rows |
|---|---:|
| done | 679 |
| skipped-port | 986 |
| skipped-held | 68 |
| rejected | 11 |

- **done (679).** Every row is ACCEPT in its final grade:
  - the HEAD object's graded symbol is the ENTRIES self, and the new object's is the PDB symbol;
  - objcheck is ALL_EQ, or ALL_EQ plus `/DEFAULTLIB:"libcp"` on the switch / header routes (NF318 Q1);
  - the grade is MATCH / RELOCATION_MATCH and the unchanged landed test passes.
  tools/verify_final.py (evidence/verify_final.txt) checks the hand-back mechanically:
  - every done row is ACCEPT in the last final run that has it;
  - rows/ is byte-equal to the graded source;
  - the landed source still equals the graded HEAD copy;
  - the NF324 locks equal rows.txt.
- **Routes.** 506 local + switch rows land with the repo as it is (batches/1-4). 168 header-route rows need hdr/ first (batches/5-6).
  5 rows use NF323's landed opt-ins (batches/7); 2 more were graded ACCEPT there and are held (Q6). By route: 89 local, 417 switch,
  173 header.
- **Link and census.** One link over 681 HEAD objects and 681 new objects (the 679 done rows plus the 2 rows held afterwards; a subset
  cannot add a duplicate) gives 0 duplicates and equal unresolved sets. The new-name census over the same 681 has no source conflict
  (census section).
- **Tables.** No table changes: no reloc_bindings line binds an old self spelling of a done row. One NAME_FIXES line (0x009E6FA0, a
  propagated AddGossip catalog label).
- **Rejected (11, REJECTED.tsv).**
  - 4 return width (NF317 Q4, retail wins);
  - 3 VC7.1 SRC_FAIL where clang passes;
  - 2 wrong graded symbol: the shared header declares the member non-virtual or protected, so the switch route cannot give the RTTI
    access. v4 has no gen-side check for this; the grade catches it;
  - 1 UND struct / class key;
  - 1 pad used by name, which the final tool now skips.
- **Skipped (1,054).** TRIAGE.tsv and the triage section give the buckets. The largest are: flat-header ports (396, Q1),
  add-only opt-ins not made (202), own tests (79, Q2), parameter stand-in types (78) and `this` as another class (68).

## What v4 changes (tools/flatten_pilot.py; v3 is the repo's)

Candidate set and shapes:
- **Entries.** `--entries ENTRIES.tsv` makes the candidate set (target, self_leaf, rtti_pdb_spelling). Without it, v3's set is used.
- **Class and method** come from the PDB symbol through llvm-undname, so underscore classes work: CAIStateGroup_X, CCreatureAction_X,
  CCombatSequence_X. The self may be named `Class_Method`, `Class__Method`, `Method_Class` or `Method`.
- **Calling conventions:**
  - `__fastcall`: ecx = this; edx = an unused dummy when the member takes stack parameters; then the stack parameters.
  - `__stdcall`: every parameter is a stack parameter, and there is no this.
  - `__cdecl`: no parameters on either side.
- **Parameters.** The stack parameters must match the PDB's in count.
  - An unused parameter takes the PDB type.
  - A used one must already have the PDB type, with three exceptions:
    - a pointer used where the PDB has a reference (`T*` -> `T&`) becomes `p->` -> `p.`, `*p` -> `p`, `p` -> `&p` (NF313 Q2);
    - same-width integers (`int` / `long`, `unsigned` / `unsigned long`) take the PDB type;
    - `char` / `unsigned char` used as the PDB `bool` take the PDB type.
- **`this` and the edx dummy.** A `(void)p;` statement for either is dropped. A used `this` parameter becomes a local alias,
  `[const] Class* p = this;` (the v3 / pilot form), or `void* p = (void*)this;`. A used `this` typed as another class is a port.
- **Returns.**
  - The return type is the PDB's.
  - An enum return needs literal returns, written `return (E)N;` (NF307 D3 / NF318 Q3).
  - In the local route, an enum also needs exactly one shared header that defines it; a new local enum model is not made.
  - By-value class returns and template types are T3.
- **Stale comment.** The row's "Stand-in (owner rule 2026-11-05)" comment for the flattened name is dropped. The `// Leaf:` line becomes
  the PDB symbol, and one provenance line is appended. A landed `// Retail...: <hex bytes>` comment line (a byte dump, in 6 HEAD rows:
  0x00445620, 0x00445670, 0x004456D0, 0x00445770, 0x00B23360, 0x008BFFB0) is dropped too, so nothing handed back carries retail bytes.

Class model (route):
- **local.**
  - The row's own local `class` / `struct` gets the declaration.
  - A virtual target turns a leading `void* vtbl` stand-in into the vptr, takes 4 bytes off a leading pad, or (new) drops a leading
    4-byte scalar placeholder (`int dummy;`) whose name nothing else in the TU uses. An empty model needs no room.
  - A model with a base clause is skipped.
  - A class with no local model and no engine header gets a one-member local declaration.
- **switch.**
  - engine/<Class>.h already declares the member. The declaration's `#if` stack is read as:
    - `#ifdef X` / `#if defined(X)` / the `#else` of `#ifndef X`: needs X;
    - `#if defined(A) || defined(B) ...`: needs the first landed one;
    - `#ifndef X` / the `#else` of `#ifdef X` or of an `||` chain: needs those macros left undefined (the row must not define them).
  - Every needed macro is defined by at least one landed row and none is `_PASS` (NF318 Q2). The default branch also counts.
  - The row defines the switches and includes the header (variant v0). Where v0 fails clang, the row instead takes the define / include
    preamble of the landed row with the shortest such preamble that defines the switches and includes the header (v1..v3). That preamble
    is then minimised by clang: the extra includes are dropped in one step, then each added line alone, and a removal is kept while clang
    passes. The class switches always stay. Pragmas are never copied.
- **header.** engine/<Class>.h does not declare the member. This needs an add-only opt-in: tools/hdr_rtti_decls.py plus `--hdr-tree`.
  The rows then go through the switch route with the opt-in macro as one more switch.

Identity (all in `gen`, before any compile):
- `labels_rtti_port.tsv` at the address is exactly {PDB symbol}, and the symbol is in at least one PDB name set (NF320 Q3).
- No other address carries the same RTTI label, and no other entry has the same PDB symbol.
- Coordinator 2026-11-24, compared by FULL mangled symbol:
  - a landed self (self_symbols.tsv or a `// Leaf:`) that defines the same symbol at another address: held as a "duplicate strong
    definition unit";
  - another landed source with an out-of-line `Class::Method(` body that is not its own self on a different overload: held the same way;
  - a table line that binds the symbol to another address: held;
  - a catalog label `Class::Method` at another address:
    - when that address's landed self is another overload, there is no conflict;
    - when its RTTI label or landed self proves another symbol, it becomes a NAME_FIXES line;
    - otherwise the row is held.
- No other landed source or test names the flattened function. When one does, the binders or tests must move with the row, so it is a unit.
  The row's own test must not call it either.
- Reservations are skipped: T3_PORT_QUEUE (address and unit columns), Codex p28 RESERVED, the brief's held list, and any lock owned by
  another agent.

Proof (`grade`, a later call than `gen`, refused unless every row is locked by the agent and its landed source is unchanged since `gen`):
- **vland --recheck-landed.** HEAD then new for each chunk, under ONE work path. Each side runs in its own `timeout 900`, and the chunks run
  two at a time.
- **Symbols.** The HEAD object's graded symbol must equal the ENTRIES self, and the new object's graded symbol must be the PDB symbol.
- **objcheck** (new in v4; objsec compares sections only), on the HEAD v new objects:
  - every non-debug section is equal, bytes and relocations, under the one rename;
  - the DEF sets are equal under the rename;
  - the UND sets are equal;
  - there is no new `??_7` / `??_R`;
  - `.drectve` is equal, or, on the switch / header routes only, HEAD's tokens plus exactly `/DEFAULTLIB:"libcp"` (NF318 Q1 / NF322 Q3).
  - The added-COMDAT and dropped-COMDAT cases (NF318 Q5 / NF320 Q2) fail on the DEF sets or the section count. Controls are in
    evidence/objcheck_controls.txt.
- **Grade.** MATCH / RELOCATION_MATCH with the landed test passing, the test unchanged.
- **Precheck** (in `gen`): clang -fsyntax-only of the new source, the HEAD source and the landed test. A new source that fails where HEAD
  passes is a SKIP.

What v4 does not cover (each case is refused or caught, never accepted wrongly):
- **Access and virtual-ness of the header's declaration.** On the switch route, gen does not compare the access / virtual-ness the header
  declares with the PDB symbol's. The grade refuses the row on the graded symbol (0x0065ED8E: `QBE`, non-virtual in the header;
  0x00933DA0: `MAE`, protected). A gen-side check is the next cheap lever.
- **Helper key mismatch.** A row's helper declared with `struct X` where the header has `class X` changes a referenced mangled name. The
  objcheck UND set refuses it (0x009143C0).
- **Clang-only acceptance.** The precheck is clang; VC7.1 still rejects 3 rows that clang passes (SRC_FAIL, refused by the grade).
- The buckets in the triage section: flat-header ports, own-test units, stand-in parameter types, `this` as another class, `_PASS` /
  unlanded switches, templates and by-value class returns.

## Step 1: triage (TRIAGE.tsv, every entry)

TRIAGE.tsv columns:
- v3_stage / v3_reason: where the repo's v3 tool stops. Its candidate filter is replayed by tools/triage_v3.py, then its flat4 verdict.
- v4_verdict / v4_route / v4_bucket / v4_reason: the final v4 tool's dry run (`gen --no-pre --hdr-tree work/tree`, run tri5).

Why 1,177 of the 1,744 entries never became v3 candidates. Each entry is counted once, at the first test it fails:

| v3 stop (first failing test) | entries |
|---|---:|
| class or method name contains `_` (v3 regex `?[A-Za-z0-9]+_[A-Za-z0-9]+@@YI`) | 664 |
| PDB member takes parameters (v3 needs `...XZ`) | 265 |
| self is `__stdcall` | 222 |
| self is `__cdecl` | 13 |
| 2 PDB symbols for Class::Method (overloads) | 13 |

The other 567 were v3 candidates (the brief's 711 counts the whole repo pool):
- 497 are in flat4 (370 "no single class-model switch", 40 "no vptr slot", 33 "body uses the local model", 26 shape, 5 accepted
  and since landed as flatten4, ...);
- 70 were never reached by flat4's run.

v4 final dry run (work/tri5: the final tool, `gen --no-pre --hdr-tree work/tree`, so route `header` assumes the hdr/ opt-ins have
landed; bucket counts in evidence/tri5_buckets.txt). TRIAGE.tsv's v4 columns are this run.
- generated: 753 (465 switch, 105 local, 183 header);
- header, flat model only (port): 396;
- header, add-only opt-in not made: 202. tools/hdr_rtti_decls.py skipped 129 classes (evidence/HDR_PLAN.tsv):
  - 50: the override probe fails (the base chain does not declare the virtual, or a parameter type is missing there);
  - 25: the class switch has no landed definer, or is `_PASS`;
  - 22: another agent's hdr/ overlay has the header (NF323);
  - 16: the probe preamble fails clang;
  - 14: two polymorphic definitions;
  - 2: a one-line class / a complex `#if`;
- own test calls the flattened name: 79;
- used parameter of another type (stand-in types: CRetailVector_dword* for CEnginePrimitiveTypeList&, CGameScriptInterface* for
  const CScriptThing&, int for const CCharString&, ...): 78;
- `this` used as another class: 68;
- declared only under an unlanded / `_PASS` switch: 30;
- stack-parameter count differs: 21 (all thunks to made-up `..._CopyImpl` stand-ins);
- template types: 19;
- identity holds: 25 (another landed body of the same symbol, an inline header body in some branch, an in-class body on a local model,
  an undisproved catalog label);
- other units, reservations, shapes and parameters: the rest.

The 753 generated rows against the batches: 667 went through batches 1-3 (the 669 graded there, less 0x009D5100 and 0x0045655E, which the
final tool holds). Of the other 86, 10 are batch-1/2 REJECTs, and 76 were never graded: 53 batch-1/2 gen skips (52 precheck, 1 override
gate; `--no-pre` runs neither), 8 batch-3 gen skips (6 precheck, 2 override gate), and 15 rows no batch reached. The 76 were regenerated
with the precheck as batch 4 (see that section).

## Batches, the final regeneration and the hand-back batch dirs (work dir names b1-b4 differ from batches/1-7)

First grades (evidence/b1/, evidence/b2/; vland status lines only, no bytes):
- **Batch 1** (work/b1, 434 rows): 425 ACCEPT (59 local, 268 switch with `.drectve` equal, 98 switch with `+libcp`), 9 REJECT:
  - 2 local and 1 switch SRC_FAIL (the new source does not compile under VC7.1 where clang passed);
  - 3 return width (0x0088E4D0, 0x0088F450, 0x0088F950): the PDB return type changes the bytes. NF317 Q4: retail wins, the row is not
    respelled, so these stay flattened;
  - 2 wrong graded symbol (0x0065ED8E, 0x00933DA0): the new object's graded symbol is not the PDB symbol;
  - 1 UND set differs (0x009143C0): a `struct` / `class` key mismatch changes a referenced mangled name.
- **Batch 2** (work/b2, 77 rows, the rows the v4 levers added after batch 1): 75 ACCEPT, 2 REJECT (0x00452BB4, a pad used by name, which led
  to the pad-use guard; 0x0051CE00, return width).
- **Link check over batches 1 + 2** (evidence/linkcheck_b12.txt): 0 duplicates, unresolved sets equal. No table changes. No
  COORDINATION_CLAIMS hit.

Final regeneration (the hand-back sources are the FINAL tool's output, regraded):
- After the tool was final and the NF321 / NF322 / NF323 header landings were in, `gen` was rerun over all 500 accepted rows (work/fin12)
  and compared with the graded source by tools/fincmp.py:
  - 397 generated: 378 the same source, 19 different (a header the preamble includes changed, so minimisation kept other lines);
  - 103 skipped by that run of the tool, both gen-side regressions, now fixed:
    - 100 by the override gate, which ran on switch-route rows (classes that introduce their own virtuals, such as CAFile, fail it). The
      gate now runs on the header route only;
    - 3 local rows (0x00B3B180, 0x00BCE9F0, 0x00BF6B50) by the pad-use guard, which counted another class's `char pad[...]` declaration
      as a use. The guard now ignores declarations.
    The 103 were regenerated in work/fin12b.
- All 397 + 103 were graded again (`grade --run runF`) on the final sources: 397 / 397 and 103 / 103 ACCEPT (evidence/fin12,
  evidence/fin12b). The first-grade evidence stays in evidence/b1, evidence/b2.
- Six rows carried a landed retail byte-dump comment (NOTES Q4). They were regenerated without it and regraded: 5 / 5 ACCEPT in work/finx
  (evidence/finx), and 1 / 1 in work/b3x (0x008BFFB0, header route, evidence/b3x). Their earlier copies are HOLD "superseded" in the
  batch they came from.

Byte scan: tools/bytescan.py over the whole hand-back (rows/, batches/, payload sources and tests, hdr/, orig/, evidence/, top-level
files, tools/) finds 0 hits.

Final tool check: after the last tool edits, a smoke `gen` (work/smoke, cleaned afterwards) reproduced the graded sources byte for byte for 0x00401650
(fin12) and 0x00445620 (finx), and `export` ran clean. The later edits after fin12 changed only gates (skip decisions), the byte-comment
drop (its 6 rows regraded), export and docs, never the generated text of other rows.

Hand-back batch dirs (each is a land_scripted.py run dir: RESULTS.tsv, rows/, head/payload.json, head/HEAD_FROM_REPO.txt,
TABLE_CHANGES.tsv, SOURCE = the work dir it was exported from):
- batches/1: fin12, 392 ACCEPT (5 HOLD: 0x009D5100 identity, 4 superseded by batches/3; 103 SKIP = the rows regraded in batches/2;
  their SKIP reason in batches/1/RESULTS.tsv is the old, false override-gate / pad-guard reason, kept as that run wrote it);
- batches/2: fin12b, 102 ACCEPT (1 HOLD: 0x00B23360 superseded by batches/3);
- batches/3: finx, 5 ACCEPT (the byte-comment rows; HEAD entries left out, NOTES Q4);
- batches/4: work/b4, 7 ACCEPT (switch route): rows the final triage generated that no earlier batch graded (batch-1/2 precheck skips
  the final tool now passes, and rows no batch reached; see the triage section);
- batches/5: work/b3, 167 ACCEPT (header route; 44 `.drectve` equal, 123 `+libcp`), 2 HOLD (0x0045655E identity found by the final
  triage, 0x008BFFB0 superseded by batches/6), 8 SKIP (6 precheck, 2 override gate);
- batches/6: work/b3x, 1 ACCEPT (0x008BFFB0, the byte-comment row of batch 3; HEAD entry left out, NOTES Q4);
- batches/7: work/b4h, 5 ACCEPT on NF323's landed opt-ins, 2 HOLD (0x008601B0 / 0x008601C0: their opt-in macro has no landed definer
  yet, NOTES Q6).
The 11 rejects (REJECTED.tsv) come from the first grades (work/exp_b1, work/exp_b2) and are in no hand-back batch dir, so
land_scripted.py will not queue them to T3_PORT_QUEUE: REJECTED.tsv / STATUS.tsv are their only record.
Landing order: batches/1-4 need nothing else. hdr/ must land before batches/5 and /6. batches/7 needs only the repo as it is.

Batch 3 / 4 procedure, as for batches 1 and 2:
1. a gen without locks with the precheck of new, HEAD and the test (work/b3pre*, work/b4pre, work/b4hpre; evidence/b4_prelock);
2. the locks;
3. gen again into the graded work dir;
4. the grade in a later call, with `--hdr-tree work/tree` for the header route.

## Step 5: the header layer (add-only opt-ins; work/b3 -> batches/5, /6)

tools/hdr_rtti_decls.py, run over the triage's "engine/<Class>.h does not declare <meth> (add-only opt-in)" bucket. The plan is in
evidence/HDR_PLAN.tsv.

For each class, the tool:
- **finds the model.** It locates exactly one polymorphic definition `class X : <base> {` in the CURRENT repo header. The `#if` stack is
  read as in the switch route, so every class switch is landed and none is `_PASS`. A class written on one line is skipped (NF319 Q3).
- **inserts the block.** One block goes just before the class's closing `};`: `#ifdef FABLE_RETAIL_<CLASS>_RTTI_DECLS`, a comment, each
  RTTI-placed member in the PDB spelling (access, virtual, const, types), the access in force restored, `#endif`. It is declarations only,
  the NF319 `_RTTI_DECLS` precedent.
- **pins asserts.** Moved asserts are pinned with lane_merge/pin_asserts.py (`FABLE_STATIC_ASSERT_AT(<old line>, ...)`); asserts already
  `_AT` stay.
- **probes the class.** The probe defines the class switches plus a landed precedent's preamble, includes the ORIGINAL header, and declares
  `struct P : X { <decl> override; };`. That probe must compile, and a made-up-name control must fail. Members that fail are left out.
- **skips other agents' headers.** A header present in another agent's hdr/ overlay is skipped (NF323: CFireballSpellLevelDef, CJoystickDX,
  CLipSyncDataBank, CNavBlockedQuadTreeNode, CNavNavigableLeafNode, the CCreatureAction_* set).

Then each row goes through the switch route with the opt-in macro as one more switch (`gen --hdr-tree work/tree`). Per row, the override
gate repeats the probe against the declaring class's base with the row's FINAL preamble, so the base chain must declare the virtual. The
grade's new side compiles with FABLE_HDR_OVERLAY = work/tree, a full include copy that tools/treesync.py keeps equal to the repo except
for the edited headers.

Header proof (hdr/):
- **Copies.** hdr/engine/<X>.h holds full copies (122 headers after pruning), and orig/hdr_<X>.h holds the repo originals at edit time.
  Every orig/ copy still equals the repo header at hand-back. hdr/<X>.diff holds per-header diffs. orig/rows/<addr>.cpp holds the HEAD
  source of each done row, except the 6 byte-comment rows (orig/rows/LEFT_OUT.txt).
- **equiv_check.** lane_merge/equiv_check.py ran repo rebuild/include against work/tree over every landed row and test, with S pointed into
  this folder. It checked 57,146 rows, every landed source and test, and the result was `EQ_DONE rc=0 lines=0` (hdr/equiv_check.tsv,
  empty; hdr/equiv_check.log). Straight after it, hdr/staleness_check.txt confirms three things: every orig/ copy still equals its repo
  header, every unedited tree file equals rebuild/include, and every hdr/ copy equals its tree copy.
- **Planted control.** hdr/equiv_planted_control.*: a token planted in the default path of engine/CCarrySlotDef.h flags exactly its 3
  landed includers (src) and one test, and none of the 3 non-includers. So the check sees header changes; the clean full run is the proof
  that the opt-ins change no landed row or test.

Cleanup: the overlay work/tree was cleaned after the final checks. To rebuild it, copy rebuild/include and overlay hdr/engine/*.h.
Objects, oracles and link outputs (retail-matching bytes) were cleaned from every work run dir. GEN / GRADE / gen / pre stay, so
tools/verify_final.py and the batches' SOURCE pointers still work.

Pruning and landing order:
- tools/hdr_prune.py kept in hdr/ (and orig/) only the headers an accepted header-route row uses. The others were moved to
  work/hdr_unused/ (LIST.txt); they are not for landing. Each header-route batch has HEADERS.tsv (address, header, opt-in macro).
- The hdr/ headers land first, then batches/5 and batches/6. batches/7 uses opt-in blocks NF323 already landed in the repo (no hdr/ change;
  NOTES Q6).
- The equiv_check run covered all 126 edits together. The kept headers are a subset of those edits, and an opt-in whose macro no row
  defines is inert, so the result carries over.

## New-name census and link check

- **New-name census** (tools/inclass_census.py, BRIEF_NF_BATCH; output evidence/census_*.txt). Every landed source and
  test other than the row's own was searched for three things:
  - an in-class body of the member inside a local model of the class;
  - an out-of-line `Class::Method(` body;
  - a `// Leaf:` naming the symbol.

  Hits:
  - **0x009D5100** `?Verify@CMovableResource@@UBEKXZ`: five landed rows (0x00BE8E60, 0x00BFBD80, 0x00AE04E0, ...) model
    CMovableResource locally with an in-class `virtual unsigned long Verify() const { return 0; }`. They emit a pick-any COMDAT of the
    same symbol, which would clash with the strong self (GOTCHAS nf80 / nf86). **Held** as a unit (0x00BE8E60 / 0x00BFBD80 are also on
    the brief's held list). v4 now refuses this case in `gen` (identity: in-class body on a local model).
  - **0x008AA330 / 0x008AA590 / 0x008AA0D0** AddGossipVillages / AddGossipFactions / AddGossip: the hits are the landed two-string
    overloads (0x008AA2F0 / 0x008AA550 / 0x008AA090, Leaf `...@UAEXVCCharString@@0@Z`), different symbols from these rows' one-string
    members (`...@UAEXVCCharString@@@Z`). That is the coordinator's case 2 (an overload), so there is no conflict.
  - **0x0045655E** CCameraModeDef::GetSizeofClass: an in-class body on a local model in NCameraModesCBase_GetDefaultFOV_0084d790.cpp,
    found by the final triage. Held like 0x009D5100.
  - The final census (evidence/census_final.txt) covers all 681 rows accepted before the last two holds. In rebuild/src its only hits are the three overload pairs
    above. Every other hit is in rebuild/tests: 2,073 in-class / out-of-line test bodies (mocks of CGameScriptInterface members and the
    like).
    - A test links alone or with its own row's object only (verify_and_land.py `behaviour_of`), and the census skips each row's own test.
    - So a test mock of a done row's symbol S meets only its own row's object in a link, and that object defines S only when the test is
      the row's own test. The census skips that test, and the grade ran it.
    - No hold follows from them.
- **Link check** (tools/linkcheck.sh, evidence/linkcheck_final*.txt; batches 1 + 2 alone in evidence/linkcheck_b12*.txt). A
  `/FORCE /NODEFAULTLIB` DLL link was run over all 681 HEAD objects and, separately, all 681 new objects of the done rows, every batch
  together:
  - 0 LNK2005 duplicates in either set;
  - the unresolved mangled-symbol sets are equal (no reference HEAD resolved is left unresolved, and none of the rows references
    another's old or new self).

## Process, locks and slips

- **Order per batch:**
  1. `gen` without locks: identity, generation, and a clang precheck of the new source, the HEAD source and the landed test;
  2. lock the GEN rows (tools/lockall.sh);
  3. `gen` again into a fresh work dir: the precheck after the lock;
  4. in a later call, `grade` (vland HEAD then new, one work path per chunk);
  5. `export`.
- **Locks.** Kept on every done row (679; tools/verify_final.py checks locks == rows.txt). Released on every reject, gen skip and hold
  (0x009D5100, 0x0045655E, 0x008601B0, 0x008601C0).
- **Final regeneration.** After the tool was final, `gen` was rerun over every accepted row (work/fin*) and compared with the graded
  source. Rows whose source changed were regraded, and rows the final tool skips were held (see the batch sections).
- **Slips and deviations** (nothing reached the repo):
  - one `sleep 0` appended to a heredoc command while writing tools/triage_v3.py (no wait happened);
  - the 21-row pilot (work/g1/runp1) was prechecked before its locks were claimed, and the locks were claimed before its grade. Its rows
    were then redone in batch 1 in the proper order;
  - batch 1's six rows 005df610 / 005df650 / 005df6d0 / 005df710 / 005df840 / 00b23360, and batch 2's 87 newly generated rows, were
    locked before their landed tests had a clang precheck. The tests were prechecked after the lock and before any grade;
  - the first triage run was killed by PID (my own) after it ran too slowly.
  - `export`, `assemble.py`, `verify_final.py`, `hdr_prune.py`, `evcopy.py` and the byte scan ran in the foreground. Each is a few seconds
    of Python with no compile; every gen, grade, link, census and equiv_check ran in the background under its own timeout.
- **Headers.** Just before the batch-3 grade and again before hand-back, orig/ was compared with the repo headers and work/tree with
  rebuild/include: no stale header, no tree drift (NF321 / NF322 / NF323 had landed before batch 3 was generated). The check covers the
  headers this hand-back edits and the whole overlay tree, not other agents' pending work.
- **Wall clock.** Box load reached 38 on 4 cores (four other equiv_check runs), so chunks were 25 rows and two ran at a time.

## Questions to rule on

- **Q1 (flat-header ports).** 396 entries (final triage) have engine/<Class>.h with only the generated flat model: `void* __vftable`, no base, no
  `virtual`. No add-only declaration can sit at an RTTI slot there.
  - By RTTI direct base: about 156 derive from CCombatActionBase and 143 from CCombatSequenceBase; the rest are spread thin
    (evidence/flatport_bases.txt, from the tri4 run).
  - engine/CCombatActionBase.h has a polymorphic model (`: public CCreatureActionBase`, 20+ virtuals) under
    `#if defined(FABLE_RETAIL_LEADER_INITIALISE) || ...`, and those switches have landed definers. So a generated opt-in derived model
    `class X : public CCombatActionBase { <Ego_r members>; <RTTI-named overrides> };` is possible per class, in NF325's genmodel.py style.
  - engine/CCombatSequenceBase.h is polymorphic only under `FABLE_RETAIL_COMBAT_SEQUENCE_SELECTION_PASS27`, a `_PASS` switch (NF318 Q2:
    out). Those 143 need the base model widened first (NF322 Q1: `|| defined(FABLE_RETAIL_CCOMBATSEQUENCEBASE)`).
  - Is the generated-opt-in route (NF322 Q2 rules: ??_7 probe against retail, asserts, equiv_check) wanted for these two families as the
    next layer? If so, it is its own pass.
- **Q2 (own tests call the flattened name, 79).** The row's landed test declares and calls `Class_Method` extern. The row cannot move
  without its test, and land_scripted.py does not copy tests. Two options:
  - give each test a member call through a cast pointer and a qualified call (`p->Class::Method()`; no vftable needed), written by script;
  - leave them as T3 test units.
  Which is wanted?
- **Q3 (parameter retypes on USED parameters).** RETYPES.tsv lists the done rows (retype and ptr2ref lines). The flattened self's same-width integer
  parameter takes the PDB type: `int` -> `long`, `unsigned` -> `unsigned long`, `char` / `unsigned char` -> `bool`. Bytes are proven equal
  by objcheck. Is this in place (R1: a type both PDBs give), or should these rows be held?
- **Q4 (landed retail byte-dump comments, 6 rows).** 0x00445620, 0x00445670, 0x004456D0, 0x00445770, 0x00B23360 and 0x008BFFB0 carry a
  landed comment line `// Retail...: <hex bytes> => <asm>` in the repo. v4 drops that line from the new source (regraded, see the batch
  RESULTS), so rows/ carries no bytes. The HEAD payload must equal the landed source for land_scripted.py's stale check, so these rows'
  HEAD entries are left out of head/payload.json and listed in head/HEAD_FROM_REPO.txt: landing holds them ("no HEAD payload") until the
  coordinator rebuilds those entries from the repo files. Is that acceptable, or should these rows go in a separate coordinator pass?
- **Q5 (header route landing order).** batches/5 and batches/6 need the hdr/ opt-in blocks landed first. Their HEADERS.tsv names the
  header and the `FABLE_RETAIL_<CLASS>_RTTI_DECLS` macro for each row. The opt-ins are declarations only, equiv_check-clean over every
  landed row and test (57,146 rows, 0 lines), with a planted control. Land hdr/ (122 headers) and then batches/5 and /6?
- **Q6 (another agent's landed opt-in as the switch, batches/7).** With `--hdr-tree`, gen counts ANY `#ifdef FABLE_RETAIL_<CLASS>_RTTI_DECLS`
  block in the tree as a landed switch: this run's own opt-ins, and ones already in the repo. The batch-7 rows take their declaration from
  `FABLE_RETAIL_CCREATUREACTION_<X>_RTTI_DECLS` blocks NF323 landed in the repo headers (none of my hdr/ edits). NF323's NAME_FIXES.tsv names
  the same PDB symbols for these addresses.
  - 0x0084A820, 0x00865A80, 0x00865A90, 0x00941BA0, 0x00941BB0: their macros have a landed definer (done, batches/7).
  - 0x008601B0 / 0x008601C0 (GetUpFromSitting): no landed row defines that macro yet. Under the strict NF318 Q2 reading ("a macro a landed
    row defines") they are held, though both graded ACCEPT (work/b4h/run1, evidence/b4h). Release them now, or wait for a definer?
- **Q7 (kept opt-in blocks that declare members no done row defines).** Each opt-in block was written per class before grading.
  The kept headers therefore still declare the members of 5 batch-5 gen skips:
  - 0x00452B04 CHairCardDef::Transfer: it failed the row's own override gate (with that row's final preamble, CSubDefClassBase does not
    declare the virtual), although hdr_rtti_decls.py's probe with a precedent preamble passed;
  - 0x00865B40, 0x00A75C10, 0x00A76530, 0x00BC5FB0: precheck failures of the row source.
  The other done rows that define the same class macro graded ACCEPT with these declarations in place, and equiv_check is clean, so no
  landed or done row changes. The declarations stay unused until those rows are fixed. Should the blocks be trimmed to the done members
  before landing? That means a re-edit, a new equiv_check and a regrade of those classes' rows.
- **Q8 (NF325 headers, heads-up).** No header in hdr/ is in NF325's hdr/ (37 headers, compared at hand-back). 16 done rows (all in batches/2,
  evidence/nf325_includers.txt) include engine/CEngineInternalPrimitiveBase.h, which NF325 changes. If NF325 lands first, land_scripted.py's
  recheck on the landed tree decides those rows (a failure restores HEAD and holds the row).
