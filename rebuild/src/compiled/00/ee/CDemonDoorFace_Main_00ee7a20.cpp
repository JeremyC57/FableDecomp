// CDemonDoorFace::Main() -- the Witchwood demon door (trophy-dealer quest): take scripted control of Me, add a core-quest minimap marker; while the stones are not in sync, each time the hero talks to the door say it is locked and (once) set the quest-card objective; once the master data's SingingStonesInSync is set, script the hero, play CS_TROPHY_DEALER_DOOR_OPENS with HERO and DOOR cast, move the objective on, make the DemonDoorDoor persistent, clear the flag and remove Me
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE7A20, 1659 bytes (+ int3 padding), plain `ret`. Literals "HUD_ORB_QUEST_CORE" 0x01258AB0,
//   "TEXT_QST_071_DOOR_LOCKED1" 0x012F53A4, "TEXT_QUEST_FIND_TROPHY_DEALER_OBJECTIVE_03" 0x012F5378 / _04 0x012F5330, "Witchwood2" 0x012DCEF8,
//   "WitchwoodCavern" 0x012DCEE8, "HERO" 0x01255174, "DOOR" 0x012DAC70, "CS_TROPHY_DEALER_DOOR_OPENS" 0x012F535C, "DemonDoorDoor" 0x012DDAAC.
// Callees: CBaseIntelligentPointer ctor 0x0099A380 / dtor 0x0099A430, scripted-thing handle IsScripting 0x00CD23B9 / Release 0x00CD2770 /
//   GetScriptThing 0x007E7490 / Speak 0x007E7390 / IsPerformingScriptTask 0x007E7450 / out-of-line dtor 0x007E74D0, movie handle
//   out-of-line dtor 0x006E7B80, CIsThingAlive (vtable 0x012C32F0, out-of-line dtor 0x00E241B0), CScriptThing copy 0x004ABE90 /
//   dtor 0x004AA840, RegisterFilter 0x00F35B10, IsActiveThreadTerminating 0x00F35B30, operator delete 0x00BFE9BC, CCharString
//   0x0099EBF0 / 0x0099EAE0; GameCalls slots 7, 8, 70 GetHero,
//   81 GetNearestWithDefName, 72 GetThingWithScriptName, 108 RemoveThing, 296 SetQuestCardObjective, 350 MiniMapAddMarker, 655 GetActiveQuestName, scripted-thing ctor 0x007E72A0 / operator= 0x008ABD10 (out of line), cast map ctor 0x00CDBF70 / dtor 0x00CDBFB0 / operator[] 0x00CD3D2E, RunCutsceneMacro 0x00CBFB7D (__fastcall), movie ctor 0x006E7B60 (out of line), 264 GetHealth, 370 StartMovieSequence,
//   379 PauseAllNonScriptedEntities, 477 SetThingPersistent;
//   CScriptThing slot 27 MsgIsTalkedToBy.
// Layout: Ego_r.pdb CDemonDoorFace (32 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (CV_TrophyDealerScript),
//   PMasterData +0x18, TimesSpoken +0x1c. CQ_SunnyvaleMasterData TrophyDealerHeroSpokenToDemonDoors +0x7C, SingingStonesInSync +0x140 (PDB +0x144; one STLport vector earlier). Handle hierarchy as the landed
//   CMeleeThunder::Main 0x00D58080.
// Shape: the CMeleeThunder handle model (implicit handle dtor expanded only at the first return, inline_depth(0) elsewhere).
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
struct CBase {
    CBase();                                            // 0x0099A2D0
    virtual ~CBase();                                   // 0x0099A2E0
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
class CScriptThing : public CBase { public:                           // vtable 0x01238C8C, 12 bytes
    VPAD16(ma) VPAD8(mb) VPAD2(mc)                      // slots 1..26
    virtual bool MsgIsTalkedToBy(const CCharString& name) const;   // slot 27 (+0x6c)
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
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
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CScriptGameResourceObjectBase : public CBaseObject { public:   // vtable 0x0126008C
    CScriptGameResourceObjectBase() { }
    __forceinline ~CScriptGameResourceObjectBase() { }
};
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_RANDOM };
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:   // vtable 0x0127094C, 0x10 bytes
    CCountedPointer<void> PImp;                         // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectScriptedThingBase() { }
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other);   // 0x008ABD10
    // implicit destructor (stores no vptr); out-of-line copy 0x007E74D0
    bool IsScripting();                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
    void Speak(const CScriptThing& listener, const char* text, ETextGroupSelectionMethod method, bool a, bool b, bool c);   // 0x007E7390
