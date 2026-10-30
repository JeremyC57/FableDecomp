// CQ_GuildTrainingScript::RunCeremonyCutscene() -- once the fresco dome is loaded, create Whisper and the guildmaster at their ceremony markers, take Whisper, the guildmaster and the hero under scripted control (Whisper in the hero faction), cast them as WHIS / GM / HERO and play CS_GUILD_CEREMONY in a paused movie sequence with the camera fixed, then remove Whisper and the guildmaster
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D49D50, length 1304 bytes.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, ~CScriptThing 0x004AA840, CBaseObject ctor
//   0x0099A380, scripted-thing handle (vtable 0x0127094C) ctor 0x007E72A0 / dtor 0x007E74D0 / IsScripting 0x00CD23B9 /
//   Release 0x00CD2770 / operator= 0x008ABD10, cast map ctor 0x00CDBF70 / dtor 0x00CDBFB0 / operator[] 0x00CD3D2E, movie
//   ctor 0x006E7B60 / dtor 0x006E7B80, cut-scene runner 0x00CBFB7D (__fastcall), CScriptBase::IsActiveThreadTerminating
//   0x00CB7940; CScriptThing::GetPos (slot 6); the game script interface (+0x40) slots 7, 8, 11, 70, 72, 91, 108, 370, 371,
//   379, 601.
// Shape: cast/movie model of the landed CQ_HeroSoulsBriarScript::PlayCutscene 0x00D823A0 (one named copy of Game for the
//   pause pair); scripted-thing handle as the landed CHobbeLeader::Main 0x00D8FA30 (`if (IsScripting()) Release();`).
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
class CScriptThing { public:                                   // vtable 0x01238C8C, 12 bytes {vptr, PImp.Data, PImp.Info}
    virtual void t000(); virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005();
    virtual const C3DVector& GetPos() const;            // slot 6 (+0x18) 0x004AA980
    void* PImpData;
    void* PImpInfo;
    virtual ~CScriptThing();                                    // 0x004AA840
};
class CBaseObject { public:                                    // vtable 0x0129A7D8
protected:
    CBaseObject();                                      // 0x0099A380
public:
    virtual ~CBaseObject();                             // 0x0099A430
    void* IntelligentPointer;                           // +0x04
};
class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C, 16 bytes
    void* PImpData;                                     // +0x08
    void* PImpInfo;                                     // +0x0c
    CScriptGameResourceObjectScriptedThingBase() : CBaseObject(), PImpData(0), PImpInfo(0) { }   // out of line 0x007E72A0
    virtual ~CScriptGameResourceObjectScriptedThingBase();          // 0x007E74D0
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other);   // 0x008ABD10
    bool IsScripting();                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
};
class CScriptGameResourceObjectMovieBase : public CBaseObject { public:   // vtable 0x01260EF4, 16 bytes
    void* PImpData;
    void* PImpInfo;
    CScriptGameResourceObjectMovieBase();               // 0x006E7B60
    virtual ~CScriptGameResourceObjectMovieBase();      // 0x006E7B80
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
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};
namespace NScript {
class CQ_GuildTrainingScript : public CScriptBase { public:
    CGameScriptInterface* Game;                         // +0x40
    void*                 Data;                         // +0x44
    long                  GameState;                    // +0x48
    void RunCeremonyCutscene();
};
void CQ_GuildTrainingScript::RunCeremonyCutscene()
{
    while (!Game->IsLevelLoaded("FrescoDome")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing whisper = Game->CreateCreature("CREATURE_RIVAL_HERO_WHISPER_APPRENTICE", Game->GetThingWithScriptName("MK_GTC_WHISSTART").GetPos(), "Whisper", false);
    CScriptThing guildmaster = Game->CreateCreature("CREATURE_GUILDKEEPER", Game->GetThingWithScriptName("MK_GTC_GMSTART").GetPos(), "GM", false);
    CScriptGameResourceObjectScriptedThingBase whisperHandle;
    if (whisperHandle.IsScripting())
        whisperHandle.Release();
    while (!Game->StartScriptingEntity(whisper, whisperHandle, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->EntitySetInFaction(whisper, "FACTION_HERO");
    CScriptGameResourceObjectScriptedThingBase guildmasterHandle;
    if (guildmasterHandle.IsScripting())
        guildmasterHandle.Release();
    while (!Game->StartScriptingEntity(guildmaster, guildmasterHandle, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase heroHandle;
    if (heroHandle.IsScripting())
        heroHandle.Release();
    while (!Game->StartScriptingEntity(Game->GetHero(), heroHandle, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CCutsceneCastMap cast;
    cast["HERO"] = heroHandle;
    cast["WHIS"] = whisperHandle;
    cast["GM"] = guildmasterHandle;
    CScriptGameResourceObjectMovieBase movie;
    Game->StartMovieSequence("", movie);
    CGameScriptInterface* game = Game;
    game->PauseAllNonScriptedEntities(true);
    Game->FixMovieSequenceCamera(true);
    RunCutsceneMacro("CS_GUILD_CEREMONY", cast, 0, 0, false, true);
    Game->FixMovieSequenceCamera(false);
    Game->RemoveThing(guildmaster, false, true);
    Game->RemoveThing(whisper, false, true);
    game->PauseAllNonScriptedEntities(false);
}
}