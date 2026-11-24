// NScript::CV_BanditTollScript::CTollBandit::SetUpBanditToll() -- place the toll bandits: teleport Me to the nearer blockade point (M_TollBanditBlockWay when it is closer to the hero than Me, else M_TollBandit), pick the near or far blockade markers, spawn archer/grunt/archer "BlockingBandit" creatures on them and face them along the markers
// Leaf: ?SetUpBanditToll@CTollBandit@CV_BanditTollScript@NScript@@AAEXXZ
// Name-fix NF38: Ego_r.pdb / FableWin.pdb ?SetUpBanditToll@CTollBandit@CV_BanditTollScript@NScript@@AAEXXZ (private), declared for
// 0x00E4E0E0 by engine/CV_BanditTollScript.h FABLE_RETAIL_CV_BANDITTOLLSCRIPT_TOLLBANDIT; the landed Main 0x00E4B5B0 binds it. Was the
// local stand-in CTollBanditScript::HandleBanditToll.
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E4E0E0, length 1811 bytes. Called from 0x00E4B68D.
// Label: catalog/autoname-t5 "CCreatureAction_TrollWhackGroundBase::HandleBanditToll" is wrong -- `this` is an entity script (Game +4, Me +8),
//   called from the entity Main at 0x00E4B5B0 (vtable slot 0x012E4D8C), next to CM_TollBandit / CM_TauntingBandit.
// Shape: the handle getters (slot 6 GetPos, slot 10 GetHeading) inline on the exact-type locals with the empty-handle
//   default first; CCountedPointer::operator= reads the source pair before the self-compare; the distance test as
//   `dist(Me, hero) > dist(blockWay, hero)`; plain-inline ~CScriptThing, the early returns call it by VC7.1's per-site frequency rule (docs/pipeline/INLINE_DEPTH_REVISIT.md).
// Callees: CCharString 0x0099EBF0 / ~ 0x0099EAE0, CBaseClass 0x0099A2D0 / ~ 0x0099A2E0, ~CScriptThing 0x004AA840,
//   GetDistanceBetweenThings 0x00CBE512 (__fastcall), IsActiveThreadTerminating 0x00F35B30, operator delete 0x00BFE9BC;
//   CGameScriptInterface slots 70 GetHero, 72 GetThingWithScriptName, 91 CreateCreature, 472 EntityTeleportToThing,
//   474 EntitySetFacingAngle. Default position 0x0143E8E0 for an empty handle.
#include "fable_compiler.h"
#include "engine/TRetVoid.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CV_BANDITTOLLSCRIPT_TOLLBANDIT
#define FABLE_RETAIL_SCRIPT_GETDISTANCEBETWEENTHINGS
#include <stdlib.h>
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CV_BanditTollScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/fable_script_distance.h"
// Retail expands these on the exact-type locals (slot 6 GetPos, slot 10 GetAngleXY, empty handle first).
inline CScriptThing::CScriptThing() { }
inline CScriptThing::~CScriptThing() { }
FABLE_FORCEINLINE CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
// NF326 one-definition respell: the body in the shape of the self row 0x004AA980 (retail's out-of-line copy), so this TU's pick-any COMDAT equals it.
inline const C3DVector& CScriptThing::GetPos() const { const CScriptThing* imp = PImp.Data; if (imp == 0) return NullCoord; return imp->GetPos(); }
// NF326 one-definition respell: the body in the shape of the self row 0x004AAA40 (retail's out-of-line copy), so this TU's pick-any COMDAT equals it.
inline float CScriptThing::GetAngleXY() const { const CScriptThing* imp = PImp.Data; if (imp == 0) return 0.0f; float angle = imp->GetAngleXY(); return angle; }

void NScript::CV_BanditTollScript::CTollBandit::SetUpBanditToll()
{
    CScriptThing blockWay = Game->GetThingWithScriptName("M_TollBanditBlockWay");
    CScriptThing point1;
    CScriptThing point2;
    CScriptThing point3;
    if (GetDistanceBetweenThings(Me, Game->GetHero()) > GetDistanceBetweenThings(blockWay, Game->GetHero())) {
        if (IsActiveThreadTerminating())
            return;
        Game->EntityTeleportToThing(Me, blockWay, false);
        point1 = Game->GetThingWithScriptName("BanditBlockadeNear1");
        point2 = Game->GetThingWithScriptName("BanditBlockadeNear2");
        point3 = Game->GetThingWithScriptName("BanditBlockadeNear3");
    } else {
        if (IsActiveThreadTerminating())
            return;
        Game->EntityTeleportToThing(Me, Game->GetThingWithScriptName("M_TollBandit"), false);
        point1 = Game->GetThingWithScriptName("BanditBlockadeFar1");
        point2 = Game->GetThingWithScriptName("BanditBlockadeFar2");
        point3 = Game->GetThingWithScriptName("BanditBlockadeFar3");
    }
    CScriptThing bandit1 = Game->CreateCreature("CREATURE_BANDIT_ARCHER_LEVEL1", point1.GetPos(), "BlockingBandit", false);
    CScriptThing bandit2 = Game->CreateCreature("CREATURE_BANDIT_GRUNT_LEVEL1", point2.GetPos(), "BlockingBandit", false);
    CScriptThing bandit3 = Game->CreateCreature("CREATURE_BANDIT_ARCHER_LEVEL1", point3.GetPos(), "BlockingBandit", false);
    Game->EntitySetFacingAngle(bandit1, point1.GetAngleXY(), true);
    Game->EntitySetFacingAngle(bandit2, point2.GetAngleXY(), true);
    Game->EntitySetFacingAngle(bandit3, point3.GetAngleXY(), true);
}