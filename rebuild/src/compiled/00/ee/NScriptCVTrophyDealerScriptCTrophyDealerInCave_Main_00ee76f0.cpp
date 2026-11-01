// CTrophyDealerInCave::Main() -- entity script: holds an alive filter on the trophy dealer, makes it unkillable and undamageable, takes the hero and the dealer under scripted control and plays CS_TROPHY_DEALER in a paused movie sequence, then re-enables the Witchwood2 creature generators and completes and retires the active quest
// Leaf: ?Main@CTrophyDealerInCave@CV_TrophyDealerScript@NScript@@EAEXXZ
// Naming wave 27 (family main, DECISIONS_W26 D1): Ego_r.pdb / FableWin.pdb nest the entity in its script class and spell
//   Main a private virtual (?Main@CTrophyDealerInCave@CV_TrophyDealerScript@NScript@@EAEXXZ); the local CEntityScriptBase carries the retail slot list of vtable 0x012C3224
//   (engine/CEntityScriptBase.h). Was: ?Main@CTrophyDealerInCave@NScript@@QAEXXZ (not nested, public non-virtual).
// Naming wave 26 (family pred): CEntityScriptBase is the global PDB class (Ego_r.pdb / FableWin.pdb) with
//   ?SetPredicate@CEntityScriptBase@@QAEXABVCPredicateBase@NScript@@@Z 0x00F35B10 and the const
//   ?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ 0x00F35B30; the predicate parameter is an NScript::CPredicateBase.
//   Was: NScript::CEntityScriptBase with SetPredicate on the local CIsThingAlive and a non-const
//   IsActiveThreadTerminating.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };

#include "engine/CCountedPointer.h"

// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)

class CScriptGameResourceObjectBase : public CBaseObject { public:    // vtable 0x0126008C
    virtual __forceinline ~CScriptGameResourceObjectBase() {}
};

struct CCountedPointerHandle {                          // CCountedPointer<CScriptedThingImp>
    void*           Data;
    CCPPointerInfo* Info;
    CCountedPointerHandle() : Data(0), Info(0) {}
    ~CCountedPointerHandle()
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

class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:   // 16 bytes, vtable 0x0127094C
    CCountedPointerHandle PImp;                         // +0x08
    CScriptGameResourceObjectScriptedThingBase() {}
    // implicit destructor; out of line: 0x007E74D0
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other);   // 0x008ABD10
    bool IsValid() const;                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
};

struct CMovieImpPointer {
    void* Data;
    void* Info;
    CMovieImpPointer() : Data(0), Info(0) {}
    ~CMovieImpPointer();                                // 0x006E7AB0
};

class CScriptGameResourceObjectMovieBase : public CScriptGameResourceObjectBase { public:   // 16 bytes, vtable 0x01260EF4
    CMovieImpPointer PImp;                              // +0x08
    __forceinline CScriptGameResourceObjectMovieBase() {}
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

#include "engine/CIsThingAlive.h"                     // NScript::CIsThingAlive : CPredicateBase, vtable 0x012C32F0, ~ 0x00E241B0
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) {}   // FableWin-only: retail expands it inline

class CScriptBase; class CPersistContext;
class CEntityScriptBase { public:                       // global PDB class, retail 0x14 bytes
    virtual ~CEntityScriptBase();                       // slot 0  0x00F35B40
    virtual void Main() = 0;                            // slot 1 (vtable 0x012C3224, engine/CEntityScriptBase.h)
    virtual void Init();                                // slot 2  0x00CDEBB0
    virtual CScriptBase* GetParentScript() const = 0;   // slot 3
    virtual void OnPersist(CPersistContext&);           // slot 4  0x00CDEBC0
    virtual void OnPredicateFail();                     // slot 5  0x00CDEBD0
    virtual void OnInterrupted();                       // slot 6  0x00CDEBE0
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    void SetPredicate(const NScript::CPredicateBase& predicate);   // 0x00F35B10
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
};

namespace NScript {

class CV_TrophyDealerScript { public:
    struct CTrophyDealerInCave;                         // Ego_r.pdb / FableWin.pdb: nested entity script
};
class CQ_SunnyvaleMasterData;
struct CV_TrophyDealerScript::CTrophyDealerInCave : CEntityScriptBase {
    CV_TrophyDealerScript*  ParentClass;                // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                // +0x18
private:
    virtual void Main();                                // private virtual (Ego_r / FableWin EAEXXZ)
};

void CV_TrophyDealerScript::CTrophyDealerInCave::Main()
{
    CScriptGameResourceObjectScriptedThingBase trophy;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->EntitySetAsKillable(Me, false, true);
    Game->EntitySetAsDamageable(Me, false);
    {
        CScriptGameResourceObjectScriptedThingBase hero;
        Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4);
        if (trophy.IsValid())
            trophy.Release();
        while (!Game->StartScriptingEntity(Me, trophy, (EScriptAIPriority)4)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        CCutsceneCastMap cast;
        cast["Hero"] = hero;
        cast["Trophy"] = trophy;
        {
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CGameScriptInterface* game = Game;
            game->PauseAllNonScriptedEntities(true);
            RunCutsceneMacro("CS_TROPHY_DEALER", cast, 0, 0, false, true);
            game->PauseAllNonScriptedEntities(false);
        }
    }
    Game->SetCreatureGeneratorsEnabled("Witchwood2", true);
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, false, false);
    Game->FadeScreenIn();
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}
}