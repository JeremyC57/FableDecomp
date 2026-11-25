# AUTH26: Account A hand-authoring agent on parked rows (2026-11-25)

## Owner's orders for this run (verbatim intent)

> "launch an authoring agent that works on parked rows. it does not re park or switch rows until it matches and passes behavior tests
> to match retail behavior. make sure it follows the rules for writing the code. and have it focus on hand authoring as much as
> possible rather than automated tool calls."

What this means for you:

1. **One row at a time, in the order below.** You may not open the next row until the current row:
   - grades MATCH or RELOCATION_MATCH on the real row, with strict headers;
   - and has a behaviour test that passes under `vt.py`.

   "Behaviour test" means a test that checks what retail's function DOES: its return values, the state it writes, the callees it
   calls, its branches. Work these out from the disassembly. A smoke test that only links or calls the function once does not count.
2. **Never re-park and never switch.** A row is never left as DIFFER for another row.
   - If you are truly stuck, write `<addr>/ESCALATE.md` and return to the coordinator. That means 25 grades in a row with no drop in
     the residue, or every idea left contradicts a rule or Ego_d.
   - The coordinator takes it to the advisor and sends you the answer, and you resume THE SAME ROW.
   - The row stays yours and locked to AUTH26.
3. **Hand authoring, not automation.**
   - Read retail's disassembly (`xdis.py`), understand the function, and write or rewrite the C++ by hand.
   - Every grade is a source you wrote deliberately for one stated hypothesis (one line in `<addr>/LOG.md` before the grade).
   - Grade with `q.py`, one source per call.
   - Do not write scripts that generate, mutate or sweep sources. Do not run flag or inline-threshold sweeps, or brute-force
     permutations of statements or declarations. Do not run multi-variant sprays with `fg.py`.
   - Small read-only helpers are fine (`xdis.py`, `xref.py`, grep, the PDB symbol dumps). So are `vt.py` and
     `addmatch.py` at the end.
   - If a flag or pragma question comes up, settle it from evidence and then grade the one setting the evidence points to. The
     evidence is the TU's other landed rows, Ego_d, and the GOTCHAS lines.
4. **The code rules are not negotiable** (Hard rules below). A match that breaks one is not a match.
5. **Correct names and headers as you author** (owner, added 2026-11-25): "ensure that the rows being authored have correct naming
   and headers as they are authored so that no more work is added to the naming queue". A row is done only when `name_gate.py`
   prints `NAME_GATE PASS` (section "Naming and headers" below). Byte match + passing test + NAME_GATE PASS, all three, before the
   next row.

## Paths
- **Repo: read-only.** `/home/user/FableDecomp`.
  - Never edit, create or delete anything in it.
  - Never run git, not even read-only (`git log` and `git show` included).
  - Each row's history (files and commits) is precomputed in `$S/w22/auth26/HISTORY.md`.
- **Lane:**
  ```
  export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
  source $S/wine/env.sh; export S; unset TMPDIR TMP
  ```
  Work with cwd = the repo root. Never run `wineserver -k`.
- **Your directory:** `$S/w22/auth26/`, with one folder per row (`<addr>/`). The rows and retail lengths are in `rows.tsv`.
  The oracle bytes are in `$S/batch/auth26_oracle.tsv`. That file is scratch only: retail bytes never go into the repo or your
  hand-back.
- **Locks:** your 10 rows are locked `AUTH26` (`$S/w22/nflocks/<addr>/owner`).
  - Before you touch any other row (a helper or a landed sibling), claim it with
    `mkdir $S/w22/nflocks/<addr> 2>/dev/null && echo AUTH26 > $S/w22/nflocks/<addr>/owner`.
  - If another owner holds it (a live `NF4xx` or `AUTH*` name), leave it and record that.
  - Naming agents NF424, NF425 and NF426 run beside you. Never touch their rows or their overlay files under `$S/w22/agentNF42x/`.
- **Disk is very tight: about 4 GB free on a fixed allowance.**
  - Delete your own objects and scratch as you go, using literal paths. Check `df -h /` before each row.
  - Never write llvm-pdbutil dumps to disk. Grep the existing dumps, or pipe pdbutil output through grep.

## Tools (`$S/tools/`)
- `python3 $S/tools/q.py <addr8> <src.cpp> [leaf] [--flags ...]`: grades one source.
  - The result is RELOCATION_MATCH or MATCH (good) or DIFFER(ours v retail), with a side-by-side disassembly.
  - Wrap every grade in `timeout 900`. A missing `#include` makes the lane's cl.exe hang (GOTCHAS), so check includes first.
  - `-v` always prints the side-by-side.
