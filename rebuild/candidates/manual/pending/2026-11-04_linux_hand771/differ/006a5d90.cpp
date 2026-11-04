// CThingPhysical::ConstructFromLevelScript(const char* script) -- construct a physical thing from its level-script block
// Grade: DIFFER(1268v1268) -- 2 lines: retail schedules the C3DAngles temporary's AngleYZ store before `fstp position.Y` (0x006A5FD4/D8); ours after
// Leaf: ?ConstructFromLevelScript@CThingPhysical@@UAE_NPBD@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A5D90, 1268 bytes, `ret 4`; the virtual override (Ego_r.pdb
//   ?ConstructFromLevelScript@CThingPhysical@@UAE_NPBD@Z; 0x00838930 is a `jmp 0x006A5D90` stub the derived classes call).
// Callees: CThing::ConstructFromLevelScript 0x004CAA00, CStringParser(const char*, ...) 0x00414E30, NextItemExists 0x009BA4F0,
//   ReadAsString 0x009BA6F0, ReadAsFloat 0x009BA650, ReadAsInteger 0x009BA540, SkipPastString 0x009B9BA0 (";" 0x0123542C),
//   CBasicString<char>::operator==(const char*) 0x004115A0, CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0,
//   CThing::GetWorldMap 0x004C79D0 then CWorldMap::GetMapPos (vtable +0x5C), CRightHandedSet(const C3DAngles&) 0x00A229E0 (C3DAngles(float, float, float) inline),
//   CTCParamsPhysicsBase(const C3DVector&, const CRightHandedSet&) 0x006A06E0, CTCBase::ConstructFromParams (vtable +0xC),
//   ~CBaseClassNonCopyable 0x0099A300, CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 /
//   dtor 0x00A01C10 (CIntelligentPointer<CThingPhysical> vtable 0x01244DCC), list<CStringParserCommentBoundaries> dtor 0x004046B0,
//   ~CBaseClass 0x0099A2E0. Keywords: AngleXY/AngleYZ/AngleXZ/PosX/PosY/PosZ/ObjectScale/CanComeBetweenCameraAndHero/Health/EndThing.
// Shape: the retail CThing / CThingPhysical / CThingBaseDef chain with the chunk-771 overlay sub-macros; TU-local CBasicString
//   Compare / operator==(const char*) bodies (VC7.1 threads the PStringData null test once over the keyword chain, as in
//   0x009E8C10); the success path is the `if` body (a shared `return false` tail); Health is stored after MaxHealth
//   (`Health = MaxHealth = ...`); Drawable is set before the hit flags. The physics call goes through a row-local inline helper so
//   the angle vector's loads precede the PhysicsTC read (retail order); a local `physics` pointer gets within 6 lines.
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
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

template<class T> inline long __fastcall CBasicString<T>::Compare(const T* a,const T* b)
{
    while(*a || *b) {
        if(*a<*b)return -1;
        if(*a>*b)return 1;
        ++a; ++b;
    }
    return 0;
}
template<class T> inline bool CBasicString<T>::operator==(const T* text) const
{ return Compare(PData,text)==0; }

class CTCParamsBase : public CBaseClassNonCopyable {
};
class CTCParamsPhysicsBase : public CTCParamsBase {
public:
    CTCParamsPhysicsBase(const C3DVector& position, const CRightHandedSet& rhSet);   // 0x006A06E0
    C3DVector Position;
    CRightHandedSet RHSet;
};

static inline void ConstructPhysics(CTCPhysicsBase* physics, const C3DVector& position, const C3DAngles& angles)
{
    physics->ConstructFromParams(CTCParamsPhysicsBase(position, CRightHandedSet(angles)));
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
    if (PhysicsTC && positionRead) {
        position.X += GetWorldMap().GetMapPos(CurrentMapNumber).X;
        position.Y += GetWorldMap().GetMapPos(CurrentMapNumber).Y;
        ConstructPhysics(PhysicsTC, position, C3DAngles(angleXY, angleYZ, angleXZ));
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
