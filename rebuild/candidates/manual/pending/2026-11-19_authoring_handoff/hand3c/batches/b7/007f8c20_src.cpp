// CTCAnimatingObject::AddQueuedAnimation(const CCharString& anim_name, bool looping) -- queue the first anim of that name (type 6, from speed)
// Leaf: ?AddQueuedAnimation@CTCAnimatingObject@@QAEXABVCCharString@@_N@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F8C20, 221 bytes, `ret 8`. Callees: CDefClassBase::GetSubDef<CAnimatingObjectDef> (out of line,
//   0x007F9330), CAnimationSet::AnimExists (0x005DC2E0), CAnimationSet::GetAnim (0x005DC340), the CAnimInit ctor (0x0070C050),
//   the STLport list<CAnimInit> node allocation (push_back inlined), and the def's release.
// Shape: FableWin.pdb body 0x02674140, statement for statement: CDefPointer(NULL_DEF); GetParentThing().PeekBaseDef().GetSubDef;
//   AnimExists(anim_name, CAnimFilterNoGroup()); GetAnim(anim_name, 0, CAnimFilterNoGroup()); CAnimInit(6, 0, entry, mode 1, looping,
//   0, 100); QueuedAnims.push_back. Ego_d locals: pdef, anim_init, anim_entry. The opt-ins are the landed sibling 0x007F8F30's
//   (PlayDefaultAnimation) plus FABLE_RETAIL_CANIMATIONSET_ANIMEXISTS.
// Headers: engine/CAnimatingObjectDef.h is new (Ego_r Animation +0x28; the generated flat model is quarantined), and
//   CTCAnimatingObject.h gains FABLE_RETAIL_CTCANIMATINGOBJECT_ADDQUEUEDANIMATION (declaration only). The earlier park (DIFFER 221v221,
//   register colouring) used a local class model.
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
