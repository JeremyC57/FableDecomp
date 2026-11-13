// Behaviour test for 0x00821A80 CMap::LoadFromFile: a standalone model of the map loader, checked on three level files (version 3
// with a named theme palette, extra map cells and a suspendable process; version 0 with the "UNINITIALISED" palette; version 2 with
// the thing-UID reset switch). The model follows the row statement for statement: the header fields, the theme palette rebuild
// (identity over the ENGINE_THEME defs, or by def name with unknown names cleared), the invalid-theme stand-in for entry 1, the
// sound theme list (class index of each known name, 0 otherwise), the height map cells (height * 2048, flags with the camera
// fix-up, blends, themes remapped to 1 when their palette entry is empty, the old per-height-cell sound theme below version 3), the
// atmos palette remap and the game map cells from version 2, the sound theme of each game cell from version 3, and the suspend
// points. The sound index bound is `> size()` as in retail (0x008220D8 / 0x00822412 `jbe`), so index == size() reads the slot after
// the list; the model gives that slot a known value. Twenty-five single-statement mutations must each change the result signature.
#include <cstdio>
#include <cstring>

#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif

enum { PALETTE = 256, MAX_DEFS = 16, MAX_SOUNDS = 8, MAX_HEIGHT_CELLS = 128, MAX_GAME_CELLS = 16 };

struct PaletteEntryFile { char name[24]; long def_index; };
struct HeaderFile {
    u64 thing_uid_count;
    unsigned long version;
    bool using_extra_map_cells;
    unsigned long no_sound_theme_entries;
    PaletteEntryFile large[PALETTE];
    PaletteEntryFile atmos[PALETTE];
};
struct HeightCellFile {
    float height;
    bool passable, camera_passable, shore_point, nav_preferred;
    signed char sound_index;
    unsigned char blend[2];
    unsigned char theme[3];
};
struct GameCellFile {
    unsigned char blend[2];
    unsigned char theme[3];
    signed char sound_index;
};
struct LevelFile {
    HeaderFile header;
    const char* sound_names[MAX_SOUNDS];             // entries 1 .. no_sound_theme_entries - 1
    HeightCellFile height_cells[MAX_HEIGHT_CELLS];
    GameCellFile game_cells[MAX_GAME_CELLS];
    long height_read, game_read, sound_read;
};

struct Defs {                                        // the definition manager: global index = slot + 1
    const char* names[MAX_DEFS];
    long class_index[MAX_DEFS];
    bool has_def[MAX_DEFS];
    long count;
    long no_engine_themes;
    long invalid_theme_standin;
    long GlobalIndexFromName(const char* name) const
    {
        for (long i = 0; i < count; ++i)
            if (std::strcmp(names[i], name) == 0)
                return i + 1;
        return 0;
    }
    bool HasDef(long global_index) const { return global_index > 0 && global_index <= count && has_def[global_index - 1]; }
    long ClassIndexFromGlobalIndex(long global_index) const { return class_index[global_index - 1]; }
};

struct ThemePaletteEntry { long theme_def_index; bool used; };
struct HeightCell {
    float height;
    bool passable, camera_passable, shore_point, nav_preferred;
    unsigned char blend[2];
    unsigned char theme[3];
};
struct GameCell {
    unsigned char atmos_blend[2];
    unsigned char atmos_theme[3];
    signed char sound_theme;
};
struct Map {
    long height_size_x, height_size_y;               // CMap::HeightMapSizeX / Y (the height map holds one extra row and column)
    long game_size_x, game_size_y;
    u64 thing_uid_count;
    bool initialise_passability_from_themes;
    long no_changed_height_areas, no_changed_game_areas;
    float sea_level;
    ThemePaletteEntry palette[PALETTE];
    HeightCell height_cells[MAX_HEIGHT_CELLS];
    GameCell game_cells[MAX_GAME_CELLS];
    long suspends;
    long suspend_trace;
    long time_calls;
};

static bool g_reset_thing_uids;
static long g_cells_per_suspend;
static int g_mutation;

