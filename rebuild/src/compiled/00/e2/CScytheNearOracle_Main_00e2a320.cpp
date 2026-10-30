// CScytheNearOracle::Main() -- Scythe waiting by the Oracle: take Me under scripted control, add the quest-core minimap marker and face the MK_OW_SCYTHE3 marker; then every frame: when the hero comes within 20 units and the 2-second timer has run out, face the hero; within 12 units, play CS_ORACLE_AWAKENS inside a paused movie sequence (Me cast as "SCYTHE"), flag the parent and remove Me; when the hero hits Me, speak the on-hit line and make Me and the hero allies
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E2A320, 1503 bytes (+ int3 padding), plain `ret`; entity-script vtable slot 1 (Main).
// Literals "HUD_ORB_QUEST_CORE" 0x01258AB0, "MK_OW_SCYTHE3" 0x012C7D28, "" 0x0122D70E, "SCYTHE" 0x012CC544,
//   "CS_ORACLE_AWAKENS" 0x012E2414, "SCRIPT_NAME_HERO" 0x0125D1C8, "TEXT_QST_B03_SCYTHE_ON_HIT_RETURNED" 0x012E23F0.
// Callees: CScriptThing copy 0x004ABE90 / dtor 0x004AA840, ~CIsThingAlive 0x00E241B0, RegisterFilter 0x00F35B10,
//   IsActiveThreadTerminating 0x00F35B30, CBaseIntelligentPointer 0x0099A380 / 0x0099A430, handle IsScripting 0x00CD23B9 /
//   Release 0x00CD2770 / GetScriptThing 0x007E7490 / Speak 0x007E7390 / IsPerformingScriptTask 0x007E7450 / operator=
//   0x008ABD10 / dtor 0x007E74D0, movie handle dtor 0x006E7B80 (counted-pointer member dtor 0x006E7AB0), CTimer dtor 0x00CD4470,
//   cast map operator[] 0x00CD3D2E, CQS_ScytheInfoScript::PlayCutscene 0x00E2A900, IsDistanceBetweenThingsUnder 0x00CBE2FF;
//   GameCalls slots 7, 8, 70, 72, 87-90 timers (through g_pGameScriptInterface 0x0143E8F8), 108, 264, 350, 352, 370, 379, 475, 599.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (CQS_ScytheInfoScript: oracle-awakened flag +0x48,
//   cutscene cast map +0x58).
// Shape: the CEndScythe (0x00D79D50) handle model; __forceinline CTimer ctor/dtor/accessors keep the timer index in ebp (home
//   slot) across the loop, inline_depth(0) in the hit block keeps the movie and CTimer destructors out of line there as retail does,
//   and the main loop is `for (;;) { if (IsActiveThreadTerminating()) return; ... }` so the loop exit keeps its own ~CTimer copy.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
class C3DVector;
struct CScriptThingImpl;
struct CBase {                                          // vtable 0x01231710
    CBase() { }
    virtual ~CBase();                                   // 0x0099A2E0
};
class CScriptThing : public CBase { public:
    virtual void s001() const; virtual void s002() const; virtual void s003() const; virtual void s004() const; virtual void s005() const; virtual void s006() const; virtual void s007() const; virtual void s008() const; virtual void s009() const; virtual void s010() const;
    virtual void s011() const; virtual void s012() const; virtual void s013() const; virtual void s014() const; virtual void s015() const; virtual void s016() const; virtual void s017() const; virtual void s018() const; virtual void s019() const; virtual void s020() const;
    virtual bool MsgIsHitBy(const CCharString& name) const;   // slot 21 (+0x54)
    virtual void s022() const; virtual void s023() const; virtual void s024() const; virtual void s025() const; virtual void s026() const; virtual void s027() const; virtual void s028() const; virtual void s029() const; virtual void s030() const; virtual void s031() const;
    virtual void s032() const; virtual void s033() const; virtual void s034() const; virtual void s035() const; virtual void s036() const; virtual void s037() const; virtual void s038() const; virtual void s039() const; virtual void s040() const;
    virtual bool MsgIsHitBySpecialAbilityFrom(long ability, const CCharString& name) const;   // slot 41 (+0xa4)
    virtual bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& name) const;   // slot 42 (+0xa8)
    void* PImpData;                                     // +0x04
    void* PImpInfo;                                     // +0x08
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    virtual ~CScriptThing();                                    // 0x004AA840
};
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
    virtual ~CIsThingAlive();                                   // 0x00E241B0
};
}
using NScript::CIsThingAlive;
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_RANDOM };
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CScriptGameResourceObjectBase : public CBaseObject { public:   // vtable 0x0126008C
    CScriptGameResourceObjectBase() { }
    __forceinline ~CScriptGameResourceObjectBase() { }
};
struct CResourcePImp {                                  // the handles' counted implementation pointer
    void* Data;
    void* Info;
    CResourcePImp() : Data(0), Info(0) { }
    ~CResourcePImp();                                   // 0x006E7AB0
};
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:   // vtable 0x0127094C, 0x10 bytes
    void* PImpData;                                     // +0x08
    void* PImpInfo;                                     // +0x0c
    CScriptGameResourceObjectScriptedThingBase() { PImpData = 0; PImpInfo = 0; }
    ~CScriptGameResourceObjectScriptedThingBase();      // 0x007E74D0
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other);   // 0x008ABD10
    bool IsScripting();                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
    void Speak(const CScriptThing& listener, const char* text, ETextGroupSelectionMethod method, bool a, bool b, bool c);   // 0x007E7390
