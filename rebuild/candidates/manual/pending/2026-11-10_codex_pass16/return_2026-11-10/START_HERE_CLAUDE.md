# Start here — Codex 40-row return, 2026-11-10

This is the hand-back for exactly the 40 functions in `fabledecomp_codex_2026-11-10.zip`. The owner requested that work stop after the last couple of trials. All workers have stopped. No commits, pushes, or other repository writes were performed.

**37/40 functions (92.5%) are verified matches.** Each has a final unmodified MSVC7.1 RTM rebuild, complete-extent byte comparison using actual COFF relocation masks, an independent target/name audit, and passing behavior tests. These represent **11,258/12,261 assigned bytes (91.82%)**. This is this batch's percentage, not the project's overall percentage.

**Three rows remain unfinished:** two near misses and one conversion blocker. All 40 assigned rows were attempted; none are untouched, skipped, or excluded for bad extent. The conversion blocker uses status PARKED in the required status schema and is explicitly marked BLOCKED in its residue.

## Verify and land

1. Read `STATUS.tsv`, `LABELS.tsv`, `NAME_FIXES.tsv`, and each row's `NAMING_EVIDENCE.md`. Catalog names were treated as hints; unresolved names are identified rather than claimed as recovered.
2. Overlay `hdr/` onto `include/` from the original owner package, preserving paths. Read `hdr/README.md`. Compile only the selected `rows/<address>/<address>.cpp` as a target, with base `/O2 /Oy /W3` plus its source flags. Use **cl13.10.3077 RTM**.
3. Extract the full exact `// Leaf:` symbol. Compare the entire assigned extent, masking actual COFF relocations only. Audit every call/global/string/table operand; `MAIN_AUDIT.json` or `RELOCATION_AUDIT.json` records the target evidence. `FINAL_GRADE.json` is the final packaged-source result. The final regrade also compared every matched object's code and relocation records against its independently audited earlier build. For original anonymous-namespace symbols, normalize only the compiler-generated translation-unit hash, never the signature.
4. Re-run each standalone test under VC7.1 `/Od /W3 /GR-` and g++ `-std=c++03`. All 40 selected tests passed fresh main runs at wrap-up. Each prints exactly `<address>_TEST PASS`. These are behavior fixtures with controlled external services, not whole-game tests. `test_history/` contains actual compiler/run logs; `TEST_RESULT.json` identifies the selected fixture hash.
5. Land only `VERIFIED_MATCH` or `RELOCATION_MATCH` rows. Recheck names before binding. Do not land the three PARKED rows or any historical `attempts/` candidate as a match.

## Newest successful recoveries

- **006509f8 — CQuestInfoElementCounter::DrawAt:** 549/549; branch-local double result values preserve the original common return conversion; 24 operands independently reviewed and 216 behavior cases.
- **00712900 — NTCScriptedControl::CActionPlayAnimation::Update:** 362/362; original debug evidence put initialized `action` at function scope and guarded deletion after both outer arms. That lifetime/control-flow correction fixed the remaining load choice. Original `GetParentCreature` and scalar `CSoundPair(0)` fidelity were restored too. 11 operands reviewed; 1,024 behavior cases, plus optimized worker runs.
- **008c7280 — CCreatureAction_TrollGroundPound::Initialise:** 268/268; exposing the genuine complete STLport4.6.2 `_Rb_tree::_M_insert` body, alongside `insert_unique`, changed the same six local objects from two slots to the native 4+8+8-byte frame. No artificial storage or callee effects. 15 operands reviewed; 224 exact-body cases. Separate real-header insertion tests checked 256 insertions plus duplicates at VC/Od and/O2, with balancing as a controlled external boundary.

## Remaining rows and strongest next leads

| Address | Selected result | What remains |
|---|---|---|
|009397e0|569/571; one instruction absent|Native pre-call world-seed store is removed by the portable visible rotate helper. A 571-byte alternative retains it but swaps this/seed saved-register roles. Compiler traces measured priorities52 versus45 for that alternative. Original GFRandom/debug caller, frame formula, sound constructor and SetOutput identity were recovered. All27 operand targets reviewed; 1,448-case fixture passed at FPS2 andFPS0 under VC/Od/O2 and g++O0/O2. More grouping, branch, receiver and intrinsic trials did not improve selected240.|
|00c11520|163/163; one store/push pair differs|X store occurs after the packet-address push. Eight final independent attempts included genuine lookup-body visibility, constructor/copy forms and coordinate loops; none improved it. Selected source unchanged. Native LHAudio/libpk private identifiers and unused virtual signatures remain descriptive/unproven; do not treat them as original PDB recoveries.|
|00c6b9d0|245/269; conversion blocker|Native uses a bare dword x87 conversion. Ordinary casts and available intrinsic/library approaches, including the qword QIfist route, did not reproduce it. Selected portable finite-domain behavior passes its tests but is not a byte match. Do not replace it with a smaller incorrect conversion just to improve length.|

For each remaining row, read PROBES.md and BUILD_HISTORY.tsv before retrying. All parked rows exceeded the minimum ten distinct mechanisms. Strategy counts are conservative categories, not compiler-invocation counts; flags, spelling repairs, regrades and equivalent variants are not counted as new ideas.

## Naming and header findings

- Exact PDB spelling, owner/class key, const/access/convention and instantiated types are recorded per row. `NAME_FIXES.tsv` lists wrong historical bindings and their corrections; provisional private proposals are labeled in their evidence.
- Campfire's0088b8f0 callee is **CAIBrain::SetOutput**, not SetBrainOutput, proved by both PDB name sets and the original debug caller. Its three delay/animation globals are proven by typed module-data records plus donor cluster order. The old generic global spellings are recorded in NAME_FIXES.
- `CSoundPair()` is empty; `CSoundPair(long)` initializes both fields. The new opt-in preserves that original distinction. Default shared-header branches remain unchanged.
-008c7280's original GetSubDef/definition ownership and CTCHaste map types were recovered. Exploratory alternate CDefPointer bodies were not required and were not merged.
-00c5a9d0 is matched, but one native EAX-pointer field name is explicitly inferred; its pointer type and offset have independent native/SDK evidence. Read EAX_LAYOUT_REVIEW.md.
-00d259a0 is matched; its supporting state setter is behavior-audited, not a separately completed target row.
- The final package check caught missing `rebuild_abi.h` includes in the two fractal sources after portability-macro substitution. The explicit dependency was added and final code/relocation equality rechecked. This was a packaging repair, not a new algorithm.

## Files and history

`rows/` contains all40 selected sources/tests, grades, audits, naming evidence and attempt notes. `hdr/` is the final merged overlay. `STATUS.tsv` is the authoritative disposition list. `UNTOUCHED.tsv` is intentionally header-only: none remain. `HISTORY_README.md` explains **662 build records and 647 archived source snapshots**, with deduplicated historical headers. Historical candidates are experiments, not approved landing sources. Inherited/diagnostic code violating current banned-token rules is omitted as source but its hash, outcome and omission reason are preserved. Earlier owner-supplied drafts remain in the original input package.

`SHA256SUMS` covers every returned file except itself. No compiler, Wine installation, object or executable, game file, retail byte dump, PDB, or private diagnostic backend is included. All agents are stopped; this is the final handoff for this run.
