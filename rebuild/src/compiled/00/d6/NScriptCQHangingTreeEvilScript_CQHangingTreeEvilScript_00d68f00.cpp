// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: test checks an unrelated local struct; CGameScriptInterface re-modelled with 87 placeholder slots)
// NScript::CQ_HangingTreeEvilScript::CQ_HangingTreeEvilScript(void* data, CGameScriptInterface* game) -- quest-script ctor:
//   script base and interface/data pair, vtable 0x012D1F98, ReasonForFailing, the four CScriptThing handles, the mission
//   timer, CurrentLevel and three empty vectors
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D68F00, `ret 8`. Identity: its factory 0x00D6ED10 (`push 0D0h`) is registered at
//   0x00CD6694 under "S_QHTE" / "Q_HangingTreeEvil". Callees: CScriptBase::CScriptBase 0x00CB8110, CCharString() 0x0099E4B0,
//   CBaseClass::CBaseClass 0x0099A2D0, g_pGameScriptInterface->RegisterTimer (slot 87).
// Leaf: ??0CQ_HangingTreeEvilScript
// Layout: PDB (Ego_r) names; retail = PDB - 0x14 (script base), and the three STLport vectors are 12 bytes (PDB 16):
//   bools +0x48..+0x58, ReasonForFailing +0x5c, HTE_State..CarryOverGuards +0x60..+0x70, Executioner +0x74, Prisoner +0x80,
//   PartySpawnPoint +0x8c, BanditExitPoint +0x98, MissionTimer +0xa4, CurrentLevel +0xa8, vectors +0xac/+0xb8/+0xc4;
//   size 0xd0. Model shared with the landed quest-script ctors (CV_ChickenKickingScript 0x00E62830).
//   Local model: engine/CGameScriptInterface.h is data-only (no virtual slots).
// CGameScriptInterface: the first 88 virtual slots of vtable 0x01260F0C, named from the slot labels (rtti-vtables.tsv +
//   manifest); only the slot position matters here, so the signatures are simplified (void, no parameters) except
//   RegisterTimer (slot 87, returns the new timer index). Overloaded names carry a _2 suffix.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct __declspec(novtable) CQuestScriptObject : CScriptBase {   // the argument-carrying intermediate base (landed model)
    CGameScriptInterface* Game;                         // +0x40 (second argument)
    void*                 Data;                         // +0x44 (first argument, the script data)
    CQuestScriptObject(void* data, CGameScriptInterface* game) : CScriptBase(), Game(game), Data(data) { }
};
class CTimer { public:                                         // a script timer: the interface's timer index
    long Index;
    CTimer() : Index(g_pGameScriptInterface->RegisterTimer()) { }
};
class CCharString { public: void* PStringData; CCharString(); };   // default ctor 0x0099E4B0
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
template<class T> class CCountedPointer { public: T* Data; void* Info; CCountedPointer() : Data(0), Info(0) { } };
class CScriptThing : public CBaseClass { public:                      // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThing> PImp;                 // +0x04
    CScriptThing() : CBaseClass(), PImp() { }
    virtual ~CScriptThing();
};
template<class T> struct CRetailVector { T* m_first; T* m_last; T* m_end; CRetailVector() : m_first(0), m_last(0), m_end(0) { } };   // STLport vector (12 bytes)
namespace NScript {
class CQ_HangingTreeEvilScript : public CQuestScriptObject {
public:
    bool IntroCutscenePlayed, BanditsStarted, BanditKilled, PartySuccessfullySetUp, MissionSucceeded, MissionFailed,
         BanditsLeftRegion, SpawnHere, AtWayPoint, SwitchingRegions, RemoveBandits, AllEnemiesDead, QuestStartScreenShown,
         BanditYelledRecently, ExecutionerInterrupted, AudienceLeggit, ShouldFollowHero;   // +0x48..+0x58
    CCharString ReasonForFailing;                       // +0x5c
    long HTE_State;                                     // +0x60
    long CurrentBandits;                                // +0x64
    long BanditsAtNextRegion;                           // +0x68
    long TimerGUIIndex;                                 // +0x6c
    long CarryOverGuards;                               // +0x70
    CScriptThing Executioner;                           // +0x74
    CScriptThing Prisoner;                              // +0x80
    CScriptThing PartySpawnPoint;                       // +0x8c
    CScriptThing BanditExitPoint;                       // +0x98
    CTimer MissionTimer;                                // +0xa4
    CCharString CurrentLevel;                           // +0xa8
    CRetailVector<CScriptThing> CurrentEnemyVector;     // +0xac
    CRetailVector<CScriptThing> CurrentBanditVector;    // +0xb8
    CRetailVector<CCharString> AreasSpawnedIn;          // +0xc4
    virtual ~CQ_HangingTreeEvilScript();
    CQ_HangingTreeEvilScript(void* data, CGameScriptInterface* game);
};
CQ_HangingTreeEvilScript::CQ_HangingTreeEvilScript(void* data, CGameScriptInterface* game)
    : CQuestScriptObject(data, game)
{
}
}