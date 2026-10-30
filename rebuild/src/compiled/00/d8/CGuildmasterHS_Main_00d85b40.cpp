// CGuildmaster_HS::Main() -- the Hero Souls guildmaster: after the guildmaster cutscene, target the hero, become unpushable, and while guards remain in the woods speak one of six comments/taunts (one every 18 timer ticks, at most six) whenever the hero comes within 15 units; then heal back to full health and idle
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D85B40, 1085 bytes (+ jump table and int3 padding), plain `ret`; entity-script vtable slot 1 (Main).
// Callees: CBaseIntelligentPointer ctor 0x0099A380, scripted-thing handle dtor 0x007E74D0, CScriptThing copy 0x004ABE90 / dtor
//   0x004AA840 (and the inline copy built in the SetIsPushableByHero argument slot), RegisterFilter 0x00F35B10,
//   IsActiveThreadTerminating 0x00F35B30, CTimer dtor 0x00CD4470 (inline copies elsewhere), IsDistanceBetweenThingsUnder
//   0x00CBE2FF (__fastcall), CCharString 0x0099EBF0 / 0x0099EAE0; GameCalls slots 7, 70 GetHero, 87-90 timers (through
//   g_pGameScriptInterface 0x0143E8F8), 264 GetHealth, 265 ModifyThingHealth, 364 AddNewConversation, 365 AddPersonToConversation,
//   366 AddLineToConversation, 451 GiveThingBestEnemyTarget, 745 SetIsThingForcePushable, 844 SetIsPushableByHero.
// Layout: Ego_r.pdb CGuildmaster_HS (28 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14
//   (CQ_HeroSoulsGuildmasterScript: PlayedGuildMasterCS PDB +0x5d = retail +0x49, GuardInWoodsCount PDB +0x64 = retail +0x50),
//   PMasterData +0x18. CTimer as the landed CHobbeLeader::Main 0x00D8FA30; the by-value CScriptThing copy as the landed
//   CHSA_ScreamingVillager::Main 0x00D7F880 (InlineCopy tag).
// Shape: the scripted-thing handle is declared but never used (only its constructor and destructor appear); the comment
//   switch has its own CCharString temporary per case and a default arm that repeats case 0.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    __forceinline ~CCountedPointer()
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
struct CScriptThingImpl;
struct CBase {                                          // vtable 0x01231710
    CBase() { }                                         // out-of-line copy 0x0099A2D0
    virtual ~CBase();                                   // 0x0099A2E0
};
class C3DVector { public: float X, Y, Z; };
class CScriptThing : public CBase { public:                           // vtable 0x01238C8C, 12 bytes
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    struct InlineCopy { };                              // tag: the inline copy retail expands at by-value call sites
    CScriptThing(const CScriptThing& other, InlineCopy) : CBase(), PImp(other.PImp) { }
    ~CScriptThing();                                    // 0x004AA840
};
class CBaseObject { public:                        // vtable 0x0129A7D8
    void* m_pIntelligentPointer;                        // +0x04
protected:
    CBaseObject();                                       // 0x0099A380
    virtual ~CBaseObject();                              // 0x0099A430
};
class CScriptGameResourceObjectBase : public CBaseObject { public:   // vtable 0x0126008C
    CScriptGameResourceObjectBase() { }
    __forceinline ~CScriptGameResourceObjectBase() { }
};
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_RANDOM };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
namespace NScript {
class CPredicateBase
{
public:
    virtual void v00();
};
}
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                                  // vtable 0x012C32F0
    CScriptThing m_thing;                               // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:   // vtable 0x0127094C, 0x10 bytes
    CCountedPointer<void> PImp;                         // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectScriptedThingBase() { }
    ~CScriptGameResourceObjectScriptedThingBase();      // 0x007E74D0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8
class CTimer { public:                                         // 4 bytes: an index into the global script timers
    long TimerIndex;
    CTimer() { TimerIndex = g_pGameScriptInterface->RegisterTimer(); }
    ~CTimer() { g_pGameScriptInterface->DeregisterTimer(TimerIndex); }   // out-of-line copy 0x00CD4470
    __forceinline void SetTime(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); }
    __forceinline long GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
struct CQ_HeroSoulsGuildmasterScript {
    unsigned char _pad[0x49];
    bool PlayedGuildMasterCS;                           // +0x49 (PDB +0x5d)
    unsigned char _pad2[0x50 - 0x4a];
    long GuardInWoodsCount;                             // +0x50 (PDB +0x64)
};
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
struct CGuildmaster_HS : CEntityScriptBase {
    CQ_HeroSoulsGuildmasterScript* ParentClass;         // +0x14
    void*                          PMasterData;         // +0x18
    void Main();
};

void CGuildmaster_HS::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!ParentClass->PlayedGuildMasterCS) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase guildmaster;
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    Game->SetIsPushableByHero(CScriptThing(Me, CScriptThing::InlineCopy()), false);
    Game->SetIsThingForcePushable(Me, false);
    CTimer commentTimer;
    long comments = 0;
    while (ParentClass->GuardInWoodsCount > 0) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (comments < 6 && commentTimer.GetTime() == 0 && IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 15.0f)) {
            if (IsActiveThreadTerminating())
                return;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            switch (comments) {
            case 0:
                Game->AddLineToConversation(conversation, "TEXT_QST_B04_GM_COMMENTS_10", false, Me, Game->GetHero());
                break;
            case 1:
                Game->AddLineToConversation(conversation, "TEXT_QST_B04_GM_COMMENTS_20", false, Me, Game->GetHero());
                break;
            case 2:
                Game->AddLineToConversation(conversation, "TEXT_QST_B04_GM_COMMENTS_30", false, Me, Game->GetHero());
                break;
            case 3:
                Game->AddLineToConversation(conversation, "TEXT_QST_B04_SOUL3_GM_BATTLE_TAUNTS_10", false, Me, Game->GetHero());
                break;
            case 4:
                Game->AddLineToConversation(conversation, "TEXT_QST_B04_SOUL3_GM_BATTLE_TAUNTS_20", false, Me, Game->GetHero());
                break;
            case 5:
                Game->AddLineToConversation(conversation, "TEXT_QST_B04_SOUL3_GM_BATTLE_TAUNTS_30", false, Me, Game->GetHero());
                break;
            default:
                Game->AddLineToConversation(conversation, "TEXT_QST_B04_GM_COMMENTS_10", false, Me, Game->GetHero());
                break;
            }
            ++comments;
            commentTimer.SetTime(18);
        }
    }
    if (IsActiveThreadTerminating())
        {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
    Game->ModifyThingHealth(Me, 1.0f - Game->GetHealth(Me), false);
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
#pragma inline_depth(0)
}
#pragma inline_depth()