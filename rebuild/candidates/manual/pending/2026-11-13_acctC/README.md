# acctC reservation (2026-11-13, owner): 180 rows for the third account, working on branch `FullDecomp-B`

`RESERVED.tsv` lists the rows. They are off-limits to the coordinator's lane and to any other lane until this list is released
(renamed `RELEASED.tsv`). The other reservation list is `../2026-11-14_codex_pass22/RESERVED.tsv` (100 rows for Codex, no overlap); the earlier Codex lists and the acctB list are released.

| kind | rows | what they are |
|---|---:|---|
| parked | 120 | a differing draft exists (`closest_version`): the 19 acctB drafts, the 2 acctB rows that matched but were not landed, and the 99 largest other parked rows outside the coordinator's author pool |
| never_worked | 60 | no draft: the 23 acctB rows without a draft (16 have a dossier and outline in `../2026-11-13_acctB_parked/notes/`) and the 37 largest other rows nobody has authored |

`length_estimate` is the distance to the next known function start (an upper bound, not the function length): take the real length
from the retail bytes before building an oracle. The rows were chosen largest first, about 960 KB by that estimate, because what is
left of the project is mostly large functions. Rows classed impossible in plain C++ (CRT or hand-written asm in retail) and the
blocked acctB rows and fragments are not in the list.

## Before the first row
1. Merge `FullDecomp` into `FullDecomp-B`. `FullDecomp` now holds everything the acctB lane matched (landing fy), the merged headers
   and this list; the acctB overlays in the pending folders are STALE copies and must not be used as a starting point.
2. Read `../../batches/2026-11-13_landing_fy/README.md` and `tools/decomp_pipeline/linux_lane/lane_tools/lane_merge/README.md`:
   they describe what it cost to land rows that were verified against old header copies.

## Rules that keep the hand-back landable (from that landing)
- **Stay on today's headers.** Merge `FullDecomp` into `FullDecomp-B` at least once a day and before every hand-back, then re-verify
  every matched row against the merged `rebuild/include`. A row verified only against an older copy is not a match yet.
- **Header changes are additions under new `#ifdef FABLE_RETAIL_*` opt-ins**, made against the CURRENT header. Hand back each change
  as a small diff or as (old text, new text) pairs per header, never as a whole-file copy of a header.
- **Never reuse an opt-in name that exists on `FullDecomp` for different content**, and never edit the body of an opt-in a landed row
  uses. To serve two rows from one existing line, extend its condition (`#if defined(A) || defined(B)`).
- **A header edit must not move a `FABLE_STATIC_ASSERT`** without pinning it: the macro names its typedef after `__LINE__`, and VC7.1's
  stack-slot order in a few rows depends on those names (landing fz). After editing a header run `lane_merge/pin_asserts.py` (it
  respells moved asserts `FABLE_STATIC_ASSERT_AT(<old line>, ...)`), then the exact `equiv_check.py` and `recheck_rows.py`; the
  steps are in `lane_merge/README.md`.
- **If a class already has a shared header, use it** (add what the row needs as an opt-in); do not write a second header or a local
  model of the class.
- **Prove header changes over the whole tree** before handing back: `lane_merge/equiv_check.py <include tree before> <include tree
  after> <out.tsv>` preprocesses every landed row against both; every row whose text differs must pass `recheck_rows.py`.
- The standing project rules apply unchanged (CLAUDE.md): byte match plus behaviour test, plain portable C++, PDB names from two
  sources, no retail bytes committed, nothing landed by the lane itself. Matched rows are staged in pending folders for the
  coordinator, with a final index like `../2026-11-13_acctB_FINAL/INDEX.tsv`, and with REAL lengths in it.

## Lane progress (acctC coordinator)
`STATUS.tsv` is the lane's ledger, one line per reserved row (OPEN / QUEUED / IN_PROGRESS / MATCHED_STAGED, true length, the pending
folder a matched row is staged in). Owner goal (2026-11-13): every reserved row matched; author agents (Opus, expert brief, large
budgets) are relaunched as they end. `lane/mkagent.py <agent> <addr>...` builds an agent's work folder and brief from
`lane/BRIEF_TEMPLATE.md` (scratch paths are filled in at run time; oracles stay in the scratch).

Re-cut rows (two reserved rows that were not function starts) and what the lander must do about them: `RECUTS.md`.

## Acceptance test for devices and fitted spellings (owner, 2026-11-14)
Before accepting a device (inline flag, scoped inline pragma, unused named locals that re-key a c2 record-count tie, a redundant
guard, a fitted named local, dead release asserts) the question is: does the row BYTE MATCH retail, does it BEHAVE THE SAME as
retail, and is it PORTABLE code. Three yeses: accept it, marked (`// Inline-budget:`, `// Fidelity:`, `// Inferred:`, or FITTED
in the header comment). Earlier in this lane the coordinator applied a stricter rule (no inline flags, no unused locals);
READMEs written before 2026-11-14 that call such a construct "not landable" should be read with this test instead. Asm has
extra rules (docs/pipeline/ASM_RULES.md: only where retail used asm; an intrinsic, else a shared-header macro kernel with a
`FABLE_PORTABLE` twin and a dual-path test; never in a row source) and the same three questions apply to it. Stand-in callee
bodies, diagnostic builds and local class models where a shared header exists fail the test.

