// NScript::CQ_AmbushTradersScript::CAT_ConvoyMember::Main() -- an Ambush Traders convoy member: wait for one of the three convoy regions, take scripted control (priority 4 in Bowerstone Jail, else 2), in the Deserted Farm hold "WaitForTrigger" members until ConvoyTrigger2 and mark them "Triggered", walk to the convoy exit point, then each frame: when the quest fails or finishes walk to the nearest REGION_EXIT_POINT and fade out; once at the exit point fade out (Bowerstone Jail: then wait for the Deserted Farm), record a teleport entry (Deserted Farm: then wait for the Gibbet Woods) or, in the Gibbet Woods, flag AnyTraderEscaped for a CREATURE_TRADER_01 convoy member and fade out
// Leaf: ?Main@CAT_ConvoyMember@CQ_AmbushTradersScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F0CAA0, 2509 bytes, thiscall, plain `ret` (three epilogue copies), no EH frame; ends at 0x00F0D46C.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?Main@CAT_ConvoyMember@CQ_AmbushTradersScript@NScript@@EAEXXZ; the address appears once in
//   .rdata, at slot 1 of the entity vtable 0x012F7DF8 stored by the landed CAT_ConvoyMember::Alloc 0x00F04280 (its Init 0x00F08C30 /
//   GetParentScript 0x00F04320 are landed); "AT_ConvoyMember" is the binding name Main 0x00F024C0 registers for this class.
// Callees: CBaseObject 0x0099A380 / ~ 0x0099A430 (the handle's inline ctor / the inline dtor at the first exit), ~ScriptedThingBase
//   0x007E74D0 (the other exits), handle IsValid 0x00CD23B9 / Release 0x00CD2770 / MoveToPosition 0x007E72F0 (direct on the local),
//   operator delete 0x00BFE9BC, CCharString 0x0099EBF0 / ~ 0x0099EAE0 / operator=(const CCharString&) 0x0099EFB0 / operator=(const char*)
//   0x0099EFE0, CBasicString<char>::Compare 0x00411570, CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30, CScriptThing::IsAlive
//   0x004AB130 / ~CScriptThing 0x004AA840, TeleportData ctor 0x00F0D470 / dtor 0x00CDBC50, vector<TeleportData>::push_back 0x00F0D490,
//   vector<CCharString>::operator= 0x00432EE9 / ~vector 0x00414EA0, IsDistanceBetweenThingsUnder 0x00CBE2FF and FadeOutAndKillEntity
//   0x00CBE716 (__fastcall); CGameScriptInterfaceBase slots 7 NewScriptFrame, 8 StartScriptingEntity, 12 IsRegionLoaded,
//   72 GetThingWithScriptName, 81 GetNearestWithDefName, 264 GetHealth, 483 EntityGetAppearanceSeed, 588 GetItemDefNamesFromContainer;
//   CScriptThing slots 1 GetName, 2 GetDefName, 3 GetDataString, 4 SetDataString, 6 GetPos.
// Layout: engine/CQ_AmbushTradersScript.h: ParentClass->QuestFailed +0x48 / QuestFinished +0x49 / ConvoyTrigger2 +0x51 / AnyTraderEscaped
//   +0x63 / TeleportInfo +0x1c4; TeleportData and CAT_ConvoyMember from the new opt-in FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_CONVOYMEMBER.
// Shape: generated entity-script code (the landed CEM_SuspiciousVillager::Main 0x00E776F0 model) with WAIT_UNTIL loops and an endless
//   frame loop; the two Bowerstone Jail arms share one hoisted terminate call; the "Triggered" arm holds only its terminate check.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE
#define FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_MAIN
#define FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_CONVOYMEMBER
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "stlport/fable_containers.h"
#include "engine/CQ_AmbushTradersScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/C3DVector.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_fade.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
template <> std::vector<CCharString, std::allocator<CCharString> >::~vector();                              // 0x00414EA0

