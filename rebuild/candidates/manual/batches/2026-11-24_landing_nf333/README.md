# Landing nf333 (Account A, 2026-11-24): T1 / T2 continuation of NF332, 31 rows onto PDB spellings (35 table replaces, 1 delete)

Hand-back: `pending/2026-11-24_nf333_handback/` (NOTES.md first); tools: `naming_batch/agent_tools/nf333/`.
- **Scope:** 173 free entries (89 standin_alias, 84 signature); 163 were triaged.
- **Results:**

  | status | entries | notes |
  |---|---:|---|
  | done | 35 | 21 changed rows; 14 table-only stale-line fixes (NF309 Q4), each proven from its witness's HEAD object |
  | done-partial | 1 | |
  | done-equivalent | 1 | |
  | skipped-held | 54 | |
  | skipped-port | 72 | |
  | rejected | 0 | |
  | not reached | 10 | listed in NOTES |

## What changed
- **CCountedPointer DeleteData:** the `Destroy_<addr>` stand-ins move to the landed NF282 hybrid form.
- **Two factories** call their constructor with `CThing&`.
- **GFCalcThingUID:** the `__stdcall` stand-in becomes the PDB `__fastcall`. Both 64-bit arguments are on the stack, so the bytes are
  equal.
- **Other rows:** pointer -> reference respells, access / const / return-type fixes, and enum retypes through existing enum-only headers.
- **Names with one PDB set:** three new names are in the FableWin set only, with retail binding evidence as the second source:
  DeleteData<CExpressionSet>, DeleteData<CSpeechFrame> and GFCalcThingUID.
- **Table:** 35 replace and 1 delete (`TABLE_CHANGES.tsv`, `apply_table.py`), 0 misses, no new multi-target symbol.
- No header change and no self rename.

## Proof
- **Agent:**
  - HEAD / new RELOCATION_MATCH at one work path;
  - objsec ALL_EQ, `.drectve` exact, DEF / UND sets equal (0x0095C1F0, see D1);
  - /FORCE links: HEAD leaves the old spelling unresolved, the new set resolves it, and there is no duplicate.
- **Coordinator:**
  - all 32 `orig/` copies equal the repo;
  - `ref_scan.py` over the 35 replaced old spellings (`logs/ref_scan_REFS.tsv`): 31 NONE. The 4 TOO_MANY (`CCharString(CAFile*, long)`,
    `CDateAndTime::Initialise(const _FILETIME*)`, two `TransferBinaryOut(CDataOutputStream*)`) were checked by an exact-signature text
    grep over every landed source and header outside the changed rows: 0 hits;
  - recheck on the landed tree: 31 / 31 RELOCATION_MATCH PASS (`logs/recheck.log`, `logs/recheck_00401f30.log`).
- **Tool fix found at this landing:** `naming_batch/apply_rows.py` missed the source of the 63 catalog entries whose `Source` sits on the
  `Address` line. It wrote 0x00401F30's test but not its source, and that row's first recheck was LINK_FAIL. The coordinator copied the
  source and rechecked it: PASS. The pattern is fixed; it now matches all 57,149 entries. Earlier apply_rows landings reported 0 problems,
  so none were affected.

## Rulings (coordinator)
- **D1 (accepted):** in 0x0095C1F0, including the enum header renumbers the jump table's compiler-local `$L` labels. Their section and
  value, the bytes, the relocation targets and `.drectve` are equal. `$L` labels compare by (section, value), the NF328 uproof standard.
- **D2:** the test placeholder `GVersionString(*static_cast<CAFile*>(0), 0)` bound a reference through a null pointer, which is
  undefined behaviour. The coordinator replaced it with a test-only default constructor on the test's CCharString stub. The spy log is
  reset before every check, so no assertion depends on it. Only the test changes; it passes.
- **D3:** a table line carries the spelling its witness binds (NF308 Q3). Entries 11 and 47-49 are queued as T3 `nf333 ... table`:
  respell each line to its witness binding, then port the binder.
- **D4:** retail is STLport (`char_traits` is a class, `V`), and the binder 0x0083EE10 already binds that form. The self row 0x0083EBD0's
  `// Leaf:` line carries the VC10 Dinkumware `U` form from the PDB name set, so the self Leaf moves to the `V` form. It is queued in
  NAME_FIXES (`nf333`).
- **D5 (accepted):** entry 132 keeps the `(EOpinionDeedType)(deed.Type)` cast, the form the row already uses. Retyping the local member is
  a wider change.
- **Slips, recorded:** one small /FORCE link ran in the foreground (under a timeout), and one test precheck ran in the same call as its
  recheck.