## LANDING GUIDE for the main-branch account (acctC coordinator, 2026-11-15)

**What is ready.** `STATUS.tsv` in this folder is the ledger (address, state, true_length, staged_in, note). Every row with
state `MATCHED_STAGED` (156 reserved rows + the bonus support row 0x00B9E5B0 on 2026-11-15, round 5) is in the folder named in its
`staged_in` column, under `rebuild/candidates/manual/pending/`. Each such folder holds `src/<addr>.cpp`, `test/<addr>.cpp`,
`payload.json` (addmatch format, only the verified rows), `hdrdiff/` (the header changes those rows need, as diffs against
rebuild/include or `NEW_<file>` for new headers), `coordverify.log` (my read-only vland output), `HDR_README.md` (the
batch's overlay opt-in list with evidence), `NAME_FIXES.tsv` / `LABELS.tsv` where present, and a `README.md` with the
review items of each row. Every staged row was byte-verified twice (author + my own read-only strict-header vland) and
rule-audited by an independent agent.

**What is NOT ready.** Folders whose README says "RULE AUDIT PENDING" (none at the moment: the twelve `2026-11-15_acctC_wf*p` rows were audited in round 5 and their
READMEs now say "RULE AUDIT DONE" with the review items) and anything under an `open/`, `*open` or `differ` folder (unmatched
drafts). Earlier audits rejected about one fresh match in five for rule problems (local class models, stand-in callee
bodies), so wait until the pending line is replaced. Those rows are not `MATCHED_STAGED` in the ledger. The rows once held for rule fix-ups (0x00439070, 0x00531EC0, 0x00B89C30, 0x00DF6010) are all rule-clean, re-audited and staged.

**This lane never ran `--land`.** Your `verify_and_land.py` run is the real gate. My check was
`lane/coordverify.sh <agent-dir> <stage-dir> <addr8>...` = vland read-only with `LANDVERIFY_STRICT_HEADERS=1` and a minimal
overlay made of exactly the headers in `hdrdiff/`.

**Things to get right when landing**
1. MERGE the header diffs, do not replace files folder by folder. Many batches add opt-ins to the same shared headers
   (fable_containers.h appears in 47 hand-back folders, C3DVector.h in 23, CVectorMap.h 14, CIVCountedPointer.h 13,
   CMatrix3x4.h 13, CWorld.h 12, ...). Every change is an additive `FABLE_RETAIL_*` opt-in with the default text untouched;
   moved asserts are pinned (`FABLE_STATIC_ASSERT_AT`). Later folders' diffs were taken against the repo at that time, so the
   same opt-in can appear in several folders: take the union. `HDR_README.md` in each folder lists each opt-in with evidence.
2. `fable_assert.h` is NOT in the repo (10 folders carry it as `NEW_fable_assert.h`; all copies are agentC6's header, and
   wfC1's copy adds one macro, `FABLE_RETAIL_ASSERT_ARM`). It must land with the rows that use dead release asserts
   (0x00CBFB7D, 0x00E50FD0, 0x00E3BB70, 0x00BBAC60, 0x00B5CA70 and others: grep `FABLE_RETAIL_ASSERT` in the sources).
   **Owner decision (2026-11-15): dead asserts stay as they are, provided each row carries a note saying why they are
   there.** Every staged source that uses `FABLE_RETAIL_ASSERT` has that note in its header comment (an `// Inferred:` line,
   or for 0x00BA5260 the header paragraph on its 18 + 1 asserts). Why they are there, in general: the debug build expands
   Lionhead's ASSERT to a real check; retail carries no assert code, but the release macro left a dead branch the compiler
   still saw before deleting it, and that dead code feeds VC7.1's ordering and inline-size decisions. A lab run on
   2026-11-15 (21 assert-using rows, independently re-run) ruled out the forms that vanish in preprocessing (`((void)0)`,
   `sizeof`), `__assume`, a stub call and `if (0 && (c)) {}`: only the debug body behind `if (0)` inside `do { } while (0)`
   matches broadly (20 of 21; 0x00CBFB7D also needs the unwrapped `FABLE_RETAIL_ASSERT_ARM`). Counts and conditions are
   inferred, not recovered: our macro is lighter than the original, so a row can need more copies than the function really
   had (0x004A3740: 79..87 identical copies with this macro, 35..38 with a full-debug-text form; only the total matters).
3. Rows with companion headers: 0x0121F86F needs 126 `engine/CAIStateGroup_*.h` overlay headers (one shared opt-in
   `FABLE_RETAIL_CAISTATEGROUP_GETNAME_ALLOC`); 0x0044C72B needs 69 `FABLE_RETAIL_<CLASS>_ALLOC` opt-ins; 0x00B1C880 /
   0x00B1F200 / 0x00B1E380 need the new `engine/CPSCDecalRenderer.h`; 0x00678760 needs new `engine/CReactionMatch*.h`.
4. 0x00CBFB7D RunCutsceneMacro: with a separate overlay DIRECTORY the repo's `fable_definition_table.h` includes the repo's
   old `fable_string.h` (MSVC searches the including file's folder first) and the row fails with C2011. Once the
   fable_string.h opt-ins are merged into rebuild/include this cannot happen (see `2026-11-14_acctC_wfC1/README.md`).
5. Rows with per-row flags carry them in `payload.json` and on a `// Flags:` line (e.g. /Gr, /GS, /EHsc /MD, `/d2inlT<N>`).
   Inline controls (`/d2inl*`, scoped `#pragma inline_depth`) are marked with an `// Inline-budget:` line (owner decision
   2026-11-08).
6. Same-TU callee bodies defined in a row source also emit that callee's symbol in the row object (e.g. 0x00772BB0 defines
   CMatrix3x4::Set 0x007767A0; 0x007EA580 defines 0x007EA2D0 / 0x007EA2F0 / 0x007EA490): mind duplicate definitions at link.
7. Re-cuts: `RECUTS.md` in this folder (0x00564D50 re-cut with a boundary start and a BOUNDS_FIX row; 0x00537E70 is a
   fragment of 0x005379F5). Catalog lengths were wrong for several rows; `true_length` in STATUS.tsv is the proven extent.
8. Fixes to ALREADY-LANDED rows that my rows prove (details in the folders' `NAME_FIXES.tsv` / READMEs):
   - `CRenderStateManager::D3DAlphaFuncModeTable` base is 0x01396DD8; landed 0x00BBC460 binds 0x01396DE8 (an element
     address) and causes RELOC_CONFLICT warnings on 0x00B555A0, 0x00A059E0, 0x00BC04F0 (and pending 0x00BA6240, 0x00B8B780).
   - Landed 0x0052E470 / 0x0052E4D0 are `NUISystem::CUIScreenUnitConverter::ConvertX` / `ConvertY` (static __fastcall), not
     Physics_ScaleFloatByGameSpeed / LengthScaleConversion; their globals are that class's static members.
   - Landed 0x00535CD0 `C2DBoxF::ContainsPoint` returns `bool` (both PDBs), not long (same bytes).
   - 0x0129BA3C is `?DELTA_NOTIONAL_ZERO@@3MB` (a label set says g_NormaliseEpsilon).
   - 0x01436E80 = GPPrimitiveRenderer, 0x01436E78 = GPPrimitiveRendererSharedData.
   - Twin 0x00651B04 (staged in `2026-11-13_acctC_c2a`) should be respelt with `long HUDRingAlpha` to agree with 0x00652FA4.
   - RELOC_CONFLICT on `_$E2` (0x01221D20, 0x0121F86F) is a per-TU compiler-local name: benign.
   - 0x004A0D90: vector<char>(unsigned) binds 0x00410F30 here and 0x00411910 in landed 0x009EC890 (byte-identical COMDATs):
     the lander decides the binding.
9. Tool gap: the strict-header regex in `verify_and_land.py` (`class Name {`) misses local classes with a base clause or a
   renamed pad struct; the auditors checked those by hand. The landed `CTCBase::RegisterTCs` 0x004EE23F would itself fail
   strict mode (75 local classes that have engine headers).
10. Behaviour tests are standalone models of each function's logic (the lane convention); they do not link the row source.

**Owner review items (landing need not wait, but a rejected item means the row comes back out).** Every README lists its
rows' FITTED spellings and devices; the heaviest are: 0x00CBFB7D (36 dead asserts with invented conditions + a dead assert
arm inside the shared-header StartsWith opt-in), 0x00D0A830 (nested `do { } while (0)` wrappers and `goto Terminate`),
0x00E50FD0 / 0x00E3BB70 (8 identical dead asserts each), 0x00652FA4 (fitted `CIntelligentPointer::operator->` opt-in),
0x005FF190 (four no-byte loop guards + a force-inline map destructor opt-in), the `/d2inlT<N>` rows, the new asm kernels
`GF_SSE_MATRIX_ALIGNED_TRANSFORM_TRANSPOSED_ASSUME_W` and `LIBPK_MMX_COPY64` (0x00A5CB90, shared-header kernels with
FABLE_PORTABLE twins), and FABLE_RETAIL_CMATRIX3X4_SETINVERSEUSRT_SCALE_SQUARED. The owner's acceptance test is in the
section above.
