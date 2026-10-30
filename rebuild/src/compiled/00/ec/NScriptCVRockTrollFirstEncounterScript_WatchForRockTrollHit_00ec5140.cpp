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
struct CScriptThingImpl {
    virtual void i000(); virtual void i001(); virtual void i002(); virtual void i003(); virtual void i004(); virtual void i005(); virtual void i006(); virtual void i007();
    virtual void i008(); virtual void i009(); virtual void i010(); virtual void i011(); virtual void i012(); virtual void i013(); virtual void i014(); virtual void i015();
    virtual void i016(); virtual void i017(); virtual void i018(); virtual void i019(); virtual void i020();
    virtual bool MsgIsHitBy(const CCharString& attacker) const;   // slot 21 (+0x54)
    virtual void i022(); virtual void i023(); virtual void i024(); virtual void i025(); virtual void i026(); virtual void i027(); virtual void i028(); virtual void i029();
    virtual void i030(); virtual void i031(); virtual void i032(); virtual void i033(); virtual void i034(); virtual void i035(); virtual void i036(); virtual void i037();
    virtual void i038(); virtual void i039(); virtual void i040(); virtual void i041(); virtual void i042();
    virtual bool MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString& attacker) const;   // slot 43 (+0xac)
};
class CScriptThing : public CBaseClass { public:                          // vtable 0x01238C8C, 12 bytes
    CCountedPointer<CScriptThingImpl> PImp;                 // +0x04 Data, +0x08 Info
    CScriptThing(const CScriptThing& other);                // 0x004ABE90
    ~CScriptThing() {}                                      // out of line 0x004AA840
    bool MsgIsHitBy(const CCharString& attacker) const      // 0x004AAB30
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->MsgIsHitBy(attacker);
    }
    bool MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString& attacker) const   // 0x004AAD60
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->MsgIsHitByAnyAggressiveSpecialAbilityFrom(attacker);
    }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase { public:                                        // retail 0x40 bytes
    virtual void v00();
    unsigned char _body[0x3c];
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};
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