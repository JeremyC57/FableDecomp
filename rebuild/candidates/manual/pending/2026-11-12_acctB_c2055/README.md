# acctB hand-back, row 0x00821A80 CMap::LoadFromFile: matched

| address | result | notes |
|---|---|---|
| 0x00821A80 `?LoadFromFile@CMap@@QAEXAAVCAFile@@PAVCASuspendableProcess@@@Z` | RELOCATION_MATCH 2648v2648, test PASS (g++ and VC7.1) | `// Flags: /GS` (cookie at 0x00821A86; same-TU landed 0x00821010). 41 grades (`grades.log`) |

Re-verified by the acctB local coordinator (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` holds the grade, both test runs and the read-only vland log. Purity and padded-struct greps clean. Nothing is landed. vland: 1 win, no RELOC_CONFLICT / RELOC_IDENTITY / NOT_GENUINE; one `WARN RELOC_ALIAS` (`_stricmp` 0x00BFEBA8
is the MSVCR71 import thunk, declared as in nine landed rows).

## What closed it (from 630 of 796)

1. `vector<long>::_M_insert_overflow` (0x00440990) visible but out of line: VC7.1 then keeps `_M_finish` in a CPU
   location across push_back's store and the frame drops from 0x98 to retail's 0x90 (743 of 796).
2. `CFileLargeThemePalette& file_palette = header->LargeThemePalette;` declared before the 64-bit ThingUIDCount copy.
3. `size_x` / `size_y` as if / else assignments on `UsingExtraMapCells` (retail 0x00821F95..0x00821FB5).
4. `SetDynamicCast` reading the source handle again for the store.

## Review items

- **noinline device on `_M_insert_overflow`** (opt-in `FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE`, macro empty
  by default; same kind as r3019's visible-but-called `find`). Check the `// Inline-budget:` line at landing.
- **PROVEN by FableWin size:** the by-name `GetDef<CSoundThemeDef>` text (`def.Get()` test, then
  `out->SetDynamicCast(def)`: 40 instructions / 139 bytes, equal to FableWin) and the small CMap inline helpers.
- **FITTED:** the `bool valid` flag with one cleared block in the palette-B loop, `file_palette` placement, the
  implicit `CCharString` conversion for `GetNoInstantiatedDefsOfType("ENGINE_THEME")`, the `SetDynamicCast` re-read
  (opt-in `FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD`).
- **`NAME_FIXES.tsv`** (6): among them 0x0050EE40 is `CFileLargeThemePalette::CFileLargeThemePalette` (landed row
  file misnamed `CLayer::SetAllPolys`) and 0x00A5B210 `CCachedFile::OpenFromFile`.
- **Headers:** opt-ins only (`CDefPointer.h`, `CDefinitionManager.h` `_GETDEF_NAME_INLINE`, `stlport/fable_containers.h`,
  plus the scout's CMap blocks); see `hdr/README.md`. Made-up names are marked in `NOTES.md`.