public:
    virtual CScriptThing GetScriptThing() const;   // 0x007E7490
    virtual bool IsPerformingScriptTask() const;   // 0x007E7450
};
class CScriptGameResourceObjectMovieBase : public CScriptGameResourceObjectBase { public:   // vtable 0x01260EF4, 0x10 bytes
    CCountedPointer<void> PImp;                         // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectMovieBase() { }
    ~CScriptGameResourceObjectMovieBase();              // 0x006E7B80
};
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CCutsceneCastMap {                              // map<CCharString, CScriptGameResourceObjectScriptedThingBase>
    void* _body[3];
    void Construct();                                  // 0x00CDBF70
    void Destroy();                                    // 0x00CDBFB0
    CScriptGameResourceObjectScriptedThingBase& operator[](const CCharString& key);   // 0x00CD3D2E
};
struct CCutsceneFlagMap;
struct CCutsceneArgMap;
void __fastcall RunCutsceneMacro(const CCharString& name, CCutsceneCastMap& cast, CCutsceneFlagMap* flags, CCutsceneArgMap* args, bool a5, bool a6);   // 0x00CBFB7D
struct CQ_SunnyvaleMasterData {
    unsigned char _pad[0x7c];
    bool TrophyDealerHeroSpokenToDemonDoors;           // +0x7C
    unsigned char _pad7d[0x140 - 0x7d];
    bool SingingStonesInSync;                          // +0x140 (PDB +0x144)
};
extern const float g_fZero;                            // 0x0122DEDC (0.0f)
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
struct CV_TrophyDealerScript;
struct CDemonDoorFace : CEntityScriptBase {
    CV_TrophyDealerScript*  ParentClass;                // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                // +0x18
    long                    TimesSpoken;                // +0x1c
    void Main();
};

void CDemonDoorFace::Main()
{
    CScriptGameResourceObjectScriptedThingBase face;
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    if (face.IsScripting())
        face.Release();
    while (!Game->StartScriptingEntity(Me, face, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            {
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            }
    }
    if (IsActiveThreadTerminating())
        {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
    Game->MiniMapAddMarker(Me, "HUD_ORB_QUEST_CORE");
    while (!IsActiveThreadTerminating()) {
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                {
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
            if (!PMasterData->SingingStonesInSync) {
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
#pragma inline_depth(0)
                CScriptGameResourceObjectMovieBase movie;
#pragma inline_depth()
                Game->StartMovieSequence("", movie);
                CGameScriptInterface* game = Game;
                game->PauseAllNonScriptedEntities(true);
                if (Game->GetHealth(face.GetScriptThing()) > g_fZero) {
                    face.Speak(Game->GetHero(), "TEXT_QST_071_DOOR_LOCKED1", (ETextGroupSelectionMethod)0, false, true, false);
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
                game->PauseAllNonScriptedEntities(false);
            }
            if (!PMasterData->TrophyDealerHeroSpokenToDemonDoors) {
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                PMasterData->TrophyDealerHeroSpokenToDemonDoors = true;
                Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_FIND_TROPHY_DEALER_OBJECTIVE_03", "Witchwood2", "");
            }
        }
        if (PMasterData->SingingStonesInSync) {
            if (IsActiveThreadTerminating())
                {
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
            {
#pragma inline_depth(0)
                CScriptGameResourceObjectScriptedThingBase hero;
#pragma inline_depth()
                Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4);
                if (face.IsScripting())
                    face.Release();
                while (!Game->StartScriptingEntity(Me, face, (EScriptAIPriority)4)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        {
#pragma inline_depth(0)
                            return;
#pragma inline_depth()
                        }
                }
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                CCutsceneCastMap cast;
                cast.Construct();
                cast["HERO"] = hero;
                cast["DOOR"] = face;
                {
#pragma inline_depth(0)
                    CScriptGameResourceObjectMovieBase movie;
#pragma inline_depth()
                    Game->StartMovieSequence("", movie);
                    CGameScriptInterface* game = Game;
                    game->PauseAllNonScriptedEntities(true);
                    RunCutsceneMacro("CS_TROPHY_DEALER_DOOR_OPENS", cast, 0, 0, false, true);
                    game->PauseAllNonScriptedEntities(false);
                }
                cast.Destroy();
#pragma inline_depth(0)
            }
#pragma inline_depth()
            Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_FIND_TROPHY_DEALER_OBJECTIVE_04", "WitchwoodCavern", "");
            CScriptThing door = Game->GetThingWithScriptName("DemonDoorDoor");
            Game->SetThingPersistent(door, true);
            PMasterData->SingingStonesInSync = false;
            Game->RemoveThing(Me, false, true);
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
        Game->NewScriptFrame();
    }
#pragma inline_depth(0)
}
#pragma inline_depth()