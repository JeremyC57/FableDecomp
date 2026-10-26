#include <cstdio>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#define FABLE_ENGINE_CAISTATEGROUP_OPINIONREACTIONROOT_H
#define FABLE_ENGINE_CAISTATEGROUPBASE_H
#define FABLE_ENGINE_CAIBRAIN_H
#define FABLE_ENGINE_CTHINGAICREATURE_H
#define FABLE_ENGINE_CTHINGSEARCHTOOLS_H
#define FABLE_ENGINE_CTHING_H
#pragma pack(push, 1)
class CAIStateGroup_OpinionReactionRoot { public: unsigned char prefix[0x6b]; unsigned char ThingReactingTo[9]; unsigned char suffix[0x24]; };
class CAIBrain { public: unsigned char prefix[0x18]; class CThingAICreature* Creature; };
class CAIStateGroupBase { public: unsigned char prefix[4]; CAIBrain* PBrain; };
class CThingAICreature { public: unsigned char bytes[0x210]; };
class CThingSearchTools { public: unsigned char bytes[0x1c]; };
class CThing { public: unsigned char bytes[0xa8]; };
#pragma pack(pop)
#endif
// BEGIN EXACT CANDIDATE SOURCE
// CAIStateGroup_OpinionReactionRoot::CommentAboutHeroIsRunnable() -- require a live, visible reacting-to thing, a nearby matching creature, and an ordered distance strictly greater than 3.0f; retail 0x009863e0, 304 bytes. Uses the generated flattened state-group header and actual CAIStateGroupBase/CAIBrain/CThingAICreature/CThing/CThingSearchTools headers. Donor caveat: OpinionReactionRoot is a flattened generated layout; its +0x04 brain link is viewed through CAIStateGroupBase. ThingReactingTo at +0x6b is a 9-byte CIntelligentPointer unavailable as a typed shared member, so the adapter is field-free. Native query helper: CThingSearchTools::GetPFirstThingInRoughArea @0x987430, filter init @0x986ed0, pointer conversion @0xa01b50. Radius constant 3.0f @0x0122ded4. FCOMP/TEST AH,0x41/JNE returns false for less, equal, or unordered; success is ordered greater-than only.
#include <math.h>
#include "engine/CAIStateGroup_OpinionReactionRoot.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingSearchTools.h"
#include "engine/_quarantine/CThing.h"

class C3DVector { public: float X, Y, Z; };
struct CThingView : CThing { C3DVector* GetPosition(); };
struct CThingAICreatureView : CThingAICreature { C3DVector* GetPosition(); };
struct CThingIntelligentPointerView {
    unsigned char NativeBytes[9];
    operator CThing*(); // retail 0x00a01b50
};
class C2DBoxI { public: long MinX, MinY, MaxX, MaxY; };
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
// END EXACT CANDIDATE SOURCE

