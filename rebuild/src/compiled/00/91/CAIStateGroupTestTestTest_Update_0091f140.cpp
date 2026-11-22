// 0091f140 CAIStateGroup_TestTestTest::Update
// Leaf: ?Update@CAIStateGroup_TestTestTest@@UAEXXZ
// Flags: /d2inlT68
// Inline-budget: the inlined vector growth in Update calls malloc at 0091F457; the cover search calls the 0091FFE0 growth body at 009203C2.
#define FABLE_RETAIL_STLP_FIND_NOINLINE_BODY
#define FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH
#define FABLE_RETAIL_CTHINGMAP_LAYERINFO
#define FABLE_RETAIL_CTCMAPWHO_PASS22_SHARED_NODE
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_STATTYPE
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_MAP_VALIDATE
#define FABLE_RETAIL_CTHINGOBJECT_STATTYPEFLAGS
#define FABLE_RETAIL_STLP_VECTOR_INDEX_START
#define FABLE_RETAIL_CAISTATEGROUP_TESTTESTTEST_PASS27
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK_PROTECTED
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_CENTRE
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_DIAMETER
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGCOMPARE_NEAREST_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CThingAICreature.h"
#include "engine/CThingObject.h"
#include "engine/CAIStateGroup_TestTestTest.h"
#include "engine/CAIBrain.h"
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
#include "engine/C2DBoxI.h"
#include "engine/C2DBoxF.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CTCMapwho.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_CanBeUsedAsCover.h"
#include "engine/CFilter_Bind2.h"
#include "stlport/fable_algorithm.h"
#include "engine/CIsNotInList.h"
#include "engine/CThingCompare_Nearest.h"

namespace {
bool CIsNotInList::operator()(const CThingObject& thing) const {
    return std::find(Things.begin(),Things.end(),&thing)==Things.end();
}
}
void CAIStateGroup_TestTestTest::Update()
{
    std::vector<const CThingObject*> visited_things;
    const CTCCreatureNavigation* ptc_creature_nav;
    GetCreature().PeekTC(&ptc_creature_nav);
    const float LOOK_FOR_THINGS_RADIUS = 15.0f;
    bool finished = false;
    do {
        std::vector<const CThingObject*> things;
        PeekThingSearchTools().GetAllThingsInRoughArea<const CThingObject>(
            C2DBoxF(GetCreature().GetPos(), LOOK_FOR_THINGS_RADIUS).ToC2DBoxI(),
            GFFilter_Bind2(CThingFilter_CanBeUsedAsCover(), CIsNotInList(visited_things)), things);
        long sizeeee = things.size();
        if (!things.empty()) {
            std::sort(things.begin(),things.end(),CThingCompare_Nearest(GetCreature().GetPos()));
            for (std::vector<const CThingObject*>::iterator it=things.begin();it!=things.end();++it) {
                const CThingObject* pthing = *it;
                float dist_sqr = (pthing->GetPos()-GetCreature().GetPos()).GetSquaredMagnitude();
            }
            const CThingObject* pthing = things.front();
            float diameter = pthing->GetPhysicsTC()->GetDiameter();
            C3DVector line = GetCreature().GetPos()-pthing->GetPos();
            line.AccurateNormalise();
            C3DVector pos = pthing->GetPos()+line*diameter;
            ptc_creature_nav->GetNearbyWanderPosition(pos,diameter / 2.0f,pos,false);
            bool nav;
            EAIActionReturn ret = AIAction_MoveToPos(ACTION_REPEAT_UNTIL_FINISHED,pos,nav,AI_MOVEMENT_RUNNING,true,true,0,true,0.0f);
            visited_things.push_back(pthing);
        } else {
            finished = true;
        }
    } while (!finished);
}