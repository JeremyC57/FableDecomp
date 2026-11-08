// NScript::CQ_HeroSoulsGuildmasterScript::CGuildmaster_HS::Main() -- the Hero Souls guildmaster: after the guildmaster cutscene, target the hero, become unpushable, and while guards remain in the woods speak one of six comments/taunts (one every 18 timer ticks, at most six) whenever the hero comes within 15 units; then heal back to full health and idle
// Leaf: ?Main@CGuildmaster_HS@CQ_HeroSoulsGuildmasterScript@NScript@@EAEXXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Naming wave 18: the global stand-in g_pGameScriptInterface (0x0143E8F8) is pDSTGame, a CGameScriptInterfaceBase* (ESTABLISHED_SPELLINGS; the
//   virtual slots are the base interface's) -- name-only (the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D85B40, 1085 bytes (+ jump table and int3 padding), plain `ret`; entity-script vtable slot 1 (Main).
// Callees: CBaseIntelligentPointer ctor 0x0099A380, scripted-thing handle dtor 0x007E74D0, CScriptThing copy 0x004ABE90 / dtor
//   0x004AA840 (and the inline copy built in the SetIsPushableByHero argument slot), RegisterFilter 0x00F35B10,
//   IsActiveThreadTerminating 0x00F35B30, CTimer dtor 0x00CD4470 (inline copies elsewhere), IsDistanceBetweenThingsUnder
//   0x00CBE2FF (__fastcall), CCharString 0x0099EBF0 / 0x0099EAE0; GameCalls slots 7, 70 GetHero, 87-90 timers (through
//   pDSTGame 0x0143E8F8), 264 GetHealth, 265 ModifyThingHealth, 364 AddNewConversation, 365 AddPersonToConversation,
//   366 AddLineToConversation, 451 GiveThingBestEnemyTarget, 745 SetIsThingForcePushable, 844 SetIsPushableByHero.
// Layout: Ego_r.pdb CGuildmaster_HS (28 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14
//   (CQ_HeroSoulsGuildmasterScript: PlayedGuildMasterCS PDB +0x5d = retail +0x49, GuardInWoodsCount PDB +0x64 = retail +0x50),
//   PMasterData +0x18. CTimer as the landed CHobbeLeader::Main 0x00D8FA30; the by-value CScriptThing copy as the landed
//   CHSA_ScreamingVillager::Main 0x00D7F880 (InlineCopy tag).
// Shape: the scripted-thing handle is declared but never used (only its constructor and destructor appear); the comment
//   switch has its own CCharString temporary per case and a default arm that repeats case 0.
// Naming wave 35: identity reg 00d84db3 "Guildmaster_HS" in 00d84d60; Alloc 00d85a30; vtable 012d4a98 slot 1; PDB ?Main@CGuildmaster_HS@CQ_HeroSoulsGuildmasterScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was under-qualified CGuildmaster_HS::Main (wave3/labels_alloc.tsv); PDB ?Main@CGuildmaster_HS@CQ_HeroSoulsGuildmasterScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Main@CGuildmaster_HS@@QAEXXZ`; this source now defines the PDB member (outer stand-in CQ_HeroSoulsGuildmasterScript moved into NScript); same bytes.
#include "fable_compiler.h"
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
struct CScriptThingImpl;
struct CBase {                                          // vtable 0x01231710
    CBase() { }                                         // out-of-line copy 0x0099A2D0
    virtual ~CBase();                                   // 0x0099A2E0
};
#include "engine/C3DVector.h"
class CScriptThing : public CBase { public:                           // vtable 0x01238C8C, 12 bytes
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    struct InlineCopy { };                              // tag: the inline copy retail expands at by-value call sites
    CScriptThing(const CScriptThing& other, InlineCopy) : CBase(), PImp(other.PImp) { }
    ~CScriptThing();                                    // 0x004AA840
};
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CScriptGameResourceObjectBase : public CBaseObject { public:   // vtable 0x0126008C
    CScriptGameResourceObjectBase() { }
    ~CScriptGameResourceObjectBase() { }
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
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer::CTimer() { TimerIndex = pDSTGame->RegisterTimer(); }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
namespace NScript {
class CQ_HeroSoulsGuildmasterScript {
public:
    unsigned char _pad[0x49];
    bool PlayedGuildMasterCS;                           // +0x49 (PDB +0x5d)
    unsigned char _pad2[0x50 - 0x4a];
    long GuardInWoodsCount;                             // +0x50 (PDB +0x64)
public:
    class CGuildmaster_HS;           // the entity class below (naming wave 35)
};
}
using NScript::CQ_HeroSoulsGuildmasterScript;
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
namespace NScript {
class CQ_HeroSoulsGuildmasterScript::CGuildmaster_HS : public CEntityScriptBase {
public:
    CQ_HeroSoulsGuildmasterScript* ParentClass;         // +0x14
    void*                          PMasterData;         // +0x18
private:
    virtual void Main();                          // PDB ?Main@CGuildmaster_HS@CQ_HeroSoulsGuildmasterScript@NScript@@EAEXXZ
};
}

void NScript::CQ_HeroSoulsGuildmasterScript::CGuildmaster_HS::Main()
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
            commentTimer = 18;
        }
    }
    if (IsActiveThreadTerminating())
        {
            return;
        }
    Game->ModifyThingHealth(Me, 1.0f - Game->GetHealth(Me), false);
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}