// 006A5D90 CThingPhysical::ConstructFromLevelScript -- parse physical state, initialise physics and reset creators/flags.
// Retail:0x006A5D90,1268bytes,thiscall ret4.
// Leaf: ?ConstructFromLevelScript@CThingPhysical@@UAE_NPBD@Z
// Flags: /d2inlS29
// Inline-budget: CCharString text-equality size28; measured S29 expands all original sites, preserving BasicString size129 calls.
// Original module392 thing_physical.obj01F1F560; actual direct receiver/accessors and complete real parameter constructor.
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CBASICSTRING_COMPARE_BODY
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_COMPARE_ZERO
#define FABLE_RETAIL_CPARSEDITEM
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_LEVELSCRIPT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGPHYSICAL_LEVELSCRIPT
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_ANGLES
#define FABLE_RETAIL_C3DANGLES_CTOR
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_C3DVECTOR
#include <string.h>
#include "fable_string.h"
#include "engine/CStringParser.h"
#include "engine/CThing.h"
#include "engine/CThingPhysical.h"
#include "engine/CThingBaseDef.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CRightHandedSet.h"
#include "engine/C3DAngles.h"
#include "engine/C2DCoordI.h"
#include "engine/CWorldMap.h"
#include "engine/CBaseClassNonCopyable.h"

#include "engine/CTCParamsPhysicsBase.h"

CTCParamsPhysicsBase::CTCParamsPhysicsBase(const C3DVector& position, const CRightHandedSet& rhs)
    : Position(position), Rhs(rhs)
{
}

bool CThingPhysical::ConstructFromLevelScript(const char* script)
{
    if (CThing::ConstructFromLevelScript(script)) {
    Scale = 1.0f;
    Health = MaxHealth = PeekBaseDef().Health;
    CanComeBetweenCameraAndObservedThing = -1;
    CStringParser parser(script);
    bool positionRead = false;
    C3DVector position(0.0f, 0.0f, 0.0f);
    float angleXY = 0.0f;
    float angleYZ = 0.0f;
    float angleXZ = 0.0f;
    while (parser.NextItemExists()) {
        CCharString token = parser.ReadAsString();
        if (token == "AngleXY") { angleXY = parser.ReadAsFloat(); positionRead = true; }
        else if (token == "AngleYZ") { angleYZ = parser.ReadAsFloat(); positionRead = true; }
        else if (token == "AngleXZ") { angleXZ = parser.ReadAsFloat(); positionRead = true; }
        else if (token == "PosX") { position.X = parser.ReadAsFloat(); positionRead = true; }
        else if (token == "PosY") { position.Y = parser.ReadAsFloat(); positionRead = true; }
        else if (token == "PosZ") { position.Z = parser.ReadAsFloat(); positionRead = true; }
        else if (token == "ObjectScale") Scale = parser.ReadAsFloat();
        else if (token == "CanComeBetweenCameraAndHero") CanComeBetweenCameraAndObservedThing = parser.ReadAsInteger();
        else if (token == "Health") Health = parser.ReadAsFloat();
        else if (token == "EndThing") break;
        parser.SkipPastString(";");
    }
    if (GetPhysicsTC() && positionRead) {
        position.X += GetWorldMap().GetMapPos(GetCurrentMapNumber()).X;
        position.Y += GetWorldMap().GetMapPos(GetCurrentMapNumber()).Y;
        GetPhysicsTC()->ConstructFromParams(CTCParamsPhysicsBase(position, CRightHandedSet(C3DAngles(angleXY, angleYZ, angleXZ))));
    }
    PLastCreator = CIntelligentPointer<CThingPhysical>(0);
    POriginalCreator = CIntelligentPointer<CThingPhysical>(0);
    Drawable = true;
    Killable = true;
    RespondToHits = true;
    EvadeAllHits = false;
    NegateAllHits = false;
    Damageable = PeekBaseDef().Damageable;
    AbleToBeEngagedInCombat = true;
    return true;
    }
    return false;
}