public:
    virtual CScriptThing GetScriptThing() const;   // 0x007E7490
    virtual bool IsPerformingScriptTask() const;   // 0x007E7450
};
class CScriptGameResourceObjectMovieBase : public CScriptGameResourceObjectBase { public:   // vtable 0x01260EF4, 0x10 bytes
    CResourcePImp PImp;                                 // +0x08
    CScriptGameResourceObjectMovieBase() { }
    // implicit destructor; out-of-line copy 0x006E7B80
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8
class CTimer { public:                                         // 4 bytes: an index into the global script timers
    long TimerIndex;
    __forceinline CTimer() { TimerIndex = g_pGameScriptInterface->RegisterTimer(); }
    __forceinline ~CTimer() { g_pGameScriptInterface->DeregisterTimer(TimerIndex); }   // out-of-line copy 0x00CD4470
    __forceinline void SetTime(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); }
    __forceinline long GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
extern const float g_fZero;                            // 0x0122DEDC (0.0f)
struct CCutsceneCastMap {
    unsigned char _rep[12];
    CScriptGameResourceObjectScriptedThingBase& operator[](const CCharString& key);   // 0x00CD3D2E
};
struct CQS_ScytheInfoScript {
    unsigned char _pad00[0x48];
    bool OracleAwakened;                                // +0x48
    unsigned char _pad49[0x58 - 0x49];
    CCutsceneCastMap Cast;                              // +0x58
    void PlayCutscene(CCharString name, bool a);        // 0x00E2A900
};
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
struct CScytheNearOracle : CEntityScriptBase {
    CQS_ScytheInfoScript* ParentClass;                  // +0x14
    void*                 PMasterData;                  // +0x18
    void Main();
};

void CScytheNearOracle::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase face;
    if (face.IsScripting())
        face.Release();
    while (!Game->StartScriptingEntity(Me, face, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->MiniMapAddMarker(Me, "HUD_ORB_QUEST_CORE");
    Game->EntitySetFacingAngleTowardsThing(Me, Game->GetThingWithScriptName("MK_OW_SCYTHE3"), false);
    CTimer timer;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 20.0f) && timer.GetTime() == 0) {
            if (IsActiveThreadTerminating())
                return;
            Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
            timer.SetTime(2);
        }
        if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 12.0f)) {
            if (IsActiveThreadTerminating())
                return;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CGameScriptInterface* game = Game;
            game->PauseAllNonScriptedEntities(true);
            Game->MiniMapRemoveMarker(Me);
            ParentClass->Cast["SCYTHE"] = face;
            ParentClass->PlayCutscene("CS_ORACLE_AWAKENS", false);
            ParentClass->OracleAwakened = true;
            Game->RemoveThing(Me, false, true);
            game->PauseAllNonScriptedEntities(false);
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(14, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                return;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CGameScriptInterface* game = Game;
            game->PauseAllNonScriptedEntities(true);
            if (Game->GetHealth(face.GetScriptThing()) > g_fZero) {
                face.Speak(Game->GetHero(), "TEXT_QST_B03_SCYTHE_ON_HIT_RETURNED", (ETextGroupSelectionMethod)2, false, true, false);
                while (face.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) {
                        game->PauseAllNonScriptedEntities(false);
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                }
                if (IsActiveThreadTerminating()) {
                    game->PauseAllNonScriptedEntities(false);
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
            }
            Game->EntitySetThingAsAllyOfThing(Me, Game->GetHero());
            Game->EntitySetThingAsAllyOfThing(Game->GetHero(), Me);
            game->PauseAllNonScriptedEntities(false);
#pragma inline_depth(0)
        }
#pragma inline_depth()
        Game->NewScriptFrame();
    }
}