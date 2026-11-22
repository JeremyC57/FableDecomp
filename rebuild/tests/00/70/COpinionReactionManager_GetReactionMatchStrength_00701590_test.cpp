#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <vector>

static void require(bool ok) { if (!ok) abort(); }
enum EOpinionReactionType {
    TestOrdinary = 3, TestOther = 6,
    OPINION_REACTION_TYPE_ATTITUDE_ATTRACTED = 11,
    OPINION_REACTION_TYPE_ATTITUDE_LOVE = 12
};
struct CTCCreatureOpinionOfHero { float offset; };
struct CTCOpinionSource { float offset; };
struct COpinionSet {
    std::vector<float> OpinionValues;
    COpinionSet() : OpinionValues(5, 0.0f) {}
};
struct CTCWife {
    bool courtable;
    mutable int checks;
    bool IsCourtableAndNotBlockingIt() const { ++checks; return courtable; }
};
struct CThingAICreature {
    const CTCCreatureOpinionOfHero* opinion;
    const CTCWife* wife;
    mutable int opinionQueries, wifeQueries;
    bool PeekTC(const CTCCreatureOpinionOfHero** out) const {
        ++opinionQueries; *out = opinion; return *out != 0;
    }
    bool PeekTC(const CTCWife** out) const {
        ++wifeQueries; *out = wife; return *out != 0;
    }
};
struct CThingPlayerCreature {
    bool alive;
    const CTCOpinionSource* source;
    mutable int sourceQueries;
    bool PeekTC(const CTCOpinionSource** out) const {
        ++sourceQueries; *out = source; return *out != 0;
    }
};
static bool IS_THING_ALIVE(const CThingPlayerCreature* p) { return p && p->alive; }
struct CPlayer {
    CThingPlayerCreature* creature;
    CThingPlayerCreature* GetPControlledCreature() const { return creature; }
};
struct CWorld {
    CPlayer player;
    const CPlayer* PeekMyPlayer() const { return &player; }
};
struct CMainGameComponent {
    CWorld world;
    static CMainGameComponent* Get() { static CMainGameComponent root; return &root; }
    const CWorld& PeekWorld() const { return world; }
};
struct CReactionMatch {
    EOpinionReactionType ReactionType;
    float response, expectedOpinion;
    int calls;
    float Match(const COpinionSet& set) {
        ++calls;
        require(set.OpinionValues.size() == 5);
        require(set.OpinionValues[3] == expectedOpinion);
        return response;
    }
};
struct CReactionFrequencyTraits {
    const CThingAICreature* expected;
    float weight;
    int calls;
    float CalculateWeight(const CThingAICreature* c) { require(c == expected); ++calls; return weight; }
};
struct COpinionReactionManagerDef {
    std::list<CReactionMatch*> Matches;
    CReactionFrequencyTraits* FrequencyTraits[13];
    COpinionReactionManagerDef() { for (int i=0; i<13; ++i) FrequencyTraits[i]=0; }
};
struct COpinionReactionManager {
    COpinionReactionManagerDef* PDef;
    const CTCCreatureOpinionOfHero* expectedOpinion;
    const CTCOpinionSource* expectedSource;
    mutable int reads;
    void GetOpinions(const CTCCreatureOpinionOfHero* o,COpinionSet& set,const CTCOpinionSource* s) const {
        ++reads; require(o && o == expectedOpinion && s == expectedSource);
        for (int i=0; i<5; ++i) require(set.OpinionValues[i] == 0.0f);
        set.OpinionValues[3] = o->offset + (s ? s->offset : 0);
    }
    float GetReactionMatchStrength(EOpinionReactionType, const CThingAICreature&) const;
};

