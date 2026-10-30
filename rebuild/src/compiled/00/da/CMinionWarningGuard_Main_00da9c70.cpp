// CMinionWarningGuard::Main() -- the minion-chase warning guard: alive filter on Me, take scripted control of Me (priority 4),
//   wait for BowerstoneJail to load; the first time play the "CS_MINIONCHASE_INTRO" cut-scene with the hero and Me cast; set
//   the quest card objective, kick off the quest start screen once, release Me, have Me follow the hero (region following
//   too), show Me's health bar, spawn the parent's GuardChatter on Me, wait for the mission to end and remove Me
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DA9C70, 1509 bytes, plain `ret`, no EH frame.
// Callees: CIsThingAlive out-of-line dtor 0x00E241B0, CScriptThing copy 0x004ABE90 / dtor 0x004AA840, RegisterFilter
//   0x00F35B10, IsActiveThreadTerminating 0x00F35B30, CBaseIntelligentPointer ctor 0x0099A380, scripted-thing handle
//   out-of-line ctor 0x007E72A0 / IsValid 0x00CD23B9 / Release 0x00CD2770 / dtor 0x007E74D0 / operator= 0x008ABD10, cast map
//   0x00CDBF70 / 0x00CDBFB0 / operator[] 0x00CD3D2E, movie 0x006E7B60 / 0x006E7B80, RunCutsceneMacro 0x00CBFB7D, CCharString
//   0x0099EBF0 / 0x0099EAE0 / operator+ 0x0099F570, operator new 0x00BFEA1A, CSpawnedFuncBase 0x00CDD450, AddSpawnedFunction
//   0x00CB7E50; GameCalls slots 7, 8, 12 IsRegionLoaded, 70, 108, 229, 296 SetQuestCardObjective, 309 KickOffQuestStartScreen,
//   321, 325, 370, 371, 379, 480, 655 GetActiveQuestName.
// Data: "BowerstoneJail" 0x012C7788, "HERO" 0x01255174, "GUARD" 0x012CB384, "" 0x0122D70E, "CS_MINIONCHASE_INTRO" 0x012D79A0,
//   "BowerstoneSlums" 0x012CB55C, "DesertedFarm" 0x012C7778, "TEXT_QUEST_MINION_CLIFFTOP_CHASE_OBJECTIVE_02" 0x012D7970,
//   "HUD_QUEST_ICON_GUARD" 0x012D2B54, "ParentClass." 0x012C9934, "GuardChatter" 0x012D7738.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (CQ_MinionClifftopChaseScript, retail = PDB - 0x14),
//   PMasterData +0x18, HealthBarID +0x1c.
// Shape: the landed CCorneredGuard::Main 0x00DA7830 model (same TU and parent) plus the CDF_ShineLight handle classes; unlike
//   CCorneredGuard the spawn argument Me goes through the out-of-line const copy 0x004ABE90 (no non-const inline copy ctor),
//   and the hero handle's ctor is kept out of line with `inline_depth(0)`.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
class CRGBColour { public:
    unsigned char B, G, R, A;
    __forceinline CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class CCPPointerInfo { public: long References; };
class CScriptThing;
struct CCPPointerImp {                                 // CCountedPointer<CScriptThingImp>: Data, Info
    CScriptThing*   Data;
    CCPPointerInfo* Info;
    __forceinline CCPPointerImp(const CCPPointerImp& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->References; }
};
#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
class CScriptThing { public:                                  // 12 bytes, vtable 0x01238C8C
    VPAD16(ta) VPAD4(tb) VPAD1(tc)                     // slots 0..20
    virtual bool MsgIsHitBy(const CCharString& name) const   // slot 21 (+0x54), 0x004AAB30
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->MsgIsHitBy(name);
    }
    VPAD32(td) VPAD16(te) VPAD4(tf) VPAD1(tg)          // slots 22..74
    virtual bool IsAlive() const                       // slot 75 (+0x12c), 0x004AB130
    {
        if (PImp.Data && PImp.Data->IsAlive())
            return true;
        return false;
    }
    CCPPointerImp PImp;                                // +0x04
    CScriptThing();                                    // 0x006E7B40
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    virtual ~CScriptThing();                                   // 0x004AA840
};
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
struct CCountedPointerWords { void* Data; void* Info; CCountedPointerWords() : Data(0), Info(0) { } };
class CScriptGameResourceObjectBase : public CBaseObject { public:   // vtable 0x0126008C
    CScriptGameResourceObjectBase() { }
};
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:   // vtable 0x0127094C, 0x10 bytes
    CCountedPointerWords PImp;                          // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectScriptedThingBase() { }    // out of line 0x007E72A0
    ~CScriptGameResourceObjectScriptedThingBase();      // 0x007E74D0
    bool IsValid() const;                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other);   // 0x008ABD10
};
class CScriptGameResourceObjectMovieBase : public CScriptGameResourceObjectBase { public:   // vtable 0x01260EF4, 0x10 bytes
    CCountedPointerWords PImp;                          // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectMovieBase();               // 0x006E7B60
    ~CScriptGameResourceObjectMovieBase();              // 0x006E7B80
};
struct CCutsceneCastMap {                               // map<CCharString, CScriptGameResourceObjectScriptedThingBase>
    void* _body[3];
    CCutsceneCastMap();                                 // 0x00CDBF70
    ~CCutsceneCastMap();                                // 0x00CDBFB0
    CScriptGameResourceObjectScriptedThingBase& operator[](const CCharString& key);   // 0x00CD3D2E
};
struct CCutsceneFlagMap;
struct CCutsceneArgMap;
void __fastcall RunCutsceneMacro(const CCharString& name, CCutsceneCastMap& cast, CCutsceneFlagMap* flags, CCutsceneArgMap* args, bool a5, bool a6);   // 0x00CBFB7D
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc1.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess
class CScriptBase { public:                                   // retail 0x40 bytes
    unsigned char _body[0x40];
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
};
struct CQ_MinionClifftopChaseScript : CScriptBase {
    unsigned char _pad40[0x49 - 0x40];
    bool MissionFailed;                                // +0x49
    bool MissionSucceeded;                             // +0x4a
    bool QuestStartScreened;                           // +0x4b
    bool _pad4c;
    bool ShownIntroCutscene;                           // +0x4d (descriptive)
    void GuardChatter(CScriptThing guard);             // 0x00DA71E0
};
namespace NScript {
class CPredicateBase
{
public:
    virtual void v00();
};
}
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                                 // vtable 0x012C32F0: a filter holding a script thing
    CScriptThing m_thing;                              // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;
