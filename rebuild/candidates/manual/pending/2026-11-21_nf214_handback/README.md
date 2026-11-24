# NF214 hand-back (Account A, 2026-11-21): pending landing, NOT applied

**LANDED 2026-11-21 as nf214** (`rebuild/candidates/manual/batches/2026-11-21_landing_nf214/README.md`). The text below is the pre-landing note.

Owner (2026-11-21): "after that task and the current agent finish, no more agents or naming for now". NF214 was the last agent. Its
hand-back is saved here unapplied so it survives the session; nothing in rebuild/src, rebuild/tests or rebuild/include has changed.
Land it with the usual naming-landing procedure (docs/HANDOFF.md, Account A) when naming resumes. The rows stay locked to NF214
(scratch nflocks) and the claim line reads handed back.

- Entries, all done: q12343, the C3DMeshStats::UnpackNames family; q6333 / q6342, the two CRetailVector rows of 0x00485DBC.
- Rows: 6, every one RELOCATION_MATCH + PASS in the agent's final recheck (`recheck_final.log`); `selfcheck.log` reports 0 problems.
  - 0x00A2ED10, 0x00A2F0F0, 0x00A2F120, 0x00A2F150: self renames onto the PDB spelling
    `?UnpackNames@C3DMeshStats@@ABEXAAV?$vector@VCCharString@@...`. The address goes 3 -> 1 spellings, and the nf212 CCharStringVector
    wrapper is gone.
  - 0x007E4B60, 0x006116E0: onto 0x00485DBC's self leaf, so all 11 referencers agree.
- Headers (`hdr/`, add-only):
  - engine/C3DMeshStats.h: FABLE_RETAIL_C3DMESHSTATS_UNPACKNAMES, declarations only, each spelling in both PDBs.
  - stlport/fable_containers.h: FABLE_RETAIL_STLP_FILL_N_NUISYSTEM_CCOMPONENT_SIZE_BODY, size-pragma scaffolding. The owner 0x0048514C sets
    the same pragma, and without it the row bodies are identical.
- Landing checks to run:
  - Put `/d2inlS12` in the catalog CompilerFlags of 0x007E4B60 and 0x006116E0.
  - Expect one LNK2005: landed 0x0047E196 strongly defines an identical 23-byte fill_n (accepted debt, nf181 / nf184). Queue making that
    copy inline.
  - Prove the 0x00A2ED10 `""` -> `?NullChar@CCharString@@1DB` (0x0129AAF4) move from reloc_bindings rather than the gate scan, because a
    huge number of rows reference that address (GOTCHAS nf120).
  - The full includer recheck covers about 2,186 rows (fable_containers.h).
- Left as debt: the local C3DMeshStats / CHelperPointMap / CDummyObjectMap models in 0x00A2ED10 (their typed header form needs
  engine/CPreAllocVectorMap.h and engine/C3DHelperPoint.h, which exist only under pending/2026-11-08_linux_hand868), the standalone HEAD
  tests, and the C3DMeshObject wrapper spellings in NAME_FIXES.tsv.
