// CMK_GHC_HERO3::Main() (0x00D29DB0) -- Gateway to Hook Coast, hero marker 3: fade in; once, pass the guildmaster's
//   "find the abbey" message; unless the abbey cut-scene already played, set the Hook Coast objective, wait until the hero
//   is within 15 of Me, take the hero under scripted control and play CS_GATEWAY_ABBEY in a paused movie sequence; then add
//   log-book story entry 250, mark the cut-scene played, remove Me's mini-map marker and set the next objective
// Leaf: ?Main@CMK_GHC_HERO3@
// Retail: Fable.exe @ 0x00D29DB0, 963 bytes (+ int3 padding), plain `ret`.
// Callees: IsActiveThreadTerminating 0x00F35B30, CCharString 0x0099EBF0 / 0x0099EAE0, IsDistanceBetweenThingsUnder
//   0x00CBE2FF (__fastcall), scripted-thing handle ctor 0x007E72A0 / dtor 0x007E74D0 / IsValid 0x00CD23B9 / Release
//   0x00CD2770, CBaseIntelligentPointer ctor 0x0099A380 / operator= 0x0099A3B0 / dtor 0x0099A430, the handle's
//   CCountedPointer operator= 0x008AB1E0, cast map ctor 0x00CDBF70 / dtor 0x00CDBFB0 / operator[] 0x00CD3D2E, movie PImp
//   dtor 0x006E7AB0, RunCutsceneMacro 0x00CBFB7D (__fastcall), AddLogbookStoryEntry 0x00CBE87F (__fastcall); GameCalls
//   slots 7 NewScriptFrame, 8 StartScriptingEntity, 70 GetHero, 274 HeroReceiveMessageFromGuildMaster, 296
//   SetQuestCardObjective, 352 MiniMapRemoveMarker, 370 StartMovieSequence, 374 FadeScreenIn, 379
//   PauseAllNonScriptedEntities, 655 GetActiveQuestName.
// Layout: entity-script base {vptr, Game +4, Me +8}, ParentClass +0x14 (flags +0x50 abbey cut-scene played, +0x53 message
//   given), PMasterData +0x18 (flag +0x60). Handle / map / movie classes as the landed CBowerstoneIntroGuard::Main
//   0x00D246C0, except that this TU calls the handle's ctor and dtor out of line.
// Shape: the movie handle is a destructor-less view destroyed by hand (Bowerstone idiom): retail expands its destructor
//   without the 0x01260EF4 vptr store (PImp dtor 0x006E7AB0, then the base vptr 0x0126008C and 0x0099A430), and VC7.1
//   stores that vptr in a user-written destructor and calls the implicit one out of line here.
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
__forceinline CScriptThing::~CScriptThing() {}
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
    bool          AbbeyCutscenePlayed;                  // +0x50
    unsigned char _pad51[2];
    bool          MotherMessageGiven;                   // +0x53
};
struct CGatewayMasterData {
    unsigned char _pad[0x60];
    bool          AbbeyCutscenePlayed;                  // +0x60
};
struct CMK_GHC_HERO3 : CEntityScriptBase {
    CQ_GatewayToHookCoastScript* ParentClass;           // +0x14
    CGatewayMasterData*          PMasterData;           // +0x18
    void Main();
};

void CMK_GHC_HERO3::Main()
{
    Game->FadeScreenIn();
    if (!ParentClass->MotherMessageGiven) {
        if (IsActiveThreadTerminating())
            return;
        Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_025_MOTHER_FIND_ABBEY_10", "", true, true);
        ParentClass->MotherMessageGiven = true;
    }
    if (ParentClass->AbbeyCutscenePlayed)
        return;
    if (IsActiveThreadTerminating())
        return;
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_GATEWAY_TO_HOOK_COAST_OBJECTIVE_02", "HookCoast", "HookCoast");
    while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 15.0f)) {
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
            RunCutsceneMacro("CS_GATEWAY_ABBEY", cast, 0, 0, false, true);
            game->PauseAllNonScriptedEntities(false);
            movie.DestroyInline();
        }
    }
    AddLogbookStoryEntry(250);
    ParentClass->AbbeyCutscenePlayed = true;
    PMasterData->AbbeyCutscenePlayed = true;
    Game->MiniMapRemoveMarker(Me);
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_GATEWAY_TO_HOOK_COAST_OBJECTIVE_03", "HeroGuildComplexInside", "HeroGuildComplexInside");
}
