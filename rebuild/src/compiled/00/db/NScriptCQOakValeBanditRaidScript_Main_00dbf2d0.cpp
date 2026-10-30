// CQ_OakValeBanditRaidScript::Main() -- wait for the bandit-camp path, set the objective, make the raid gate persistent, play the gate-start cut-scene with the hero cast, show the start screen, bind the raid bandits and restart marker, add the raid rumours and music, populate the level, spawn the level/gate/whistling watchers, show the raid tutorial once and idle until the thread ends
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DBF2D0, 1785 bytes (int3 padded to 1792).
// Callees: CCharString 0x0099EBF0 / 0x0099EC30 / ~ 0x0099EAE0, ~CScriptThing 0x004AA840, IsActiveThreadTerminating 0x00CB7940,
//   scripted-thing handle (CBaseObject 0x0099A380, vtable 0x0127094C, IsValid 0x00CD23B9, Release 0x00CD2770, operator=
//   0x008ABD10, dtor 0x007E74D0), movie handle (vtable 0x01260EF4, PImp dtor 0x006E7AB0, ~CBaseObject 0x0099A430), cast map
//   0x00CDBF70 / 0x00CDBFB0 / operator[] 0x00CD3D2E, RunCutsceneMacro 0x00CBFB7D (__fastcall), operator new 0x00BFEA1A,
//   AddEntityScriptBinding 0x00CB8230 (binding vtable 0x012D9EC0; allocators 0x00DBFDD0 / 0x00DC0090),
//   PostAddScriptedEntities 0x00CB8930, CSpawnedFuncBase 0x00CDD450 (spawn vtable 0x012D9E2C), AddSpawnedFunction 0x00CB7E50,
//   PopulateLevel 0x00DC0300; threads WatchForLevelChanges 0x00DC0FF0, WatchForGateTimer 0x00DC08C0, WatchForWhistling
//   0x00DBF9E0; CGameScriptInterface slots 7, 8, 12, 41, 64, 70, 72, 118, 296, 309, 370, 379, 477, 696, 717, 718, 720.
// Shape: the cut-scene block of the landed CQ_HeroSoulsBriarScript::PlayCutscene 0x00D823A0 (inline movie dtor, out-of-line
//   handle dtor) with the out-of-line cast map; the idle tail is `for(;;){NewScriptFrame(); if(IsActiveThreadTerminating()) return;}` with a trailing terminate check after the tutorial wait (VC tail-merges it into the loop test); the five frame waits are written out (retail unrolls them).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const char* text);           // 0x0099EFE0
};
class CWideString { public:
    void* PStringData;
    CWideString(const unsigned short* text);            // 0x0099B6B0
    ~CWideString();                                     // 0x0099B510
};
class CScriptThing { public:                                   // 12 bytes {vptr, PImp.Data, PImp.Info}
    void* VTable;
    void* PImpData;
    void* PImpInfo;
    ~CScriptThing();                                    // 0x004AA840
};
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess

class CScriptThing;
class CScriptDataBase;
class CEntityScriptBase;
class CActiveEntityScriptBase;
#include "engine/CCountedPointer.h"

class __declspec(novtable) CEntityScriptBindingBase { public:
    virtual const CCharString& GetEntityScriptName() const = 0;
};
template <class T> struct CEntityScriptBinding : CEntityScriptBindingBase {
    typedef CCountedPointer<CEntityScriptBase> (__fastcall *TAllocFunc)(T& parent, CScriptDataBase* data, const CScriptThing& thing);
    CCharString     EntityScriptName;                   // +0x04
    T*              PParentScript;                      // +0x08
    void (T::*Func)();                                  // +0x0c
    TAllocFunc      ClassAllocFunc;                     // +0x10
    bool            ClassAllocValid;                    // +0x14
    unsigned long   Flags;                              // +0x18
    __forceinline CEntityScriptBinding(const CCharString& name, T* parent, TAllocFunc alloc, unsigned long flags)
        : EntityScriptName(name), PParentScript(parent), ClassAllocFunc(alloc), ClassAllocValid(true), Flags(flags) {}
    virtual const CCharString& GetEntityScriptName() const { return EntityScriptName; }
};

