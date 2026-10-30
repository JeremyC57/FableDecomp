// CQR_EscortTraderScript::ExpressionEnableFollow(const CScriptThing& trader, CScriptGameResourceObjectScriptedThingBase& handle) -- the hero used the "follow me" expression: if the trader's scripting handle is still running a follow on the hero, re-take the trader and stop that follow; then (two script frames later) mark the trader as following, drop its minimap marker and information, make it follow the hero, show its emote icon and region-follow the hero
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E242D0, length 371 bytes (+ int3 padding), `ret 8`.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, the handle's IsValid 0x00CD23B9 / Release
// 0x00CD2770 (as the landed CFH_BriarRose::Main 0x00D37330) and its virtual StopFollowingThing (slot 8, +0x20) /
// IsFollowActionRunning (slot 9, +0x24) (CScriptGameResourceObjectScriptedThing vtable 0x0128D86C); through the game
// script interface (+0x40): NewScriptFrame (7), StartScriptingEntity (8), GetHero (70), EntityFollowThing (229),
// MiniMapRemoveMarker (352), SetThingHasInformation (360), SetEntityAsRegionFollowing (480),
// EntitySetAsDisplayingEmoteIcon (530).
// Layout: Ego_r.pdb CQR_EscortTraderScript - 0x14 (base) and - 0x18 more after its six STLport vectors, as the landed
// ForceEnableWait 0x00E23EB0: TraderFollowing +0x50, ExpressionDetectedFollow +0x70, ExpressionDetectedWait +0x71,
// RegionChangeDetected +0x118, QuestStage +0x11C.
// Shape: the script wait idiom; a nested `GSI->F(GSI->GetHero(), ...)` / `handle.F(GSI->GetHero())` fetches the
// callee's vtable before the inner GetHero call.
class CScriptThing { public: unsigned char _body[12]; };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_HIGHEST = 4 };

class CScriptGameResourceObjectScriptedThingBase { public:     // vtable 0x0128D86C (CScriptGameResourceObjectScriptedThing)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void StopFollowingThing(const CScriptThing& thing);              // slot 8 (+0x20)
    virtual bool IsFollowActionRunning(const CScriptThing& thing) const;     // slot 9 (+0x24)
    unsigned char _body[0x0c];
    bool IsValid() const;                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"

enum EscortTraderQuestStage { ESCORT_STAGE_2 = 2 };

struct CQR_EscortTraderScript : CScriptBase {
    CGameScriptInterface*  GSI;                         // +0x40
    void*                  Data;                        // +0x44
    unsigned char          _pad48[0x50 - 0x48];
    bool                   TraderFollowing;             // +0x50 (PDB +0x64)
    unsigned char          _pad51[0x70 - 0x51];
    bool                   ExpressionDetectedFollow;    // +0x70 (PDB +0x84)
    bool                   ExpressionDetectedWait;      // +0x71 (PDB +0x85)
    unsigned char          _pad72[0x118 - 0x72];
    bool                   RegionChangeDetected;        // +0x118 (PDB +0x144)
    EscortTraderQuestStage QuestStage;                  // +0x11c (PDB +0x148)
    void ExpressionEnableFollow(const CScriptThing& trader, CScriptGameResourceObjectScriptedThingBase& handle);
};

void CQR_EscortTraderScript::ExpressionEnableFollow(const CScriptThing& trader, CScriptGameResourceObjectScriptedThingBase& handle)
{
    if (handle.IsValid() && handle.IsFollowActionRunning(GSI->GetHero())) {
        if (IsActiveThreadTerminating())
            return;
        if (handle.IsValid())
            handle.Release();
        while (!GSI->StartScriptingEntity(trader, handle, SCRIPT_AI_PRIORITY_HIGHEST)) {
            GSI->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        handle.StopFollowingThing(GSI->GetHero());
        if (handle.IsValid())
            handle.Release();
    }
    GSI->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    GSI->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    ExpressionDetectedFollow = false;
    ExpressionDetectedWait = false;
    TraderFollowing = true;
    RegionChangeDetected = false;
    QuestStage = ESCORT_STAGE_2;
    GSI->MiniMapRemoveMarker(trader);
    GSI->SetThingHasInformation(trader, false, false, false);
    GSI->EntityFollowThing(trader, GSI->GetHero(), 1.0f, true);
    GSI->EntitySetAsDisplayingEmoteIcon(trader, true);
    GSI->SetEntityAsRegionFollowing(GSI->GetHero(), trader, true);
}