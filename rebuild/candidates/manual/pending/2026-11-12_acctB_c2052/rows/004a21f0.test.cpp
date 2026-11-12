// Behaviour test for 0x004A21F0 CWorld::LoadGameStateInternal: a model of the load flow (signature gate, binary "FableSav" versus
// text loading, the HEADER defaults, the section order, the hero-state early return, the play-clock restore, the menu reset and
// the stale autosave deletion) checked on three worlds.
#include <cstdio>
#include <cstring>

struct Menu { double AccumulativePausedTime; };
struct World {
    bool SerialisingHeroState, AutoSaveLocked, TeleportingEnabled, GuildMasterMessagesEnabled, signature_ok, has_menu, autosave_exists;
    long MostRecentSaveType; double StartTime, TimePlayed; int activated, deleted, sections; char log[256]; Menu menu;
    void Section(const char* name) { std::strcat(log, name); std::strcat(log, ";"); ++sections; }
    bool Load(const unsigned char* file, unsigned int size, double now, double saved_time)
    {
        if (!signature_ok) return false;
        log[0] = 0; sections = 0;
        double total_time_played = 0.0;
        AutoSaveLocked = true;
        bool binary = size >= 8 && std::memcmp(file, "FableSav", 8) == 0;
        if (binary) {
            unsigned int header_size, body_size;
            std::memcpy(&header_size, file + 16, 4); std::memcpy(&body_size, file + 20, 4);
            long long total = (long long)header_size + body_size;
            if (total < 0 || total > 0x7fffffff) return false;
            Section("BIN");
        } else Section("TEXT");
        Section("HEADER");
        TeleportingEnabled = true; GuildMasterMessagesEnabled = false;   // the HEADER defaults (absent keys)
        if (!SerialisingHeroState) { MostRecentSaveType = 2; total_time_played = saved_time; }
        if (!SerialisingHeroState) Section("TIME");
        Section("ENTITIES");
        if (!SerialisingHeroState) { Section("PLAYER"); Section("QUESTS"); Section("REGIONS"); Section("FACTIONS"); }
        AutoSaveLocked = false;
        if (SerialisingHeroState) return true;
        StartTime = now - total_time_played;
        TimePlayed = total_time_played;
        if (has_menu) menu.AccumulativePausedTime = 0.0;
        ++activated;
        if (autosave_exists) ++deleted;
        return true;
    }
};
static World Make()
{
    World w; std::memset(&w, 0, sizeof(w));
    w.signature_ok = true; w.has_menu = true; w.autosave_exists = true; w.menu.AccumulativePausedTime = 5.0; w.MostRecentSaveType = 7;
    return w;
}
int main()
{
    unsigned char bin[24] = { 'F','a','b','l','e','S','a','v' }; unsigned int h = 3, b = 4;
    std::memcpy(bin + 16, &h, 4); std::memcpy(bin + 20, &b, 4);
    World a = Make();
    if (!a.Load(bin, 24, 100.0, 40.0) || std::strcmp(a.log, "BIN;HEADER;TIME;ENTITIES;PLAYER;QUESTS;REGIONS;FACTIONS;") != 0
        || a.StartTime != 60.0 || a.TimePlayed != 40.0 || a.menu.AccumulativePausedTime != 0.0 || a.activated != 1 || a.deleted != 1
        || a.AutoSaveLocked || a.MostRecentSaveType != 2) { std::printf("004a21f0_TEST FAIL\n"); return 1; }
    World h2 = Make(); h2.SerialisingHeroState = true;
    const unsigned char text[] = "HEADER";
    if (!h2.Load(text, 6, 100.0, 40.0) || std::strcmp(h2.log, "TEXT;HEADER;ENTITIES;") != 0 || h2.activated != 0 || h2.deleted != 0
        || h2.menu.AccumulativePausedTime != 5.0 || h2.MostRecentSaveType != 7 || h2.AutoSaveLocked) { std::printf("004a21f0_TEST FAIL\n"); return 1; }
    World s = Make(); s.signature_ok = false;
    if (s.Load(bin, 24, 1.0, 1.0) || s.AutoSaveLocked) { std::printf("004a21f0_TEST FAIL\n"); return 1; }
    World o = Make(); unsigned int big = 0x7fffffffU; std::memcpy(bin + 16, &big, 4); std::memcpy(bin + 20, &big, 4);
    if (o.Load(bin, 24, 1.0, 1.0)) { std::printf("004a21f0_TEST FAIL\n"); return 1; }
    std::printf("004a21f0_TEST PASS\n");
    return 0;
}
