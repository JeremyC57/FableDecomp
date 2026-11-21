# FableDecomp -- Codex hand-off pass 25 (2026-11-21): 8 large stalled rows

## Who you are and what is asked
You are a professional decompiler of Microsoft Visual C++ 7.1 output (Visual Studio .NET 2003, `cl` 13.10.3077 RTM). For every row in
`ROWS.tsv` the goal is C++ that this compiler turns into the **exact retail bytes** of PC `Fable.exe` (Fable: The Lost Chapters, Steam,
SHA-1 `39fd416ba23a91a89713ec80c789f57ddb22e2c8`, ImageBase 0x400000), written the way the original Lionhead / Big Blue Box engineers
wrote it: their classes, member names, headers and idioms. The owner's words: "readable clean c++ that is portable and byte matches
retail code and behaves the same as retail."

These 8 rows are reserved for you (`RESERVED.tsv` in the repo's `pending/2026-11-21_codex_pass25/`); the project's own agents have been
taken off them. Every one is large (2.6-14.5 KB) and close: what is left is a compiler decision (inline split, frame-slot packing,
register choice), not missing code. Several agents have already worked each one; their measurements are in each row folder. Do not
repeat a recorded dead end.

## The rows (`ROWS.tsv`; per-row detail in `rows/<addr>/ROW.md`)
| Row | Function | Size | Best | Residue class |
|---|---|---|---|---|
| 0x00E20600 | NScript::CQR_EscortTraderScript::CTraderToEscort::Main | 14512 | 14526v14510 | inline split: two predicate dtors called in retail, expanded in ours |
| 0x00795300 | CTCUnholyPower::FrameUpdate | 9312 | 9291v9268 (Ego_d base 9292) | stack frame-slot packing (5 tie groups) |
| 0x0078FD60 | CTCDivineWrath::FrameUpdate | 9280 | twin of 0x00795300 | same; solve 0x00795300 first |
| 0x00F1EED0 | NScript::CQ_ArenaScript::PlayWave | 9264 | natural 11221v9262 | per-site inline split (~CTimer / ~vector), frequency model |
| 0x00C4F600 | CEAXManager::SetEAXProperty (descriptive name) | 9216 | 9204v9204 | one register-piece rotation; no text oracle |
| 0x00B0DC20 | NParticleEngine::CPSCUpdateNormal::Update | 5616 | 5604v5602 | see notes |
| 0x00D01630 | NScript::CQ_BanditCampScript::CGate1GuardOuter::Main | 5184 | 5161v5178 | an inline expansion must be forced |
| 0x0064FDB4 | NPlayerGui::CDrawQuestInfo::CQuestInfoElementBar::DrawAt | 2593 | 2593v2593 | see notes |

Each row folder has:
- `ROW.md`: extent, where it stands, file list, callers, landed same-TU neighbours.
- `best_acctC_or_B_draft.cpp`: the draft on record. **Read the residue summary first**: several drafts reach their grade only with
  devices the rules or Ego_d reject (named in ROW.md); for those, the `B_base_*.cpp` / `B_natural_base_*.cpp` file is the admissible
  starting point even though it grades worse.
- `overlay_*`: the header overlay that draft was graded with (drop over `include/` for that row only; most of it is already in `include/`).
- `egod_locals.txt`, `egod_lines.txt`, `egod_tu.txt`: Ego_d evidence (below). `neighbours/`: landed same-TU rows. `prior/`: every earlier file.

## Evidence sources (use them; each name needs two sources)
- **Ego_d.pdb** (download: @@EGOD_PDB_LINK@@; SHA-256 `d7aa26ef34d4c1857e7cebf035f4a897b77751b9d5ca308535a618062efbeae1`) is the VC10 /Od
  debug PDB of the same source tree. Already extracted per row:
  - `egod_locals.txt`: parameter / local names, types and `{ }` scope blocks. No local = the source wrote one expression. Frame offsets are
    VC10's, not retail's.
  - `egod_lines.txt`: per source line, the debug byte count (`+offset line bytes`). It is a size oracle for statement shape: compile your
    text at VC7.1 /Od and compare, using the calibration in `docs/FINDINGS_LOG.md` (2026-11-20/21 lines: VC10 adds ~+2 per virtual call, +5 per
    float-constant argument, +3/+4 per float ctor argument, -2 per constant float store; an oversize line with no locals is usually a
    debug-only check).
  - `egod_tu.txt`: the row's object file in Ego_d order, with landed retail addresses.
  - Tools: `tools/egod_locals.py`, `tools/egod_type.py`, `tools/egod_proc.py` + `tools/egod_lines.py` for any other function.
- **Name lists** in `names/`: `egod_procnames.txt` / `egod_datanames.txt` (every Ego_d function / data symbol), `egor_pdb_names.tsv` and
  `fablewin_pdb_names.tsv` (the two other PDBs: mangled names with addresses), `struct_layouts_egor.tsv` + `fable_types.h` (3,807 classes,
  real member names), `egod_carved_functions.tsv` (object + order), `rtti-vtables.tsv`, `reloc_bindings.tsv` (which retail address binds which
  symbol), `catalog_names.tsv` / `landed_sources.tsv` (every landed row), `labels/`, `fse_api_manifest.json`.
- **Headers** in `include/` are the project's tree as of this package (every landing through 2026-11-21).

## Rules that bind these rows (docs/ADVISOR_DECISIONS_B.md has the full rulings; docs/EXTRA_RULES.md overrides older text)
1. Genuine compiler-emitted C++ only: no `__asm` / `naked` / `_emit` / `register`; `volatile` only where retail used it.
2. Shared headers for `this`; header changes are ADD-ONLY behind new `#ifdef FABLE_RETAIL_*` opt-ins, default path unchanged; move a
   `FABLE_STATIC_ASSERT` only by pinning it (`tools/lane_merge/pin_asserts.py`). Hand header changes back as old-text / new-text pairs.
3. Inline devices (owner, 2026-11-08), only for residues that are purely inline decisions, in this order: plain C++; a per-row
   `/d2inlT<N>` / `/d2inlS<N>` flag; a scoped source control (`#pragma inline_depth` region around ONE statement, `auto_inline`,
   `__declspec(noinline)` / `FABLE_FORCEINLINE` on a row-local helper). Each needs a `// Inline-budget:` line. Not allowed: a row-local
   definition of a member a shared header already models (put it in the header as an opt-in); never a bare `__forceinline`.
4. Text Ego_d contradicts (an absent or extra local, a contradicted statement order or line size) is not landable, not even as
   `// Inferred:`. A "different revision" lifts this only per block, by a mechanical test: Ego_d's line decodes (calibrated size) as a call to
   a named function that retail's function never calls, or the reverse. Untestable blocks stay bound.
5. Fitted choices Ego_d does not contradict may land as `// Inferred:` with the measurement; fitted counts the PDBs contradict may not.
6. Names exactly as in the PDBs (two sources); a descriptive name only where none exists anywhere (0x00C4F600).

## What the project already measured about VC7.1 (read docs/FINDINGS_LOG.md 2026-11-20/21)
- c2 inline-site frequency multipliers (for-loop x5 body, while x9 body, if-arm x0.5, `&&` x0.25 ...).
- c1 sets the inline-info flag for every function under /Og; c2 takes the "budget path" for a callee not yet compiled (running total
  35000; accepted when callee size <= 40 or <= limit); c2 compiles callees first (topological sort, cycles broken at back edges, vtable-only
  functions later). Retail's COMDAT placement does NOT show compile order (retail TUs put all source functions first, then COMDATs).
- The caller-size inline penalty is function-global (position of dead code does not matter).
- c2's stack packer weight is +1 per memory reference, no frequency weighting.
- The harness that measured these (gdb on c1xx.dll / c2.dll under Wine) is in `tools/c2_symbol_order_kit/tools_c1harness/` (README inside).

## How to work and what to return
- Cut retail bytes from YOUR Fable.exe with `tools/cut_row.py`; compile with the real VC7.1 RTM cl.exe (`/nologo /c /O2 /Oy /W3`, plus
  any flags a same-TU neighbour uses); compare with relocated operands masked, then check every relocation names the right symbol.
- At least 10 genuinely different source-shape attempts per row before giving up on it; log each in `rows/<addr>/PROBES.md`.
- Return per row: `<addr>.cpp` (one TU, `// Leaf:` line, Ego_d-consistent text, labelled `// Inferred:` / `// Inline-budget:` lines),
  `<addr>_test.cpp` (self-contained behaviour test, C++03, builds /Od /W3 without /EHsc, prints `<addr>_TEST PASS`), header patches,
  NAME_FIXES.tsv for any landed spelling you prove wrong, and STATUS.tsv (row, grade, one-line residue). No retail bytes, game files or
  PDB contents beyond the names and types you cite.
