// Behaviour test for CThing::ConstructFromLevelScript (0x004CAA00): a host model of the same control flow over a small level script
// (identity unless only the hero state loads, map number from the script or the level being loaded, the two persistence flags, the
// ScriptName / ScriptData keys up to EndThing, the remapped UID, def / editor / map-who components, then every live component's level
// state).
#include <cstdio>
#include <cstring>

#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
static long ReadNumber(const char* p) { long v = 0; while (*p >= '0' && *p <= '9') v = v * 10 + (*p++ - '0'); return v; }

struct Parser {                                   // CStringParser over whitespace / ';' separated items; '"' is a symbol item
    const char* s; unsigned long pos;
    explicit Parser(const char* text) : s(text), pos(0) {}
    void Skip() { while (s[pos] == ' ' || s[pos] == '\n') ++pos; }
    bool NextItemExists() { Skip(); return s[pos] != 0; }
    bool PeekSymbolQuote() { Skip(); return s[pos] == '"'; }
    void ReadAsString(char* out) {
        Skip(); unsigned long n = 0;
        while (s[pos] && s[pos] != ' ' && s[pos] != ';' && s[pos] != '"' && s[pos] != '\n') out[n++] = s[pos++];
        out[n] = 0;
    }
    void ReadQuote() { Skip(); if (s[pos] == '"') ++pos; }
    void ReadUntilQuote(char* out) { unsigned long n = 0; while (s[pos] && s[pos] != '"') out[n++] = s[pos++]; out[n] = 0; if (s[pos]) ++pos; }
    void SkipPastSemicolon() { while (s[pos] && s[pos] != ';') ++pos; if (s[pos]) ++pos; }
    bool FindBool(const char* key) {              // the CPersistContext text transfer: "<key> TRUE;" anywhere, default false
        const char* p = std::strstr(s, key); if (!p) return false;
        p += std::strlen(key); while (*p == ' ') ++p;
        return std::strncmp(p, "TRUE", 4) == 0;
    }
};

struct TC { bool to_be_removed; long level_state_calls; bool last_loading_arg; };

struct Thing {
    unsigned long ThingTypeFlags; bool hero_world, editor, serialising_game_state, reset_uids, has_def, init_ok, level_less;
    bool CreatedBySerialisation, GameStatePersisted, LevelPersistent, AddingTCsPermitted;
    u64 UniqueID, hero_uid, info_uid, created_uid; long CurrentMapNumber, level_being_loaded, created_from_def;
    char script_name[32], script_data[32], added[4][16]; long nadded, creatable_calls;
    TC comps[3]; long ncomps;
    u64 Remap(bool, bool lless, long map, u64 uid) { return uid + (lless ? 1000 : 0) + (u64)map * 10; }
    bool Construct(const char* script) {
        bool hero_state_only = false;
        if ((ThingTypeFlags & 2) && hero_world) hero_state_only = true;
        else { CreatedBySerialisation = true; LevelPersistent = false; }
        Parser parser(script);
        u64 uid = 0;
        if (hero_state_only) uid = hero_uid;
        else { if (!init_ok) return false; uid = info_uid; }
        if (!hero_state_only) {
            if (serialising_game_state) { const char* p = std::strstr(script, "ThingCurrentMapID "); CurrentMapNumber = p ? (short)ReadNumber(p + 18) : 0; }
            else CurrentMapNumber = (short)level_being_loaded;
        }
        bool g = parser.FindBool("ThingGamePersistent"), l = parser.FindBool("ThingLevelPersistent");
        GameStatePersisted = g; LevelPersistent = l;
        while (parser.NextItemExists()) {
            char key[32]; parser.ReadAsString(key);
            if (std::strcmp(key, "ScriptName") == 0) parser.ReadAsString(script_name);
            else if (std::strcmp(key, "ScriptData") == 0) {
                if (parser.PeekSymbolQuote()) { parser.ReadQuote(); parser.ReadUntilQuote(script_data); }
            } else if (std::strcmp(key, "EndThing") == 0) break;
            parser.SkipPastSemicolon();
        }
        if (!serialising_game_state) {
            UniqueID = Remap(false, level_less, CurrentMapNumber, uid);
            if (reset_uids) UniqueID = 0;
            if (UniqueID == 0) UniqueID = created_uid;
        }
        if (!hero_state_only) {
            if (has_def) ++created_from_def;
            if (editor) std::strcpy(added[nadded++], "CTCEditor");
            std::strcpy(added[nadded++], "CTCMapwho");
            ++creatable_calls;
        }
        AddingTCsPermitted = false;
        for (long i = 0; i < ncomps; ++i) {
            if (comps[i].to_be_removed) continue;
            ++comps[i].level_state_calls; comps[i].last_loading_arg = !editor;
        }
        AddingTCsPermitted = true;
        return true;
    }
};

