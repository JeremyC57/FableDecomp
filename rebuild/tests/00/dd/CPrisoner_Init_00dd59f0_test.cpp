// Behaviour test for CPrisoner::Init (0x00DD59F0): ReleasedAndFollowing cleared; ally of the hero, not allowed to
// follow, no opinion reactions -- in that order, all on the prisoner's thing.
#include <cstdio>
struct CScriptThing { long m_id; };
static int g_seq = 0;
struct CInterface {
    CScriptThing hero; int allyAt; const CScriptThing* allyThing; const CScriptThing* allyOf;
    int followAt; const CScriptThing* followThing; bool allowed; int opinionAt; const CScriptThing* opinionThing; bool enabled;
    CScriptThing& GetHero() { return hero; }
    void EntitySetThingAsAllyOfThing(const CScriptThing& t, const CScriptThing& a) { allyAt = ++g_seq; allyThing = &t; allyOf = &a; }
    void EntitySetAsAllowedToFollowHero(const CScriptThing& t, bool b) { followAt = ++g_seq; followThing = &t; allowed = b; }
    void EntitySetOpinionReactionsEnabled(const CScriptThing& t, bool b) { opinionAt = ++g_seq; opinionThing = &t; enabled = b; }
};
struct CPrisoner {
    CInterface* PQuest; CScriptThing Thing; bool ReleasedAndFollowing;
    void Init()
    {
        ReleasedAndFollowing = false;
        PQuest->EntitySetThingAsAllyOfThing(Thing, PQuest->GetHero());
        PQuest->EntitySetAsAllowedToFollowHero(Thing, false);
        PQuest->EntitySetOpinionReactionsEnabled(Thing, false);
    }
};
int main()
{
    CInterface i; i.hero.m_id = 1; i.allyAt = i.followAt = i.opinionAt = 0; i.allowed = i.enabled = true;
    CPrisoner p; p.PQuest = &i; p.Thing.m_id = 5; p.ReleasedAndFollowing = true;
    p.Init();
    bool ok = !p.ReleasedAndFollowing && i.allyAt == 1 && i.allyThing == &p.Thing && i.allyOf == &i.hero
           && i.followAt == 2 && i.followThing == &p.Thing && !i.allowed && i.opinionAt == 3 && i.opinionThing == &p.Thing && !i.enabled;
    if (!ok) { std::printf("00dd59f0_TEST FAIL\n"); return 1; }
    std::printf("00dd59f0_TEST PASS\n");
    return 0;
}