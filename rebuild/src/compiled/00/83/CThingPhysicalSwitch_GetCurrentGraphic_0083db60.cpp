// CThingPhysicalSwitch::GetCurrentGraphic() const -- the switch definition's triggered or untriggered graphic, by the switch's state
// Leaf: ?GetCurrentGraphic@CThingPhysicalSwitch@@IBEABVCEngineGraphic@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083DB60, 125 bytes, `ret`. Ego_d.pdb: no locals (debug length 53: two PeekDefinition calls);
//   retail expands PeekDefinition (the landed 0x0083D900 body, same TU) in each arm: GetDef<CThingPhysicalSwitchDef> 0x0083E0D0, the
//   def pointer's release, then the member address from the released pointer (0x0083DBA1 `lea eax,[esi+82h]`).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHINGPHYSICALSWITCH_PEEKDEFINITION
#define FABLE_RETAIL_CTHINGPHYSICALSWITCH_PEEKDEFINITION_SAMETU
#define FABLE_RETAIL_CTHINGPHYSICALSWITCH_GETCURRENTGRAPHIC
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingPhysicalSwitchDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThingPhysicalSwitch.h"

const CEngineGraphic& CThingPhysicalSwitch::GetCurrentGraphic() const
{
    if (Triggered)
        return PeekDefinition().TriggeredGraphic;
    return PeekDefinition().UntriggeredGraphic;
}