float COpinionReactionManager::GetReactionMatchStrength(EOpinionReactionType reaction_type,const CThingAICreature& creature) const
{
    const CTCCreatureOpinionOfHero* ptc_opinion=0;
    if(creature.PeekTC(&ptc_opinion)) {
        CThingPlayerCreature* pplayer=CMainGameComponent::Get()->PeekWorld().PeekMyPlayer()->GetPControlledCreature();
        if(IS_THING_ALIVE(pplayer)) {
            const CTCOpinionSource* ptc_opinion_source=0;
            pplayer->PeekTC(&ptc_opinion_source);
            COpinionSet creature_opinions;
            GetOpinions(ptc_opinion,creature_opinions,ptc_opinion_source);
            float value=0.0f;
            for(std::list<CReactionMatch*>::const_iterator it=PDef->Matches.begin(),end_it=PDef->Matches.end();it!=end_it;++it) {
                CReactionMatch* reaction_match=*it;
                if(reaction_match->ReactionType==reaction_type) {
                    float t_val=0.0f;
                    const CTCWife* ptc_wife;
                    if(!(reaction_match->ReactionType==OPINION_REACTION_TYPE_ATTITUDE_ATTRACTED ||
                         reaction_match->ReactionType==OPINION_REACTION_TYPE_ATTITUDE_LOVE) ||
                       (creature.PeekTC(&ptc_wife) && ptc_wife->IsCourtableAndNotBlockingIt()))
                        t_val=reaction_match->Match(creature_opinions);
                    if(value<t_val) value=t_val;
                }
            }
            CReactionFrequencyTraits* pfreq_trait=PDef->FrequencyTraits[reaction_type];
            if(pfreq_trait) value*=pfreq_trait->CalculateWeight(&creature);
            return value;
        }
    }
    return 0.0f;
}

static void exercise(int mask, int playerMode, int wifeMode, int kind, int weightMode, int listMode)
{
    EOpinionReactionType kinds[] = {TestOrdinary, OPINION_REACTION_TYPE_ATTITUDE_ATTRACTED, OPINION_REACTION_TYPE_ATTITUDE_LOVE};
    EOpinionReactionType kindValue = kinds[kind];
    CTCCreatureOpinionOfHero opinion = {1.5f};
    CTCOpinionSource source = {0.25f};
    CTCWife wife = {wifeMode == 2, 0};
    CThingAICreature creature = {(mask & 1) ? &opinion : 0, wifeMode ? &wife : 0, 0, 0};
    CThingPlayerCreature player = {playerMode == 2, (mask & 2) ? &source : 0, 0};
    CMainGameComponent::Get()->world.player.creature = playerMode ? &player : 0;
    COpinionReactionManagerDef def;
    COpinionReactionManager manager = {&def, creature.opinion, player.source, 0};
    float weights[] = {1.0f, 0.0f, -0.5f, 2.0f};
    CReactionFrequencyTraits trait = {&creature,weights[weightMode],0};
    if (weightMode) def.FrequencyTraits[kindValue] = &trait;
    float responses[] = {-0.5f, 0.0f, 0.25f, 0.25f, 1.5f, 0.5f};
    CReactionMatch matches[6];
    int count = listMode;
    int matchingTypes = 0;
    float maximum = 0.0f;
    bool reached = creature.opinion && playerMode == 2;
    bool eligible = kind == 0 || wifeMode == 2;
    for (int i=0; i<count; ++i) {
        matches[i].ReactionType = (mask & (4 << i)) ? kindValue : TestOther;
        matches[i].response = responses[i];
        matches[i].expectedOpinion = player.source ? 1.75f : 1.5f;
        matches[i].calls = 0;
        def.Matches.push_back(&matches[i]);
        if (matches[i].ReactionType == kindValue) {
            ++matchingTypes;
            if (eligible && responses[i] > maximum) maximum = responses[i];
        }
    }
    float result = manager.GetReactionMatchStrength(kindValue,creature);
    require(result == (reached ? maximum * weights[weightMode] : 0.0f));
    require(manager.reads == int(reached) && player.sourceQueries == int(reached));
    require(creature.opinionQueries == 1);
    require(creature.wifeQueries == ((kind && reached) ? matchingTypes : 0));
    require(wife.checks == ((wifeMode && kind && reached) ? matchingTypes : 0));
    require(trait.calls == int(reached && weightMode));
    for (int i=0; i<count; ++i)
        require(matches[i].calls == int(reached && eligible && matches[i].ReactionType == kindValue));
}
int main()
{
    for (int mask=0; mask<256; ++mask)
        for (int player=0; player<3; ++player)
            for (int wife=0; wife<3; ++wife)
                for (int kind=0; kind<3; ++kind)
                    for (int weight=0; weight<4; ++weight) {
                        exercise(mask,player,wife,kind,weight,6);
                        if (mask < 4) exercise(mask,player,wife,kind,weight,0);
                    }
    puts("00701590_TEST PASS");
    return 0;
}