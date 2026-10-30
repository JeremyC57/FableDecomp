// CMazeJournal::Main() -- Gateway to Hook Coast maze journal: remove Me unless the maze is active; once, pause, pass the
//   guildmaster's "mother in struggle" message, wait for the hero at MK_HCM_HERO1 and play CS_GATEWAY_MOTHER, then set
//   objective 04; then each time the hero uses Me: hide Me, give the maze journal, show its game info, complete the quest
//   and make the guild map usable
// Leaf: ?Main@CMazeJournal@
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
int __cdecl rand();                                     // 0x00BFEB16
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
struct CScriptThingImpl;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (the row's local CBase)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing() {}
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
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
    CScriptGameResourceObjectScriptedThingBase();       // 0x007E72A0
    virtual ~CScriptGameResourceObjectScriptedThingBase();      // 0x007E74D0
    __forceinline CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other)
    {
        CBaseObject::operator=(other);
        PImp = other.PImp;
        return *this;
    }
    bool IsValid() const;                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
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
void __fastcall AddLogbookStoryEntry(long entry);       // 0x00CBE87F
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CEntityScriptBase { public:                              // retail 0x14 bytes
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
};
struct CQ_GatewayToHookCoastScript {
    unsigned char _pad[0x50];
    bool          MazeActive;                           // +0x50
    unsigned char _pad51;
    bool          MotherCutscenePlayed;                 // +0x52
};
struct CMazeJournal : CEntityScriptBase {
    CQ_GatewayToHookCoastScript* ParentClass;           // +0x14
    void Main();
};
static inline bool IsUsedBy(const CScriptThing& thing, const CCharString& user) { return thing.MsgIsUsedBy(user); }

void CMazeJournal::Main()
{
    if (!ParentClass->MazeActive) {
        if (IsActiveThreadTerminating())
            return;
        Game->RemoveThing(Me, false, false);
        return;
    }
    if (!ParentClass->MotherCutscenePlayed) {
        if (IsActiveThreadTerminating())
            return;
        Game->Pause(2.0f);
        Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_025_MOTHER_IN_STRUGGLE_10", "", true, true);
        CScriptThing marker = Game->GetThingWithScriptName("MK_HCM_HERO1");
        while (!IsDistanceBetweenThingsUnder(marker, Game->GetHero(), 5.0f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
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
            cast["HERO"] = hero;
            {
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("", movie);
                CGameScriptInterface* game = Game;
                game->PauseAllNonScriptedEntities(true);
                RunCutsceneMacro("CS_GATEWAY_MOTHER", cast, 0, 0, false, true);
                game->PauseAllNonScriptedEntities(false);
                movie.DestroyInline();
            }
        }
        ParentClass->MotherCutscenePlayed = true;
        Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_GATEWAY_TO_HOOK_COAST_OBJECTIVE_04", "HeroGuildComplexInside", "HeroGuildComplexInside");
    }
    while (!IsActiveThreadTerminating()) {
        while (!IsUsedBy(Me, "SCRIPT_NAME_HERO")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->EntitySetAsDrawable(Me, false);
        Game->GiveHeroObject("OBJECT_MAZE_JOURNAL", -1, false);
        while (!Game->IsObjectInThingsPossession("OBJECT_MAZE_JOURNAL", Game->GetHero())) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->DisplayGameInfo("TEXT_OBJECT_BOOKS_MAZE_JOURNAL");
        while (!Game->MsgIsGameInfoClickedPast()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, false, false);
        Game->SetThingAsUsable(Game->GetThingWithScriptName("MainGuildMap"), true);
        Game->SetHeroGuideToShowQuestCardsWhenSpokenTo(true);
        Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
        AddLogbookStoryEntry(0x104);
        Game->NewScriptFrame();
    }
}