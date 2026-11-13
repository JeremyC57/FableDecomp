// Behaviour test for NThingManager::CEntityRuntimePersistence::LoadFromGameStateScript (0x005248B0): a host model of the row,
// statement for statement as src/005248b0.cpp:
//  - Clear(), then skip to the first START_THING_LEVEL; for each START_THING_LEVEL block read the level number, remap it through
//    the world map's GetMapNumberFromMapUID when it is not a map index (keeping it when that gives 0), read the script up to
//    END_THING_LEVEL and store it as the game-persistent script of that level (table grown to level + 1, slot marked valid);
//  - then, after START_DEAD_THINGS, each START_NEW_DEAD_THING ... END_NEW_DEAD_THING record transfers UID / SectionNameCRC (default 0)
//    and adds a game-persistent dead entity unless the UID is 0; finally skip past END_DEAD_THINGS.
#include <cstdio>
#include <cstring>
#include <cstdlib>

static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { std::printf("check failed line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)

static bool IsSpace(char c) { return c == ' ' || c == '\t' || c == '\r' || c == '\n'; }

struct Parser {
    const char* text; unsigned long pos;
    void SkipWhitespace() { while (text[pos] && IsSpace(text[pos])) ++pos; }
    void SkipUntilWhitespace() { while (text[pos] && !IsSpace(text[pos])) ++pos; }
    bool SkipPastNextItemIfIs(const char* s)
    {
        SkipWhitespace();
        unsigned long n = (unsigned long)std::strlen(s);
        if (std::strncmp(text + pos, s, n) == 0 && (text[pos + n] == 0 || IsSpace(text[pos + n]))) { pos += n; return true; }
        return false;
    }
    long ReadAsInteger() { SkipWhitespace(); char* end; long v = std::strtol(text + pos, &end, 10); pos = (unsigned long)(end - text); return v; }
    bool SkipPastString(const char* s)
    {
        const char* f = std::strstr(text + pos, s);
        if (!f) return false;
        pos = (unsigned long)(f - text) + (unsigned long)std::strlen(s);
        return true;
    }
    void ReadAsStringUntilString(const char* s, char* out)
    {
        const char* f = std::strstr(text + pos, s);
        unsigned long end = f ? (unsigned long)(f - text) : (unsigned long)std::strlen(text);
        std::memcpy(out, text + pos, end - pos); out[end - pos] = 0;
        pos = end;
    }
    void ReadAsStringUntilPastString(const char* s, char* out)
    {
        ReadAsStringUntilString(s, out);
        if (text[pos]) pos += (unsigned long)std::strlen(s);
    }
};

// text CPersistContext model: "<name> <value>;" items, the default when the name is absent
static unsigned long TransferULong(const char* text, const char* name, unsigned long def)
{
    const char* f = std::strstr(text, name);
    if (!f) return def;
    return std::strtoul(f + std::strlen(name), 0, 10);
}

struct ScriptInfo { char Script[64]; bool Valid; };
struct DeadEntity { unsigned long UID; bool GamePersistent; unsigned long SectionNameCRC; };

struct WorldMap {
    long NoMaps;
    unsigned long UIDs[8];
    long GetNoMaps() const { return NoMaps; }
    long GetMapNumberFromMapUID(unsigned long uid) const
    {
        for (long i = 1; i < NoMaps; ++i) if (UIDs[i] == uid) return i;
        return 0;
    }
};

struct Persistence {
    ScriptInfo Game[16]; unsigned long GameSize;
    DeadEntity Dead[8]; long DeadCount;
    void Clear() { GameSize = 0; DeadCount = 0; }
    void SetGamePersistentScript(long index, const char* script)
    {
        unsigned long n = (unsigned long)(index + 1) > GameSize ? (unsigned long)(index + 1) : GameSize;
        for (unsigned long i = GameSize; i < n; ++i) { Game[i].Script[0] = 0; Game[i].Valid = false; }
        GameSize = n;
        std::strcpy(Game[index].Script, script);
        Game[index].Valid = true;
    }
    void AddPersistedDeadEntity(unsigned long uid, bool game_persistent, unsigned long crc)
    {
        if (uid != 0) { Dead[DeadCount].UID = uid; Dead[DeadCount].GamePersistent = game_persistent; Dead[DeadCount].SectionNameCRC = crc; ++DeadCount; }
    }
    void LoadFromGameStateScript(Parser& parser, const WorldMap& world_map)
    {
        Clear();
        char tmp[256];
        parser.ReadAsStringUntilString("START_THING_LEVEL", tmp);
        while (parser.SkipPastNextItemIfIs("START_THING_LEVEL")) {
            parser.SkipUntilWhitespace();
            unsigned long level = parser.ReadAsInteger();
            if (level >= (unsigned long)world_map.GetNoMaps()) {
                long map_number = world_map.GetMapNumberFromMapUID(level);
                if (map_number > 0)
                    level = map_number;
            }
            char script[256];
            parser.ReadAsStringUntilString("END_THING_LEVEL", script);
            parser.SkipPastString("END_THING_LEVEL");
            SetGamePersistentScript(level, script);
        }
        parser.SkipWhitespace();
        if (parser.SkipPastNextItemIfIs("START_DEAD_THINGS")) {
            while (parser.SkipPastNextItemIfIs("START_NEW_DEAD_THING")) {
                char text[256];
                parser.ReadAsStringUntilPastString("END_NEW_DEAD_THING", text);
                const unsigned long default_uid = 0;
                const unsigned long default_crc = 0;
                unsigned long uid = TransferULong(text, "UID", default_uid);
                unsigned long crc = TransferULong(text, "SectionNameCRC", default_crc);
                AddPersistedDeadEntity(uid, true, crc);
            }
        }
        parser.SkipPastString("END_DEAD_THINGS");
    }
};

int main()
{
    WorldMap world; world.NoMaps = 4; world.UIDs[0] = 0; world.UIDs[1] = 1001; world.UIDs[2] = 1002; world.UIDs[3] = 1005;
    static const char text[] =
        "HEADER junk START_THING_LEVEL 2 aaa END_THING_LEVEL\n"
        "START_THING_LEVEL 1005 bbb END_THING_LEVEL\n"
        "START_THING_LEVEL 6 ccc END_THING_LEVEL\n"
        "START_DEAD_THINGS\n"
        "START_NEW_DEAD_THING UID 77; SectionNameCRC 9; END_NEW_DEAD_THING\n"
        "START_NEW_DEAD_THING UID 0; SectionNameCRC 3; END_NEW_DEAD_THING\n"
        "START_NEW_DEAD_THING SectionNameCRC 4; UID 55; END_NEW_DEAD_THING\n"
        "END_DEAD_THINGS tail";
    Parser parser; parser.text = text; parser.pos = 0;
    Persistence p; p.GameSize = 3; p.DeadCount = 2;
    p.LoadFromGameStateScript(parser, world);
    // level 2 kept, UID 1005 -> map 3, 6 (not a map index, no map with that UID) stays 6; the stale table was cleared first
    CHECK(p.GameSize == 7);
    CHECK(std::strcmp(p.Game[2].Script, " aaa ") == 0 && std::strcmp(p.Game[3].Script, " bbb ") == 0 && std::strcmp(p.Game[6].Script, " ccc ") == 0);
    CHECK(p.Game[6].Valid && !p.Game[5].Valid);
    CHECK(p.DeadCount == 2);
    CHECK(p.Dead[0].UID == 77 && p.Dead[0].SectionNameCRC == 9 && p.Dead[0].GamePersistent);
    CHECK(p.Dead[1].UID == 55 && p.Dead[1].SectionNameCRC == 4 && p.Dead[1].GamePersistent);
    CHECK(std::strcmp(text + parser.pos, " tail") == 0);

    static const char text2[] =
        "START_THING_LEVEL 2 aaa END_THING_LEVEL\n"
        "START_THING_LEVEL 1005 bbb END_THING_LEVEL\n"
        "START_THING_LEVEL 9 ccc END_THING_LEVEL\n"
        "NO_DEAD_THINGS END_DEAD_THINGS!";
    Parser parser2; parser2.text = text2; parser2.pos = 0;
    Persistence q; q.GameSize = 0; q.DeadCount = 5;
    q.LoadFromGameStateScript(parser2, world);
    CHECK(q.GameSize == 10);
    CHECK(q.Game[2].Valid && std::strcmp(q.Game[2].Script, " aaa ") == 0);
    CHECK(q.Game[3].Valid && std::strcmp(q.Game[3].Script, " bbb ") == 0);
    CHECK(q.Game[9].Valid && std::strcmp(q.Game[9].Script, " ccc ") == 0);
    CHECK(!q.Game[0].Valid && !q.Game[1].Valid && !q.Game[4].Valid && !q.Game[8].Valid);
    CHECK(q.DeadCount == 0);
    CHECK(std::strcmp(text2 + parser2.pos, "!") == 0);

    if (g_fail) { std::printf("005248b0_TEST FAIL\n"); return 1; }
    std::printf("005248b0_TEST PASS\n");
    return 0;
}