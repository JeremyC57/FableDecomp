// CV_RockTrollFirstEncounterScript::WatchForRockTrollHit(CScriptThing) -- wait until the hero hits the rock troll (melee or an aggressive special ability), then, if the troll still has health left, show its red quest-info health bar and turn the quest info on
// Leaf: ?WatchForRockTrollHit@CV_RockTrollFirstEncounterScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EC5140, 421 bytes (432 with padding), `ret 0Ch` (the by-value CScriptThing).
// Label: manifest "CQ_AmbushTradersScript::WatchForHeroAttackOnBandit" is propagated; the only reference is the
//   CSpawnedFunc<CV_RockTrollFirstEncounterScript> store at 0x00EC4E51 next to the name "WatchForRockTrollHit"
//   (0x012F1774), and the health-bar ID is the class's RockTrollHealthBarID.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 ("SCRIPT_NAME_HERO" 0x0125D1C8,
//   "HUD_QUEST_ICON_ROCK_TROLL" 0x012D75D4), CScriptBase::IsActiveThreadTerminating 0x00CB7940, ~CBaseClass
//   0x0099A2E0, operator delete 0x00BFE9BC, ~CScriptThing 0x004AA840 (out of line); through the interface (+0x40)
//   NewScriptFrame (slot 7), GetHealth (slot 264, +0x420), DisplayQuestInfo (slot 321, +0x504),
//   AddQuestInfoBarHealth (slot 325, +0x514); the thing's implementation slots 21 MsgIsHitBy (+0x54) and 43
//   MsgIsHitByAnyAggressiveSpecialAbilityFrom (+0xac) behind the inlined null-safe wrappers. Float 0.0001f 0x0129BA3C.
// Shape: as the landed sibling WatchForRockTrollKilled 0x00EC5010: the `||` of two hit tests builds each hero-name
//   temporary under a construction flag (ebx |1, |2); the by-value parameter's inline destructor is expanded at the
//   first exit (the yield's return) and the out-of-line ~CScriptThing called at the others.
// Layout (retail = Ego_r.pdb - 0x14): RockTrollHealthBarID +0x50.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
void operator delete(void* p);                              // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                                     // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);              // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() {}
inline bool CScriptThing::MsgIsHitBy(const CCharString& attacker) const { if (!PImp.Data) return false; return PImp.Data->MsgIsHitBy(attacker); }
inline bool CScriptThing::MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString& attacker) const { if (!PImp.Data) return false; return PImp.Data->MsgIsHitByAnyAggressiveSpecialAbilityFrom(attacker); }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* Game;                             // +0x40
    void*                 Data;                             // +0x44
};
namespace NScript {
class CV_RockTrollFirstEncounterScript : public CQuestScriptBase {
public:
    unsigned char _pad48[2];                                // +0x48 MissionSucceeded, MissionFailed
    bool          MissionOver;                              // +0x4a
    bool          TrollAlive;                               // +0x4b
    bool          TrollAwake;                               // +0x4c
    bool          Activated;                                // +0x4d
    bool          RockTrollTriggered;                       // +0x4e
    long          RockTrollHealthBarID;                     // +0x50
    void WatchForRockTrollHit(CScriptThing troll);
};
} // namespace NScript
using NScript::CV_RockTrollFirstEncounterScript;

void NScript::CV_RockTrollFirstEncounterScript::WatchForRockTrollHit(CScriptThing troll)
{
    while (!(troll.MsgIsHitBy("SCRIPT_NAME_HERO") || troll.MsgIsHitByAnyAggressiveSpecialAbilityFrom("SCRIPT_NAME_HERO"))) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (Game->GetHealth(troll) > 0.0001f) {
        if (IsActiveThreadTerminating())
            return;
        {
            CCharString icon("HUD_QUEST_ICON_ROCK_TROLL");
            RockTrollHealthBarID = Game->AddQuestInfoBarHealth(troll, CRGBColour(255, 0, 0, 255), icon, 1.0f);
        }
        Game->DisplayQuestInfo(true);
    }
}