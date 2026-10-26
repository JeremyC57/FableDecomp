// Behaviour test for CQR_EscortTraderScript::ForceEnableWait (0x00E23EB0): flags, quest stage and the three
// interface calls with their arguments, in order.
#include <cstdio>
class CScriptThing { public: long m_id; };
static int g_seq = 0;
struct CInterface {
    CScriptThing hero; int stopAt; const CScriptThing* stopThing; int emoteAt; const CScriptThing* emoteThing; bool emote;
    int followAt; const CScriptThing* follower; const CScriptThing* leader; bool following;
    CScriptThing& GetHero() { return hero; }
    void EntityStopFollowing(const CScriptThing& t) const { const_cast<CInterface*>(this)->stopAt = ++g_seq; const_cast<CInterface*>(this)->stopThing = &t; }
    void EntitySetAsDisplayingEmoteIcon(const CScriptThing& t, bool d) const { CInterface* m = const_cast<CInterface*>(this); m->emoteAt = ++g_seq; m->emoteThing = &t; m->emote = d; }
    void SetEntityAsRegionFollowing(const CScriptThing& f, const CScriptThing& l, bool b) const { CInterface* m = const_cast<CInterface*>(this); m->followAt = ++g_seq; m->follower = &f; m->leader = &l; m->following = b; }
};
struct CScript {
    CInterface* GSI; bool TraderFollowing; long QuestStage;
    void ForceEnableWait(const CScriptThing& trader)
    {
        TraderFollowing = false;
        QuestStage = 3;
        GSI->EntityStopFollowing(trader);
        GSI->EntitySetAsDisplayingEmoteIcon(trader, false);
        GSI->SetEntityAsRegionFollowing(GSI->GetHero(), trader, false);
    }
};
int main()
{
    CInterface i; i.hero.m_id = 1; i.stopAt = i.emoteAt = i.followAt = 0; i.emote = i.following = true;
    CScript s; s.GSI = &i; s.TraderFollowing = true; s.QuestStage = 1;
    CScriptThing trader; trader.m_id = 7;
    s.ForceEnableWait(trader);
    bool ok = !s.TraderFollowing && s.QuestStage == 3 && i.stopAt == 1 && i.stopThing == &trader
           && i.emoteAt == 2 && i.emoteThing == &trader && !i.emote
           && i.followAt == 3 && i.follower == &i.hero && i.leader == &trader && !i.following;
    if (!ok) { std::printf("00e23eb0_TEST FAIL\n"); return 1; }
    std::printf("00e23eb0_TEST PASS\n");
    return 0;
}