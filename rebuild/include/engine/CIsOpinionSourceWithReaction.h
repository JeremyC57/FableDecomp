#ifndef FABLE_ENGINE_CISOPINIONSOURCEWITHREACTION_H
#define FABLE_ENGINE_CISOPINIONSOURCEWITHREACTION_H
#if defined(FABLE_RETAIL_OPINION_FILTER_PASS27) || defined(FABLE_RETAIL_OPINIONREACTIONROOT_ISRUNNABLE_PASS27)
#include "engine/EOpinionReactionType.h"
class CAIStateGroup_OpinionReactionRoot;
class CTCCreatureOpinionOfHero;
class COpinionReactionManager;
class CThing;
class CIsOpinionSourceWithReaction {
    CAIStateGroup_OpinionReactionRoot* PStateGroup;
    CTCCreatureOpinionOfHero* PTCOpinion;
    COpinionReactionManager* ReactionManager;
    bool IsFollowingPlayer;
    bool IsAwareOfPlayerForReactions;
    mutable EOpinionReactionType Reaction;
public:
    CIsOpinionSourceWithReaction(CAIStateGroup_OpinionReactionRoot*);
    bool operator()(const CThing&) const;
    EOpinionReactionType PeekReaction() const { return Reaction; }
};
FABLE_STATIC_ASSERT(sizeof(CIsOpinionSourceWithReaction) == 20);
#endif
#endif
