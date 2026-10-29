// Behaviour test for NScript::CV_BookCollectingScript::CBS_Teacher::AdjustHeroReputation 0x00E56D10: the hero's deed goes to
// the teacher first, then to boy0, boy1, ... while a living thing has that script name, then to girl0, girl1, ... the same
// way; a dead or missing pupil ends that pass. Also checks the counted handle bookkeeping of the reused `thing` local.
#include <cstdio>
#include <cstring>
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static int g_live;                                   // live counted handles
struct Thing {                                       // a script-thing handle: name + alive flag, counted
    char name[16]; bool valid, alive;
    Thing() { name[0] = 0; valid = false; alive = false; ++g_live; }
    Thing(const Thing& o) { std::strcpy(name, o.name); valid = o.valid; alive = o.alive; ++g_live; }
    ~Thing() { --g_live; }
    Thing& operator=(const Thing& o) { std::strcpy(name, o.name); valid = o.valid; alive = o.alive; return *this; }
    bool IsAlive() const { return valid && alive; }
};
static const char* g_alive[] = { "boy0", "boy1", "boy2", "girl0", 0 };
static const char* g_dead[] = { "girl1", 0 };
struct Game {
    Thing hero;
    Thing GetThingWithScriptName(const char* n) const
    {
        Thing t; std::strcpy(t.name, n);
        for (int i = 0; g_alive[i]; ++i) if (std::strcmp(g_alive[i], n) == 0) { t.valid = true; t.alive = true; }
        for (int i = 0; g_dead[i]; ++i) if (std::strcmp(g_dead[i], n) == 0) { t.valid = true; t.alive = false; }
        return t;
    }
    void EntityPostOpinionDeedToRecipient(const Thing& entity, int deed, const Thing& recipient) const
    {
        char b[64]; std::sprintf(b, "%s>%d>%s", entity.name, deed, recipient.name); Log(b);
    }
    Thing& GetHero() { return hero; }
};
struct Teacher {
    Game* Game_;
    Thing Me;
    void AdjustHeroReputation(int deed)
    {
        Game_->EntityPostOpinionDeedToRecipient(Game_->GetHero(), deed, Me);
        long i = 0;
        char name[16];
        std::sprintf(name, "boy%ld", i);
        Thing thing = Game_->GetThingWithScriptName(name);
        while (thing.IsAlive()) {
            Game_->EntityPostOpinionDeedToRecipient(Game_->GetHero(), deed, thing);
            ++i;
            std::sprintf(name, "boy%ld", i);
            thing = Game_->GetThingWithScriptName(name);
        }
        i = 0;
        std::sprintf(name, "girl%ld", i);
        thing = Game_->GetThingWithScriptName(name);
        while (thing.IsAlive()) {
            Game_->EntityPostOpinionDeedToRecipient(Game_->GetHero(), deed, thing);
            ++i;
            std::sprintf(name, "girl%ld", i);
            thing = Game_->GetThingWithScriptName(name);
        }
    }
};
int main()
{
    g_log[0] = 0;
    bool ok;
    {
        Game g; std::strcpy(g.hero.name, "HERO");
        Teacher t; t.Game_ = &g; std::strcpy(t.Me.name, "Teacher");
        int before = g_live;
        t.AdjustHeroReputation(0x2c);
        ok = g_live == before
            && std::strcmp(g_log, "HERO>44>Teacher;HERO>44>boy0;HERO>44>boy1;HERO>44>boy2;HERO>44>girl0;") == 0;
    }
    ok = ok && g_live == 0;
    if (!ok) { std::printf("00e56d10_TEST FAIL\n"); return 1; }
    std::printf("00e56d10_TEST PASS\n");
    return 0;
}