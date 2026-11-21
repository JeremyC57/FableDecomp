# Landing nf213 (Account A, 2026-11-21): FP-VECTORINSERT, the vector<NUISystem::CComponent*> overflow binders (4 rows)

Agent NF213 was the single running agent (owner order: one agent at a time). It handed back (context) partway through the 0x00485DBC unit,
which stays blocked with two rows left. The UnpackNames family (q12343) was not reached and goes back to the queue. Its files are in
`naming/2026-11-21_agentNF213/`, including the option-(d) scratch overlays (`work/hdr_d/`) for the two rows left.

- 4 rows land, all RELOCATION_MATCH, every test PASS.
  - 0x00485DBC: the self row. Only its truncated `// Leaf:` line is completed (same symbol). The catalog name becomes
    `std::vector<NUISystem::CComponent *, std::allocator<NUISystem::CComponent *>>::_M_insert_overflow` (was
    `_global::Std_Vector_Insert_Range_Fill`). The old fill_n reading is disproved (nf212 census).
  - 0x00541390: the ScrollingArray stand-in becomes std::vector, and the overflow stays declaration-only. The test is rewritten on the shared
    headers.
  - 0x005414A0: the ChildVector stand-in becomes std::vector. Its 0x005418C0 call binds that row's own self leaf (`insert`), so 0x005418C0 goes
    2 -> 1. It has a per-row `/d2inlS12` with an `// Inline-budget:` line, which only the size-scaffolding COMDATs see; the catalog
    CompilerFlags carry it. Its IAE overflow copy reaches in-object and equals 0x00485DBC (137 = 137): the nf212 refinement.
  - 0x00563130 CTCHeroOnlineScoreboard::ShowScoreboard: whole row on the shared headers, with no local model left. Slot 48 is
    `NUISystem::CComponent::ChangeState(unsigned long)` (was the stand-in SetVisible). CreateComponent 0x0041DB1D moves to the shared spelling
    (3 -> 2).
- Headers (add-only, `orig` equals HEAD, pinned 0, no landed includer defines the macros):
  - stlport/fable_containers.h: PUSHBACK_NUISYSTEM_CCOMPONENT_SIZE_BODY (3 rows) and OVERFLOW_NUISYSTEM_CCOMPONENT_SIZE_BODY (0x005414A0). Both
    are size-pragma scaffolding (nf184 / nf210 precedent): the owner rows set the same pragmas, and without the macro only the copy changes.
  - engine/CTCHeroOnlineScoreboard.h: SHOWSCOREBOARD declares ShowScoreboard and the inline `GetSignedInPassword` accessor (both PDBs).
    Coordinator fix: NF213 declared the accessor public. Both PDBs spell it private (`?GetSignedInPassword@CTCHeroOnlineScoreboard@@AAE...`,
    retail out of line at 0x005D7780), and the row's object emits it, so the opt-in now puts it under `private:`. Access changes the
    name, not the bytes; the landing's recheck grades the row RELOCATION_MATCH with the private spelling.
- Pick-any (`logs/pickany_check.txt`): push_back equals 0x00486024 (49 = 49) in three rows. bitset<274>::test equals 0x00410DE0 (63 = 63).
  NF213 checked LowerBound against 0x0040F020 (60 = 60). The CThing::HasTC copy in 0x00563130 has no owner row (copy-against-copy debt, the
  nf210 ruling; queued). Its "bitset" string literal is a content-named COMDAT.
- Link: 0 LNK2005, the same as HEAD. Recheck (`logs/recheck.log`): 1982 good, 0 bad (1889 header includers plus referencing rows). Split
  gate 0 created.
- tablecheck: 0x00485DBC 5 -> 2 and 0x0041DB1D 3 -> 2; none grows. No out-of-image oracle line.
- Left in the unit (q6333 / q6342, blocked): 0x006116E0 / 0x007E4B60 (CRetailVector). They match only with the plain /O2 visible overflow
  (167 against the owner's 137). NF213's option (d) is the next form: fill_n<CComponent**> visible and noinline under optimize("s", on).
- Queued: 0x0056AD40's differing overflow / push_back copies (HEAD debt), a 0x004118C0 HasTC row, a 0x005D7780 GetSignedInPassword row.
