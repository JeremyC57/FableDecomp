// CQ_GuildTrainingScript::RunSaveXPCutscene2() -- take the real guildmaster and the hero under scripted control, cast them as "GM" and "HERO" and play CS_GUILD_SAVEXP2 in a paused movie sequence with the movie camera fixed
// Layout: only Game (+0x40, right after the 0x40-byte retail CScriptBase) is touched; no rebuild/include/engine header exists for the class.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D49A20, length 815 bytes. Twin of RunSaveXPCutscene 0x00D496F0 (the
//   manifest names both "RunSaveXPCutscene2"); they differ only in the cut-scene literal ("CS_GUILD_SAVEXP2" 0x012CE910
//   here, "CS_GUILD_SAVEXP" 0x012CE900 there).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, ~CScriptThing 0x004AA840 (out of line)
//   / ~CBaseClass 0x0099A2E0 / operator delete 0x00BFE9BC, CBaseObject ctor 0x0099A380 / dtor 0x0099A430, scripted-thing
//   handle IsValid 0x00CD23B9 / Release 0x00CD2770 / dtor 0x007E74D0 (out of line) / operator= 0x008ABD10, cast map
//   ctor 0x00CDBF70 / dtor 0x00CDBFB0 / operator[] 0x00CD3D2E, movie PImp dtor 0x006E7AB0, RunCutsceneMacro 0x00CBFB7D
//   (__fastcall), CScriptBase::IsActiveThreadTerminating 0x00CB7940; the game script interface slots 7, 8, 70, 72, 370,
//   371, 379.
// Shape: the CScriptThing model of the landed CV_ChapelOfEvilScript::WatchForWeaponUnsheathed 0x00E6A340 and the handle /
//   movie model of the landed CQS_ScytheInfoScript::PlayCutscene 0x00E2A900 (inline destructors; VC7.1 expands them at
//   the guildmaster loop's in-loop return and calls the out-of-line copies on the shared exit tail).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
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
struct CScriptThingImpl;
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() {}
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CScriptGameResourceObjectBase : public CBaseObject { public:        // vtable 0x0126008C
    virtual ~CScriptGameResourceObjectBase() {}
};
struct CScriptThingImp;
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:   // vtable 0x0127094C, 0x10 bytes
    CCountedPointer<CScriptThingImp> PImp;                  // +0x08
    bool IsValid() const;                                     // 0x00CD23B9
    void Release();                                         // 0x00CD2770
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other);   // 0x008ABD10
};
struct CMovieImpPointer {
    void* Data;
    void* Info;
    CMovieImpPointer() : Data(0), Info(0) {}
    ~CMovieImpPointer();                                    // 0x006E7AB0
};
class CScriptGameResourceObjectMovieBase : public CScriptGameResourceObjectBase { public:   // vtable 0x01260EF4
    CMovieImpPointer PImp;                                  // +0x08
};
struct CCutsceneCastMap {                                   // map<CCharString, CScriptGameResourceObjectScriptedThingBase>
    void* _body[3];
    CCutsceneCastMap();                                     // 0x00CDBF70
    ~CCutsceneCastMap();                                    // 0x00CDBFB0
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
namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_GuildTrainingScript : public CScriptBase { public:
    CGameScriptInterface* Game;                             // +0x40
    void*                 Data;                             // +0x44
    void RunSaveXPCutscene2();
};
void CQ_GuildTrainingScript::RunSaveXPCutscene2()
{
    CScriptThing guildmaster = Game->GetThingWithScriptName("TheRealGuildmaster");
    CScriptGameResourceObjectScriptedThingBase gm;
    if (gm.IsValid())
        gm.Release();
    while (!Game->StartScriptingEntity(guildmaster, gm, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
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
    cast["GM"] = gm;
    cast["HERO"] = hero;
    CScriptGameResourceObjectMovieBase movie;
    Game->StartMovieSequence("", movie);
    CGameScriptInterface* game = Game;
    game->PauseAllNonScriptedEntities(true);
    Game->FixMovieSequenceCamera(true);
    RunCutsceneMacro("CS_GUILD_SAVEXP2", cast, 0, 0, false, true);
    Game->FixMovieSequenceCamera(false);
    game->PauseAllNonScriptedEntities(false);
}
}