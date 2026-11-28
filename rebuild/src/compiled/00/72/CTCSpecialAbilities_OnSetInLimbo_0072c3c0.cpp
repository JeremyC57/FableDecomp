// Flags: 
// 0072c3c0 CTCSpecialAbilities::OnSetInLimbo
// Leaf: ?OnSetInLimbo@CTCSpecialAbilities@@UAEX_N@Z
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CTCSPECIALABILITIES_GUI_DRAW_PASS23
#define FABLE_RETAIL_UNDEAD_FOLLOWER_LIMBO_PASS30
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_NCREATUREMODE_UNCONSCIOUS_FLASH_PASS30
#include "engine/CThingGameObject.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/ThingAlive.h"
#include "engine/NCreatureMode.h"
void CTCSpecialAbilities::OnSetInLimbo(bool in_limbo)
{
    if (in_limbo) {
        for (std::list<CUndeadFollower>::iterator it = UndeadFollowers.begin(); it != UndeadFollowers.end(); ++it) {
            if (IS_THING_ALIVE(it->PThing) && !NCreatureMode::IsUnconscious(*it->PThing))
                ReclaimUndeadFollower(*static_cast<CThingGameObject*>(it->PThing.GetPItem()));
        }
    }
}