struct CQ_SunnyvaleMasterData;
class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

class CMinionWarningGuard : public CEntityScriptBase {
public:
    void*                         __vftable;           // +0x00
    CGameScriptInterface*         Game;                // +0x04
    CScriptThing                  Me;                  // +0x08
    CQ_MinionClifftopChaseScript* ParentClass;         // +0x14
    CQ_SunnyvaleMasterData*       PMasterData;         // +0x18
    long                          HealthBarID;         // +0x1c
    void Main();
};

void CMinionWarningGuard::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase guard;
    if (guard.IsValid())
        guard.Release();
    while (!Game->StartScriptingEntity(Me, guard, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!Game->IsRegionLoaded("BowerstoneJail")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (!ParentClass->ShownIntroCutscene) {
        if (IsActiveThreadTerminating())
            return;
#pragma inline_depth(0)
        CScriptGameResourceObjectScriptedThingBase hero;
#pragma inline_depth()
        if (hero.IsValid())
            hero.Release();
        while (!Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        CCutsceneCastMap cast;
        cast["HERO"] = hero;
        cast["GUARD"] = guard;
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CGameScriptInterface* game = Game;
        game->PauseAllNonScriptedEntities(true);
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro("CS_MINIONCHASE_INTRO", cast, 0, 0, false, true);
        Game->FixMovieSequenceCamera(false);
        ParentClass->ShownIntroCutscene = true;
        game->PauseAllNonScriptedEntities(false);
    }
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_MINION_CLIFFTOP_CHASE_OBJECTIVE_02", "DesertedFarm", "BowerstoneSlums");
    if (!ParentClass->QuestStartScreened) {
        if (IsActiveThreadTerminating())
            return;
        Game->KickOffQuestStartScreen(Game->GetActiveQuestName(), true, false);
        ParentClass->QuestStartScreened = true;
    }
    guard.Release();
    Game->EntityFollowThing(Me, Game->GetHero(), 1.0f, true);
    Game->SetEntityAsRegionFollowing(Game->GetHero(), Me, true);
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->DisplayQuestInfo(true);
    HealthBarID = Game->AddQuestInfoBarHealth(Me, CRGBColour(0, 255, 0, 255), "HUD_QUEST_ICON_GUARD", 1.0f);
    ParentClass->AddSpawnedFunction(new CSpawnedFunc1<CQ_MinionClifftopChaseScript, CScriptThing>(CCharString("ParentClass.") + CCharString("GuardChatter"), 0, ParentClass, &CQ_MinionClifftopChaseScript::GuardChatter, Me), CCharString(""));
    while (!ParentClass->MissionFailed && !ParentClass->MissionSucceeded) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->RemoveThing(Me, false, true);
}