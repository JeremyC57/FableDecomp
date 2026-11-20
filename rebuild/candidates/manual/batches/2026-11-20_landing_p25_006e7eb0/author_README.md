# 0x006E7EB0 `anonymous namespace'::GetPreferredCombatDistanceForAttacker (NEW catalog row)

**Grade:** RELOCATION_MATCH (57/57). No flag, no pragma, no device, no header overlay.
- **Strict vland:** passes (`vland.log`). It ran with LANDVERIFY_STRICT_HEADERS=1, an empty overlay and a fresh LANDVERIFY_WORK.
  - **Shadow root:** `FABLE_ROOT` was a symlink tree of the repo, with real copies of `functions.tsv`, `boundary-starts.tsv` and `manifest_add_boundaries.py`. The boundary line below was appended to the copy, and the command below was run there.
  - So the log is against the repo manifest plus this one row. Against the bare repo the row is OUTSIDE_MANIFEST until main applies the line.
  - The one warning is RELOC_NAME: the labels DB has the old name for 0x0129BA3C. It is in NAME_FIXES.tsv, as in the 0x006F2DE0 batch.
- **Retail:** 0x006E7EB0..0x006E7EE8 (57 B), measured with `rdis.sh`.
  - The int3 pad runs before it (0x006E7EA5..AF) and after it (0x006E7EE9..EF).
  - It starts with `push ecx` and ends with `pop ecx ; ret` at +0x37.
  - It is **unreferenced**: `xref.py` finds no E8/E9 site, and there is no absolute dword 0x006E7EB0 anywhere in the file.
- **Name:** Ego_r.pdb / FableWin.pdb `?GetPreferredCombatDistanceForAttacker@?A0xd8b789b9@@YIMABVCThingCreatureBase@@0@Z`.
  - Ego_d gives the parameters (defender, attacker), `float distance` (type 0x40), combat_def, and a block { far_range, middle_range }.
  - Ego_d lines 70..85 (`egod_lines/`, debug length 126) follow the text's statement order.
- **Catalogue precedent:** module `_global` and the function in `namespace { }`, with leaf `?GetPreferredCombatDistanceForAttacker@`. This is as landed 0x006E9F60 / 0x006E7F00.
- **Text:** verbatim the landed 0x006F2DE0 file's helper. A script compared the 13 normalised lines.
  - That landed file's helper COMDAT also grades RELOCATION_MATCH against this oracle (`evidence/landed_006f2de0_copy.txt`).

## Manifest (main applies)
1. Append `boundary-starts.line` (one TSV line) to `rebuild/manifest/boundary-starts.tsv`.
2. Run `python3 tools/decomp_pipeline/crawl/manifest_add_boundaries.py --write`. In the shadow root it reported "1 boundary starts to add (57843 manifest rows) / written" and added row 17618:
   `006e7eb0  GetPreferredCombatDistanceForAttacker  _global  unknown  undefined  0  0  discovered  boundary`.
3. Land `payload.json` / `oracle.tsv`. The oracle's 57 bytes come from retail; they are also in `evidence/oracle_override.tsv`.

## Tests
- **`test/006e7eb0.cpp`** is the payload test. It copies the row's statements over stand-ins and passes in vland (self-contained link).
  - This follows the 0x006E7F00 / 0x006F2DE0 precedent: the ?A0x hash depends on the compile path, and vland's path is not fixed.
- **`test/006e7eb0_realobj.cpp`** links the REAL row object through `/alternatename`, with ANON = `f3cb6405`, the hash of mut.py's fixed path `/tmp/claude-0/-home-user-FableDecomp/923afea2-3bc0-53db-a7ad-a8bbc78e03e2/scratchpad/w26/p25M/tmp/mut/m.cpp`.
  - It has the same fixtures and checks: retail layouts, the attacker's def read once, the defender's radius, and preferred distances above, at and below 1e-4.
- **Mutants:** 14/14 KILLED (`mutants/006e7eb0.out`). They cover each operand, the comparison, the midpoint, the accumulation, the return and the local's type.

## Ablation (`evidence/ablations/results.txt`, preflighted by ablq.py; undefined symbols compared with `evidence/undefs.txt`)
| change | grade |
|---|---|
| final source | RELOCATION_MATCH |
| `double distance` with the landed 0x006E89B0 spelling `if ((float)distance < ...)` | RELOCATION_MATCH (same undefined symbols) |
| `double distance` without the cast | DIFFER(61v57) |
| cdecl (the landed 0x006E89B0 convention) | DIFFER(63v57) |
| (attacker, defender) parameter order (landed 0x006E89B0's) | DIFFER(55v57) |
| no FABLE_RETAIL_CTHING | RELOCATION_MATCH (kept, as the 0x006F2DE0 precedent: it selects the retail CThing layout the row reads) |
| no FABLE_RETAIL_CCOMBATTYPEDEF | RELOCATION_MATCH (kept, same reason: CCombatTypeDef offsets +0x6C/+0x70/+0x80) |
| any other define | SRC_FAIL |

- **float vs double is NOT decided by this row's bytes.** `double` with the landed cast spelling also matches.
  - `float` rests on Ego_d's `distance` type 0x40.
  - It also rests on the 0x006F2DE0 expansion: there, `double` gives DIFFER(379v375) (p25K, `batches/006f2de0/evidence/comdat_checks.txt`).
  - p25K's note "double: SAME 57" was measured with the cast spelling; without the cast this row DIFFERs (61 B).

## For the coordinator
- **0x006F2DE0 follow-up.** After landing, the landed 0x006F2DE0 file's helper becomes a same-TU copy of a catalogued row.
  - Its comment block should become `// Fidelity: same-TU strong definition of 0x006E7EB0 (retail TU-mate)`, replacing the Inferred / "No row or label holds 0x006E7EB0 yet" lines.
  - The body is already verbatim, and its COMDAT already matches.
- **0x006E89B0** keeps its own helper copy: cdecl, (attacker, defender) and double, which emits a 63-B COMDAT. That is the re-port already logged.
- **Not chased:** 0x006E7E80 (`mov eax,[ecx+6Ch] ; ret`), 0x006E7E90 (`mov eax,[esp+4] ; mov [ecx+6Ch],eax ; ret 4`) and 0x006E7EA0 (`mov al,1 ; ret 8`) look uncatalogued too. Nothing in functions.tsv lies between 0x006E7D50 and 0x006E7F00.
- newly_flagged_rows.tsv: none, since no header changed.