static HeightCell& HeightCellAt(Map& map, long x, long y)        // CHeightMap::GetCellIncludingExtraCellsAt
{
    return map.height_cells[(map.height_size_y - y) * (map.height_size_x + 1) + x];
}
static GameCell& GameCellAt(Map& map, long x, long y)            // CGameMap::GetCell
{
    if (g_mutation == 25)
        return map.game_cells[y * map.game_size_x + x];
    return map.game_cells[(map.game_size_y - y - 1) * map.game_size_x + x];
}
static void Suspend(Map& map, long tag)
{
    ++map.suspends;
    map.suspend_trace = map.suspend_trace * 31 + tag;
}

static void Load(Map& map, LevelFile& file, const Defs& defs, bool have_process)
{
    const int m = g_mutation;
    HeaderFile& header = file.header;
    file.height_read = 0;
    file.game_read = 0;
    file.sound_read = 0;
    std::memset(map.height_cells, 0, sizeof(map.height_cells));  // HeightMap->Clear()
    map.no_changed_height_areas = 0;
    map.no_changed_game_areas = 0;
    map.sea_level = 0.0f;
    ++map.time_calls;
    map.thing_uid_count = header.thing_uid_count;
    const unsigned long version = header.version;
    if (version < (m == 1 ? 3u : 1u))
        map.initialise_passability_from_themes = true;
    if (g_reset_thing_uids)
        map.thing_uid_count = (m == 2 ? 2 : 1);
    if (std::strcmp(header.large[0].name, "UNINITIALISED") == 0) {
        const long no_themes = defs.no_engine_themes;
        for (long i = 0; i < PALETTE; ++i) {
            if (m == 3 ? i <= no_themes : i < no_themes) {
                map.palette[i].theme_def_index = i;
                map.palette[i].used = true;
            } else {
                map.palette[i].theme_def_index = 0;
                map.palette[i].used = (m == 4);
            }
        }
    } else {
        for (long i = 0; i < PALETTE; ++i) {
            if (header.large[i].def_index) {
                const long def_index = defs.GlobalIndexFromName(header.large[i].name);
                bool valid = false;
                if (def_index) {
                    if (defs.HasDef(def_index) || m == 5) {
                        map.palette[i].theme_def_index = (m == 6 ? defs.ClassIndexFromGlobalIndex(def_index) : def_index);
                        map.palette[i].used = true;
                        valid = true;
                    }
                }
                if (!valid) {
                    map.palette[i].theme_def_index = 0;
                    map.palette[i].used = false;
                }
            }
        }
    }
    if (have_process) {
        map.time_calls += 2;
        Suspend(map, -1);
    }
    if (map.palette[1].theme_def_index == 0 || m == 7) {
        map.palette[1].theme_def_index = defs.invalid_theme_standin;
        map.palette[1].used = true;
        for (long i = 2; i < PALETTE; ++i) {
            if (map.palette[i].theme_def_index == (m == 8 ? 2 : 1)) {
                map.palette[i].theme_def_index = 0;
                map.palette[i].used = false;
            }
        }
    }
    if (have_process) {
        map.time_calls += 2;
        Suspend(map, -2);
    }
    long sound_list[MAX_SOUNDS + 1];
    unsigned long sound_count = 0;
    sound_list[sound_count++] = 0;
    for (unsigned long i = 1; i < header.no_sound_theme_entries; ++i) {
        const char* name = file.sound_names[file.sound_read++];
        const long global_index = defs.GlobalIndexFromName(name);
        if (!defs.HasDef(global_index))
            sound_list[sound_count++] = (m == 9 ? 1 : 0);
        else
            sound_list[sound_count++] = (m == 10 ? global_index : defs.ClassIndexFromGlobalIndex(global_index));
    }
    sound_list[sound_count] = 77;                    // the slot after the list (index == size() passes retail's bound)
    long size_x;
    long size_y;
    if (header.using_extra_map_cells) {
        size_x = map.height_size_x + 1;
        size_y = map.height_size_y + (m == 11 ? 0 : 1);
    } else {
        size_x = map.height_size_x;
        size_y = map.height_size_y;
    }
    const long cells_per_suspend = g_cells_per_suspend;
    const long no_cells = size_x * size_y;
    for (long i = 0; i < no_cells; ++i) {
        const HeightCellFile& file_cell = file.height_cells[file.height_read++];
        const long x = i % size_x;
        const long y = i / size_x;
        HeightCell& cell = HeightCellAt(map, x, y);
        cell.height = file_cell.height * (m == 12 ? 1024.0f : 2048.0f);
        cell.passable = file_cell.passable;
        cell.camera_passable = file_cell.camera_passable;
        if (!cell.camera_passable && cell.passable && m != 13)
            cell.camera_passable = true;
        cell.shore_point = file_cell.shore_point;
        cell.nav_preferred = file_cell.nav_preferred;
        if (version < (m == 14 ? 2u : 3u)) {
            const long game_x = x / (m == 16 ? 2 : 4);
            const long game_y = y / (m == 16 ? 2 : 4);
            if (game_x < map.game_size_x && game_y < map.game_size_y) {
                long sound_index = file_cell.sound_index;
                if (sound_index < 0 || (m == 15 ? (unsigned long)sound_index >= sound_count : (unsigned long)sound_index > sound_count))
                    sound_index = 0;
                GameCellAt(map, game_x, game_y).sound_theme = (signed char)sound_list[sound_index];
            }
        }
        for (long c = 0; c < 2; ++c)
            cell.blend[m == 18 ? 1 - c : c] = file_cell.blend[c];
        for (long c = 0; c < 3; ++c) {
            unsigned char theme = file_cell.theme[c];
            if (theme > 0 && map.palette[theme].theme_def_index == 0 && m != 17)
                theme = 1;
            cell.theme[c] = theme;
        }
        if (have_process && i % cells_per_suspend == (m == 19 ? 1 : 0)) {
            map.time_calls += 2;
            Suspend(map, i);
        }
    }
    if (version >= (m == 20 ? 3u : 2u)) {
        for (long c1 = 0; c1 < PALETTE; ++c1) {
            if (header.atmos[c1].def_index) {
                const long new_theme_def_index = defs.GlobalIndexFromName(header.atmos[c1].name);
                if (new_theme_def_index)
                    header.atmos[c1].def_index = (m == 22 ? new_theme_def_index : defs.ClassIndexFromGlobalIndex(new_theme_def_index));
                else
                    header.atmos[c1].def_index = (m == 21 ? 0 : 1);
            }
        }
        const unsigned long no_game_cells = map.game_size_y * map.game_size_x;
        for (unsigned long i = 0; i < no_game_cells; ++i) {
            const GameCellFile& file_game_cell = file.game_cells[file.game_read++];
            GameCell& game_cell = map.game_cells[i];
            for (long c = 0; c < 2; ++c)
                game_cell.atmos_blend[c] = file_game_cell.blend[c];
            for (long c = 0; c < 3; ++c)
                game_cell.atmos_theme[c] = (m == 23 ? file_game_cell.theme[c] : (unsigned char)header.atmos[file_game_cell.theme[c]].def_index);
            if (m == 24 ? version > 3 : version >= 3) {
                long sound_index = file_game_cell.sound_index;
                if (sound_index < 0 || (unsigned long)sound_index > sound_count)
                    sound_index = 0;
                game_cell.sound_theme = (signed char)sound_list[sound_index];
            }
        }
    }
    ++map.time_calls;
}

