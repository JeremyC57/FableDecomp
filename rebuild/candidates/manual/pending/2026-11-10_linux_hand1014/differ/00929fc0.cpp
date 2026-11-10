// CAIStateGroup_ApprenticeHero::GetObjectWithScriptName(const CCharString& script_name) -- collect every game object carrying the script name and return one picked with the world's random seed (null when there are none)
// Grade: DIFFER(265v242) -- the found-path ~list is inlined (retail calls the list dtor COMDAT 0x006FE660) and the GFRandom result lands in edx not ecx
// Leaf: ?GetObjectWithScriptName@CAIStateGroup_ApprenticeHero@@IAEPAVCThingGameObject@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00929FC0, 242 bytes, __thiscall, `ret 4`. Ego_r.pdb
//   ?GetObjectWithScriptName@CAIStateGroup_ApprenticeHero@@IAEPAVCThingGameObject@@ABVCCharString@@@Z. Called by GetDummy 0x0092A0C0
//   and LookAtMap 0x0092AFE0.
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (the STLport list nodes), CCharString(const CCharString&) 0x0099EC30 / ~CCharString
//   0x0099EAE0 (the filter temporary), CAIStateGroupBase::PeekThingSearchTools 0x008FD000, CThingSearchTools::GetAllThings<CThingGameObject,
//   list<CThingGameObject*>, CThingFilter_HasScriptName> 0x0092B2D0, CAIStateGroupBase::GetPWorldSeed 0x008FCFD0, GFROR13 0x00497890
//   (GFRandom inline), the list destructor 0x006FE660.
#define FABLE_RETAIL_CCHARSTRING
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

CThingGameObject* CAIStateGroup_ApprenticeHero::GetObjectWithScriptName(const CCharString& script_name)
{
    std::list<CThingGameObject*> objects;
    PeekThingSearchTools().GetAllThings<CThingGameObject>(CThingFilter_HasScriptName(script_name), objects);
    if (objects.size() != 0) {
        long index = GFRandom(objects.size(), GetPWorldSeed());
        for (std::list<CThingGameObject*>::iterator it = objects.begin(); it != objects.end(); ++it) {
            if (index == 0)
                return *it;
            --index;
        }
    }
    return 0;
}
