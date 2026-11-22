// 00701590 COpinionReactionManager::GetReactionMatchStrength
// Leaf: ?GetReactionMatchStrength@COpinionReactionManager@@QBEMW4EOpinionReactionType@@ABVCThingAICreature@@@Z
// Flags: none
#define FABLE_RETAIL_OPINION_STRENGTH_PASS26
#define FABLE_RETAIL_COPINIONREACTIONMANAGER_REACTIONMATCHSTRENGTH
#define FABLE_RETAIL_REACTIONFREQUENCYTRAITS_CLASS_CHAIN_PASS24
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_VECTOR_FILL_OUTOFLINE
// Inline-budget: 004538EB expands CArray<float>(5) and calls the vector fill constructor.
#define FABLE_RETAIL_CARRAY_SIZE_CTOR_FORCEINLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_GREET
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_SERIALISE
#define FABLE_RETAIL_COPINIONSET_CTOR_INDEX
#define FABLE_RETAIL_CTCOPINIONSOURCE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTCWIFE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_STLP_LIST
#include "fable_string.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayer.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
// Native COpinionSet at 004538EB has its frame pointer and size-mode vector<float> fill call.
#ifdef _MSC_VER
#pragma optimize("s",on)
#pragma optimize("y",off)
#endif
#include "engine/CTCCreatureOpinionOfHero.h"
#ifdef _MSC_VER
#pragma optimize("",on)
#endif
#include "engine/CTCOpinionSource.h"
#include "engine/CTCWife.h"
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#include "engine/CDataOutputStream.h"
#include "engine/COpinionReactionManagerDef.h"
#include "engine/CReactionMatch.h"
#include "engine/COpinionReactionManager.h"
#include "engine/ThingAlive.h"

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