static unsigned long Mix(unsigned long h, unsigned long v) { return (h ^ v) * 16777619u; }
static unsigned long Signature(const Map& map, const LevelFile& file)
{
    unsigned long h = 2166136261u;
    h = Mix(h, (unsigned long)map.thing_uid_count);
    h = Mix(h, map.initialise_passability_from_themes);
    h = Mix(h, (unsigned long)map.suspends);
    h = Mix(h, (unsigned long)map.suspend_trace);
    h = Mix(h, (unsigned long)map.time_calls);
    h = Mix(h, (unsigned long)file.height_read);
    h = Mix(h, (unsigned long)file.game_read);
    for (long i = 0; i < PALETTE; ++i) {
        h = Mix(h, (unsigned long)map.palette[i].theme_def_index);
        h = Mix(h, map.palette[i].used);
        h = Mix(h, (unsigned long)file.header.atmos[i].def_index);
    }
    for (long i = 0; i < MAX_HEIGHT_CELLS; ++i) {
        const HeightCell& cell = map.height_cells[i];
        h = Mix(h, (unsigned long)(long)cell.height);
        h = Mix(h, (unsigned long)(cell.passable | cell.camera_passable << 1 | cell.shore_point << 2 | cell.nav_preferred << 3));
        h = Mix(h, (unsigned long)(cell.blend[0] | cell.blend[1] << 8));
        h = Mix(h, (unsigned long)(cell.theme[0] | cell.theme[1] << 8 | cell.theme[2] << 16));
    }
    for (long i = 0; i < MAX_GAME_CELLS; ++i) {
        const GameCell& cell = map.game_cells[i];
        h = Mix(h, (unsigned long)(cell.atmos_blend[0] | cell.atmos_blend[1] << 8));
        h = Mix(h, (unsigned long)(cell.atmos_theme[0] | cell.atmos_theme[1] << 8 | cell.atmos_theme[2] << 16));
        h = Mix(h, (unsigned long)(unsigned char)cell.sound_theme);
    }
    return h;
}

