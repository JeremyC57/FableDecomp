// CMap::LoadFromFile(CAFile& file, CASuspendableProcess* psuspendable_process) -- load the height map, game map and theme palettes of one map from a level file
// Leaf: ?LoadFromFile@CMap@@QAEXAAVCAFile@@PAVCASuspendableProcess@@@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00821A80, 2648 bytes, `ret 8`. Name: Ego_r.pdb 0x0067CFA0 + FableWin.pdb 0x022327B0.
// /GS evidence: the cookie 0x0139C8A8 is loaded at 0x00821A86 and homed at [esp+9Ch], __security_check_cookie 0x00BFE9F9 is
//   called at 0x008224CA; the same-TU landed row 0x00821010 carries the flag too.
// What it does: allocates a CFileFormatMapHeader (0x10822 bytes), opens a 0x4B000-byte CCachedFile on the level file, reads the
//   level header, clears the height map, seeks to the map data and reads the map header. Then (1) rebuilds ThemePaletteEntryList
//   (identity over the ENGINE_THEME defs when the file palette is "UNINITIALISED", else by def name with unknown names cleared),
//   (2) gives entry 1 the global def's InvalidThemeStandin when it is empty and clears every entry that pointed at 1, (3) reads the
//   sound theme name list into class indices, (4) reads every height map cell (height * 2048, flags, engine blends / themes, and for
//   versions below 3 the sound theme of the game cell it covers), (5) for version 2+ remaps the atmos palette and reads every game
//   map cell (atmos blends / themes, for version 3+ the sound theme). The process is suspended between the phases and every
//   g_LoadMapCellsPerSuspend height cells.
// Callees: CGameDefinitionManager::Get 0x0044C6B0, operator new 0x00BFEA1A / delete 0x00BFE9BC, ??0CFileLargeThemePalette 0x0050EE40
//   (x2, the header's implicit ctor), ??0CCachedFile(long) 0x00A5B590, CCachedFile::OpenFromFile 0x00A5B210,
//   CReplaceSerialise<CFileFormatLevel,..>::ReadFromFile 0x0051A240, CHeightMap::Clear 0x008F6050, CCachedFile::SetPosition
//   0x00A5B020, GFGetTime 0x009E1BC0 (x8, result dropped), CReplaceSerialise<CFileFormatMapHeader,..>::ReadFromFile 0x0051CBB0,
//   _stricmp 0x00BFEBA8, ??0CCharString(const char*, long) 0x0099EBF0, GetNoInstantiatedDefsOfType 0x009AD3B0, ??1CCharString
//   0x0099EAE0, GetDefGlobalIndexFromName 0x009AD410, GetDef<T>(long, ..) 0x0042B144 and GetDef<CEngineThemeDef>(long, ..) 0x0063F3A0,
//   vector<long>::_M_insert_overflow 0x00440990, ??0CCharString 0x0099E4B0, CCharString::LoadFromFile 0x0099F2E0,
//   GetPDefFromInstantiationName 0x009ADA40, GetDefClassIndexFromGlobalIndex 0x009ACCA0, CReplaceSerialise<CFileFormatHeightMapCell,
//   ..>::ReadFromFile 0x008275B0, malloc 0x00BFEA0E / free 0x00BFEA14, and the expanded ~CCachedFile (Close 0x00A5AFA0, ~CWideString
//   0x0099B510, ~CBaseClassNonCopyable 0x0099A300). Virtual: CASuspendableProcess slot 2 SuspendSuspendableProcess.
// Shape notes (each is what the retail bytes need; PROVEN = two sources or a FableWin debug-size match, FITTED = chosen by the bytes):
//  - PROVEN helpers (FableWin.pdb COMDATs first emitted after this function, /Od sizes equal): SetThemePaletteIndexUsed,
//    HeightMapUnitToGameMapUnit, GetThemePaletteEntry, SetEngineBlend / SetEngineThemePaletteIndex, SetAtmosBlend / SetAtmosTheme /
//    SetSoundTheme, CGameMap::GetCell, GetSizeX / GetSizeY, the by-name GetDef<CSoundThemeDef> text (40 instructions / 139 bytes).
//  - FITTED: file_palette is bound BEFORE the ThingUIDCount copy (0x00821B54 `lea edi,[ebx+16h]` sits between the high dword's load
//    and store). The palette-B loop keeps ONE cleared block behind a `bool valid` flag: two else blocks cost the entry pointer
//    temporary two priority points and move the constant zero into ebx for the whole function.
//  - FITTED: `GetNoInstantiatedDefsOfType("ENGINE_THEME")` is the implicit const char* conversion (the temporary is re-addressed
//    with `lea`); the explicit CCharString("..") spelling passes the constructor's eax.
//  - FITTED: the sound loop is `if (!GetDef(name, &def)) push_back(0); else push_back(index);` with the by-name GetDef expanded;
//    SetDynamicCast reads the source handle again for the store (FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD), which keeps the
//    def's release null test at 0x00821F5B; vector<long>::_M_insert_overflow is visible but out of line
//    (FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE), which keeps _M_finish in a CPU location and gives the 0x90 frame.
//  - FITTED: size_x / size_y are if / else ASSIGNMENTS (0x00821F95..0x00821FB5: both loads hoisted above one `je`, `inc ecx ; inc eax`,
//    one store of size_x after the join); `++` on initialised locals stores size_x after each definition.
//  - FITTED: the game map bounds and cell are read through GameMap-> each time (retail reloads SizeX / SizeY); the sound index
//    bound is `>` size() (0x008220D8 / 0x00822412 `jbe`), as retail has it.
//  - Descriptive names (found in no PDB / string / FSE source): g_bResetThingUIDsOnMapLoad 0x013B8636, g_LoadMapCellsPerSuspend
//    0x01375718; T of GetDef 0x0042B144 is written CDefClassBase and the sound def type CSoundThemeDef (FableWin's by-name
//    instantiation), neither changes a byte here.
// Same-TU donor block: rebuild/src/compiled/00/82/CMap_LoadBrushesFromFile_00821010.cpp (defines + /GS); additions marked (2055).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CFILEFORMATBRUSH
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_DEF_CLASS_BASE                     // (2055) CDefPointer.h
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEFCLASSINDEX  // (2055) GetDefClassIndexFromGlobalIndex 0x009ACCA0
#define FABLE_RETAIL_CDEFINITIONMANAGER_NOINSTANTIATED  // (2055) GetNoInstantiatedDefsOfType 0x009AD3B0
#define FABLE_RETAIL_SUSPENDABLE_PROCESS                // (2055) polymorphic CASuspendableProcess (slot 2 SuspendSuspendableProcess)
#define FABLE_RETAIL_SCOPED_PTR                         // (2055) scoped_ptr operator-> for GameMap / HeightMap
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE     // (2055 overlay) _M_insert_overflow 0x00440990 visible, called, never expanded
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD  // (2055 overlay) 0x00821F5B: the sound def keeps its release null test
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_INLINE  // (2055 overlay) by-name GetDef<T> expanded at 0x00821E95
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY  // GetDef(long,..) 0x0042B144 / 0x0063F3A0 visible, out of line
#define FABLE_RETAIL_STLP_VECTOR                        // (2055) STLport vector<long> push_back / size / operator[]
#define FABLE_RETAIL_CMAP_THEMEPALETTE                  // (2055) typed ThemePaletteEntryList / GameMap / HeightMap
#define FABLE_RETAIL_CMAP_LOADFROMFILE                  // (2055 overlay) LoadFromFile decl, HeightMapUnitToGameMapUnit, SetThemePaletteIndexUsed
#define FABLE_RETAIL_CHEIGHTMAPCELL                     // (2055) cell flag bits
#define FABLE_RETAIL_CHEIGHTMAPCELL_SETTERS             // (2055 overlay) SetEngineThemePaletteIndex / SetEngineBlend
#define FABLE_RETAIL_CHEIGHTMAP                         // (2055 overlay, new header)
#define FABLE_RETAIL_CHEIGHTMAP_CELLINDEX               // (2055 overlay) GetExtraCells / GetIndexIntoCells / GetCellIncludingExtraCellsAt
#define FABLE_RETAIL_CGAMEMAP                           // (2055 overlay, new header)
#define FABLE_RETAIL_CFILEFORMAT_MAPHEADER              // (2055 overlay) CFileFormatMapHeader / ..HeightMapCell / ..GameMapCell
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER                 // no CGameDefinitionManager::GetDef in the PDBs: the CDefinitionManager templates
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_GLOBALDEF   // (2055) PeekGlobalDef() (+0xD0) -> CGlobalDef::InvalidThemeStandin (+0x50), 0x00821D0F..0x00821D1B
#include "stlport/fable_containers.h"                   // first, so the opt-in block is the copy every later header sees
#include "engine/CCachedFile.h"                         // overlay header (it pulls engine/CAFile.h)
#include "fable_string.h"
#include "engine/CMap.h"
#include "engine/CHeightMap.h"
#include "engine/CGameMap.h"
#include "engine/CFileFormatBrush.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CASuspendableProcess.h"
#include "engine/GFGetTime.h"
#include "engine/CGlobalDef.h"
extern "C" int __cdecl _stricmp(const char* a, const char* b);   // 0x00BFEBA8 (MSVCR71 import)
class CEngineThemeDef;                                  // ??$GetDef@VCEngineThemeDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCEngineThemeDef@@@@@Z 0x0063F3A0
extern bool g_bResetThingUIDsOnMapLoad;                 // 0x013B8636 (descriptive; no PDB data symbol)
extern long g_LoadMapCellsPerSuspend;                   // 0x01375718 (descriptive; .data 0x800)


