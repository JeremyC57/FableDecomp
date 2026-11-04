// behaviour test for 0x006A5D90 CThingPhysical::ConstructFromLevelScript (parked row source)
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

// ---- test stubs ----
#include <stdio.h>
#include <stdlib.h>
struct FakeStringData { char* PData; unsigned long StringLength; unsigned long DataLength : 31; bool UseFastExtend : 1; long NoRefs; };
const char CCharString::NullChar = 0;
static const char* g_tokens[32]; static int g_pos;
static bool g_baseOk; static int g_physCalls; static float g_physPos[3]; static float g_angles[3]; static int g_setPItem;
CCharString::CCharString(const char* text, long)
{
    FakeStringData* d = new FakeStringData;
    d->StringLength = (unsigned long)strlen(text); d->PData = new char[d->StringLength + 1]; strcpy(d->PData, text);
    d->DataLength = d->StringLength; d->UseFastExtend = false; d->NoRefs = 1;
    PStringData = (CCharStringData*)d;
}
CCharString::CCharString() { PStringData = 0; }
CCharString::~CCharString() { }
CBaseClass::CBaseClass() { }
CBaseClass::~CBaseClass() { }
CBaseClassNonCopyable::CBaseClassNonCopyable() { }
CBaseClassNonCopyable::~CBaseClassNonCopyable() { }
CStringParser::CStringParser(const char*, void (__fastcall*)(const CCharString&), const CCharString&, long) { g_pos = 0; }
bool CStringParser::NextItemExists() { return g_tokens[g_pos] != 0; }
static const char* Take() { const char* t = g_tokens[g_pos]; if (t) ++g_pos; return t ? t : ""; }
CCharString CStringParser::ReadAsString() { return CCharString(Take()); }
float CStringParser::ReadAsFloat() { return (float)atof(Take()); }
long CStringParser::ReadAsInteger() { return atol(Take()); }
bool CStringParser::SkipPastString(const CCharString& s)
{ const char* t = g_tokens[g_pos]; if (t && !strcmp(t, (*(FakeStringData* const*)&s)->PData)) { ++g_pos; return true; } return false; }
bool CThing::ConstructFromLevelScript(const char*) { return g_baseOk; }
struct FakeWorldMap {
    virtual void s0(); virtual void s1(); virtual void s2(); virtual void s3(); virtual void s4(); virtual void s5();
    virtual void s6(); virtual void s7(); virtual void s8(); virtual void s9(); virtual void s10(); virtual void s11();
    virtual void s12(); virtual void s13(); virtual void s14(); virtual void s15(); virtual void s16(); virtual void s17();
    virtual void s18(); virtual void s19(); virtual void s20(); virtual void s21(); virtual void s22();
    virtual C2DCoordI* GetMapPos(C2DCoordI* out, long map) { out->X = 100 * map; out->Y = 7; return out; }
};
void FakeWorldMap::s0(){} void FakeWorldMap::s1(){} void FakeWorldMap::s2(){} void FakeWorldMap::s3(){} void FakeWorldMap::s4(){}
void FakeWorldMap::s5(){} void FakeWorldMap::s6(){} void FakeWorldMap::s7(){} void FakeWorldMap::s8(){} void FakeWorldMap::s9(){}
void FakeWorldMap::s10(){} void FakeWorldMap::s11(){} void FakeWorldMap::s12(){} void FakeWorldMap::s13(){} void FakeWorldMap::s14(){}
void FakeWorldMap::s15(){} void FakeWorldMap::s16(){} void FakeWorldMap::s17(){} void FakeWorldMap::s18(){} void FakeWorldMap::s19(){}
void FakeWorldMap::s20(){} void FakeWorldMap::s21(){} void FakeWorldMap::s22(){}
static FakeWorldMap g_map;
CWorldMap& CThing::GetWorldMap() { return *(CWorldMap*)&g_map; }
struct FakePhysicsTC {
    virtual void s0() {} virtual void s1() {} virtual void s2() {}
    virtual void ConstructFromParams(const CTCParamsPhysicsBase& p)
    { ++g_physCalls; g_physPos[0] = p.Position.X; g_physPos[1] = p.Position.Y; g_physPos[2] = p.Position.Z; }
};
static FakePhysicsTC g_phys;
CRightHandedSet::CRightHandedSet(const C3DAngles& angles) { g_angles[0] = angles.AngleXY; g_angles[1] = angles.AngleYZ; g_angles[2] = angles.AngleXZ; }
CTCParamsPhysicsBase::CTCParamsPhysicsBase(const C3DVector& position, const CRightHandedSet& rhSet) : Position(position), RHSet(rhSet) { }
CBaseIntelligentPointer::CBaseIntelligentPointer() { PData = 0; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return (CBaseObject*)PData; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { ++g_setPItem; PData = (CBaseObjectPointer*)item; }

static unsigned char g_def[0x70];
static unsigned char* MakeThing(unsigned char* t, bool damageable)
{
    memset(t, 0, 0xc0); memset(g_def, 0, sizeof(g_def));
    *(float*)(g_def + 0x50) = 100.0f; if (damageable) g_def[0x42] |= 0x40;
    *(void**)(t + 0x70) = g_def;
    *(short*)(t + 0x8e) = 2;
    t[0xbc] = 0x18;                                  // EvadeAllHits / NegateAllHits set beforehand
    *(void**)(t + 0xa4) = (void*)0x1234;             // PLastCreator PData (cleared by the call)
    return t;
}
static void Script(const char** t) { int i = 0; for (; t[i]; ++i) g_tokens[i] = t[i]; g_tokens[i] = 0; }
int main()
{
    bool ok = true;
    unsigned char mem[0xc0];
    CThingPhysical* thing = (CThingPhysical*)MakeThing(mem, true);
    // base construction fails: nothing is touched
    g_baseOk = false; { const char* t[] = { "Health", "5", ";", 0 }; Script(t); }
    ok = ok && !thing->CThingPhysical::ConstructFromLevelScript("x") && *(float*)(mem + 0x9c) == 0.0f && g_pos == 0;
    // full script with a physics TC: position offset by the map position, angles passed through
    g_baseOk = true; *(void**)(mem + 0x60) = &g_phys;
    { const char* t[] = { "PosX", "1.5", ";", "AngleYZ", "0.25", ";", "PosZ", "3", ";", "ObjectScale", "2", ";", "Health", "40", ";",
                          "CanComeBetweenCameraAndHero", "1", ";", "Unknown", ";", "EndThing", "PosY", "9", 0 }; Script(t); }
    ok = ok && thing->CThingPhysical::ConstructFromLevelScript("x");
    ok = ok && *(float*)(mem + 0x9c) == 2.0f && *(float*)(mem + 0xb0) == 100.0f && *(float*)(mem + 0xb4) == 40.0f;
    ok = ok && *(long*)(mem + 0xb8) == 1 && g_pos == 21;                     // stopped at EndThing
    ok = ok && g_physCalls == 1 && g_physPos[0] == 201.5f && g_physPos[1] == 7.0f && g_physPos[2] == 3.0f;
    ok = ok && g_angles[0] == 0.0f && g_angles[1] == 0.25f && g_angles[2] == 0.0f;
    ok = ok && (mem[0xbc] & 0x3f) == 0x27 && (mem[0x92] & 2) && *(void**)(mem + 0xa4) == 0 && g_setPItem == 4;
    // no position keyword: the physics TC is left alone; Damageable follows the definition
    MakeThing(mem, false); *(void**)(mem + 0x60) = &g_phys; g_physCalls = 0;
    { const char* t[] = { "ObjectScale", "0.5", 0 }; Script(t); }
    ok = ok && thing->CThingPhysical::ConstructFromLevelScript("x") && g_physCalls == 0 && *(long*)(mem + 0xb8) == -1;
    ok = ok && (mem[0xbc] & 0x3f) == 0x26 && *(float*)(mem + 0x9c) == 0.5f;
    printf(ok ? "006a5d90_TEST PASS\n" : "006a5d90_TEST FAIL\n");
    return ok ? 0 : 1;
}
