// CTCCreatureGenerator::DoNavCheck -- Remove positions the generator navigation component cannot reach.
// Grade: DIFFER(232v234) -- The entity reference reload at 0074A512, loop scheduling, and the removal path differ. The direct array/position form is the closest retained port.
// Leaf: ?DoNavCheck@CTCCreatureGenerator@@IAEXABVCThingCreatureBase@@@Z
// Retail: 0x0074A4E0, 234 bytes, thiscall.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCCREATUREGENERATOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#include "fable_string.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CTCCreatureGenerator.h"

void CTCCreatureGenerator::DoNavCheck(const CThingCreatureBase& pCreatureToTest)
{
    const long pointsBeforeCheck = OpenPositions.size();
    if (pointsBeforeCheck > 0) {
        C3DVector startPos;
        startPos=GetParentThing().GetPos();
        const CTCCreatureNavigation* pTCNavigate;
        pCreatureToTest.PeekTC(&pTCNavigate);
        long numPoints = pointsBeforeCheck;
        for (long i = 0; i < numPoints;) {
            if (!pTCNavigate->IsAbleToNavigateFromPosToPos(startPos, OpenPositions[i], 0.0f)) {
                OpenPositions[i] = OpenPositions[--numPoints];
                OpenPositions.pop_back();
            } else {
                ++i;
            }
        }
    }
}
