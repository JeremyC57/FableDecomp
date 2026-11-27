// CAIStateGroup_GoToPub::GetObjectWithScriptName(const CCharString&) -- collect every object carrying the script name and return one picked with the world's random seed
// Leaf: ?GetObjectWithScriptName@CAIStateGroup_GoToPub@@IAEPAVCThingObject@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00947EB0, 242 bytes, protected, `ret 4`.
// PDB: Ego_r 0x0096E1B0 and FableWin 0x02BC4810, both this leaf. The landed twin
//   CAIStateGroup_ApprenticeHero::GetObjectWithScriptName 0x00929FC0 is the same shape.
// Shape: shared list (destructor stays the call 0x00792240) and the header GFRandom
//   (if (range) return seed % range; return 0). `if (false) {} else for` is the VC6 for-scope guard.
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
#define FABLE_RETAIL_CAISTATEGROUP_GOTOPUB
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CThingGameObject.h"
#include "engine/CThingObject.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_HasScriptName.h"
#include "engine/CAIStateGroup_GoToPub.h"
#include "engine/GFRandom.h"

CThingObject* CAIStateGroup_GoToPub::GetObjectWithScriptName(const CCharString& name)
{
    std::list<CThingObject*> things;
    PeekThingSearchTools().GetAllThings<CThingObject>(CThingFilter_HasScriptName(name), things);
    if (things.size() != 0) {
        long thing_index = GFRandom(things.size(), GetPWorldSeed());
        if (false) {} else for (std::list<CThingObject*>::iterator thing_it = things.begin(); thing_it != things.end(); ++thing_it) {
            if (thing_index == 0)
                return *thing_it;
            --thing_index;
        }
    }
    return 0;
}