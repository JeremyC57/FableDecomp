# Landing nf283 (Account A, 2026-11-22): T1-access + T2, NF281's continuation (47 rows)

Hand-back: `pending/2026-11-22_nf283_handback/`. Tracker batch T1-access + T2, NF281's ENTRIES lines 1-60 of the 191 it left. No header, flag,
test or catalog change.

- 39 addresses closed (35 done, 3 done-already: the stale table witnesses already bind the PDB spelling in their HEAD objects, 1 done-partial:
  0x00692AA0, whose last binder 0x00694020 still declares it public and fails the clang precheck, so its QAE line stays, rewitnessed to
  0x00694020). 47 rows changed, all zero line delta, in place in each row's own local model (`private:` / `protected:`, `virtual`, `const`,
  existing `// Leaf:` lines). objsec HEAD v new ALL_EQ under the renames for all 47; vtmap of the five table-emitting rows unchanged; HEAD link
  sets leave the wrong spellings unresolved, the new sets resolve them, duplicate counts equal on both sides. 10 self-leaf renames (each new
  name defined only by its self row, `evidence/name_census.txt`), 43 caller respellings.
- **Wider fixes accepted (R1; each byte-identical, objsec ALL_EQ):** 0x00692EC0 (the const self body calls slot 36 of the local
  CCreatureActionBase model; that slot is made `const`, which the PDB-const caller forces; no symbol, called through the vtable),
  0x00B4B830 (`virtual` on a non-polymorphic local model, the leading pad shrinks 0x2af -> 0x2ab so every member keeps its offset; no table),
  0x009FE380 (return `long` -> `unsigned long`, both PDBs `K`: QAEJXZ -> QBEKXZ), 0x007E5230 (`friend class CTCBetting;`, the real class, as
  nf281's 0x00A25E70).
- Rejected (6, T3): entry 5 (the const self body writes a member; `mutable` would be needed), 8 / 29 (another row's strong same-TU copy
  already carries the PDB spelling: D3), 20 (declared in the shared C3DPrimitive2.h), 39 (the caller is a `_Methods` stand-in), 49 (const
  cascades into the LowerBound stand-in callee). Held: entry 10 (0x0052C7E0 rule D4). Decision D2: CDiskFileWin32 Read 0x0099A960 / Write
  0x0099AA90, whose only fixable binders 0x0055D070 / 0x00B2C8E0 emit `??_7CDiskFileWin32` from local models: queued as a T3 port onto
  engine/CDiskFileWin32.h, no table change.
- Skipped on a failing precheck (12): ruling D1 for the continuation NF286 lets a VC7.1 `cl /Zs` syntax check stand in where the clang
  failure is only inside toolchain headers (`<cwchar>`, iostream, `_assert`, `short wchar_t`) or a shared FABLE_STATIC_ASSERT typedef.
- Table: 22 lines deleted, 13 respelled in place, 1 added, 1 rewitnessed. No symbol gains a second target.
- NAME_FIXES: 9 findings outside these rows (stand-in self rows 0x009CBC20, 0x009CBE10, 0x00A01F80, 0x00B4B880; 0x00A26D40's own leaf and
  same-TU copies), queued `land1122nf283`.

## Recheck
`logs/recheck.log` (landed tree, 47 rows): 47 RELOCATION_MATCH PASS, 0 DIFFER, 0 RELOC_CONFLICT; warnings are pre-existing (198
SHARED_HEADER_AVAILABLE, 17 RELOC_NAME on callee catalog labels, 2 RELOC_ALIAS, 1 STANDIN_LEAF), none on a new spelling.
Rule slips (noted; the NF286 brief repeats the rules): 14 locks claimed and released uncompiled after the landed test failed its precheck;
three rows compiled before their duplicate-definition conflict was found (not handed back); the clang census ran without locks.
