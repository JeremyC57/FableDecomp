// NScript::CV_StatueMasterScript::Main() -- the statue-master village script main: bind the statue, cellar-door and chest entity scripts, post them, then store in AnglesToFaceList the XY angle from the SM_Center marker to each of the SM_Nothing, SM_Bowerstone, SM_Guild and SM_Greatwood markers
// Leaf: ?Main@CV_StatueMasterScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ED3B30, 1402 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven): slot 2 (Main) of the script vtable 0x012F2E6C (stored by the landed ctor 0x00ED3A50; the landed RegisterMain
//   0x00ED3A90 spawns it); Ego_r.pdb name set ?Main@CV_StatueMasterScript@NScript@@UAEXXZ. The bound Allocs are the landed
//   CStatueMasterStatue 0x00ED40C0 / CStatueMasterCellarDoors 0x00ED44C0 / CStatueMasterChest 0x00ED4770 ::Alloc rows (binding-name
//   literal = class name less its `C`). The FSE Lua port (refs/script_recovery/seed_corpus/sources/StatueMaster) gives the same
//   markers and the centre-to-marker angles (retail has no null checks).
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0,
//   CScriptBase::AddEntityScriptBinding 0x00CB8230, CScriptBase::PostAddScriptedEntities 0x00CB8930, _CIfmod 0x00BFEB28 (the MSVCR71 import thunk, the
//   intrinsic's helper), operator delete 0x00BFE9BC and ~CBaseClass 0x0099A2E0 (the five expanded ~CScriptThing);
//   CGameScriptInterfaceBase slots 64 PostAddScriptedEntities, 72 GetThingWithScriptName; CScriptThing slot 6 GetPos expanded
//   (NullCoord 0x0143E8E0); C3DVector::GetAngleXY expanded (atan2 / (2 pi), wrapped into [0, 1)).
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_C3DVECTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CV_StatueMasterScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/C3DVector.h"
#include <math.h>
#pragma intrinsic(atan2,fmod)

namespace NScript {
#define FABLE_SM_ENTITY(Name) \
    class CV_StatueMasterScript::Name : public CEntityScriptBase { \
    public: \
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_StatueMasterScript&, CScriptDataBase*, const CScriptThing&); \
    };
FABLE_SM_ENTITY(CStatueMasterStatue)          // Alloc 0x00ED40C0
FABLE_SM_ENTITY(CStatueMasterCellarDoors)     // Alloc 0x00ED44C0
FABLE_SM_ENTITY(CStatueMasterChest)           // Alloc 0x00ED4770
#undef FABLE_SM_ENTITY
}

inline CScriptThing::~CScriptThing() { }
inline const C3DVector& CScriptThing::GetPos() const { if (!PImp.Data) return NullCoord; return PImp.Data->GetPos(); }
inline float __fastcall GFArcTan(double opposite, double adjacent)
{ return (float)atan2(opposite, adjacent) / (2.0f * 3.14159265358979323846f); }
FABLE_FORCEINLINE float __fastcall GET_ROUNDED_ANGLE(float angle)
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod(angle, 1.0f);
        if (angle < 0.0f) angle += 1.0f;
    }
    return angle;
}
FABLE_FORCEINLINE float C3DVector::GetAngleXY() const { return GET_ROUNDED_ANGLE(GFArcTan(X, Y)); }

void NScript::CV_StatueMasterScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CV_StatueMasterScript>("StatueMasterStatue", this, &CStatueMasterStatue::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_StatueMasterScript>("StatueMasterCellarDoors", this, &CStatueMasterCellarDoors::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_StatueMasterScript>("StatueMasterChest", this, &CStatueMasterChest::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    CScriptThing Nothing = Game->GetThingWithScriptName("SM_Nothing");
    CScriptThing Bowerstone = Game->GetThingWithScriptName("SM_Bowerstone");
    CScriptThing Guild = Game->GetThingWithScriptName("SM_Guild");
    CScriptThing Greatwood = Game->GetThingWithScriptName("SM_Greatwood");
    CScriptThing Center = Game->GetThingWithScriptName("SM_Center");
    AnglesToFaceList[0] = (Nothing.GetPos() - Center.GetPos()).GetAngleXY();
    AnglesToFaceList[1] = (Bowerstone.GetPos() - Center.GetPos()).GetAngleXY();
    AnglesToFaceList[2] = (Guild.GetPos() - Center.GetPos()).GetAngleXY();
    AnglesToFaceList[3] = (Greatwood.GetPos() - Center.GetPos()).GetAngleXY();
}