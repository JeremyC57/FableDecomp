# Hand chunk 2055 (CMap .. CMap): 1 of 1 byte-exact

## What matched (1)
- 0x00821A80 `CMap::LoadFromFile(CAFile&, CASuspendableProcess*)` (2648 bytes, /GS) -- RELOCATION_MATCH, test PASS under g++ and
  VC7.1, read-only vland RELOCATION_MATCH + PASS (one RELOC_ALIAS warning: `_stricmp` 0x00BFEBA8 is the MSVCR71 import thunk, as in
  the nine landed rows that declare it the same way). 41 full-row grades over two sessions.
  Loads one map from a level file: map header, theme palette rebuild, invalid-theme stand-in, sound theme list, height map cells,
  atmos palette remap and game map cells, with suspend points between the phases and every g_LoadMapCellsPerSuspend height cells.
  What closed it, in the order found (each one is a lesson in LESSONS.md):
  1. palette-B loop with ONE cleared block behind a `bool valid` flag (whole-function register allocation: header in ebx, zero in ebp);
  2. implicit `const char*` -> CCharString conversion for GetNoInstantiatedDefsOfType("ENGINE_THEME"); game map bounds / cell through
     `GameMap->` each time;
  3. vector<long>::_M_insert_overflow VISIBLE but out of line (frame 0x98 -> 0x90, _M_finish kept in a CPU location): 630 -> 743 of 796;
  4. `file_palette` bound before the 64-bit ThingUIDCount copy (0x00821B54);
  5. size_x / size_y as if / else assignments (0x00821F95..0x00821FB5);
  6. SetDynamicCast reading the source handle again for the store: the sound def keeps its release null test (0x00821F5B).

### FITTED v PROVEN
- PROVEN (two sources): the function name (Ego_r.pdb + FableWin.pdb), every callee in LABELS.tsv, the inline helpers whose FableWin
  debug sizes equal ours at /Od (SetThemePaletteIndexUsed, HeightMapUnitToGameMapUnit, GetExtraCells, GetIndexIntoCells, SetAtmosTheme,
  SetAtmosBlend, SetSoundTheme, GetSizeX / GetSizeY, the implicit CFileFormatMapHeader / CFileFormatGameMapCell ctors), and the
  by-name GetDef<CSoundThemeDef> text (`def.Get()` + SetDynamicCast: 40 instructions / 139 bytes = FableWin 0x02247E20 at /Od /EHsc).
- FITTED (chosen by the retail bytes; same semantics as the alternatives):
  - `bool valid` flag in the palette-B loop (the duplicated else and `continue` forms give other register maps);
  - declaration order `file_palette` then `ThingUIDCount = header->ThingUIDCount`;
  - sound loop `if (!GetDef(name, &def)) push_back(0); else push_back(index);` (not a ternary, not one push_back);
  - if / else assignments of size_x / size_y; named `cells_per_suspend` / `no_cells` locals;
  - the SetDynamicCast re-read body (FableWin's debug copy is 34 instructions with its debug type check; ours is 24 -- the text
    of the debug check is not recovered, the re-read is what the release bytes need);
  - the visible out-of-line _M_insert_overflow (STLport's real stl/_vector.c body is out of line in retail; ours is the repo body
    with `__declspec(noinline)`);
  - `sound_index > sound_list.size()` (retail `jbe`: index == size() passes and reads one slot past the list);
  - CHeightMapCell::SetEngineBlend / SetEngineThemePaletteIndex bodies (FableWin's are assert builds) and
    GetCellIncludingExtraCellsAt's index `(SizeY - y) * (SizeX + GetExtraCells()) + x` (from the retail arithmetic).
- Descriptive names (original found in no PDB, string, FSE or sibling source): `g_bResetThingUIDsOnMapLoad` 0x013B8636,
  `g_LoadMapCellsPerSuspend` 0x01375718; T of GetDef 0x0042B144 written CDefClassBase; the sound def handle's type CSoundThemeDef
  (FableWin's by-name instantiation; the type changes no byte here); local names (`file_palette`, `sound_list`, `valid`, ...).

## Parked for Fable 5.1 (0)

## Skipped
None.

## Label corrections
See NAME_FIXES.tsv (carried from the scout, none new): 0x0050EE40 is `??0CFileLargeThemePalette@@QAE@XZ` (landed row file named
CLayer::SetAllPolys), 0x00A5B210 `CCachedFile::OpenFromFile` spelling, 0x009AD3B0 `GetNoInstantiatedDefsOfType` (already queued),
wrong binders for 0x008F6050, 0x0051CBB0, 0x0063F3A0.
