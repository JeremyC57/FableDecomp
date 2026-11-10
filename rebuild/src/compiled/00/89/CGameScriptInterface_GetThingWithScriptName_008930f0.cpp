// CGameScriptInterface::GetThingWithScriptName(const CCharString& mapName, const CCharString& scriptName) const -- the script thing of the first live game object with that script name on the named map, else the null script thing
// Leaf: ?GetThingWithScriptName@CGameScriptInterface@@UBE?AVCScriptThing@@ABVCCharString@@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008930F0, 186 bytes (+ int3 padding), thiscall `ret 0Ch` (hidden result, two strings).
// Identity: retail CGameScriptInterface vtable 0x01260F0C slot 73 (engine/CGameScriptInterface.h); Ego_r.pdb / FableWin.pdb
//   ?GetThingWithScriptName@CGameScriptInterface@@UBE?AVCScriptThing@@ABVCCharString@@0@Z (the one-string overload is slot 72, 0x008A7D60).
// Callees (PDB spellings): CWorld slots 13 PeekWorldMap / 12 GetWorldMap, CWorldMap::GetMapNumberFromScriptName(const CCharString&)
//   const 0x004FB880, CCharString copy ctor 0x0099EC30 (the HasScriptName filter, built in the dead `mapName` slot) / dtor 0x0099EAE0,
//   CThingSearchTools::GetPFirstThing<CFilter_Bind2<CThingFilter_HasScriptName, CThingFilter_InMap>, CThingGameObject> 0x008ABDF0,
//   CGameScriptThing::CreateNullThing() 0x008CFF40 (static fastcall, hidden result in ecx), CThing::GetScriptThing() const 0x004C7E50.
// Model: shared headers only (overlay opt-ins CWORLDMAP_MAPNUMBERFROMSCRIPTNAME, CTHINGSEARCHTOOLS_FIRSTTHING(_BODY),
//   CTHINGGAMEOBJECT_STATTYPEFLAGS, CGAMESCRIPTTHING_CREATENULLTHING; new engine/CThingFilter_HasScriptName.h). The filters are bound
//   with the PDB's GFFilter_Bind2 (??$GFFilter_Bind2@VCThingFilter_HasScriptName@@VCThingFilter_InMap@@@@YI...).
// Shape: (1) the GetPFirstThing body is visible in the TU (same-TU noinline template, the landed 0x008ABDF0 logic): with it the
//   `thing = 0` store sits after the map-name push (0x00893102) and the tools load before the bind stores (0x00893137), as retail;
//   (2) the not-found flag is a named bool (`neg/sbb/inc` into bl, kept across the filter's string dtor), and the two null returns
//   are separate statements -- VC7.1 cross-jumps them so the null block falls through and the alive path is the `je` target
//   (0x0089317D); one `if (notFound || !IS_THING_ALIVE(thing))` (or &&, if/else, nested) lays the GetScriptThing path first.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_MAPNUMBERFROMSCRIPTNAME
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTTHING
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTTHING_BODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGGAMEOBJECT_STATTYPEFLAGS
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CGAMESCRIPTTHING
#define FABLE_RETAIL_CGAMESCRIPTTHING_CREATENULLTHING
#define FABLE_RETAIL_SCOPED_PTR
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/CThing.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CThingGameObject.h"
#include "engine/CThingSearchTools.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CThingFilter_InMap.h"
#include "engine/CThingFilter_HasScriptName.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CGameScriptThing.h"

// ??$IS_THING_ALIVE@PAVCThingGameObject@@@@YI_NABQAVCThingGameObject@@@Z (FableWin.pdb) -- the PDB template `bool __fastcall IS_THING_ALIVE<T>(const T&)`, expanded here.
template <class T> inline bool __fastcall IS_THING_ALIVE(const T& thing)
{
    if (thing) {
        if (!thing->IsDead())
            return true;
    }
    return false;
}

CScriptThing CGameScriptInterface::GetThingWithScriptName(const CCharString& mapName, const CCharString& scriptName) const
{
    CThingGameObject* thing = 0;
    long mapNo = World->PeekWorldMap().GetMapNumberFromScriptName(mapName);
    bool notFound = !World->GetThingSearchTools().GetPFirstThing(
        GFFilter_Bind2(CThingFilter_HasScriptName(scriptName), CThingFilter_InMap(World->GetWorldMap(), mapNo)), &thing);
    if (notFound)
        return CGameScriptThing::CreateNullThing();
    if (!IS_THING_ALIVE(thing))
        return CGameScriptThing::CreateNullThing();
    return thing->GetScriptThing();
}