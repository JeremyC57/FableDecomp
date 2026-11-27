// 0091c220 CAIStateGroup_NymphAttack::IsTargetInLineOfSight
// Leaf: ?IsTargetInLineOfSight@CAIStateGroup_NymphAttack@@AAE_NPAVCThingCreatureBase@@@Z
// Flags:
// Inline-budget: retail calls the pointer vector overflow from its rough-area scan at 0091CB62.
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CGAMECAMERAMANAGER_CAMTOOLS_TYPED
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS_PASS22_LOS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D_ORIGINAL_SUM_PASS23
#define FABLE_RETAIL_IS_THING_ALIVE
#include "fable_assert.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C2DCoordI.h"
#include "engine/C2DBoxI.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroup_NymphAttack.h"
#include "engine/CAIBrain.h"
#include "engine/CWorld.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/NCameraModes_CTools.h"
#include "engine/CGameCameraManager.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CFilter_Bind2.h"
#include "engine/C2DExtentsI.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/ThingAlive.h"

bool CAIStateGroup_NymphAttack::IsTargetInLineOfSight(CThingCreatureBase* pTarget)
{
    if (IS_THING_ALIVE(pTarget)) {
        C3DVector targPos = pTarget->GetPos();
        std::vector<const CThing*> things;
        C3DVector midPoint = (GetCreature().GetPos() + targPos) * 0.5f;
        C3DVector delta = targPos - midPoint;
        float rad = (delta.GetMagnitude2D()) * 1.1f;
        GetWorld().PeekThingSearchTools().GetAllThingsInRoughArea<const CThing>(
            C2DBoxI(midPoint.ToC2DCoordI(), static_cast<long>(rad)),
            GFFilter_Bind2(CThingFilter_IsNot(GetCreature()), CThingFilter_IsNot(*pTarget)), things);
        const CGameCameraManager* pCamMan;
        if (!GetWorld().PeekGameCamera()->GetCameraManager(&pCamMan)) {
            FABLE_RETAIL_ASSERT_ARM(false);
        }
        C3DVector int_pos, int_normal;
        const CThing* phit_thing;
        float cam_rad;
        targPos.Z += 1.0f;
        return pCamMan->GetCameraTools().IsLineOfSight(things, GetCreature().GetPos(), targPos,
            int_pos, int_normal, cam_rad, &phit_thing, NCameraModes::CTools::TEST_ALL_BUT_CREATURES, false);
    }
    return false;
}