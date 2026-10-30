// CTargetForKey::Main() -- the Minion Camp silver-key target: register an alive filter over Me; once the target is done
// or the Minion Camp quest is active, remove Me (and later code still runs); otherwise wait until Me is hit, then
// spawn the silver key at "GenerateKeyHere", play CS_STANDING_STONE with the hero cast inside a paused movie sequence,
// make the end-game focal site persistent and give it the "nothing stuck" click text, mark the target done and remove Me
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CE15E0, plain `ret`; entity-script vtable slot 1 (Main).
// Callees: CIsThingAlive filter (vtable 0x012C32F0) over CScriptThing copy 0x004ABE90 / ~CScriptThing 0x004AA840,
//   RegisterFilter 0x00F35B10, IsActiveThreadTerminating 0x00F35B30, CCharString 0x0099EBF0 / 0x0099EAE0,
//   CBaseIntelligentPointer ctor 0x0099A380 / operator= 0x0099A3B0 / dtor 0x0099A430, handle PImp operator= 0x008AB1E0,
//   handle dtor 0x007E74D0, cast map ctor 0x00CDBF70 / dtor 0x00CDBFB0 / operator[] 0x00CD3D2E, movie PImp dtor
//   0x006E7AB0, RunCutsceneMacro 0x00CBFB7D (__fastcall); CScriptThing slot 21 MsgIsHitBy and its PImp's slot 4;
//   CGameScriptInterface slots 7 NewScriptFrame, 8 StartScriptingEntity, 70 GetHero, 72 GetThingWithScriptName,
//   77 GetNearestWithScriptName, 99 CreateObjectOnEntity, 108 RemoveThing, 284 IsQuestActive, 370 StartMovieSequence,
//   376 Pause, 379 PauseAllNonScriptedEntities, 477 SetThingPersistent.
// Layout: Ego_r.pdb CTargetForKey (32 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14,
//   PMasterData +0x18, TargetForKeyDone +0x1c. Handle / map / movie classes as the landed CMK_GHC_HERO3::Main 0x00D29DB0
//   and CRockTrollTrigger::Main 0x00E0AD40.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class CCPPointerInfo { public: long RefCount; void (__fastcall* DeleteFunc)(void* data); void* Data; };
struct CScriptThingImpl {
    virtual void i000(); virtual void i001(); virtual void i002(); virtual void i003();
    virtual void SetClickText(const CCharString& text);   // slot 4 (+0x10)
};
class CScriptThing { public:                                  // vtable 0x01238C8C, 12 bytes
    virtual ~CScriptThing();                           // slot 0 (out of line 0x004AA840)
    virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005(); virtual void t006(); virtual void t007(); virtual void t008();
    virtual void t009(); virtual void t010(); virtual void t011(); virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015(); virtual void t016();
    virtual void t017(); virtual void t018(); virtual void t019(); virtual void t020();
    virtual bool MsgIsHitBy(const CCharString& name) const;   // slot 21 (+0x54)
    CScriptThingImpl* PImp_Data;                       // +0x04
    CCPPointerInfo*   PImp_Info;                       // +0x08
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    void SetClickText(const CCharString& text) { if (PImp_Data) PImp_Data->SetClickText(text); }
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
class CBaseObject { public:                                    // CBaseIntelligentPointer, vtable 0x0129A7D8
    virtual void b000();
    void* m_pIntelligentPointer;                        // +0x04
protected:
    CBaseObject();                                      // 0x0099A380
    CBaseObject& operator=(const CBaseObject& other);   // 0x0099A3B0
public:
};
struct CBaseObjectDtor {                                // destructor-only view of CBaseObject
    void* m_pIntelligentPointer;
    virtual ~CBaseObjectDtor();                         // 0x0099A430
};
struct CScriptGameResourceObjectBaseDtor : CBaseObjectDtor {   // vtable 0x0126008C
    __forceinline virtual ~CScriptGameResourceObjectBaseDtor() { }
};
struct CHandlePImp {
    void*           Data;
    CCPPointerInfo* Info;
    CHandlePImp& operator=(const CHandlePImp& other);   // 0x008AB1E0
};
class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C, 0x10 bytes
    CHandlePImp PImp;                                   // +0x08 Data, +0x0c Info
    __forceinline CScriptGameResourceObjectScriptedThingBase() { PImp.Data = 0; PImp.Info = 0; }
    virtual void h001();
    virtual ~CScriptGameResourceObjectScriptedThingBase();      // 0x007E74D0
    __forceinline CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other)
    {
        CBaseObject::operator=(other);
        PImp = other.PImp;
        return *this;
    }
};
struct CMovieImpPointer {
    void* Data;
    void* Info;
    ~CMovieImpPointer();                                // 0x006E7AB0
};
struct CMovieImpWords { void* Data; void* Info; };
class CScriptGameResourceObjectMovieBase : public CBaseObject { public:   // vtable 0x01260EF4; destructor-less view, destroyed by hand
    CMovieImpWords PImp;                                // +0x08
    __forceinline CScriptGameResourceObjectMovieBase() { PImp.Data = 0; PImp.Info = 0; }
    virtual void m001();
    __forceinline void DestroyInline()
    {
        reinterpret_cast<CMovieImpPointer*>(&PImp)->~CMovieImpPointer();
        reinterpret_cast<CScriptGameResourceObjectBaseDtor*>(this)->CScriptGameResourceObjectBaseDtor::~CScriptGameResourceObjectBaseDtor();
    }
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
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
struct CTargetForKey : CEntityScriptBase {
    void* ParentClass;                                 // +0x14 CPersonalScript_GlobalThingsScript*
    void* PMasterData;                                 // +0x18 CQ_SunnyvaleMasterData*
    bool  TargetForKeyDone;                            // +0x1c
    void Main();
};

void CTargetForKey::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    if (TargetForKeyDone || Game->IsQuestActive("Q_MinionCamp")) {
        if (IsActiveThreadTerminating())
            return;
        Game->RemoveThing(Me, false, true);
    }
    while (!Me.MsgIsHitBy("")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing keyHere = Game->GetNearestWithScriptName(Me, "GenerateKeyHere");
    {
    CScriptGameResourceObjectScriptedThingBase hero;
    Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4);
    Game->CreateObjectOnEntity("OBJECT_SILVER_KEY", keyHere, "");
    CCutsceneCastMap cast;
    cast["HERO"] = hero;
    CScriptGameResourceObjectMovieBase movie;
    Game->StartMovieSequence("", movie);
    CGameScriptInterface* game = Game;
    game->PauseAllNonScriptedEntities(true);
    Game->Pause(1.0f);
    RunCutsceneMacro("CS_STANDING_STONE", cast, 0, 0, false, true);
    game->PauseAllNonScriptedEntities(false);
    movie.DestroyInline();
    }
    CScriptThing site = Game->GetThingWithScriptName("EndGameFocalSite");
    Game->SetThingPersistent(site, true);
    site.SetClickText("TXT_CLICK_NOTHING_STUCK");
    TargetForKeyDone = true;
    Game->RemoveThing(Me, false, true);
}