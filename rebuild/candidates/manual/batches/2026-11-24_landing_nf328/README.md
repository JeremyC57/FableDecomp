# Landing nf328 (Account A, 2026-11-24): the ~CScriptThing / copy-constructor pick-any unit, 9 rows (NF326 Q4)

Hand-back: `pending/2026-11-24_nf328_handback/` (UNIT.md first); tools: `naming_batch/agent_tools/nf328/`.

## What it does
`??1CScriptThing@@UAE@XZ` (0x004AA840) and `??0CScriptThing@@QAE@ABV0@@Z` (0x004ABE90) are expanded inline in retail's script TUs, and
529 / 296 landed objects emit them as COMDATs. Before this landing, 8 objects defined the destructor strongly (LNK2005 against every other
copy) and the copy self defined its member strongly. Every strong definer now defines the member `inline`, so it is pick-any:
- **0x004AA840, 0x004AB170 (`??_G`), 0x004ABE90:** the member is `inline`, plus one `?A0x` helper that emits it out of line: a qualified
  explicit destructor call, or a return of a copy by value.
- **0x00CE0920, 0x00CE0B00, 0x00CE0CE0, 0x00D151D0, 0x00DDE230, 0x00E6B750:** the strong destructor definition becomes `inline`; nothing
  else changes.

## Rulings (advisor decision, owner delegated)
- **Helper device:** each helper is `#pragma inline_depth(0)` over that one function, restored right after it, below the row's own code.
  Accepted for this unit only.
  - The helpers are scaffolding debt, like NF326's ten. Retail emitted these bodies from caller TUs that did not inline them.
  - At landing the coordinator relabelled the agent's comment line in place, `// Fidelity: scaffolding` -> `// Inline-budget:
    scaffolding` (comment only; the line count is unchanged).
- **D1, debt and not a hold:** 21 emitted copies differ from retail:
  - 19 destructor copies: 14 "vptr-late" in-class bodies, 4 implicit 5-byte bodies and 0x00AF69D0;
  - 2 copy-constructor copies: 0x00CBD310 and 0x00CF68C0.

  They are unchanged at HEAD, and this landing adds none. It is the NF326 Q2 shape. **Pick-any is not yet sound for either member**: the
  final link can pick a differing copy. The 21 are queued as T3 `nf328` debt.
- **D2:** the real `CCountedPointer<CScriptThing>` member fix (one PDB source for its `??1`, measured on 1 of 14 rows) is the follow-up
  pass. It is not in this landing.
- **D3 (accepted):** 0x004AB170 takes a helper. Its object differs from HEAD by exactly that helper, and `??_G` is unchanged (equal to
  retail).
- **D4 (accepted, helper only):** the copy self keeps its local model. Its existing 1-slot `??_7CScriptThing` and short `??_G` copies are
  debt left in place; the T3 `nf328` port line covers them.
- **D5 (done by the coordinator):** the five Codex-reserved census holes were compiled read-only. 0x00CFCE90, 0x00D90290, 0x00E4FA20,
  0x00E96110 and 0x00F00C90 all define the members pick-any and BYTES_EQUAL (`logs/census_d5_reserved_rows.tsv`). 0x00DDB720 is covered
  by NF326. No strong definer remains anywhere.
- **D6:** the `??1CBase` stand-in, 0x0099A2E0, is queued PRIORITY in NAME_FIXES_QUEUE as `??1CBaseClass@@UAE@XZ`.

## Proof
- **Agent:**
  - pair grades HEAD / new: RELOCATION_MATCH for all 9;
  - uproof: only the selection 1 -> 2 plus the one helper; vftable, `??_G`, UND set and `.drectve` equal;
  - census after: 0 strong definers;
  - full /FORCE link of 1,017 census objects: duplicate lines 938 -> 115. The removed lines are exactly 528 `??1CScriptThing` plus 295
    `??0CScriptThing`; none were added, and the unresolved set is identical.
- **Coordinator, at landing:**
  - every `orig/` copy equals the repo;
  - no claim or Codex reservation touches the 9 rows;
  - recheck on the landed tree: 9 RELOCATION_MATCH PASS (`logs/recheck.log`);
  - on those objects:
    - census: every member DEF selection 2, BYTES_EQUAL with retail (`logs/census_landing_objects.tsv`);
    - pickany_vs_retail: 18 EQ, and the same 4 NEQ as HEAD, none leaf-referenced;
    - DEF / UND symbol sets equal to the agent's proven objects.

## Queue
- **T3:** the two `nf326 ... unit` lines are closed (`T3_CLOSED.tsv`); the port lines stay. 24 `nf328` lines:
  - the 21 debt copies;
  - the D4 model port;
  - the `??4CScriptThing` operator= unit (75 duplicate lines);
  - 0x004AB170's missing `// Leaf:` line.
- **Report only:** 108 census objects emit a truncated pick-any `??_7CScriptThing@@6B@` (slot count is not retail's 79). This is
  local-model debt, reported to B in the log.
