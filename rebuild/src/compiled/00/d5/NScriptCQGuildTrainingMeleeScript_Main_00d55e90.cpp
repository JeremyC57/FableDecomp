// CQ_GuildTrainingMeleeScript::Main() -- the guild melee lesson's main: bind the TheRealGuildmaster / MeleeOpponent / MeleeThunder entity scripts, teleport the guildmaster to his teaching stand, create the teen Whisper at her marker (hero faction), take Whisper and the hero under scripted control, play CS_GUILD_MELEE_INTRO in a paused movie sequence, set the quest-card objective, add story log entry 40 and show the melee tutorial until it is clicked past
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D55E90, length 1820 bytes.
// Callees: operator new 0x00BFEA1A, operator delete 0x00BFE9BC, CCharString(const char*, long) 0x0099EBF0 /
//   CCharString(const CCharString&) 0x0099EC30 / ~CCharString 0x0099EAE0, ~CBaseClass 0x0099A2E0, ~CScriptThing 0x004AA840,
//   CBaseObject ctor/dtor 0x0099A380 / 0x0099A430, scripted-thing handle IsValid 0x00CD23B9 / Release 0x00CD2770 /
//   operator= 0x008ABD10 / dtor 0x007E74D0, movie PImp dtor 0x006E7AB0, cast map ctor 0x00CDBF70 / dtor 0x00CDBFB0 /
//   operator[] 0x00CD3D2E, RunCutsceneMacro 0x00CBFB7D (__fastcall), AddLogbookStoryEntry 0x00CBE87F (__fastcall),
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940 / AddEntityScriptBinding 0x00CB8230 / PostAddScriptedEntities
//   0x00CB8930; binding allocators 0x00D565C0 / 0x00D566A0 / 0x00D57FA0 (binding vtable 0x012D0304); the game script
//   interface (+0x40) slots 7, 8, 41, 64, 70, 72, 91, 118, 296, 370, 371, 379, 472, 521, 601; CScriptThing::GetPos (slot 6).
// Layout (retail = Ego_r.pdb - 0x14): GenericTutorialCounter +0x48, TutorialState +0x4c, TalkedToWhisper +0x50.
// Shape: bindings as the landed CQ_GuildTrainingWoodsMeleeScript::Main 0x00D66620; cast/movie as the landed
//   CQ_HeroSoulsBriarScript::PlayCutscene 0x00D823A0; handle / thing destructors as the landed CMeleeThunder::Main 0x00D58080
//   (inline only at the first wait loop's return; every later exit, and the closing return that the tutorial-not-shown
//   path shares, sits under in-body inline_depth(0) and calls the out-of-line copies; an inline_depth(0) at the closing
//   brace instead pushes the movie's ctor/base dtor out of line too).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                                 // CCountedPointer's shared block
    long RefCount;                                      // +0
    void (__fastcall* DeleteFunc)(void* data);          // +4
    void* Data;                                         // +8
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
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
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() {}
inline void CScriptThing::SetFriendsWithEverythingFlag(bool on) { if (PImp.Data) PImp.Data->SetFriendsWithEverythingFlag(on); }
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CScriptGameResourceObjectBase : public CBaseObject { public:    // vtable 0x0126008C
    CScriptGameResourceObjectBase() { }
    __forceinline ~CScriptGameResourceObjectBase() { }
};
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:   // vtable 0x0127094C, 0x10 bytes
    CCountedPointer<void> PImp;                         // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectScriptedThingBase() { }
    // implicit destructor (stores no vptr); out-of-line copy 0x007E74D0
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other);   // 0x008ABD10
    bool IsValid() const;                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
};
struct CMovieImpPointer {
    void* Data;
    void* Info;
    __forceinline CMovieImpPointer() : Data(0), Info(0) {}
    ~CMovieImpPointer();                                // 0x006E7AB0
};
class CScriptGameResourceObjectMovieBase : public CScriptGameResourceObjectBase { public:   // vtable 0x01260EF4
    CMovieImpPointer PImp;
};
struct CCutsceneCastMap {                               // map<CCharString, CScriptGameResourceObjectScriptedThingBase> (12 bytes)
    void* _body[3];
    CCutsceneCastMap();                                 // 0x00CDBF70
    ~CCutsceneCastMap();                                // 0x00CDBFB0
    CScriptGameResourceObjectScriptedThingBase& operator[](const CCharString& key);   // 0x00CD3D2E
};
struct CCutsceneFlagMap;
struct CCutsceneArgMap;
void __fastcall RunCutsceneMacro(const CCharString& name, CCutsceneCastMap& cast, CCutsceneFlagMap* flags, CCutsceneArgMap* args, bool a5, bool a6);   // 0x00CBFB7D
void __fastcall AddLogbookStoryEntry(long index);       // 0x00CBE87F
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
enum ETutorialCategory { TUTORIAL_CATEGORY_NONE };
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptDataBase;
class CEntityScriptBase;
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
    CEntityScriptBinding(const CCharString& name, T* parent, TAllocFunc alloc, unsigned long flags)
        : EntityScriptName(name), PParentScript(parent), ClassAllocFunc(alloc), ClassAllocValid(true), Flags(flags) {}
    virtual const CCharString& GetEntityScriptName() const { return EntityScriptName; }
};
namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_GuildTrainingMeleeScript : public CScriptBase {
public:
    struct CTheRealGuildmaster { static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_GuildTrainingMeleeScript& parent, CScriptDataBase* data, const CScriptThing& thing); };   // 0x00D565C0
    struct CMeleeOpponent { static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_GuildTrainingMeleeScript& parent, CScriptDataBase* data, const CScriptThing& thing); };        // 0x00D566A0
    struct CMeleeThunder { static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_GuildTrainingMeleeScript& parent, CScriptDataBase* data, const CScriptThing& thing); };         // 0x00D57FA0
    CGameScriptInterface* Game;                         // +0x40
    void*                 Data;                         // +0x44
    long                  GenericTutorialCounter;       // +0x48
    long                  TutorialState;                // +0x4c
    bool                  TalkedToWhisper;              // +0x50
    void Main();
};
void CQ_GuildTrainingMeleeScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_GuildTrainingMeleeScript>("TheRealGuildmaster", this, &CTheRealGuildmaster::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_GuildTrainingMeleeScript>("MeleeOpponent", this, &CMeleeOpponent::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_GuildTrainingMeleeScript>("MeleeThunder", this, &CMeleeThunder::Alloc, 1));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    Game->EntityTeleportToThing(Game->GetThingWithScriptName("TheRealGuildmaster"), Game->GetThingWithScriptName("M_MeleeTeacherStand"), false);
    CScriptThing whisper = Game->CreateCreature("CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE", Game->GetThingWithScriptName("MK_GTWU_WHISPER").GetPos(), "MeleeOpponent", false);
    Game->EntitySetInFaction(whisper, "FACTION_HERO");
    whisper.SetFriendsWithEverythingFlag(true);
    CScriptGameResourceObjectScriptedThingBase whisperHandle;
    if (whisperHandle.IsValid())
        whisperHandle.Release();
    while (!Game->StartScriptingEntity(whisper, whisperHandle, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        {
            return;
        }
    Game->EntitySetAllowBossPhaseChanges(whisper, false);
    CScriptGameResourceObjectScriptedThingBase heroHandle;
    if (heroHandle.IsValid())
        heroHandle.Release();
    while (!Game->StartScriptingEntity(Game->GetHero(), heroHandle, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            {
                return;
            }
    }
    if (IsActiveThreadTerminating())
        {
            return;
        }
    {
        CCutsceneCastMap cast;
        cast["HERO"] = heroHandle;
        cast["WHISPER"] = whisperHandle;
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CGameScriptInterface* game = Game;
        game->PauseAllNonScriptedEntities(true);
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro("CS_GUILD_MELEE_INTRO", cast, 0, 0, false, true);
        Game->FixMovieSequenceCamera(false);
        TalkedToWhisper = true;
        game->PauseAllNonScriptedEntities(false);
    }
    Game->SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_03", "", "");
    AddLogbookStoryEntry(40);
    if (Game->DisplayTutorial((ETutorialCategory)26)) {
        if (IsActiveThreadTerminating())
            {
                return;
            }
        while (!Game->MsgIsTutorialClickedPast()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                {
                    return;
                }
        }
        if (IsActiveThreadTerminating())
            {
                return;
            }
    }
    {
        return;
    }
}
}