void NScript::CQ_AmbushTradersScript::CAT_ConvoyMember::Main()
{
    CScriptGameResourceObjectScriptedThingBase seh_me;
    while (!(Game->IsRegionLoaded("BowerstoneJail") || Game->IsRegionLoaded("DesertedFarm") || Game->IsRegionLoaded("GibbetWoods"))) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (Game->IsRegionLoaded("BowerstoneJail")) {
        if (IsActiveThreadTerminating())
            return;
        if (seh_me.IsValid())
            seh_me.Release();
        while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
    } else {
        if (IsActiveThreadTerminating())
            return;
        if (seh_me.IsValid())
            seh_me.Release();
        while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_MEDIUM)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
    }
    if (IsActiveThreadTerminating())
        return;
    if (Game->IsRegionLoaded("DesertedFarm")) {
        if (IsActiveThreadTerminating())
            return;
        if (Me.GetDataString() == "WaitForTrigger") {
            if (IsActiveThreadTerminating())
                return;
            while (!ParentClass.ConvoyTrigger2) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            Me.SetDataString("Triggered");
        } else if (Me.GetDataString() == "Triggered") {
            if (IsActiveThreadTerminating())
                return;
        }
    }
    CScriptThing exit_point = Game->GetThingWithScriptName("M_AT_ConvoyExitPoint");
    seh_me.MoveToPosition(exit_point.GetPos(), 1.0f, (EScriptEntityMoveType)0, false, true);
    if (IsActiveThreadTerminating())
        return;
    for (;;) {
        if (ParentClass.QuestFailed || ParentClass.QuestFinished) {
            if (IsActiveThreadTerminating())
                return;
            if (seh_me.IsValid())
                seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            CScriptThing region_exit = Game->GetNearestWithDefName(Me, "REGION_EXIT_POINT");
            if (region_exit.IsAlive()) {
                if (IsActiveThreadTerminating())
                    return;
                if (seh_me.IsValid()) {
                    if (IsActiveThreadTerminating())
                        return;
                    seh_me.MoveToPosition(region_exit.GetPos(), 1.0f, (EScriptEntityMoveType)1, false, true);
                }
                while (!IsDistanceBetweenThingsUnder(Me, region_exit, 2.0f)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
                FadeOutAndKillEntity(Me, 1.0f, true, true);
            }
        }
        if (IsDistanceBetweenThingsUnder(Me, exit_point, 5.0f)) {
            if (IsActiveThreadTerminating())
                return;
            if (Game->IsRegionLoaded("BowerstoneJail")) {
                if (IsActiveThreadTerminating())
                    return;
                FadeOutAndKillEntity(Me, 1.0f, true, true);
                while (!Game->IsRegionLoaded("DesertedFarm")) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
            } else if (Game->IsRegionLoaded("DesertedFarm")) {
                if (IsActiveThreadTerminating())
                    return;
                TeleportData teleport;
                Game->EntityGetAppearanceSeed(Me, teleport.appearanceSeed);
                teleport.defName = Me.GetDefName();
                teleport.health = Game->GetHealth(Me);
                teleport.scriptName = "AT_ConvoyMember";
                std::vector<CCharString> items;
                Game->GetItemDefNamesFromContainer(Me, items);
                teleport.itemsCarried = items;
                ParentClass.TeleportInfo.push_back(teleport);
                FadeOutAndKillEntity(Me, 1.0f, true, true);
                while (!Game->IsRegionLoaded("GibbetWoods")) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
            } else if (Game->IsRegionLoaded("GibbetWoods")) {
                if (IsActiveThreadTerminating())
                    return;
                if (Me.GetDefName() == "CREATURE_TRADER_01" && Me.GetName() == "AT_ConvoyMember" && !ParentClass.AnyTraderEscaped) {
                    if (IsActiveThreadTerminating())
                        return;
                    ParentClass.AnyTraderEscaped = true;
                }
                FadeOutAndKillEntity(Me, 1.0f, true, true);
            }
        }
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
}