class CSoundThemeDef;

void CMap::LoadFromFile(CAFile& file, CASuspendableProcess* psuspendable_process)
{
    CGameDefinitionManager& def_manager = CGameDefinitionManager::Get();
    CFileFormatMapHeader* header = new CFileFormatMapHeader;
    {
        CCachedFile cached_file(0x4B000);
        cached_file.OpenFromFile(file, psuspendable_process);
        CFileFormatLevel level_header;
        level_header.ReadFromFile(cached_file, psuspendable_process);
        HeightMap->Clear();
        NoChangedHeightAreas = 0;
        NoChangedGameAreas = 0;
        SeaLevel = 0.0f;
        cached_file.SetPosition(level_header.MapDataOffset);
        GFGetTime();
        header->ReadFromFile(cached_file, NULL);
        CFileLargeThemePalette& file_palette = header->LargeThemePalette;
        ThingUIDCount = header->ThingUIDCount;
        const unsigned long version = header->VersionNumber;
        if (version < 1)
            InitialisePassabilityFromThemes = true;
        if (g_bResetThingUIDsOnMapLoad)
            ThingUIDCount = 1;
        if (_stricmp(file_palette.GetThemePaletteEntry(0).ThemePaletteEntryName, "UNINITIALISED") == 0) {
            long no_themes = DefinitionManager.GetNoInstantiatedDefsOfType("ENGINE_THEME");
            for (long i = 0; i < 256; ++i) {
                if (i < no_themes) {
                    ThemePaletteEntryList[i].ThemeDefIndex = i;
                    ThemePaletteEntryList[i].SetThemePaletteIndexUsed(true);
                } else {
                    ThemePaletteEntryList[i].ThemeDefIndex = 0;
                    ThemePaletteEntryList[i].SetThemePaletteIndexUsed(false);
                }
            }
        } else {
            for (long i = 0; i < 256; ++i) {
                if (file_palette.GetThemePaletteEntry(i).ThemePaletteThemeDefIndex) {
                    CCharString name(file_palette.GetThemePaletteEntry(i).ThemePaletteEntryName);
                    long def_index = DefinitionManager.GetDefGlobalIndexFromName(name);
                    bool valid = false;
                    if (def_index) {
                        CDefPointer<const CDefClassBase> def;
                        DefinitionManager.GetDef(def_index, &def);
                        if (!def.IsNull()) {
                            ThemePaletteEntryList[i].ThemeDefIndex = def_index;
                            CDefPointer<const CEngineThemeDef> engine_def;
                            DefinitionManager.GetDef(def_index, &engine_def);
                            ThemePaletteEntryList[i].SetThemePaletteIndexUsed(true);
                            valid = true;
                        }
                    }
                    if (!valid) {
                        ThemePaletteEntryList[i].ThemeDefIndex = 0;
                        ThemePaletteEntryList[i].SetThemePaletteIndexUsed(false);
                    }
                }
            }
        }
        if (psuspendable_process) {
            GFGetTime();
            GFGetTime();
            psuspendable_process->SuspendSuspendableProcess();
        }
        if (ThemePaletteEntryList[1].ThemeDefIndex == 0) {
            ThemePaletteEntryList[1].ThemeDefIndex = DefinitionManager.PeekGlobalDef().InvalidThemeStandin;
            ThemePaletteEntryList[1].SetThemePaletteIndexUsed(true);
            for (long i = 2; i < 256; ++i) {
                if (ThemePaletteEntryList[i].ThemeDefIndex == 1) {
                    ThemePaletteEntryList[i].ThemeDefIndex = 0;
                    ThemePaletteEntryList[i].SetThemePaletteIndexUsed(false);
                }
            }
        }
        if (psuspendable_process) {
            GFGetTime();
            GFGetTime();
            psuspendable_process->SuspendSuspendableProcess();
        }
        std::vector<long> sound_list;
        sound_list.push_back(0);
        for (unsigned long i = 1; i < header->NoSoundThemeEntries; ++i) {
            unsigned long length;
            cached_file.ReadVar(length, NULL);
            CCharString name;
            name.LoadFromFile(cached_file, NULL, length);
            CDefPointer<const CSoundThemeDef> def;
            if (!def_manager.GetDef(name, &def))
                sound_list.push_back(0);
            else
                sound_list.push_back(def_manager.GetDefClassIndexFromGlobalIndex(def_manager.GetDefGlobalIndexFromName(name)));
        }
        long size_x;
        long size_y;
        if (header->UsingExtraMapCells) {
            size_x = HeightMapSizeX + 1;
            size_y = HeightMapSizeY + 1;
        } else {
            size_x = HeightMapSizeX;
            size_y = HeightMapSizeY;
        }
        const long cells_per_suspend = g_LoadMapCellsPerSuspend;
        const long no_cells = size_x * size_y;
        for (long i = 0; i < no_cells; ++i) {
            CFileFormatHeightMapCell file_cell;
            file_cell.ReadFromFile(cached_file, NULL);
            const long x = i % size_x;
            const long y = i / size_x;
            CHeightMapCell& cell = HeightMap->GetCellIncludingExtraCellsAt(C2DCoordI(x, y));
            cell.Height = file_cell.Height * 2048.0f;
            cell.Passable = file_cell.Passable;
            cell.CameraPassable = file_cell.CameraPassable;
            if (!cell.CameraPassable && cell.Passable)
                cell.CameraPassable = true;
            cell.ShorePoint = file_cell.ShorePoint;
            cell.NavPreferred = file_cell.NavPreferred;
            if (version < 3) {
                const long game_x = HeightMapUnitToGameMapUnit(x);
                const long game_y = HeightMapUnitToGameMapUnit(y);
                if (game_x < GameMap->GetSizeX() && game_y < GameMap->GetSizeY()) {
                    long sound_index = file_cell.SoundLayerPaletteIndex;
                    if (sound_index < 0 || sound_index > sound_list.size())
                        sound_index = 0;
                    GameMap->GetCell(C2DCoordI(game_x, game_y)).SetSoundTheme((signed char)sound_list[sound_index]);
                }
            }
            for (long c = 0; c < 2; ++c)
                cell.SetEngineBlend(file_cell.EngineBlend[c], c);
            for (long c = 0; c < 3; ++c) {
                unsigned char theme = file_cell.EngineTheme[c];
                if (theme > 0 && ThemePaletteEntryList[theme].ThemeDefIndex == 0)
                    theme = 1;
                cell.SetEngineThemePaletteIndex(theme, c);
            }
            if (psuspendable_process && i % cells_per_suspend == 0) {
                GFGetTime();
                psuspendable_process->SuspendSuspendableProcess();
                GFGetTime();
            }
        }
        if (version >= 2) {
            CFileLargeThemePalette& atmos_palette = header->AtmosThemePalette;
            for (long c1 = 0; c1 < 256; ++c1) {
                if (atmos_palette.GetThemePaletteEntry(c1).ThemePaletteThemeDefIndex) {
                    long new_theme_def_index = def_manager.GetDefGlobalIndexFromName(atmos_palette.GetThemePaletteEntry(c1).ThemePaletteEntryName);
                    if (new_theme_def_index)
                        atmos_palette.GetThemePaletteEntry(c1).ThemePaletteThemeDefIndex = def_manager.GetDefClassIndexFromGlobalIndex(new_theme_def_index);
                    else
                        atmos_palette.GetThemePaletteEntry(c1).ThemePaletteThemeDefIndex = 1;
                }
            }
            CFileFormatGameMapCell file_game_cell;
            const unsigned long no_game_cells = GameMapSizeY * GameMapSizeX;
            for (unsigned long i = 0; i < no_game_cells; ++i) {
                file_game_cell.ReadFromFile(cached_file, NULL);
                CGameMapCell& game_cell = GameMap->Cells[i];
                for (long c = 0; c < 2; ++c)
                    game_cell.SetAtmosBlend(c, file_game_cell.AtmosBlends[c]);
                for (long c = 0; c < 3; ++c)
                    game_cell.SetAtmosTheme(c, (unsigned char)header->AtmosThemePalette.GetThemePaletteEntry(file_game_cell.AtmosThemes[c]).ThemePaletteThemeDefIndex);
                if (version >= 3) {
                    long sound_index = file_game_cell.SoundThemePaletteIndex;
                    if (sound_index < 0 || sound_index > sound_list.size())
                        sound_index = 0;
                    game_cell.SetSoundTheme((signed char)sound_list[sound_index]);
                }
            }
        }
        GFGetTime();
    }
    delete header;
}