- `python3 $S/tools/xdis.py <va> <len-hex>` disassembles retail. `xref.py <va>` lists callers and absolute references.
- `python3 $S/tools/vt.py <addr> <test.cpp>` builds and runs a behaviour test with the lander's VC7.1 check.
- `FABLE_HDR_OVERLAY=$S/w22/auth26/hdr python3 $S/tools/name_gate.py (repo copy: tools/decomp_pipeline/linux_lane/lane_tools/name_gate.py) <addr> <src.cpp> <full mangled leaf> --test <test.cpp>
  [--descriptive $S/w22/auth26/DESCRIPTIVE.tsv] [--local-ok $S/w22/auth26/LOCAL_OK.tsv] [--flags ...]`: the naming and header
  gate (read its docstring once). It recompiles the row and judges every name the object uses the way the naming tracker will
  after landing. That covers the self symbol, each callee, global and vftable at the retail address it lands on, and the shared
  header and code rules. The last line is `NAME_GATE PASS` or `NAME_GATE FAIL (n)`.
- `tools/decomp_pipeline/linux_lane/lane_tools/udiff.py <q.py output file>` shows only the differing instructions, and
  `lane_tools/fun.py 0x<va>` shows the EH frame (repo copies; run them from the repo root).
- `python3 $S/tools/addmatch.py $S/w22/auth26 <addr> '<module>' '<method>' <src.cpp> <test.cpp> [--flags ...]` writes a matched
  row in the landing formats.
- Types:
  - `rebuild/include/engine/<Class>.h` (PDB layouts);
  - `ghidra_out/struct_layouts_egor.tsv`;
  - Ego_d type dump: `/tmp/claude-0/egod_pdb/types.txt` (150 MB: grep it, never read it whole).
- PDBs and dumps, never copied anywhere: `/tmp/claude-0/egod_pdb/`.
  - `Ego_d.pdb`: debug, with line tables, locals and blocks; use llvm-pdbutil.
  - `Ego_r.pdb`, with the dumps `Ego_r.symbols.txt`, `Ego_r.publics.txt`, `Ego_r.types.txt` and `Ego_r.modules.txt`: release
    names, types and inline sites.
  - `FableWin.pdb`, with the same dump set: the debug editor.

## Read first
- `docs/pipeline/GOTCHAS.md` (the 2026-11-1x and 11-2x lines at least).
- `docs/pipeline/ASM_RULES.md`.
- `tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md`. Its last section, added today, covers typed-in addresses.
- `docs/journal/2026-11/2026-11-20_ADVISOR_DECISIONS_B.md` (rulings on fitted temporaries, redundant reads and assert arms).
- `rebuild/candidates/manual/pending/2026-11-17_egod_parked/lane/LESSONS_EGOD.md`: the source must not contradict Ego_d's line
  table or code.
- The last AUTH run's lessons: `rebuild/candidates/manual/pending/2026-11-22_auth24/BRIEF.md` section "Read first", point 4, and
  `rebuild/candidates/manual/pending/2026-11-22_auth14/` (the inline model).
- For each row, BEFORE writing anything: its `HISTORY.md` section. Read the newest draft and NOTES, and every negative already
  measured. Do not repeat a variant a note already measured as negative.
  - The `closest_version` in `rows.tsv` is not always the newest draft.
  - The three `egod3a` rows (0091c220, 00935c40, 0068ef50) are old local-model drafts. Port them onto shared headers first.

## Rows (in this order; smallest first)
| # | row | function | retail len | best on record |
|---|---|---|---|---|
| 1 | 0x005DCC00 | CAnimationSet::GetRandomAnim | 264 | DIFFER(278v264) |
| 2 | 0x00723E50 | CCharacter_UpdateOrientation (name it from the PDBs) | 310 | DIFFER(306v310) |
| 3 | 0x006C0B20 | CTCBuyableHouse::FindPosToCreateNewVillager | 368 | DIFFER(383v368) |
| 4 | 0x00644677 | CDrawLetterBoxBlackBars::Draw | 454 | DIFFER(439v454) |
| 5 | 0x008F45F0 | CCreatureAction_DragonSwoop::FrameUpdate | 493 | DIFFER(507v493) |
| 6 | 0x00935C40 | CAIStateGroup_MinionPutOutFires::GoToWell | 502 | DIFFER(499v502), egod3a local model |
| 7 | 0x0091C220 | CAIStateGroup_NymphAttack::IsTargetInLineOfSight | 419 | DIFFER(416v419), egod3a local model |
| 8 | 0x0062BD80 | CWorld::EAControlledCreatureStrafe | 354 | DIFFER(339v354) |
| 9 | 0x009F3DA0 | CSurface::CopyToMemoryB8G8R8A8 | 582 | DIFFER(589v582) |
| 10 | 0x0068EF50 | CInputProcessUseRangedWeapon::ProcessRangedAttackInput | 550 | DIFFER(547v550), /GS, egod3a local model. PARKED_INDEX: "SSE block = retail inline asm: retail_asm candidate (owner-gated)". If retail really used inline asm here, follow ASM_RULES exactly (an intrinsic first, else a macro-wrapped `__asm` in a SHARED HEADER with a `FABLE_PORTABLE` twin and a dual-path test); if even that cannot be settled, escalate. |

