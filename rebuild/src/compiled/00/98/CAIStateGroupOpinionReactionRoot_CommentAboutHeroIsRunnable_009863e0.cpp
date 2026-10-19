// CAIStateGroup_OpinionReactionRoot::CommentAboutHeroIsRunnable() -- require a live, visible reacting-to thing, a nearby matching creature, and an ordered distance strictly greater than 3.0f; retail 0x009863e0, 304 bytes. Uses the generated flattened state-group header and actual CAIStateGroupBase/CAIBrain/CThingAICreature/CThing/CThingSearchTools headers. Donor caveat: OpinionReactionRoot is a flattened generated layout; its +0x04 brain link is viewed through CAIStateGroupBase. ThingReactingTo at +0x6b is a 9-byte CIntelligentPointer unavailable as a typed shared member, so the adapter is field-free. Native query helper: CThingSearchTools::GetPFirstThingInRoughArea @0x987430, filter init @0x986ed0, pointer conversion @0xa01b50. Radius constant 3.0f @0x0122ded4. FCOMP/TEST AH,0x41/JNE returns false for less, equal, or unordered; success is ordered greater-than only.
#include <math.h>
#include "engine/CAIStateGroup_OpinionReactionRoot.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingSearchTools.h"
#include "engine/_quarantine/CThing.h"

struct C3DVector { float X, Y, Z; };
struct CThingView : CThing { C3DVector* GetPosition(); };
struct CThingAICreatureView : CThingAICreature { C3DVector* GetPosition(); };
struct CThingIntelligentPointerView {
    unsigned char NativeBytes[9];
    operator CThing*(); // retail 0x00a01b50
};
struct C2DBoxI { long MinX, MinY, MaxX, MaxY; };
struct CNearbyCreatureFilter {
    unsigned char NativeStorage[0x2c];
    CNearbyCreatureFilter(CThingAICreature*, float, bool); // retail 0x00986ed0
};
struct CThingSearchToolsView : CThingSearchTools {
    bool GetPFirstThingInRoughArea(C2DBoxI*, const CNearbyCreatureFilter&, CThingAICreature**); // retail 0x00987430
};
struct CAIStateGroupBaseView : CAIStateGroupBase {
    CThingSearchToolsView* PeekThingSearchTools(); // retail 0x008fd000
};
extern float OpinionReactionCommentRadius; // 0x0122ded4, 3.0f
struct CAIStateGroup_OpinionReactionRootView : CAIStateGroup_OpinionReactionRoot {
    bool CommentAboutHeroIsRunnable();
};

bool CAIStateGroup_OpinionReactionRootView::CommentAboutHeroIsRunnable()
{
    CThingIntelligentPointerView* reactingTo = (CThingIntelligentPointerView*)ThingReactingTo;
    if ((CThing*)*reactingTo != 0 &&
        (*(unsigned char*)((char*)(CThing*)*reactingTo + 0x91) & 1) == 0 &&
        (*(unsigned long*)((char*)(CThing*)*reactingTo + 0x28) & 0x20000) != 0) {
        CThingAICreature* creature = ((CAIStateGroupBase*)this)->PBrain->Creature;
        C3DVector* center = ((CThingAICreatureView*)creature)->GetPosition();
        long centerX = center->X;
        long centerY = center->Y;
        C2DBoxI area = { centerX - 3, centerY - 3, centerX + 3, centerY + 3 };
        CThingAICreature* nearby;
        if (((CAIStateGroupBaseView*)this)->PeekThingSearchTools()->GetPFirstThingInRoughArea(&area, CNearbyCreatureFilter(creature, 5.0f, true), &nearby)) {
            creature = ((CAIStateGroupBase*)this)->PBrain->Creature;
            CThing* thing = *reactingTo;
            C3DVector* creaturePosition = ((CThingAICreatureView*)creature)->GetPosition();
            C3DVector* thingPosition = ((CThingView*)thing)->GetPosition();
            float dx = thingPosition->X - creaturePosition->X;
            float dy = thingPosition->Y - creaturePosition->Y;
            float dz = thingPosition->Z - creaturePosition->Z;
            float distance = sqrtf(dx * dx + dy * dy + dz * dz);
            return distance > OpinionReactionCommentRadius;
        }
    }
    return false;
}