// Behaviour test for CTCWeaponTrail::InitialiseTrails (0x0076EB00): nothing happens until the appearance is loaded; then
// weapon_pos_a/b and weapon_pos_c/d each make a trail when either helper exists, any further "weapon_pos_*" helper present
// exactly twice (and not already used) makes one more trail named after itself, a projectile trail exists when both of the
// weapon def's projectile helpers exist, and the per-trail vectors are sized to the trail count (ticks start at the global
// fade-out frame count).
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

struct Appearance {
    bool loaded;
    std::map<std::string, int> helpers;            // helper name -> number of points
    std::vector<std::string> names;                // GetAllHelperPointNames order
    bool Has(const std::string& n) const { return helpers.count(n) != 0; }
    int Count(const std::string& n) const { std::map<std::string, int>::const_iterator it = helpers.find(n); return it == helpers.end() ? 0 : it->second; }
};
struct Trail {
    long NoTrails, NoProjectileTrails;
    std::vector<std::pair<std::string, std::string> > HelperNames;
    std::vector<bool> ActiveTrail;
    std::vector<long> InactiveTicks;
    std::vector<int> Trails;
};

static void InitialiseTrails(Trail& t, const Appearance& a, bool hasWeaponDef, const char* tip, const char* base, long fadeFrames)
{
    if (!a.loaded) return;
    t.NoTrails = 0; t.NoProjectileTrails = 0;
    std::vector<std::string> used;
    if (a.Has("weapon_pos_a") || a.Has("weapon_pos_b")) {
        used.push_back("weapon_pos_a"); used.push_back("weapon_pos_b");
        t.HelperNames.push_back(std::make_pair(std::string("weapon_pos_a"), std::string("weapon_pos_b"))); ++t.NoTrails;
    }
    if (a.Has("weapon_pos_c") || a.Has("weapon_pos_d")) {
        used.push_back("weapon_pos_c"); used.push_back("weapon_pos_d");
        t.HelperNames.push_back(std::make_pair(std::string("weapon_pos_c"), std::string("weapon_pos_d"))); ++t.NoTrails;
    }
    for (size_t i = 0; i < a.names.size(); ++i) {
        const std::string& n = a.names[i];
        if (std::strncmp(n.c_str(), "weapon_pos_", 11) == 0 &&
            std::find(used.begin(), used.end(), n) == used.end() && a.Count(n) == 2) {
            t.HelperNames.push_back(std::make_pair(n, n)); ++t.NoTrails; used.push_back(n);
        }
    }
    if (hasWeaponDef && a.Has(base) && a.Has(tip)) t.NoProjectileTrails = 1;
    t.ActiveTrail.resize(t.NoTrails, false);
    t.InactiveTicks.resize(t.NoTrails, fadeFrames);
    t.Trails.resize(t.NoTrails);
}

int main()
{
    bool ok = true;
    Appearance a;
    a.loaded = false;
    Trail t; t.NoTrails = -1; t.NoProjectileTrails = -1;
    InitialiseTrails(t, a, true, "tip", "base", 5);
    ok = ok && t.NoTrails == -1;                                    // not loaded: untouched
    a.loaded = true;
    a.helpers["weapon_pos_b"] = 1;                                 // one of a/b is enough
    a.helpers["weapon_pos_x"] = 2;                                 // a pair of its own
    a.helpers["weapon_pos_y"] = 1;                                 // only one point: ignored
    a.helpers["other"] = 2;                                        // wrong prefix: ignored
    a.helpers["tip"] = 1; a.helpers["base"] = 1;
    a.names.push_back("weapon_pos_b"); a.names.push_back("weapon_pos_x"); a.names.push_back("weapon_pos_x");
    a.names.push_back("weapon_pos_y"); a.names.push_back("other");
    InitialiseTrails(t, a, true, "tip", "base", 5);
    ok = ok && t.NoTrails == 2 && t.NoProjectileTrails == 1 && t.HelperNames.size() == 2
            && t.HelperNames[0].first == "weapon_pos_a" && t.HelperNames[1].second == "weapon_pos_x"
            && t.ActiveTrail.size() == 2 && !t.ActiveTrail[1] && t.InactiveTicks.size() == 2 && t.InactiveTicks[0] == 5
            && t.Trails.size() == 2;
    Trail u; u.NoTrails = -1; u.NoProjectileTrails = -1;
    InitialiseTrails(u, a, false, "tip", "base", 5);
    ok = ok && u.NoProjectileTrails == 0;
    if (!ok) { std::printf("0076eb00_TEST FAIL\n"); return 1; }
    std::printf("0076eb00_TEST PASS\n");
    return 0;
}