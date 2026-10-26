#include <cstdio>
// Behaviour test for the CIsOpinionSourceWithReaction constructor (0x0095F9A0): the state group, the world's reaction manager,
// "is following the player" (AI follower OR region follower of the hero), "aware of player" (the creature's opinion targets the
// hero with non-zero awareness and the aware flag) and the creature's opinion component are captured.
class CThing;
class CTCFollowed { public: CThing* Follower; bool IsAIFollower(CThing* c) { return c == Follower; } };
class CTCRegionFollower { public: CThing* Follower; bool IsRegionFollower(CThing* c) { return c == Follower; } };
class CTCCreatureOpinionOfHero { public: CThing* Target; float Awareness; bool AwareOfPlayer; CThing* GetTargetThing() { return Target; } };
class CThing { public:
    CTCFollowed* Followed; CTCRegionFollower* Region; CTCCreatureOpinionOfHero* Opinion;
    template <class T> bool GetInterface(T** out, long id)
    {
        void* p = id == 0xbc ? (void*)Followed : id == 0x7a ? (void*)Region : (void*)Opinion;
        *out = (T*)p;
        return p != 0;
    }
};
struct CCharacter { CThing* Thing; CThing* GetThing() { return Thing; } };
class CPlayer { public: CCharacter* Character; CCharacter* GetCharacter() { return Character; } };
struct CAIStateGroup { CPlayer* Player; CThing* Creature; CPlayer* GetPlayer() { return Player; } CThing* GetCreature() { return Creature; } };
struct CReactionManager { int dummy; };
static CReactionManager g_reactionManager;
struct CIsOpinionSourceWithReaction {
    CAIStateGroup* PStateGroup; CTCCreatureOpinionOfHero* PTCOpinion; CReactionManager* ReactionManager;
    bool IsFollowingPlayer; bool IsAwareOfPlayerForReactions;
    CIsOpinionSourceWithReaction(CAIStateGroup* group) : PStateGroup(group), PTCOpinion(0), ReactionManager(&g_reactionManager)
    {
        CThing* hero;
        if (group->GetPlayer()->GetCharacter()) hero = group->GetPlayer()->GetCharacter()->GetThing(); else hero = 0;
        CTCFollowed* followed; CTCRegionFollower* regionFollower;
        IsFollowingPlayer = hero && ((hero->GetInterface(&followed, 0xbc) && followed->IsAIFollower(group->GetCreature()))
            || (hero->GetInterface(&regionFollower, 0x7a) && regionFollower->IsRegionFollower(group->GetCreature())));
        CTCCreatureOpinionOfHero* opinion;
        IsAwareOfPlayerForReactions = PStateGroup && PStateGroup->GetCreature()->GetInterface(&opinion, 0x34)
            && opinion->GetTargetThing() == hero && opinion->Awareness != 0.0f && opinion->AwareOfPlayer;
        if (PStateGroup) PStateGroup->GetCreature()->GetInterface(&PTCOpinion, 0x4f);
    }
};
int main()
{
    CThing hero = { 0, 0, 0 };
    CThing creature = { 0, 0, 0 };
    CCharacter ch = { &hero };
    CPlayer player = { &ch };
    CAIStateGroup group = { &player, &creature };
    CTCRegionFollower region = { &creature };
    CTCCreatureOpinionOfHero op = { &hero, 0.5f, true };
    hero.Region = &region;
    creature.Opinion = &op;
    CIsOpinionSourceWithReaction a(&group);
    if (!a.IsFollowingPlayer || !a.IsAwareOfPlayerForReactions || a.PTCOpinion != &op || a.PStateGroup != &group
        || a.ReactionManager != &g_reactionManager) { std::printf("0095f9a0_TEST FAIL\n"); return 1; }
    op.Awareness = 0.0f; hero.Region = 0;
    CIsOpinionSourceWithReaction b(&group);
    if (b.IsFollowingPlayer || b.IsAwareOfPlayerForReactions) { std::printf("0095f9a0_TEST FAIL\n"); return 1; }
    ch.Thing = 0; player.Character = 0; op.Awareness = 1.0f; op.Target = 0;
    CIsOpinionSourceWithReaction c(&group);   // no hero: target 0 == hero 0 -> aware
    if (c.IsFollowingPlayer || !c.IsAwareOfPlayerForReactions) { std::printf("0095f9a0_TEST FAIL\n"); return 1; }
    std::printf("0095f9a0_TEST PASS\n");
    return 0;
}