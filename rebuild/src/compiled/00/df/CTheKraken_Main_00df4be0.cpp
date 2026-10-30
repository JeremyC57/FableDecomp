// CTheKraken::Main() -- the kraken boss entity script: register the alive-filter, run a frame, wait until the kraken's health reaches zero, then take the hero under scripted control and play CS_KRAKEN_OUTRO in a paused movie sequence, and idle until the script is done
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DF4BE0, length 531 bytes. Entity-script slot 1 (Main) of the vtable
//   0x012DE5AC built by the landed factory 0x00DF4730 (sizeof 0x1c = Ego_r.pdb CTheKraken); the manifest label
//   "CQ_GuildTrainingScript::RunSaveXPCutscene2" is propagated.
// Callees: CScriptThing copy ctor 0x004ABE90 / dtor 0x004AA840 (NScript::CIsThingConscious filter, vtable 0x012C2FE8), filter
//   registration 0x00F35B10 / IsDone 0x00F35B30, CBaseObject ctor 0x0099A380 / dtor 0x0099A430, scripted-thing handle
//   dtor 0x007E74D0 / operator= 0x008ABD10, cast map ctor 0x00CDBF70 / dtor 0x00CDBFB0 / operator[] 0x00CD3D2E, movie
//   PImp dtor 0x006E7AB0, RunCutsceneMacro 0x00CBFB7D (__fastcall), CCharString 0x0099EBF0 / 0x0099EAE0; the game
//   script interface slots 7 NewScriptFrame, 8 StartScriptingEntity, 70 GetHero, 264 GetHealth, 370 StartMovieSequence,
//   379 PauseAllNonScriptedEntities.
// Layout: entity-script base {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18 (Ego_r.pdb CTheKraken).
// Shape: the landed CKrakenMinion::Main 0x00DF4610 filter / handle model and the CMK_GHC_HERO3::Main 0x00D29DB0
//   cast-map / destructor-less movie model; a first handle is constructed (never used) before the health wait.
namespace NScript { class CPredicateBase; }
class CScriptThing { public:                          // 12 bytes
    unsigned char _body[12 - 4];
    CScriptThing(const CScriptThing& other);  // 0x004ABE90
    virtual ~CScriptThing();                          // 0x004AA840
};
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
class CBaseObject { public:                           // CBaseIntelligentPointer root (vtable 0x0129A7D8 stored by its ctor)
    virtual void v00();
    void* m_pIntelligentPointer;               // +0x04
protected:
    CBaseObject();                             // 0x0099A380
public:
};
struct CBaseObjectDtor {                       // destructor-only view of CBaseObject
    void* m_pIntelligentPointer;
    virtual ~CBaseObjectDtor();                // 0x0099A430
};
struct CScriptGameResourceObjectBaseDtor : CBaseObjectDtor {   // vtable 0x0126008C
    __forceinline virtual ~CScriptGameResourceObjectBaseDtor() { }
};
struct CCountedPointerWords {                  // {Data, Info}
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};
class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C, 0x10 bytes
    CCountedPointerWords PImp;                 // +0x08 Data, +0x0c Info
    virtual void v00();
    CScriptGameResourceObjectScriptedThingBase() : CBaseObject(), PImp() { }
    virtual ~CScriptGameResourceObjectScriptedThingBase();   // 0x007E74D0
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other);   // 0x008ABD10
};
struct CMovieImpPointer {
    void* Data;
    void* Info;
    ~CMovieImpPointer();                       // 0x006E7AB0
};
struct CMovieImpWords { void* Data; void* Info; };
class CScriptGameResourceObjectMovieBase : public CBaseObject { public:   // vtable 0x01260EF4; destructor-less view, destroyed by hand
    CMovieImpWords PImp;                       // +0x08
    __forceinline CScriptGameResourceObjectMovieBase() { PImp.Data = 0; PImp.Info = 0; }
    virtual void m001();
    __forceinline void DestroyInline()
    {
        reinterpret_cast<CMovieImpPointer*>(&PImp)->~CMovieImpPointer();
        reinterpret_cast<CScriptGameResourceObjectBaseDtor*>(this)->CScriptGameResourceObjectBaseDtor::~CScriptGameResourceObjectBaseDtor();
    }
};
struct CCutsceneCastMap {                      // map<CCharString, CScriptGameResourceObjectScriptedThingBase>
    void* _body[3];
    CCutsceneCastMap();                        // 0x00CDBF70
    ~CCutsceneCastMap();                       // 0x00CDBFB0
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
class CPredicateBase
{
public:
    virtual void v00();
};
class CIsThingConscious : public CPredicateBase { public:                         // vtable 0x012C2FE8 (operator(): IsAlive() && !IsUnconscious(), CScriptThing slots 75/61)
    CScriptThing m_thing;                     // +0x04
    CIsThingConscious(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingConscious() { }
};
}
struct CQ_TentacleKrakenBossFightScript;
struct CQ_SunnyvaleMasterData;
class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

class CTheKraken : public CEntityScriptBase {
public:
    void*                 __vftable;          // +0x00
    CGameScriptInterface* Game;               // +0x04
    CScriptThing          Me;                 // +0x08
    CQ_TentacleKrakenBossFightScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData*           PMasterData;   // +0x18
    void Main();
};

void CTheKraken::Main()
{
    {
        NScript::CIsThingConscious filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase kraken;
    while (!(Game->GetHealth(Me) <= 0.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    {
        CScriptGameResourceObjectScriptedThingBase hero;
        Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4);
        CCutsceneCastMap cast;
        cast["Hero"] = hero;
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CGameScriptInterface* game = Game;
        game->PauseAllNonScriptedEntities(true);
        RunCutsceneMacro("CS_KRAKEN_OUTRO", cast, 0, 0, false, true);
        game->PauseAllNonScriptedEntities(false);
        movie.DestroyInline();
    }
    for (;;) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
}