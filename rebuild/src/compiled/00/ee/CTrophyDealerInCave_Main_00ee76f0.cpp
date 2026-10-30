// CTrophyDealerInCave::Main() -- entity script: holds an alive filter on the trophy dealer, makes it unkillable and undamageable, takes the hero and the dealer under scripted control and plays CS_TROPHY_DEALER in a paused movie sequence, then re-enables the Witchwood2 creature generators and completes and retires the active quest
// Leaf: Main
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };

class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};

class CScriptThing { public:                                   // 12 bytes, vtable 0x01238C8C
    void* PImpData;
    void* PImpInfo;
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    virtual ~CScriptThing();                            // 0x004AA840
};

#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)

class CScriptGameResourceObjectBase : public CBaseObject { public:    // vtable 0x0126008C
    virtual __forceinline ~CScriptGameResourceObjectBase() {}
};

struct CCountedPointerHandle {                          // CCountedPointer<CScriptedThingImp>
    void*           Data;
    CCPPointerInfo* Info;
    CCountedPointerHandle() : Data(0), Info(0) {}
    __forceinline ~CCountedPointerHandle()
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

namespace NScript {
class CIsThingAlive { public:                                  // vtable 0x012C32F0: a filter holding a script thing
    virtual void f00();
    CScriptThing Thing;
    CIsThingAlive(const CScriptThing& thing) : Thing(thing) {}
    virtual ~CIsThingAlive();                                   // 0x00E241B0
};

struct CEntityScriptBase {
    virtual void e00();
    CGameScriptInterface* GSI;                          // +0x04
    CScriptThing          Me;                           // +0x08
    void SetPredicate(const CIsThingAlive& predicate);  // 0x00F35B10
    bool IsActiveThreadTerminating();                   // 0x00F35B30
};

class CV_TrophyDealerScript;
class CQ_SunnyvaleMasterData;
struct CTrophyDealerInCave : CEntityScriptBase {
    CV_TrophyDealerScript*  ParentClass;                // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                // +0x18
    void Main();
};

void CTrophyDealerInCave::Main()
{
    CScriptGameResourceObjectScriptedThingBase trophy;
    SetPredicate(CIsThingAlive(Me));
    GSI->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    GSI->EntitySetAsKillable(Me, false, true);
    GSI->EntitySetAsDamageable(Me, false);
    {
        CScriptGameResourceObjectScriptedThingBase hero;
        GSI->StartScriptingEntity(GSI->GetHero(), hero, (EScriptAIPriority)4);
        if (trophy.IsValid())
            trophy.Release();
        while (!GSI->StartScriptingEntity(Me, trophy, (EScriptAIPriority)4)) {
            GSI->NewScriptFrame();
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
        }
#pragma inline_depth(0)
        if (IsActiveThreadTerminating())
            return;
#pragma inline_depth()
        CCutsceneCastMap cast;
        cast["Hero"] = hero;
        cast["Trophy"] = trophy;
        {
            CScriptGameResourceObjectMovieBase movie;
            GSI->StartMovieSequence("", movie);
            CGameScriptInterface* game = GSI;
            game->PauseAllNonScriptedEntities(true);
            RunCutsceneMacro("CS_TROPHY_DEALER", cast, 0, 0, false, true);
            game->PauseAllNonScriptedEntities(false);
        }
#pragma inline_depth(0)
    }
#pragma inline_depth()
    GSI->SetCreatureGeneratorsEnabled("Witchwood2", true);
    GSI->SetQuestAsCompleted(GSI->GetActiveQuestName(), true, false, false);
    GSI->FadeScreenIn();
    GSI->DeactivateQuestLater(GSI->GetActiveQuestName(), 0);
#pragma inline_depth(0)
}
#pragma inline_depth()
}