static int failures, conversions, searches, filterCalls, positionCalls;
static bool queryResult, mutateOnQuery;
static bool mutateOnFourth;
static CThing* conversionScript[4];
static bool scripted;
static CThingAICreature* queryCreature;
static CThingAICreature creatureA, creatureB, queryOutput;
static CThing thingOne, thingTwo;
static CAIBrain brain;
static C3DVector posCreatureA, posCreatureB, posThingOne, posThingTwo;
static C3DVector* positionLog[8];
static CThingSearchToolsView searchTools;
static CAIStateGroup_OpinionReactionRootView* stateObject;
float OpinionReactionCommentRadius = 3.0f;
static void check(bool value, const char* label) { if (!value) { ++failures; std::printf("FAIL: %s\n", label); } }
static void setThingPointer(CThing* p) {
    std::memset((char*)stateObject + 0x6b, 0, 9);
    std::memcpy((char*)stateObject + 0x6b, &p, sizeof(p));
}
static void setThingFlags(CThing* p, unsigned char dead, unsigned long flags) {
    std::memset(p, 0, sizeof(*p));
    *((unsigned char*)p + 0x91) = dead;
    *((unsigned long*)((char*)p + 0x28)) = flags;
}
static void resetCounts() {
    conversions = searches = filterCalls = positionCalls = 0;
    queryResult = true; mutateOnQuery = false; mutateOnFourth = false; scripted = false;
}
CThingIntelligentPointerView::operator CThing*() {
    ++conversions; CThing* p = 0; std::memcpy(&p, NativeBytes, sizeof(p)); if (mutateOnFourth && conversions == 4) brain.Creature = &creatureA; if (scripted && conversions <= 4) p = conversionScript[conversions-1]; return p;
}
C3DVector* CThingView::GetPosition() {
    if ((CThing*)this == &thingOne) { positionLog[positionCalls++] = &posThingOne; return &posThingOne; }
    if ((CThing*)this == &thingTwo) { positionLog[positionCalls++] = &posThingTwo; return &posThingTwo; }
    ++failures; return 0;
}
C3DVector* CThingAICreatureView::GetPosition() {
    if ((CThingAICreature*)this == &creatureA) { positionLog[positionCalls++] = &posCreatureA; return &posCreatureA; }
    if ((CThingAICreature*)this == &creatureB) { positionLog[positionCalls++] = &posCreatureB; return &posCreatureB; }
    ++failures; return 0;
}
CNearbyCreatureFilter::CNearbyCreatureFilter(CThingAICreature* center, float radius, bool includeFaction) {
    ++filterCalls; check(center == queryCreature, "filter center creature");
    check(radius == 5.0f, "filter radius 5.0"); check(includeFaction, "filter include flag");
}
bool CThingSearchToolsView::GetPFirstThingInRoughArea(C2DBoxI* area, const CNearbyCreatureFilter& filter, CThingAICreature** out) {
    ++searches; (void)filter;
    check(this == &searchTools, "search tools receiver from PeekThingSearchTools");
    check(area->MinX == -5 && area->MaxX == 1, "box truncates negative X toward zero");
    check(area->MinY == 1 && area->MaxY == 7, "box truncates positive Y toward zero");
    check(out != 0, "search output pointer supplied"); if (out) *out = &queryOutput;
    if (mutateOnQuery) { brain.Creature = &creatureB; setThingPointer(&thingTwo); }
    return queryResult;
}
CThingSearchToolsView* CAIStateGroupBaseView::PeekThingSearchTools() {
    check(this == (CAIStateGroupBase*)stateObject, "PeekThingSearchTools receiver is state group"); return &searchTools;
}
static bool runCase(CThing* p, unsigned char dead, unsigned long flags, bool found, float x, float y, float z) {
    resetCounts(); setThingPointer(p); if (p) setThingFlags(p, dead, flags);
    queryResult = found; queryCreature = &creatureA; brain.Creature = &creatureA;
    posCreatureA.X = -2.5f; posCreatureA.Y = 4.8f; posCreatureA.Z = 0.0f;
    posThingOne.X = x; posThingOne.Y = y; posThingOne.Z = z;
    posCreatureB.X = 0.0f; posCreatureB.Y = 0.0f; posCreatureB.Z = 0.0f;
    posThingTwo.X = 3.2f; posThingTwo.Y = 0.0f; posThingTwo.Z = 0.0f;
    return stateObject->CommentAboutHeroIsRunnable();
}
int main() {
    union { long double align; unsigned char bytes[0x98]; } storage;
    std::memset(storage.bytes, 0, sizeof(storage.bytes));
    stateObject = new(storage.bytes) CAIStateGroup_OpinionReactionRootView;
    brain.Creature = &creatureA; CAIBrain* brainPointer = &brain;
    std::memcpy(storage.bytes + 4, &brainPointer, sizeof(brainPointer));
    setThingFlags(&thingOne, 0, 0x20000); setThingFlags(&thingTwo, 0, 0x20000);
    check(!runCase(0, 0, 0x20000, true, 4.0f, 4.8f, 0.0f), "null target rejected");
    check(conversions == 1 && searches == 0, "null target conversion count");
    check(!runCase(&thingOne, 1, 0x20000, true, 4.0f, 4.8f, 0.0f), "dead target rejected");
    check(conversions == 2 && searches == 0, "dead target conversion count");
    check(!runCase(&thingOne, 0, 0, true, 4.0f, 4.8f, 0.0f), "missing 0x20000 flag rejected");
    check(conversions == 3 && searches == 0, "flag target conversion count");
    check(!runCase(&thingOne, 0, 0x20000, false, 4.0f, 4.8f, 0.0f), "empty rough-area result rejected");
    check(conversions == 3 && searches == 1 && filterCalls == 1, "query false call counts");
    check(positionCalls == 1 && positionLog[0] == &posCreatureA, "pre-query center position only");
    check(!runCase(&thingOne, 0, 0x20000, true, 0.2f, 4.8f, 0.0f), "distance below 3 rejected");
    check(conversions == 4 && searches == 1, "successful path conversion count");
    check(positionCalls == 3 && positionLog[0] == &posCreatureA && positionLog[1] == &posCreatureA && positionLog[2] == &posThingOne,
          "position order is query center, creature, target");
    check(!runCase(&thingOne, 0, 0x20000, true, 0.5f, 4.8f, 0.0f), "distance equal to 3 rejected");
    check(runCase(&thingOne, 0, 0x20000, true, 1.1f, 4.8f, 0.0f), "ordered distance above 3 accepted");
    union { unsigned int bits; float value; } nanValue; nanValue.bits = 0x7fc00000u;
    check(!runCase(&thingOne, 0, 0x20000, true, nanValue.value, 4.8f, 0.0f), "unordered NaN distance rejected");

    resetCounts(); setThingPointer(&thingOne); setThingFlags(&thingOne, 0, 0x20000);
    queryCreature = &creatureA; queryResult = true; mutateOnQuery = true; mutateOnFourth = true; brain.Creature = &creatureA;
    posCreatureA.X = -2.5f; posCreatureA.Y = 4.8f; posCreatureA.Z = 0.0f;
    posCreatureB.X = 0.0f; posCreatureB.Y = 0.0f; posCreatureB.Z = 0.0f;
    posThingTwo.X = 3.2f; posThingTwo.Y = 0.0f; posThingTwo.Z = 0.0f;
    check(stateObject->CommentAboutHeroIsRunnable(), "query callback mutation followed by >3 result");
    check(conversions == 4 && searches == 1, "three gate conversions and one post-query conversion");
    check(positionCalls == 3 && positionLog[0] == &posCreatureA && positionLog[1] == &posCreatureB && positionLog[2] == &posThingTwo,
          "callback mutations precede creature reload and target conversion");

    // Separate results prove the gates perform fresh smart-pointer conversions.
    resetCounts(); setThingPointer(&thingOne); brain.Creature = &creatureA;
    queryCreature = &creatureA; scripted = true;
    setThingFlags(&thingOne, 1, 0); setThingFlags(&thingTwo, 0, 0x20000);
    conversionScript[0] = &thingOne; conversionScript[1] = &thingTwo;
    conversionScript[2] = &thingTwo; conversionScript[3] = &thingOne;
    posThingOne.X = 1.1f; posThingOne.Y = 4.8f; posThingOne.Z = 0.0f;
    check(stateObject->CommentAboutHeroIsRunnable(), "each gate uses its own conversion result");
    check(conversions == 4 && searches == 1 && positionLog[2] == &posThingOne, "post-query conversion remains distinct");
    if (failures) { std::printf("009863e0_TEST FAIL %d\n", failures); return 1; }
    std::printf("009863e0_TEST PASS\n"); return 0;
}