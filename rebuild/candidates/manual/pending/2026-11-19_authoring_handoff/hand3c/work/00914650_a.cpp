// CAIStateGroup_BirdStartled::IsRunnable() -- a bird on the A* navigator is startled when a creature that scares birds is in the rough area
// Leaf: ?IsRunnable@CAIStateGroup_BirdStartled@@UAE_NXZ
// Flags: none
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH
#define FABLE_RETAIL_C2DBOXI_CTORS
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGATORTYPE
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CThingCompare_Nearest.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/GFFloatToLongCeil.h"
#include "engine/GFWorldDistance.h"
#include "engine/CThingFilter_ScaresBirds.h"
#include "engine/CAIStateGroup_BirdStartled.h"

extern const unsigned long NAVIGATOR_TYPE_A_STAR;                          // 0x0129CBA4 ?NAVIGATOR_TYPE_A_STAR@@3KB
static const float DIST_CREATURES_SCARE_BIRDS = 5.0f;                      // FableWin S_LDATA32 (file static), folded in retail

bool CAIStateGroup_BirdStartled::IsRunnable()
{
    CTCCreatureNavigation* pnav;
    GetCreature().GetTC(&pnav);
    if (pnav->GetNavigatorType() == NAVIGATOR_TYPE_A_STAR)
    {
        CThingCreatureBase* pcreature = 0;
        C2DBoxI area(GetCreature().PeekPos().ToC2DCoordI(), GFFloatToLongCeil(DIST_CREATURES_SCARE_BIRDS * 0.5));
        if (PeekThingSearchTools().GetPBestThingInRoughArea(area, CThingFilter_ScaresBirds(GetCreature()),
                CThingCompare_Nearest(GetCreature().PeekPos()), &pcreature) &&
            GFWorld2DDistanceSmallerThan(GetCreature().PeekPos(), GetCreature().PeekPos(), DIST_CREATURES_SCARE_BIRDS, false))
            return true;
    }
    return false;
}