Bounds are checked: PARKED_INDEX gives bounds_class OK for rows 3-10 (the corrected extent equals the retail length the grade uses); rows 1-2 are not in it. No row has a TERMINAL.md.
Order (coordinator, 2026-11-25, after row 1): rows 3-10 reordered by real attempt history (fewest earlier passes first; 0x0062BD80 has 28 earlier work folders, 0x009F3DA0 is a Codex pass-26 pick-any hold, 0x0068EF50 a possible retail-asm row). The 2026-11-21 Codex pass 26 RESERVED lines for 0x00723E50 / 0x009F3DA0 are stale (that pass returned and landed 2026-11-22; both rows were parked again) and are released.
The lane is checked: `q.py 005dcc00 <closest_version>` reproduces DIFFER(278v264) today.

## Naming and headers: done as you author, never left for the naming queue
The naming tracker (`tools/decomp_pipeline/naming_status.py`) counts every retail address by the spellings landed rows bind there.
A row that lands with a stand-in, a made-up spelling where the PDB has a name, or a local class model adds open items to the
naming queue. This run must add none. So, from the first draft of each row:

- **Self name.** The exact PDB spelling: class, method, access, virtual, const, calling convention, parameter and return types.
  - Sources: Ego_r / FableWin publics and symbols, the type dumps, RTTI, the FSE manifest, EgoCore, and the spellings the landed
    callers already bind (`grep -i <addr> rebuild/oracles/reloc_bindings.tsv`).
  - Two sources per name. The UNMATCHED_ROWS / catalog name is a starting point, not a source.
- **Every callee, global and vftable** the row references gets its PDB spelling too.
  - Look up what the landed rows bind at that retail address (`reloc_bindings.tsv`) and what the naming tracker says
    (`rebuild/candidates/manual/naming/status/OPEN.tsv`, which has a `pdb_spelling` column).
  - Where the tracker knows the PDB spelling, use exactly that, even when other landed rows still use a wrong one. They are
    already in the queue; you are not adding to it.
  - Where no source names the target at all, a made-up name is allowed only by the made-up-name rule: descriptive, used
    consistently, with a `DESCRIPTIVE.tsv` line (address, spelling, the sources you searched).
  - Never use a stand-in form (`_<addr>`, `FUN_`, `DAT_`, `T::`, `Unproven`, `VirtualNN`, `g_obj_`, `thunk_`).
- **Headers.**
  - Every class the row or its test touches comes from `rebuild/include/engine/<Class>.h` or `libpk/<Class>.h`, or from your
    overlay.
  - A class with no header gets one in `$S/w22/auth26/hdr/engine/<Class>.h`, built from the PDB layout
    (`ghidra_out/struct_layouts_egor.tsv` / `fable_types.h`, the way `tools/decomp_pipeline/gen_class_headers.py` writes them),
    with real member names. Record it in `hdr/README.md` as a NEW header for the coordinator to land.
  - Changes to an existing header are `#ifdef` opt-ins only (Hard rules).
  - No local `struct` or `class` bodies in the row: `name_gate.py` fails them. The one exception is a type retail really
    defined in that TU, with PDB evidence, listed in `LOCAL_OK.tsv`.
- **Gate.** Run `name_gate.py` as soon as the row first matches. A FAIL means the row is not done; fix the names or headers and
  regrade.
  - Renaming a callee or moving to a header can change the bytes. That is part of matching the row, not a reason to keep a
    wrong name.
  - `DEBT` lines are callee addresses that are already open with no PDB spelling anywhere, where you reused the tree's existing
    spelling. They pass. Still look for the real name; if two sources give it, use it and add a `NAME_FIXES.tsv` line for the
    landed binders.
  - If a name genuinely cannot be settled from the sources, do not guess: put the question in ESCALATE.md like any other stall.

## Method per row (by hand)
1. Read `HISTORY.md` and the newest draft and notes, then read the whole retail function once with `xdis.py`.
2. Derive the source structure from the disassembly yourself:
   - statement order, locals and their scopes, early returns;
   - which callees are inline, and their real bodies;
   - parameter and return types, `const`, signedness;
   - the class's real members from the shared header.

   Check every identity (callee, member, global) against Ego_r. Settle names now, not at the end.
3. Port onto shared headers: `#include "engine/<Class>.h"`, never a local model of a class that has a header.
   - If a class has no header, generate it the way the repo does (`tools/decomp_pipeline/gen_class_headers.py`) into your overlay.
   - Otherwise follow the existing header conventions in `hdr/`. Record it in `hdr/README.md`.
