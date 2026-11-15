# START HERE — Pass22 final handoff for Claude

This is the complete hand-back for the **100 reserved rows in the 2026-11-14 package**. Work is stopped at the owner's request. **No Git commit or push was made.** This archive is the current handoff; historical checkpoints inside are evidence, not competing instructions or newer selections.

## Result and what can be landed

- **87 reservations: RELOCATION_MATCH**, with selected compiler output, exact emitted symbols, relocation-identity/extent reviews and behavior tests. Regrade them with your compiler and verifier before landing.
- **6 reservations: PARKED**, meaning unfinished at owner-requested wind-down, not abandoned after a token attempt. Every one has18–56 conservatively recorded source-strategy families plus further variants. Their best sources and full available histories are preserved.
- **7 reservations: EXTENT_WRONG**. They begin inside already-landed functions; the real starts and parent evidence are recorded. No fabricated fragment source or extra match credit.
- **No valid reserved function remains untouched.** Successfully matching a function does not require ten more attempts. Invalid fragments received boundary work rather than ten meaningless compiles.
- Progress uses the original100-row denominator: **87% functions**. Unique verified complete-function bytes: **92,329**. Unique matched reserved bytes: **91,869/105,353 =87.20112384%**. Complete corrected functions add460bytes beyond reservations; a77byte parent/handler overlap is counted only once. Helper bytes never increase these totals.

These are local reviewed results, not a claim that they have been merged into your repository. Some matches have `retail_asm`, library-integration, parent-function or compiler-generated EH qualifications; retain each ROW.json classification and specialist evidence. They are not all ordinary standalone C++ leaves.

## Read in this order

1. This file and `STATUS.tsv` (the authoritative final selection/status table).
2. `UNMATCHED_ROWS.md`, `ROW_DETAILS.md`, and each selected row's `ROW.json`, `BUILD.json`, `PROBES.md`, `NAMING_EVIDENCE.md`.
3. `hdr/README.md`, `hdr/CHANGES.json`, `hdr/OPT_IN_USERS.tsv`. Merge the per-header patches rather than overwriting your newer headers.
4. `NAME_FIXES.tsv` and `NAME_FIXES_CONFLICTS_REVIEW.json`; unresolved historical disagreements are explicitly separated, never silently chosen to quiet a checker. `LABELS.tsv` identifies selected leaves.
5. `LESSONS.md`, `validation/`, and the relevant original rules in `reference/README_CODEX.md` and `reference/docs/`.

## Reproduce the selected source exactly

All best/finished sources and tests are directly under `rows/<address>/`. `BUILD.json` specifies the selected frozen header profile, flags and dependencies. For an isolated, exact original context:

```sh
python3 tools/materialize.py --row 00f3e120 --out /your/work/f3_selected
```

The resulting `include/` is the supplied baseline plus that exact frozen overlay. For the87 accepted rows, `hdr/` is the final globally regressed overlay45. The six unfinished rows have additional private changes; **do not blindly merge them into the accepted overlay**. Reconstruct their contexts separately.

Use **MSVC7.1 RTM cl13.10.3077**, base `/O2 /Oy /W3`, plus the row's actual extra flags and correct Leaf. Use your own retail executable with SHA1 `39fd416ba23a91a89713ec80c789f57ddb22e2c8`. Cut the proved extent, compare with relocated operands masked, and independently check the target and addend of every relocation. Parent and funclet rows need their recorded special grader/boundary rules; the common largest-function fallback is not safe.

Tests use VC71 `/Od /W3 /GR-` without EHsc and GCC64 C++03. They have controlled external contracts, not a whole running game. Consult per-row test scope; exact source-body comparisons and behavior cases support different claims. The SIMD row00B98E10 needs `FABLE_PORTABLE` for GCC and an output-trace filename argument for either test executable. Inventory00590235's current test uses the corrected `test_include` exported here, corresponding to the former `continue_a_test_include`; the obsolete adapter set is historical only. `validation/TEST_CONFIG.json` records these details.

No game executable, retail byte cuts, PDB binaries, object files, test executables, compiler, Wine installation, Git history or private instrumented compiler is included. Existing shared helper source required by the approved retail-asm rows is included, with its portable branch and provenance. Any raw byte payload in evidence JSON was removed with a hash/length marker; summary facts and names are retained.

## Remaining six — best versions and next work

