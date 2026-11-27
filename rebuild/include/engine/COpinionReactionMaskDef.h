#ifndef FABLE_OPINION_REACTION_MASK_DEF_H
#define FABLE_OPINION_REACTION_MASK_DEF_H
#ifdef FABLE_RETAIL_OPINION_REACTION_MASK_PASS29
#include "engine/CParentDefClassBase.h"
#include "engine/EOpinionReactionType.h"
#include "stlport/fable_map.h"
class COpinionReactionMaskDef : public CParentDefClassBase {
public:
    bool ReactionEnabledDefault;
    std::map<EOpinionReactionType,bool> ReactionEnabled;
};
#endif
#endif