static const char* kScript =
    "NewThing Object \"Box\";\n ThingCurrentMapID 12;\n ThingGamePersistent TRUE;\n ThingLevelPersistent FALSE;\n"
    " ScriptName Gate;\n ScriptData \"open sesame\";\n Health 5;\n EndThing;\n ScriptName Wrong;";

int main()
{
    Thing t; std::memset(&t, 0, sizeof(t));
    t.editor = true; t.has_def = true; t.init_ok = true; t.info_uid = 7; t.level_being_loaded = 3; t.ncomps = 3;
    t.comps[1].to_be_removed = true; t.created_uid = 99;
    if (!t.Construct(kScript) || !t.CreatedBySerialisation || t.CurrentMapNumber != 3 || !t.GameStatePersisted || t.LevelPersistent
        || std::strcmp(t.script_name, "Gate") != 0 || std::strcmp(t.script_data, "open sesame") != 0 || t.UniqueID != 37
        || t.nadded != 2 || std::strcmp(t.added[0], "CTCEditor") != 0 || std::strcmp(t.added[1], "CTCMapwho") != 0 || t.created_from_def != 1
        || t.creatable_calls != 1 || t.comps[0].level_state_calls != 1 || t.comps[1].level_state_calls != 0 || t.comps[2].last_loading_arg
        || !t.AddingTCsPermitted) {
        std::printf("004caa00_TEST FAIL\n"); return 1;
    }
    // Game-state load: the map number comes from the script, the UID is not remapped.
    Thing u; std::memset(&u, 0, sizeof(u)); u.init_ok = true; u.serialising_game_state = true; u.info_uid = 5; u.UniqueID = 42;
    if (!u.Construct(kScript) || u.CurrentMapNumber != 12 || u.UniqueID != 42 || u.nadded != 1) { std::printf("004caa00_TEST FAIL\n"); return 1; }
    // Hero-state only: no identity, no components added; the hero's UID is remapped; the reset flag forces a fresh UID.
    Thing h; std::memset(&h, 0, sizeof(h)); h.ThingTypeFlags = 2; h.hero_world = true; h.hero_uid = 4; h.level_less = true; h.CurrentMapNumber = 1;
    if (!h.Construct(kScript) || h.CreatedBySerialisation || h.UniqueID != 1014 || h.nadded != 0 || h.creatable_calls != 0) { std::printf("004caa00_TEST FAIL\n"); return 1; }
    Thing r; std::memset(&r, 0, sizeof(r)); r.init_ok = true; r.reset_uids = true; r.created_uid = 55;
    if (!r.Construct(kScript) || r.UniqueID != 55) { std::printf("004caa00_TEST FAIL\n"); return 1; }
    // A failed InitFromThingInfo stops before anything is parsed.
    Thing f; std::memset(&f, 0, sizeof(f));
    if (f.Construct(kScript) || f.script_name[0] != 0 || f.nadded != 0) { std::printf("004caa00_TEST FAIL\n"); return 1; }
    std::printf("004caa00_TEST PASS\n");
    return 0;
}