| Address | Function | Best root | Remaining obstacle |
|---|---|---|---|
|00B6FA40|CEngineWaterRenderer::RenderSeaMidground|2019/2019 bytes;3 differing bytes|TEST operands, OR destination and corresponding store; same issue in Background. Selected `continue_c47_original_plain_state_budget128`.|
|00B70230|CEngineWaterRenderer::RenderSeaBackground|2138/2138 bytes;3 differing bytes|Same three-instruction operand/destination pattern. The older note saying9instructionlines/3blocks is stale; current independent comparison proves3bytes/3lines in one block.|
|00BF0B20|CEnginePrimitiveRenderUtil::SetupOutlineStencilStates|1290/1290;8 differing bytes|One blue-channel load is scheduled differently. Selected `main72_original_state_guard_and_formals_preserve_defaults`.|
|00F3E120|CTriangleStripifier2::FindBestStartVertex|1411/1411;8 differing bytes|Two pairs of EDI/map versus EBX/this restores reversed. Also three actually reached bool-container body debts:82vs49,268vs287,201vs177. Selected `main_joint179_original_vendor_scope_chain`.|
|0064BFBA|NPlayerGui::CDrawHighlightingTargetingCursor::Draw|2217/2223;4difference regions|Height/dimension call order, with a six-byte length deficit. Selected `main_joint12_current_colour_copy_inline`; actual RGB copy-chain corrections are retained.|
|00675E20|CCreatureDef::Transfer|1912/1912;184unmasked differing positions,34regions|Scratch EAX/ECX/EDX allocation differs repeatedly. Selected `main75_correct_original_types_primary`; full-generic74 and newly reviewed bool94/96 are alternatives.|

Allsix have extensive prior source and flag attempts. Do not repeat a rejected form merely because its metadata is stored in another worker directory. Start with each PROBES.md and `history/INDEX.json`.

### Last F3 owner-requested tests

All seven180–186 were actually compiled before switching away. Zero-left equality180, address-of-front pointer arithmetic181, full-expression const reference183, multiplication reversal185 and declaration-only reorder186 keep the same8bytes. Initialized-local reorder182 worsens to18bytes; nonconstant equality reversal184 worsens to9. Both original load pairs remain wrong in all seven. All previous attempts and counts are preserved:228 actual target compiler invocations (including separately counted diagnostics),42 conservative source families. This is not228 distinct strategies.

The real original inner names `poly`, `id`, `id1`, `id2` are proven in FableWin debug lexical scopes; the earlier negative assertion looked only at optimized Ego scopes. Preferred179 includes the corrected comment and authentic protected/in-class STLport helper scopes. All25 actually reached bodies are mapped,21whole exact; there is no hidden/unmapped helper credit. Five genuine engine helpers total4059exact bytes, excluded from headline progress. Behavior/ABI/default-profile scope and exact equivalence to tested171 are documented;179 runtime tests were not falsely claimed as rerun.

### Last creature-persist result

Worker94 restores the actual complete CPersistTraits<bool> interface and bodies from both original PDBs/C13/donor evidence. It emits a **whole114-byte exact reached input helper at00403EB0**, including its sole canonical00993CA0 ReadWithSrcChunkOverflow binding. Main96 independently rebuilt and verified it. Root bytes still have the same residue. The helper gains no reserved-row credit.

This is a **reviewed alternative, not silently adopted as selected75**. Its315,394 fixture cases passed on each compiler; Read/ReadType/ReadEBOOL and the real bool-traits body execute, while overflow remains an external contract.171 paired header profiles pass (167successful preprocess pairs and4identical pre-existing prerequisite errors). Use `review/joint_persist_a/bool94` via the research materializer and `validation/BOOL94_MAIN_REVIEW.json`.96actual target compiles comprise93ordinary grades,2PCH consumers and1older diagnostic; test/helper/proxy-builder compiles are separate.

The final read-only C2 investigation proves opcode2D3 is a zeroed/no-operand marker that resets scratch cursors, but **does not establish its source meaning**. A supposed opcode mapping was actually diagnostics C4723/C4724 and was retracted. Next investigation would identify the real input-record dispatch or passively correlate the marker with known source. Do not invent statements solely to reset allocator state or modify the compiler's decisions.

## All histories and findings

`history/INDEX.json` lists every available graded checkpoint, including failed compiles and repeated merged-header regressions. Those records are not a count of distinct strategies. `history/attempts/<address>/<tag>/` contains each RESULT plus content references for its exact candidate, compile logs, relocation table and dependencies. Restore one with:

```sh
python3 tools/materialize.py --attempt 00f3e120/main_joint184_successor_equality_operands_reversed --out /your/work/f3_old_trial
```

`history/RESEARCH_FILES.tsv` indexes all available text work from the rows and review directories: drafts, best alternatives, naming/layout proofs, header-contract checks, validation logs, probe scripts, private-diagnostic findings, and prior failure records. The content-addressed text store avoids duplicating the same headers thousands of times. Restore a relevant subtree with:

```sh
python3 tools/materialize.py --research-prefix review/joint_persist_a/bool94 --out /your/work/recovered
python3 tools/materialize.py --research-prefix rows/00f3e120 --out /your/work/recovered
```

Historical source variants can be rejected or incomplete. **Only final selected rows plus their current validation receipts are landing candidates.** Do not treat a historical MASKED_MATCH_PENDING_AUDIT as an accepted match, or a compiler diagnostic as a permissible reconstruction. Some original scripts contain workspace-specific paths and need adapting; the materializer and manifests are portable Python3.

`SHA256SUMS` covers every archive file except itself. Verify it before review. Check `PACKAGING_INVENTORY.json` and `validation/FINAL_VERIFICATION.json` for the exact packaged counts and fresh test results. Keep all rejected strategies and measured dead ends when continuing; none of the six needs to be restarted from scratch.