4. Grade. Read the differing hunks. Write ONE hypothesis in `LOG.md`, edit by hand, and grade again. Track the residue: the
   differing instruction count and the length.
5. When ten grades leave the same residue, change the LEVEL of the idea instead of the spelling:
   - callee body visibility;
   - the real structure re-derived from the top;
   - identities;
   - the TU's flags or pragmas from evidence;
   - the class's real base and member types;
   - a shared-header opt-in that changes inline cost.
6. On a match:
   - write the behaviour test by hand from what the function does, and check it with `vt.py` (PASS);
   - regrade once with `LANDVERIFY_STRICT_HEADERS=1`;
   - run `name_gate.py` (with `--test`) until it prints `NAME_GATE PASS`. Save its output as `<addr>/NAME_GATE.txt`;
   - run `addmatch.py`;
   - update `STATUS.tsv` and `MATCHES.tsv`;
   - send a short report (see Hand-back).

   Only then open the next row.

## Hard rules
- **Plain C++ that VC7.1 3077 compiles to the retail bytes.**
  - No `__asm`, `naked`, `_emit`, `register` or bare `__forceinline`.
  - `volatile` only where retail used it.
  - No `#error` probes, no `#pragma` you cannot justify.
  - Asm only per ASM_RULES.
- **No typed-in retail addresses.** No string, vftable, global, IAT or function addresses as integers: these are the
  `baked_string` and `baked_address` grades, not genuine. Spell the literal, the class or the bound symbol.
- **The source must not contradict Ego_d** (LESSONS_EGOD). A match that only texts Ego_d contradicts can reach is evidence, not a
  match.
- **Shared headers.** Model `this` with `#include "engine/<Class>.h"`. No new local model of a class that has a shared header, in
  a row or in a test.
  - Header changes go in `$S/w22/auth26/hdr/` as `#ifdef FABLE_RETAIL_<CLASS>_<MEMBER>` opt-ins only, with the default path
    token-identical. Base copies go in `$S/w22/auth26/orig/`, copied from the CURRENT repo header.
  - After editing an overlay header, run
    `python3 tools/decomp_pipeline/linux_lane/lane_tools/lane_merge/pin_asserts.py rebuild/include $S/w22/auth26/hdr <header>`
    and record in `hdr/README.md` what it pinned.
  - Never move a `FABLE_STATIC_ASSERT`. A rename inside a shared header is not yours: note it as "needs header claim".
- **Names right the first time** (EXTRA_RULES).
  - Use exact PDB spellings, two sources per name, one retail address = one name.
  - Made-up names only where no source names it anywhere, descriptive, and marked descriptive in NOTES.
  - No `Unproven`, `VirtualNN_` or `FUN_` placeholders.
  - Name fixes you find for landed rows go in `NAME_FIXES.tsv` (address, correct symbol, evidence x2, wrong spelling). Do not
    apply them.
- **Pick-any COMDAT copies** must equal the owner row's bytes AND relocation targets.
- **Loop prevention for tools:** never repeat a failing command more than twice with the same arguments. The first grade after a
  container restart can time out while wineserver starts: retry once.
- **Every number you report comes from your files.**
- **Describe every command** (owner, 2026-11-25). Give every Bash call a `description`: one plain sentence on what it does and
  why ("Compile g18 with SetUp before SetForward"). Don't echo the command, flags or paths. The owner follows your work through
  these descriptions.

## Hand-back
Keep it current after EVERY grade session, because the container can restart without warning.

- **`$S/w22/auth26/STATUS.tsv`:** address, state (working / matched / escalated), best grade, grades used, best source path, note.
- **Per matched row:**
  - `<addr>/<addr>.cpp`. Its header comment gives what it does, the address and length, callees by address, flags and pragmas,
    and the `// Leaf:` full mangled line.
  - `<addr>/<addr>_test.cpp`, printing `<addr>_TEST PASS`.
  - `<addr>/NOTES.md`: the route to the match, the opt-ins used, and the evidence for every name: the self name, each
    callee, global and vftable, with two sources each.
  - `<addr>/NAME_GATE.txt` (the PASS run).
  - Any `hdr/` changes.
- **`DESCRIPTIVE.tsv` / `LOCAL_OK.tsv` / `NAME_FIXES.tsv`** when used.
- **`MATCHES.tsv`:** address, leaf, flags, source, test. Also `payload.json` and `oracle.tsv` via `addmatch.py`.
- **Report to the coordinator** (your final message, or a return) when:
  - a row matches with its test passing (one line each: row, grade, test result);
  - a row needs the advisor (the ESCALATE.md path);
  - or your context is getting heavy. Write STATUS and NOTES first. The coordinator lands matched rows and restarts you on the
    same row.