#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CScriptGameResourceObjectBase : public CBaseObject { public:    // vtable 0x0126008C
    virtual ~CScriptGameResourceObjectBase() {}
};
struct CCountedPointerWords { void* Data; void* Info; CCountedPointerWords() : Data(0), Info(0) { } };
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:   // vtable 0x0127094C
    CCountedPointerWords PImp;
    virtual ~CScriptGameResourceObjectScriptedThingBase();   // 0x007E74D0
    bool IsValid() const;                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other);   // 0x008ABD10
};
struct CMovieImpPointer {
    void* Data;
    void* Info;
    CMovieImpPointer() : Data(0), Info(0) {}
    ~CMovieImpPointer();                                // 0x006E7AB0
};
class CScriptGameResourceObjectMovieBase : public CScriptGameResourceObjectBase { public:   // vtable 0x01260EF4
    CMovieImpPointer PImp;
};
struct CCutsceneCastMap {                               // map<CCharString, CScriptGameResourceObjectScriptedThingBase>
    void* _body[3];
    void Construct();                                   // 0x00CDBF70 (out-of-line ctor)
    void Destroy();                                     // 0x00CDBFB0 (out-of-line dtor)
    CScriptGameResourceObjectScriptedThingBase& operator[](const CCharString& key);   // 0x00CD3D2E
};
struct CCutsceneFlagMap;
struct CCutsceneArgMap;
void __fastcall RunCutsceneMacro(const CCharString& name, CCutsceneCastMap& cast, CCutsceneFlagMap* flags, CCutsceneArgMap* args, bool a5, bool a6);   // 0x00CBFB7D
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_OakValeBanditRaidScript : public CScriptBase {
public:
    struct COakValeBanditRaidBandit { static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_OakValeBanditRaidScript& parent, CScriptDataBase* data, const CScriptThing& thing); };   // 0x00DBFDD0
    struct CRestartMarker { static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_OakValeBanditRaidScript& parent, CScriptDataBase* data, const CScriptThing& thing); };             // 0x00DC0090
    CGameScriptInterface* Game;                         // +0x40
    void Main();
    bool PopulateLevel();                               // 0x00DC0300
    void WatchForLevelChanges();                        // 0x00DC0FF0
    void WatchForGateTimer();                           // 0x00DC08C0
    void WatchForWhistling();                           // 0x00DBF9E0
};
} // namespace NScript
using NScript::CQ_OakValeBanditRaidScript;

void NScript::CQ_OakValeBanditRaidScript::Main()
{
    while (!Game->IsRegionLoaded("BanditCampPathEntrance")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetQuestCardObjective("Q_BanditCamp", "TEXT_QUEST_BANDIT_CAMP_OBJECTIVE_02", "", "OakBay");
    CScriptThing gate = Game->GetThingWithScriptName("OakValeBanditRaidGate");
    Game->SetThingPersistent(gate, true);
    {
        CScriptGameResourceObjectScriptedThingBase hero;
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
        cast.Construct();
        cast["HERO"] = hero;
        {
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CGameScriptInterface* game = Game;
            game->PauseAllNonScriptedEntities(true);
            RunCutsceneMacro("CS_BANDITRAID_GATESTART", cast, 0, 0, false, true);
            game->PauseAllNonScriptedEntities(false);
        }
        cast.Destroy();
    }
    Game->KickOffQuestStartScreen("Q_BanditCamp", true, false);
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_OakValeBanditRaidScript>("OakValeBanditRaidBandit", this, &COakValeBanditRaidBandit::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_OakValeBanditRaidScript>("M_RestartMarker", this, &CRestartMarker::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    Game->AddRumourCategory("OAKVALE_BANDITRAID");
    Game->AddNewRumourToCategory("OAKVALE_BANDITRAID", "TEXT_SCR_BANDITRAID");
    Game->SetCategoryActivity("OAKVALE_BANDITRAID", true);
    Game->OverrideMusic((EMusicSetType)1, false, false);
    PopulateLevel();
    AddSpawnedFunction(new CSpawnedFunc<CQ_OakValeBanditRaidScript>("WatchForLevelChanges", 0, this, &NScript::CQ_OakValeBanditRaidScript::WatchForLevelChanges), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_OakValeBanditRaidScript>("WatchForGateTimer", 0, this, &NScript::CQ_OakValeBanditRaidScript::WatchForGateTimer), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_OakValeBanditRaidScript>("WatchForWhistling", 0, this, &NScript::CQ_OakValeBanditRaidScript::WatchForWhistling), CCharString(""));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    if (Game->DisplayTutorial((ETutorialCategory)0x22)) {
        if (IsActiveThreadTerminating())
            return;
        while (!Game->MsgIsTutorialClickedPast()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
    }
    for (;;) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
}