// CTCAnimatingObject::AddQueuedAnimation(const CCharString& anim_name, bool looping) -- queue the first anim of that name (type 6, from speed)
// Leaf: ?AddQueuedAnimation@CTCAnimatingObject@@QAEXABVCCharString@@_N@Z
// Flags: none
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CANIMINIT_CTORS
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CANIMFILTER
#define FABLE_RETAIL_CANIMATIONSET_ANIMEXISTS
#define FABLE_RETAIL_CTCANIMATINGOBJECT_ADDQUEUEDANIMATION
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CDefClassBase.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefPointer.h"
#include "engine/CAnimatingObjectDef.h"
#include "engine/CAnimationEntry.h"
#include "engine/CAnimFilterNoGroup.h"
#include "engine/CAnimInit.h"
#include "engine/CDefinitionManager.h"
#include "engine/CTCAnimatingObject.h"

void CTCAnimatingObject::AddQueuedAnimation(const CCharString& anim_name, bool looping)
{
    CDefPointer<const CAnimatingObjectDef> pdef(NULL_DEF);
    if (GetParentThing().PeekBaseDef().GetSubDef(&pdef))
    {
        if (pdef->Animation.AnimExists(anim_name, CAnimFilterNoGroup()))
        {
            const CAnimationEntry& anim_entry = pdef->Animation.GetAnim(anim_name, 0, CAnimFilterNoGroup());
            CAnimInit anim_init(6, 0, anim_entry.BankIndex, ANIM_UPDATE_FROM_SPEED, looping, 0, 100);
            QueuedAnims.push_back(anim_init);
        }
    }
}
