# Landing nf214 (Account A, 2026-11-21): the UnpackNames family and the two CRetailVector overflow binders (6 rows)

Owner (2026-11-21): "pause the naming agent and do the landing first". Agent NF214's hand-back was saved unapplied in
`pending/2026-11-21_nf214_handback/` (its BRIEF, STATUS, APPLIED, RENAMES, NOTES, LESSONS stay there). This batch applies it.
Naming agent NF215 was paused for the landing and resumes after it.

- 6 rows land, all RELOCATION_MATCH, every test PASS (`logs/recheck.log`).
  - 0x00A2ED10 C3DMeshStats::UnpackNames: self rename onto the PDB spelling
    `?UnpackNames@C3DMeshStats@@ABEXAAV?$vector@VCCharString@@...@std@@_NABVCCharString@@1@Z` (private, const this; both PDBs). The nf212
    `CCharStringVector` wrapper is gone. Its `""` reads bind `?NullChar@CCharString@@1DB` = 0x0129AAF4, which the reloc_bindings witness
    0x00AE8D50 agrees with (checked from the table, not the gate scan: GOTCHAS nf120).
  - 0x00A2F0F0 / 0x00A2F120 / 0x00A2F150 (GetAllDummyObjectNames, GetAllHelperPointNames, GetDummyObjectNamesStartingWith): self renames onto
    the PDB `QBE ... vector<CCharString>` spellings, and their UnpackNames calls bind the new self leaf. 0x00A2ED10 goes from 3 spellings to 1.
  - 0x007E4B60 CTCCreditsUI::Activate and 0x006116E0 CTCInventoryMap::OpenInventory: the CRetailVector stand-in becomes
    `std::vector<NUISystem::CComponent*>`, so both growth calls bind 0x00485DBC's IAE self leaf (all 11 referencers agree).
- Headers (add-only, each block wholly inside its own `#ifdef`; no landed row but these defines the macros):
  - engine/C3DMeshStats.h: `FABLE_RETAIL_C3DMESHSTATS_UNPACKNAMES` (declarations only; spellings in both PDBs).
  - stlport/fable_containers.h: `FABLE_RETAIL_STLP_FILL_N_NUISYSTEM_CCOMPONENT_SIZE_BODY` (size-pragma scaffolding: owner 0x0048514C sets the
    same pragma; nf184 / nf210 / nf213 pattern).
- Catalog: `CompilerFlags = '/O2 /Oy /W3 /d2inlS12'` on 0x007E4B60 and 0x006116E0 (their `// Flags:` / `// Inline-budget:` lines). Written in
  full because the build's per-entry CompilerFlags REPLACE the default `/O2 /Oy /W3` (FINDINGS_LOG 2026-11-21: 17 older entries carry a bare
  `/d2inl*` / `/Gr` value).
- Recheck (`logs/recheck_summary.txt`, rows in `logs/recheck_rows.txt`): 2,220 rows = the direct includers of both headers, the six rows,
  their owners 0x00485DBC / 0x0048514C, 0x00A2F170, and every landed referencer of 0x00A2ED10 / 0x00485DBC / 0x0048514C. 2,218 good on the
  first pass. 0x005414A0 and 0x00E0A510 graded DIFFER only because recheck.py adds `/Oy-` to a bare-flag entry; with their own `// Flags:`
  lines both are RELOCATION_MATCH (`logs/regrade_partial_flags.txt`). Two rows matched under the lander's pragma sweep (0x005BBF80 "s",
  0x0065A5BB /EHsc), as at HEAD. The changed headers' default path is token-identical, so the transitive includers cannot change.
- Symbol diff vs HEAD objects (`logs/symbol_diff.txt`): own leaves plus the shared-header pick-any STL COMDATs of vector<CComponent*>.
  Pick-any (`logs/pickany_check.txt`): push_back 49 = 49 (0x00486024), _M_insert_overflow 137 = 137 (0x00485DBC), fill_n 23 = 23
  (0x0048514C) in both vector rows.
- Link (`logs/link_check.txt`, all 2,220 objects, `/FORCE`): HEAD control 313 LNK4006, landing 315. The two new ones are the expected
  single duplicate: fill_n<CComponent**> against landed 0x0047E196's strong 23-byte definition (accepted debt nf181 / nf184; queued to make
  that copy inline).
- reloc_bindings.tsv (`logs/tablecheck.txt`): 3 stale stand-in witnesses removed, none added; 0x00A2ED10 3 -> 1, 0x00485DBC 3 -> 2; none grows.
  0x006116E0 also binds 0x0066E799 / 0x0066E7BD under spellings that differ from the table's (SetCurrentScreenType v SelectScreenOfType,
  non-const v const GetCurrentScreen). HEAD's row does the same, so this split predates the landing. Both are queued and not recorded.
- Queued (`naming/NAME_FIXES_QUEUE.tsv`, tag land1121nf214): NF214's three C3DMeshObject wrapper spellings (0x00A35B30 / 0x00A35AF0 /
  0x00A35B50), the 0x0047E196 inline fill_n copy, and the two 0x0066E7xx binder spellings.
- Left as debt (NF214 NOTES): the local C3DMeshStats / CHelperPointMap / CDummyObjectMap models in 0x00A2ED10 (the typed form needs
  engine/CPreAllocVectorMap.h and engine/C3DHelperPoint.h, only under pending/2026-11-08_linux_hand868) and the standalone HEAD tests.
