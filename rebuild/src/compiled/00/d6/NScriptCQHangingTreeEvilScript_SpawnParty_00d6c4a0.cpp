// CQ_HangingTreeEvilScript::SpawnParty(long, const CScriptThing&) -- once the bandits have left the region, create the given number of friendly bandit grunts at the spawn point (allied both ways with the hero, in the friendly bandit faction, with a health bar and a green minimap orb), then start the exit, region and follow watcher threads
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D6C4A0, length 909 bytes (`ret 8`); called from 0x00D6C1F1.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString(const char*, long) 0x0099EBF0 / ~CCharString
//   0x0099EAE0, operator delete 0x00BFE9BC / ~CBaseClass 0x0099A2E0 (inline ~CScriptThing), operator new 0x00BFEA1A,
//   CSpawnedFuncBase::CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc vtable 0x012D1FE8), CScriptBase::AddSpawnedFunction
//   0x00CB7E50, WatchForHeroNearExit 0x00D6CA90, MonitorBanditRegion 0x00D6CCC0, CheckBanditFollowStatus 0x00D6C830;
//   CScriptThing::GetPos (slot 6); through the interface (+0x40): NewScriptFrame (7), GetHero (70),
//   CreateCreatureNearby (92), AddQuestInfoBarHealth (325), MiniMapAddMarker (350), EntitySetThingAsAllyOfThing (599),
//   EntitySetInFaction (601).
// Strings: "HTE_FreedomFighter" 0x012D2050, "CREATURE_BANDIT_GRUNT" 0x012D23A0, "FACTION_BANDITS_FRIENDLY" 0x012D218C,
//   "HUD_QUEST_ICON_BANDIT" 0x012C88C8, "HUD_ORB_GREEN_SMALL" 0x012C7F50, "WatchForHeroNearExit" 0x012D2388,
//   "MonitorBanditRegion" 0x012D2374, "CheckBanditFollowStatus" 0x012D235C, "" 0x0122D70E.
// Layout (retail = Ego_r.pdb - 0x14): BanditsLeftRegion +0x4e.
// Shape: spawn model of CQ_MinionCampScript::PhaseOne 0x00DA05D0 (flag word guarding the name temporaries);
//   CScriptThing model of the landed CQ_HangingTreeGoodScript::ReassignLeader 0x00D74F80 (inline destructor).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
class C3DVector { public: float X, Y, Z; };
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() {}
class CRGBColour { public:                                     // D3DCOLOR byte order
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_HangingTreeEvilScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void*                 Data;                         // +0x44
    unsigned char         _pad48[0x4e - 0x48];
    bool                  BanditsLeftRegion;            // +0x4e
    void SpawnParty(long count, const CScriptThing& spawnPoint);
    void CheckBanditFollowStatus();                     // 0x00D6C830
    void WatchForHeroNearExit();                        // 0x00D6CA90
    void MonitorBanditRegion();                         // 0x00D6CCC0
};
void CQ_HangingTreeEvilScript::SpawnParty(long count, const CScriptThing& spawnPoint)
{
    if (count > 0) {
        while (!BanditsLeftRegion) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
    }
    for (long i = 0; i < count; ++i) {
        if (IsActiveThreadTerminating())
            return;
        CScriptThing bandit = Game->CreateCreatureNearby("CREATURE_BANDIT_GRUNT", spawnPoint.GetPos(), 3.0f, "HTE_FreedomFighter", false);
        Game->EntitySetThingAsAllyOfThing(bandit, Game->GetHero());
        Game->EntitySetThingAsAllyOfThing(Game->GetHero(), bandit);
        Game->EntitySetInFaction(bandit, "FACTION_BANDITS_FRIENDLY");
        Game->AddQuestInfoBarHealth(bandit, CRGBColour(255, 255, 255, 255), "HUD_QUEST_ICON_BANDIT", 1.0f);
        Game->MiniMapAddMarker(bandit, "HUD_ORB_GREEN_SMALL");
    }
    if (IsActiveThreadTerminating())
        return;
    AddSpawnedFunction(new CSpawnedFunc<CQ_HangingTreeEvilScript>("WatchForHeroNearExit", 0, this, &CQ_HangingTreeEvilScript::WatchForHeroNearExit), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_HangingTreeEvilScript>("MonitorBanditRegion", 0, this, &CQ_HangingTreeEvilScript::MonitorBanditRegion), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_HangingTreeEvilScript>("CheckBanditFollowStatus", 0, this, &CQ_HangingTreeEvilScript::CheckBanditFollowStatus), CCharString(""));
}
}