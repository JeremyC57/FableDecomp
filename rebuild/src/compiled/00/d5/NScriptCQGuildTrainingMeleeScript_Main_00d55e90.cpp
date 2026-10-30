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
struct CScriptThingImpl {                               // the game-side object behind a CScriptThing (PImp)
    virtual void h000(); virtual void h001(); virtual void h002(); virtual void h003(); virtual void h004(); virtual void h005(); virtual void h006(); virtual void h007();
    virtual void h008(); virtual void h009(); virtual void h010(); virtual void h011(); virtual void h012(); virtual void h013(); virtual void h014(); virtual void h015();
    virtual void h016(); virtual void h017(); virtual void h018(); virtual void h019(); virtual void h020(); virtual void h021(); virtual void h022(); virtual void h023();
    virtual void h024(); virtual void h025(); virtual void h026(); virtual void h027(); virtual void h028(); virtual void h029(); virtual void h030(); virtual void h031();
    virtual void h032(); virtual void h033(); virtual void h034(); virtual void h035(); virtual void h036(); virtual void h037(); virtual void h038(); virtual void h039();
    virtual void h040(); virtual void h041(); virtual void h042(); virtual void h043(); virtual void h044(); virtual void h045(); virtual void h046(); virtual void h047();
    virtual void h048(); virtual void h049(); virtual void h050(); virtual void h051(); virtual void h052(); virtual void h053(); virtual void h054(); virtual void h055();
    virtual void h056(); virtual void h057(); virtual void h058(); virtual void h059(); virtual void h060(); virtual void h061(); virtual void h062(); virtual void h063();
    virtual void h064(); virtual void h065(); virtual void h066();
    virtual void SetFriendsWithEverythingFlag(bool on); // slot 67 (+0x10c)
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptThing : public CBaseClass { public:                      // 12 bytes, vtable 0x01238C8C
    virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005();
    virtual const C3DVector& GetPos() const;            // slot 6 (+0x18) 0x004AA980
    CCountedPointer<CScriptThingImpl> PImp;                       // +0x04
    __forceinline ~CScriptThing() {}                    // inline here; out-of-line copy 0x004AA840
    // virtual slot 67 (+0x10c), out of line 0x004AB060; called on a by-value thing, so expanded inline here
    void SetFriendsWithEverythingFlag(bool on) { if (PImp.Data) PImp.Data->SetFriendsWithEverythingFlag(on); }
};
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
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                                                 // 0x00CB7940
    void AddEntityScriptBinding(CEntityScriptBindingBase* binding);                   // 0x00CB8230
    void PostAddScriptedEntities();                                                   // 0x00CB8930
};
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
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
    Game->EntitySetAllowBossPhaseChanges(whisper, false);
    CScriptGameResourceObjectScriptedThingBase heroHandle;
    if (heroHandle.IsValid())
        heroHandle.Release();
    while (!Game->StartScriptingEntity(Game->GetHero(), heroHandle, (EScriptAIPriority)4)) {
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
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            }
        while (!Game->MsgIsTutorialClickedPast()) {
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
    }
    {
#pragma inline_depth(0)
        return;
#pragma inline_depth()
    }
}
}