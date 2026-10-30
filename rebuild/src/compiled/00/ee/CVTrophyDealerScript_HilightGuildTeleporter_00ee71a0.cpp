// CV_TrophyDealerScript::HilightGuildTeleporter() -- once the Guild interior is loaded, put a quest-core orb on the Guild teleporter's minimap marker until the hero reaches Witchwood
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE71A0, 405 bytes (416 with padding), plain `ret`.
// Callees: CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0, ~CScriptThing 0x004AA840 (out of line),
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940; through the interface (+0x40) NewScriptFrame (slot 7),
//   IsRegionLoaded (slot 12), GetThingWithScriptName (slot 72, hidden result), MiniMapAddMarker (slot 350),
//   MiniMapRemoveMarker (slot 352).
// Strings: "HeroGuildComplexInside" 0x01238FCC, "HERO_GUILD_TELEPORT_MARKER" 0x01257238, "HUD_ORB_QUEST_CORE" 0x01258AB0,
//   "Witchwood1" 0x012CC268.
// Shape: two WAIT_UNTIL loops (`while (!x) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }` -- the
//   plain negation gives retail's neg/sbb/inc bool), each followed by the macro's terminating test; every string is an
//   implicit conversion; the marker handle's destructor is the out-of-line one at both later exits.
// Layout: interface pointer at +0x40 (CQuestScriptBase). Local model: CGameScriptInterface.h carries no virtual slots.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CV_TrophyDealerScript {
    unsigned char _base[0x40];                              // CScriptBase
    CGameScriptInterface* Game;                             // +0x40
    bool IsActiveThreadTerminating();                       // 0x00CB7940 (CScriptBase)
    void HilightGuildTeleporter();
};
void CV_TrophyDealerScript::HilightGuildTeleporter()
{
    while (!Game->IsRegionLoaded("HeroGuildComplexInside")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing teleporter = Game->GetThingWithScriptName("HERO_GUILD_TELEPORT_MARKER");
    Game->MiniMapAddMarker(teleporter, "HUD_ORB_QUEST_CORE");
    while (!Game->IsRegionLoaded("Witchwood1")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->MiniMapRemoveMarker(teleporter);
}