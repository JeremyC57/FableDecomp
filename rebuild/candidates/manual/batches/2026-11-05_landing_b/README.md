# Landing 2026-11-05b: chunk 775 (the last 4 matchable oracle bakes, as plain C++)

Authored by one expert agent (owner, 2026-11-05). The asm rules are in `docs/pipeline/ASM_RULES.md`. All four rows grade
`genuine` by purity, so ASM_RULES rules 2-4 never applied. No row source or test has `__asm`, `naked`, `_emit`, `volatile`, an
asm helper or a pragma workaround.

## Landed

| Address | Name | Grade | Purity | How |
|---|---|---|---|---|
| 0x009A8840 | CBankFileManager::OpenRetailBank | RELOCATION_MATCH PASS | genuine | `vland --land` (`logs/land.log`) |
| 0x009A8170 | CBankFileManager::OpenIniFile | RELOCATION_MATCH PASS | genuine | `vland --land` |
| 0x004167A7 | CMainGameComponent::GetFrameDifferenceFromCurrent | RELOCATION_MATCH PASS | genuine | replaces the bake in place, `/O1` TU |
| 0x005DACF0 | CTCWeapon::IsAbleToBeUsedToBlock | RELOCATION_MATCH PASS | genuine | replaces the bake in place |

- **The bank rows were never catalog rows before.** Their byte bakes were moved, not deleted, to
  `rebuild/runtime/openretailbank/bake/` (`*_bake.cpp`, `*_bake_test.cpp`). The runtime probe scripts and the README there were
  repointed to the new paths. The new genuine sources sit at the old `rebuild/src/compiled/00/9a/` paths.
- **The two in-place rows:** the sources and tests replace the bake files at the same paths. In `build_candidates.ps1`, the
  `Grade = 'asm_bake'` lines are removed and the PassPatterns are now `<addr>_TEST PASS`.
  - 0x004167A7 has `CompilerFlags = '/O2 /Oy /W3 /O1'`.
  - The first recheck used plain `'/O1'`, and the recheck tools appended `/Oy-` to it, which gave DIFFER(36v33)
    (`logs/recheck_inplace_O1only.log`). With the explicit `/Oy`, the row grades RELOCATION_MATCH PASS
    (`logs/recheck_inplace.log`).
- **Headers:** the 10 changed headers each read `default=same` in hdrcheck2. The new switches sit behind `FABLE_RETAIL_*`
  sub-macros (CBANKFILEMANAGER, CMEMORYFILE, CTHREADEDFILE, CAFILE_SPLITPATH_WIDE, CMAINGAMECOMPONENT_GAMETIME, ...). All 3,727
  landed sources that include those headers preprocess to the same text as before (ppcmp).
- **The key spellings and every attempt** (89 grades and probe compiles) are in `agent_NOTES.md` and `ATTEMPTS.tsv`.

## Names
- `ghidra_out/labels_landing_2026-11-05b.tsv`: 35 labels.
  - The 9 template labels were written by hand in catalog form (`CCountedPointer<CDiskFileWin32>::operator=`, the
    `std::map<CCharString, CBankFileManager::CBankInfo, ...>::operator[]` family).
  - The catalog's count of mangled names is unchanged (44).
- The NAME_FIXES are queued (tag `land0511b`), among them:
  - 0x0099B2C0 `CWideString::ToCharString`, landed as ConvertToCharString;
  - the CCountedPointer ctor, dtor and `operator=` spellings of 0x009A9B60 / 0x009A9BB0 / 0x009A9BF0 / 0x009A9D10;
  - the CFileDataInputStream / CMemoryDataInputStream reloc-binding spellings.

## The 9 old bakes, after this landing
- **Fixed: 6.**
  - 0x0042DF9E and 0x00551340 (landing a);
  - the 4 rows above.
- **Parked: 3,** in `pending/2026-11-04_linux_hand774/`:
  - 0x0053C332 DoRecomputeOffsets (one store pair swapped);
  - 0x00597BF2 GotoPreviousScreen;
  - 0x009BEF80 SetViewport (register allocation).