static void SetName(PaletteEntryFile& entry, const char* name, long def_index)
{
    std::strcpy(entry.name, name);
    entry.def_index = def_index;
}
static void BuildDefs(Defs& defs)
{
    static const char* const names[] = { "THEME_GRASS", "THEME_ROCK", "THEME_GHOST", "SOUND_WOODS", "SOUND_CAVE", "ATMOS_DAY", "ATMOS_FOG" };
    static const long class_index[] = { 11, 12, 13, 21, 22, 31, 32 };
    static const bool has_def[] = { true, true, false, true, true, true, true };
    defs.count = 7;
    for (long i = 0; i < defs.count; ++i) {
        defs.names[i] = names[i];
        defs.class_index[i] = class_index[i];
        defs.has_def[i] = has_def[i];
    }
    defs.no_engine_themes = 5;
    defs.invalid_theme_standin = 9;
}
static void BuildFile(LevelFile& file, int scenario)
{
    std::memset(&file, 0, sizeof(file));
    HeaderFile& header = file.header;
    header.thing_uid_count = ((u64)3 << 32) | 1234u;
    for (long i = 0; i < PALETTE; ++i) {
        SetName(header.large[i], "", 0);
        SetName(header.atmos[i], "", 0);
    }
    if (scenario == 0) {
        header.version = 3;
        header.using_extra_map_cells = true;
        SetName(header.large[0], "THEME_GRASS", 0);              // index 0: its def index is 0, the entry is left alone
        SetName(header.large[1], "THEME_NOWHERE", 5);            // unknown name: entry 1 cleared, then given the stand-in
        SetName(header.large[2], "THEME_GRASS", 7);              // global index 1: cleared again by the stand-in pass
        SetName(header.large[3], "THEME_ROCK", 7);               // global index 2
        SetName(header.large[4], "THEME_GHOST", 7);              // a name with no def: cleared
        SetName(header.large[6], "THEME_ROCK", 1);
    } else if (scenario == 1) {
        header.version = 0;
        header.using_extra_map_cells = false;
        SetName(header.large[0], "UNINITIALISED", 0);
    } else {
        header.version = 2;
        header.using_extra_map_cells = true;
        SetName(header.large[1], "THEME_ROCK", 1);
        SetName(header.large[2], "THEME_GRASS", 1);
        SetName(header.large[5], "THEME_ROCK", 1);
    }
    SetName(header.atmos[1], "ATMOS_DAY", 4);
    SetName(header.atmos[2], "ATMOS_NOWHERE", 4);
    SetName(header.atmos[3], "ATMOS_FOG", 4);
    header.no_sound_theme_entries = 4;
    file.sound_names[0] = "SOUND_WOODS";
    file.sound_names[1] = "SOUND_NOWHERE";
    file.sound_names[2] = "SOUND_CAVE";
    for (long i = 0; i < MAX_HEIGHT_CELLS; ++i) {
        HeightCellFile& cell = file.height_cells[i];
        cell.height = (float)(i % 7) * 0.25f + 0.125f;
        cell.passable = (i % 2) == 0;
        cell.camera_passable = (i % 3) == 0;
        cell.shore_point = (i % 5) == 0;
        cell.nav_preferred = (i % 4) == 1;
        cell.sound_index = (signed char)((i + 6) % 7 - 1);      // -1 .. 5: negative, in range, == size() and above
        cell.blend[0] = (unsigned char)(i * 3 + 1);
        cell.blend[1] = (unsigned char)(i * 5 + 2);
        cell.theme[0] = (unsigned char)(i % 8);
        cell.theme[1] = (unsigned char)((i + 3) % 8);
        cell.theme[2] = (unsigned char)((i + 5) % 8);
    }
    for (long i = 0; i < MAX_GAME_CELLS; ++i) {
        GameCellFile& cell = file.game_cells[i];
        cell.blend[0] = (unsigned char)(i + 40);
        cell.blend[1] = (unsigned char)(i * 2 + 60);
        cell.theme[0] = (unsigned char)(i % 4);
        cell.theme[1] = (unsigned char)((i + 1) % 4);
        cell.theme[2] = (unsigned char)((i + 2) % 4);
        cell.sound_index = (signed char)(i % 6 - 1);
    }
}
static void BuildMap(Map& map)
{
    std::memset(&map, 0, sizeof(map));
    map.height_size_x = 8;
    map.height_size_y = 8;
    map.game_size_x = 2;
    map.game_size_y = 2;
    for (long i = 0; i < PALETTE; ++i) {
        map.palette[i].theme_def_index = 100 + i;                 // stale entries: only rewritten where the loader says so
        map.palette[i].used = true;
    }
    for (long i = 0; i < MAX_GAME_CELLS; ++i)
        map.game_cells[i].sound_theme = 55;
}

