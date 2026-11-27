// 00929fc0 CAIStateGroup_ApprenticeHero::GetObjectWithScriptName
// Leaf: ?GetObjectWithScriptName@CAIStateGroup_ApprenticeHero@@IAEPAVCThingGameObject@@ABVCCharString@@@Z
// Flags:
// The editor emits the VC6 for-scope guard before constructing thing_it.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY_EGOD
#define FABLE_RETAIL_CTHINGGAMEOBJECT_STATTYPEFLAGS
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTHING_SCANINCLUSION_ISALIVE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_APPRENTICEHERO
#define FABLE_RETAIL_CAISTATEGROUP_APPRENTICEHERO_SCRIPTNAME
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CThingGameObject.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_HasScriptName.h"
#include "engine/CAIStateGroup_ApprenticeHero.h"
#include "engine/GFRandom.h"

CThingGameObject* CAIStateGroup_ApprenticeHero::GetObjectWithScriptName(const CCharString& name)
{
    std::list<CThingGameObject*> things;
    PeekThingSearchTools().GetAllThings<CThingGameObject>(CThingFilter_HasScriptName(name), things);
    if (things.size() != 0) {
        long thing_index = GFRandom(things.size(), GetPWorldSeed());
        if (false) {} else for (std::list<CThingGameObject*>::iterator thing_it = things.begin(); thing_it != things.end(); ++thing_it) {
            if (thing_index == 0)
                return *thing_it;
            --thing_index;
        }
    }
    return 0;
}