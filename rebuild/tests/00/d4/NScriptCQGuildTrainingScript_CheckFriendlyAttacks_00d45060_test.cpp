// Behaviour test for NScript::CQ_GuildTrainingScript::CheckFriendlyAttacks (0x00D45060): a state model of the Guild guard thread.
// At start (and again every time the hero comes back into HeroGuildComplex after the level was unloaded, once it is loaded again)
// every creature except CREATURE_BIRD_GUILD_SPARROW, CREATURE_RIVAL_HERO_WHISPER_APPRENTICE, ..._WHISPER_TEEN_APPRENTICE and
// CREATURE_RIVAL_HERO_MAZE becomes friends with everything and unkillable. Each frame, when the hero hit a friend with bare hands,
// a melee or a ranged weapon, the Guild complex is loaded, the maze rival was not hit by the hero and was hit by no special ability
// of the hero other than Heal Life: warnings 0..2 speak the FIRST / SECOND / THIRD guild-seal line and count up; the fourth sets
// GuildWarningOccuring and, unless a skill or will test is running (then it only waits that out), scripts the maze rival and the
// hero and plays CS_GUILD_BADHERO with the FOURTH warning; then the warnings and GuildWarningOccuring reset.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Creature { const char* Def; bool Friends, Unkillable; };
struct World {
    int Frame, TerminateAt;
    Creature C[8]; int NC;
    int UnloadFrom, UnloadTo;       // level unloaded on frames [UnloadFrom, UnloadTo)
    int HitAt[8]; int NHit;         // frames on which the hero hits a friend (kind rotates bare / melee / ranged)
    bool MazeHitByHero, MazeAnySpecial, MazeHeal;
    int TestUntil;                  // skill test runs while Frame < TestUntil
    long HeroWarnings; bool GuildWarningOccuring;
    void NSF() { ++Frame; }
    bool T() const { return Frame >= TerminateAt; }
    bool Loaded() const { return !(Frame >= UnloadFrom && Frame < UnloadTo); }
    bool Hit() const { for (int i = 0; i < NHit; ++i) if (HitAt[i] == Frame) return true; return false; }
    bool Testing() const { return Frame < TestUntil; }
    bool FriendAll()
    {
        for (int i = 0; i < NC; ++i) {
            if (T()) return false;
            const char* d = C[i].Def;
            if (std::strcmp(d, "CREATURE_BIRD_GUILD_SPARROW") && std::strcmp(d, "CREATURE_RIVAL_HERO_WHISPER_APPRENTICE")
                && std::strcmp(d, "CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE") && std::strcmp(d, "CREATURE_RIVAL_HERO_MAZE")) {
                C[i].Friends = true; C[i].Unkillable = true;
            }
        }
        return !T();
    }
    void Run()
    {
        if (!FriendAll()) return;
        for (;; NSF()) {
            if (T()) return;
            if (!Loaded()) {
                while (!Loaded()) { NSF(); if (T()) return; }
                Log("reloaded");
                if (!FriendAll()) return;
            }
            if (Hit() && Loaded() && !MazeHitByHero && (!MazeAnySpecial || MazeHeal)) {
                if (HeroWarnings < 3) {
                    static const char* w[3] = { "FIRST", "SECOND", "THIRD" };
                    Log(w[HeroWarnings]);
                    ++HeroWarnings;
                } else {
                    GuildWarningOccuring = true;
                    if (!Testing()) { Log("FOURTH"); Log("CS_GUILD_BADHERO"); }
                    else { while (Testing()) { NSF(); if (T()) return; } Log("waited"); }
                    HeroWarnings = 0; GuildWarningOccuring = false;
                }
            }
        }
    }
};
static World Make()
{
    World w; std::memset(&w, 0, sizeof w);
    w.TerminateAt = 40; w.UnloadFrom = w.UnloadTo = -1;
    static const char* d[5] = { "CREATURE_GUILD_GUARD", "CREATURE_BIRD_GUILD_SPARROW", "CREATURE_RIVAL_HERO_MAZE",
                                "CREATURE_RIVAL_HERO_WHISPER_APPRENTICE", "CREATURE_GUILDMASTER" };
    for (int i = 0; i < 5; ++i) w.C[i].Def = d[i];
    w.NC = 5;
    return w;
}
int main()
{
    bool ok = true;
    World a = Make(); a.NHit = 5; a.HitAt[0] = 2; a.HitAt[1] = 4; a.HitAt[2] = 6; a.HitAt[3] = 8; a.HitAt[4] = 10;
    a.Run();   // excluded creatures stay hostile/killable; three warnings, the cut scene, then the count starts again
    ok = ok && !std::strcmp(g_log, "FIRST;SECOND;THIRD;FOURTH;CS_GUILD_BADHERO;FIRST;") && a.HeroWarnings == 1
         && a.C[0].Friends && a.C[0].Unkillable && !a.C[1].Friends && !a.C[2].Friends && !a.C[3].Unkillable && a.C[4].Friends;
    g_log[0] = 0;
    World b = Make(); b.NHit = 2; b.HitAt[0] = 3; b.HitAt[1] = 5; b.MazeHitByHero = true;
    b.Run();   // the rival in the maze was hit by the hero: no warning
    ok = ok && !std::strcmp(g_log, "") && b.HeroWarnings == 0;
    g_log[0] = 0;
    World c = Make(); c.NHit = 2; c.HitAt[0] = 3; c.HitAt[1] = 5; c.MazeAnySpecial = true;
    c.Run();   // a special ability on the maze rival (not Heal Life): no warning
    ok = ok && !std::strcmp(g_log, "");
    g_log[0] = 0;
    World e = Make(); e.NHit = 1; e.HitAt[0] = 3; e.MazeAnySpecial = true; e.MazeHeal = true;
    e.Run();   // Heal Life still counts
    ok = ok && !std::strcmp(g_log, "FIRST;");
    g_log[0] = 0;
    World f = Make(); f.UnloadFrom = 5; f.UnloadTo = 9; f.NHit = 2; f.HitAt[0] = 6; f.HitAt[1] = 9; f.C[1].Def = "CREATURE_BANDIT";
    f.C[0].Def = "CREATURE_BIRD_GUILD_SPARROW";
    f.Run();   // a hit while the complex is unloaded is ignored; coming back re-friends; the hit on frame 9 counts
    ok = ok && !std::strcmp(g_log, "reloaded;FIRST;") && f.C[1].Friends && !f.C[0].Friends;
    g_log[0] = 0;
    World g = Make(); g.HeroWarnings = 3; g.NHit = 2; g.HitAt[0] = 2; g.HitAt[1] = 20; g.TestUntil = 12;
    g.Run();   // fourth hit during a skill test: wait it out, no cut scene, reset; next hit is a first warning again
    ok = ok && !std::strcmp(g_log, "waited;FIRST;") && g.HeroWarnings == 1 && !g.GuildWarningOccuring;
    std::printf("00d45060_TEST %s\n", ok ? "PASS" : "FAIL");
    return 0;
}