#define CHECK(c) do { if (!(c)) { if (ok) std::printf("check failed at line %d\n", __LINE__); ok = false; } } while (0)

static Map g_map;
static LevelFile g_file;

static unsigned long Run(int scenario, int mutation)
{
    Defs defs;
    BuildDefs(defs);
    BuildFile(g_file, scenario);
    BuildMap(g_map);
    g_mutation = mutation;
    g_reset_thing_uids = (scenario == 2);
    g_cells_per_suspend = 16;
    Load(g_map, g_file, defs, scenario != 1);
    g_mutation = 0;
    return Signature(g_map, g_file);
}

int main()
{
    bool ok = true;
    unsigned long base[3];
    for (int s = 0; s < 3; ++s)
        base[s] = Run(s, 0);

    // Scenario 0: version 3, named palette, extra cells, a process to suspend.
    Run(0, 0);
    CHECK(g_map.thing_uid_count == (((u64)3 << 32) | 1234u) && !g_map.initialise_passability_from_themes);
    CHECK(g_map.palette[0].theme_def_index == 100 && g_map.palette[0].used);             // def index 0 in the file: untouched
    CHECK(g_map.palette[1].theme_def_index == 9 && g_map.palette[1].used);               // unknown name -> stand-in
    CHECK(g_map.palette[2].theme_def_index == 0 && !g_map.palette[2].used);              // pointed at 1 -> cleared
    CHECK(g_map.palette[3].theme_def_index == 2 && g_map.palette[3].used);
    CHECK(g_map.palette[4].theme_def_index == 0 && !g_map.palette[4].used);              // name without a def
    CHECK(g_map.palette[6].theme_def_index == 2 && g_map.palette[5].theme_def_index == 105);
    CHECK(g_file.height_read == 81 && g_file.game_read == 4);
    CHECK(g_map.suspends == 2 + 6 && g_map.time_calls == 1 + 2 + 2 + 2 * 6 + 1);         // i = 0, 16, 32, 48, 64, 80
    CHECK(g_file.header.atmos[1].def_index == 31 && g_file.header.atmos[2].def_index == 1 && g_file.header.atmos[3].def_index == 32);
    CHECK(g_map.game_cells[0].atmos_theme[0] == 0 && g_map.game_cells[0].atmos_theme[1] == 31 && g_map.game_cells[0].atmos_theme[2] == 1);
    CHECK(g_map.game_cells[0].atmos_blend[0] == 40 && g_map.game_cells[1].atmos_blend[1] == 62);
    CHECK(g_map.game_cells[0].sound_theme == 0 && g_map.game_cells[1].sound_theme == 0);  // index -1 and 0
    CHECK(g_map.game_cells[2].sound_theme == 21 && g_map.game_cells[3].sound_theme == 0); // SOUND_WOODS, then the unknown name
    {
        const HeightCell& first = HeightCellAt(g_map, 0, 0);                                  // file cell 0
        CHECK(first.height == 0.125f * 2048.0f && first.passable && first.camera_passable && first.shore_point);
        CHECK(first.blend[0] == 1 && first.blend[1] == 2);
        CHECK(first.theme[0] == 0 && first.theme[1] == 3 && first.theme[2] == 5);          // entries 3 and 5 are not empty
        const HeightCell& remapped = HeightCellAt(g_map, 1, 0);                                // file cell 1: themes 1, 4, 6
        CHECK(remapped.theme[0] == 1 && remapped.theme[1] == 1 && remapped.theme[2] == 6);  // entry 4 was cleared -> 1
        const HeightCell& second = HeightCellAt(g_map, 2, 0);                                  // file cell 2: passable, camera fixed up
        CHECK(second.passable && second.camera_passable && !second.nav_preferred);
        const HeightCell& last = HeightCellAt(g_map, 8, 8);                                    // file cell 80, the extra corner
        CHECK(last.height == (3.0f * 0.25f + 0.125f) * 2048.0f && last.blend[0] == (unsigned char)(80 * 3 + 1));
    }

    // Scenario 1: version 0, "UNINITIALISED" palette, no extra cells, no process.
    Run(1, 0);
    CHECK(g_map.initialise_passability_from_themes && g_map.suspends == 0 && g_map.time_calls == 2);
    CHECK(g_map.palette[0].theme_def_index == 0 && g_map.palette[0].used);
    CHECK(g_map.palette[1].theme_def_index == 1 && g_map.palette[4].theme_def_index == 4 && g_map.palette[4].used);
    CHECK(g_map.palette[5].theme_def_index == 0 && !g_map.palette[5].used && !g_map.palette[255].used);
    CHECK(g_file.height_read == 64 && g_file.game_read == 0);
    CHECK(g_map.game_cells[0].atmos_blend[0] == 0);                                       // no game cells below version 2
    // old sound layer: height cell (x, y) covers game cell (x / 4, y / 4); the last cell written wins
    CHECK(GameCellAt(g_map, 0, 0).sound_theme == 77);                                     // cell 27: index 4 == size() -> the slot after the list
    CHECK(GameCellAt(g_map, 1, 0).sound_theme == 21 && GameCellAt(g_map, 0, 1).sound_theme == 21);   // cells 31 and 59: SOUND_WOODS
    CHECK(GameCellAt(g_map, 1, 1).sound_theme == 0);                                      // cell 63: index 5 > size() -> 0

    // Scenario 2: version 2 with the UID reset switch: game cells are read, their sound theme is not.
    Run(2, 0);
    CHECK(g_map.thing_uid_count == 1 && !g_map.initialise_passability_from_themes);
    CHECK(g_map.palette[1].theme_def_index == 2 && g_map.palette[2].theme_def_index == 1 && g_map.palette[2].used);  // no stand-in pass
    CHECK(g_file.game_read == 4 && g_map.game_cells[3].atmos_blend[0] == 43);
    CHECK(g_map.game_cells[3].atmos_theme[0] == 32);

    // Every mutation must change at least one scenario's signature.
    for (int m = 1; m <= 25; ++m) {
        bool changed = false;
        for (int s = 0; s < 3; ++s)
            if (Run(s, m) != base[s])
                changed = true;
        if (!changed) {
            std::printf("mutation %d not detected\n", m);
            ok = false;
        }
    }
    // The model is deterministic.
    for (int s = 0; s < 3; ++s)
        CHECK(Run(s, 0) == base[s]);

    if (!ok) { std::printf("00821a80_TEST FAIL\n"); return 1; }
    std::printf("00821a80_TEST PASS\n");
    return 0;
}
