# Landing nf212 (Account A, 2026-11-21): FP-VECTORINSERT, the last four vector<CCharString> overflow binders (4 rows)

Agent NF212 was the single running agent (owner order: one agent at a time). It finished the 0x00433530 unit (q6332 / q6341 done: all 39
referencing rows now bind the IAE `_M_insert_overflow`). It handed back (context) after the 0x00485DBC census (q6333 / q6342, blocked for
a family pass). Its files are in `naming/2026-11-21_agentNF212/`, with the census in `work/probe_00485dbc.txt`.

- 4 rows land, all RELOCATION_MATCH, every test PASS.
  - 0x004333B3 `vector<CCharString>::_M_fill_insert`, a leaf-only self rename onto the allocator spelling.
    - It is on the shared vector body (overflow declaration-only as at HEAD, FALSE_TAG_POD, RANDOM_ACCESS_COPY_BACKWARD).
    - All 3 landed referencers already bound the new spelling, so the address goes 2 -> 1. The old self leaf is bound nowhere.
    - Its call to 0x00433504 moves onto the template spelling that row defines.
    - Per-row flag `/d2inlS34`, with an `// Inline-budget:` line: the smallest N, since 33 and below leave copy_backward / fill out of
      line. The catalog CompilerFlags carry it (vland reads the catalog), and the landing's row line grades RELOCATION_MATCH with it.
  - 0x004B2FE0 / 0x004B4A10 (CQuestManager): the vector stand-ins (CStringVector / CCharStringVector) move onto stlport/fable_containers.h.
    0x004B4A10 also moves onto fable_string.h and engine/CArray.h with OVERFLOW_OUTOFLINE plus the owner's UNINIT_COPY_FORCEINLINE (the
    nf208 recipe).
  - 0x00A2ED10 C3DMeshStats::UnpackNames: the stand-in overflow is gone and the row binds the IAE spelling.
- Advisor decision (owner delegated), 0x00A2ED10's transitional wrapper: `struct CCharStringVector : std::vector<CCharString> {}`. The
  row's landed self leaf spells CCharStringVector, so the token stays until the UnpackNames family is respelled to the PDB form (queued:
  self, 0x00A2F0F0, 0x00A2F150, 0x00A2F120). The checks: HEAD's source already declares the token; the empty struct adds no layout; the
  only defined symbol spelled with it is the self leaf. This is NOT a pattern for other rows.
- Advisor decision (owner delegated), in-object targets. In 0x00A2ED10 and 0x004B4A10, `push_back` reaches an `_M_insert_overflow`
  defined in the same object. That counts as the same target, and EXTRA_RULES gets an nf212 refinement of the nf205 amendment. All three
  conditions hold:
  - Owner 0x0044BFF0's push_back names the same IAE symbol (objdump -dr).
  - The copy is a pick-any COMDAT: no LNK2005 on it.
  - It equals 0x00433530 (316 = 316).

  The coordinator rebuilt the 0x004B2FE0 / 0x004B4A10 objects (the agent's were older than its sources) and scanned all four rows for
  in-object targets. The others are the HEAD-local CCharString / FindString / CDummyObjectMap models (debt, queued).
- Header: stlport/fable_containers.h FABLE_RETAIL_STLP_UNINITIALIZED_FILL_N_DECL_ONLY. It is add-only and declaration-only; `orig` equals
  HEAD; pinned 0; none of the landed includers defines it. The visible noinline body gave a 41-byte copy against 0x00433504's 38.
- Pick-any (`logs/pickany_check.txt`): the IAE overflow equals 0x00433530 (316), push_back equals 0x0044BFF0 (59) in three rows, and
  ~vector equals 0x00414EA0 (49). The MISSING `vector(const allocator&)` grades MATCH at 0x00414DE0 (coordinator q.py; a folded body
  whose table line another row made).
- Link: 1 LNK2005, the same as HEAD: 0x004B4A10's 89-byte ActivateMultipleQuests stand-in against the owner 0x004B4260. That copy is nf203
  debt (DIFFER before and after), and making it declaration-only is queued.
- Recheck (`logs/recheck.log`): 1895 good, 0 bad (1873 header includers plus referencing rows). Split gate 0 created.
- tablecheck: 0x00433530 shrinks 4 -> 1. 0x00A31790 1 -> 2 is SAME: 0x00A2ED10 already bound `LowerBound@CDummyObjectMap` at HEAD, and
  the table lacked the line. No out-of-image oracle line.
- 0x00485DBC census (q6333 / q6342): the self row already defines the claim spelling; the earlier fill_n reading is disproved. 6 of 11
  referencers bind it. 5 bind stand-ins over the same element type: 0x00541390, 0x005414A0, 0x00563130, 0x006116E0 and 0x007E4B60.
  These are ports for the next single-agent slice.
