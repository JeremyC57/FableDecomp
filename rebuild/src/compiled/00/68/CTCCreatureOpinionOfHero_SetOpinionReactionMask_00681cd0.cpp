// 00681cd0 CTCCreatureOpinionOfHero::SetOpinionReactionMask
// Leaf: ?SetOpinionReactionMask@CTCCreatureOpinionOfHero@@QAEXJ@Z
// Flags:
#define FABLE_RETAIL_OPINION_REACTION_MASK_PASS29
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRING
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CTCBASE_ADDTOGLOBALTCS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRINGTABLE_ASCHARARRAY
#define FABLE_RETAIL_CDEFSTRING_INLINE_EQ
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_DEEDS
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_SERIALISE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_PEEKDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_CLASSTYPENAME
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#define FABLE_RETAIL_STLP_RB_INCREMENT_SIZE_OWNER_PASS27
#define FABLE_RETAIL_STLP_BOOL_VECTOR_SIZE_OWNERS_PASS26
#include <string.h>
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CDefString.h"
#include "engine/CTCCreatureOpinionOfHero.h"
#include "engine/COpinionReactionMaskDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CWorld.h"
void CTCCreatureOpinionOfHero::SetOpinionReactionMask(long reaction_mask_def)
{
    for (int c0 = 0; c0 < OPINION_REACTION_TYPE_LAST; ++c0)
        ReactionMask[c0] = c0 < OPINION_REACTION_TYPE_WIFE_FIRST;

    if (!reaction_mask_def)
        return;

    const CGameDefinitionManager& def_mgr = GetWorld().PeekDefinitionManager();
    CDefPointer<const COpinionReactionMaskDef> pdef;
    if (!(def_mgr.GetDefClassTypeNameFromGlobalIndex(reaction_mask_def) == "OPINION_REACTION_MASK"))
        return;

    if (def_mgr.GetDef(reaction_mask_def, &pdef)) {
        for (int c0 = 0; c0 < OPINION_REACTION_TYPE_WIFE_FIRST; ++c0)
            ReactionMask[c0] = pdef->ReactionEnabledDefault;

        std::vector<bool> found_reaction(OPINION_REACTION_TYPE_LAST, false);
        typedef std::map<EOpinionReactionType, bool> ReactionMap;
        for (ReactionMap::const_iterator it = pdef->ReactionEnabled.begin(),
             end_it = pdef->ReactionEnabled.end(); it != end_it; ++it) {
            const EOpinionReactionType reaction_type = it->first;
            if (reaction_type < OPINION_REACTION_TYPE_LAST) {
                found_reaction[reaction_type] = true;
                ReactionMask[reaction_type] = it->second;
            }
        }
    }
}