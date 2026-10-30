// CV_BordelloScript::WatchForDeedStatus() -- once the Bordello is loaded, bring the hidden deeds out of limbo when the hero finds their hiding place and record when the hero picks them up
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E3A150, 497 bytes (512 with padding), plain `ret`.
// Callees: CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0, ~CScriptThing 0x004AA840 (out of line),
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940; through the interface (+0x40) NewScriptFrame (slot 7),
//   IsRegionLoaded (slot 12), GetHero (slot 70), GetThingWithScriptName (slot 72, hidden result),
//   IsObjectInThingsPossession (slot 184), EntitySetInLimbo (slot 453).
// Strings: "Bordello" 0x012E39A4, "M_HiddenDeeds" 0x012E39C8, "OBJECT_DEEDS_BORDELLO" 0x012E39B0.
// Shape: three WAIT_UNTIL loops (`while (!x) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }` plus the
//   macro's terminating test), every string an implicit conversion; the already-found early exit still makes the
//   terminating call (`IsActiveThreadTerminating(); return;`).
// Layout: retail quest scripts sit 0x14 below Ego_r.pdb: HeroFoundDeeds +0x4f (PDB 0x63), HeroFoundDeedsLocation
//   +0x50 (PDB 0x64); interface pointer +0x40. Local model: CGameScriptInterface.h carries no virtual slots.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CV_BordelloScript {
public:
    unsigned char _base[0x40];                              // CScriptBase
    CGameScriptInterface* Game;                             // +0x40
    void* Data;                                             // +0x44
    bool HeroPartying;                                      // +0x48
    bool HeroTricking;                                      // +0x49
    bool MagicianSleeping;                                  // +0x4a
    bool CutscenePlaying;                                   // +0x4b
    bool PlayerOwned;                                       // +0x4c
    bool BecomeNunnery;                                     // +0x4d
    bool PlayerHasLeftRegion;                               // +0x4e
    bool HeroFoundDeeds;                                    // +0x4f
    bool HeroFoundDeedsLocation;                            // +0x50
    bool IsActiveThreadTerminating();                       // 0x00CB7940 (CScriptBase)
    void WatchForDeedStatus();
};
} // namespace NScript
using NScript::CV_BordelloScript;
void NScript::CV_BordelloScript::WatchForDeedStatus()
{
    while (!Game->IsRegionLoaded("Bordello")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (HeroFoundDeeds) {
        IsActiveThreadTerminating();
        return;
    }
    CScriptThing deeds = Game->GetThingWithScriptName("M_HiddenDeeds");
    Game->EntitySetInLimbo(deeds, true, true);
    while (!HeroFoundDeedsLocation) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->EntitySetInLimbo(deeds, false, true);
    while (!Game->IsObjectInThingsPossession("OBJECT_DEEDS_BORDELLO", Game->GetHero())) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    HeroFoundDeeds = true;
}