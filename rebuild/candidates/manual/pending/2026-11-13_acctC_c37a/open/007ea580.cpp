// CTCTargeted::GetAllTargetPoints(std::vector<std::pair<C3DVector, long> >& points, float scale) const -- list every targeting-cursor helper point of the thing with its index, or the single default targeting position when the graphic has none
// Grade: DIFFER(457v457) -- two displacements: retail copies Y / Z of the default point into the vector from the returned position ([esp+1Ch] / [esp+20h], 0x007EA70B / 0x007EA712), ours from the pair temporary ([esp+28h] / [esp+2Ch]); everything else byte-equal
// Leaf: ?GetAllTargetPoints@CTCTargeted@@QBEXAAV?$vector@U?$pair@VC3DVector@@J@std@@V?$allocator@U?$pair@VC3DVector@@J@std@@@2@@std@@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007EA580, 457 bytes, three `ret 8` (last 0x007EA746).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCTARGETED
#define FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FLARE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_HELPERNAMES
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CTCTARGETED_GETALLTARGETPOINTS
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/NTargeting.h"
#include "engine/CTCTargeted.h"

extern const CCharString TARGETING_CURSOR_POSITION_NAME;                 // 0x013BAC50 "TARGET_CURSOR_POINT"

// CTCTargeted::GetNumTargetPoints 0x007EA2D0 (landed row; same TU, expanded in GetAllTargetPoints).
long CTCTargeted::GetNumTargetPoints() const
{
    const NGraphicAppearance::CTCGraphicAppearance* appearance = ParentThing->GetTCGraphicAppearance();
    if (appearance)
        return appearance->GetNoHelperPoints(TARGETING_CURSOR_POSITION_NAME, 1.0f);
    return 0;
}

// CTCTargeted::GetTargetPoint 0x007EA490 (landed row; same TU).
C3DVector CTCTargeted::GetTargetPoint(long index, float radius) const
{
    if (index >= 0) {
        const NGraphicAppearance::CTCGraphicAppearance* appearance = ParentThing->GetTCGraphicAppearance();
        C3DVector point;
        if (appearance->GetHelperPoint(TARGETING_CURSOR_POSITION_NAME, index, 0x20, point, 1.0f))
            return point;
    }
    const CThing& thing = *ParentThing;
    return NTargeting::GetTargetingPositionOnThing(PeekWorld(), thing, radius);
}

void CTCTargeted::GetAllTargetPoints(std::vector<std::pair<C3DVector, long> >& points, float scale) const
{
    long num_points = GetNumTargetPoints();
    long i = 0;
    if (num_points != 0) {
        for (; i != num_points; ++i)
            points.push_back(std::pair<C3DVector, long>(GetTargetPoint(i, scale), i));
    } else {
        points.push_back(std::pair<C3DVector, long>(GetTargetPoint(-1, scale), 0));
    }
}
