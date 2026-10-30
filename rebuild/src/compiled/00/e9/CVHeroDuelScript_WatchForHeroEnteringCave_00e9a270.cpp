// CV_HeroDuelScript::WatchForHeroEnteringCave() -- once the duel has started wait for the hero to reach the cave, take control of Thunder and the hero and play the cave-start cutscene (which one depends on who got there first)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E9A270, 0x347 bytes, plain `ret`. PDB ?WatchForHeroEnteringCave@CV_HeroDuelScript@NScript@@QAEXXZ.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, IsActiveThreadTerminating 0x00CB7940,
//   ~CScriptThing 0x004AA840, scripted-thing handle ctor 0x007E72A0 / dtor 0x007E74D0 / operator= 0x008ABD10 /
//   IsValid 0x00CD23B9 / Release 0x00CD2770, cast map ctor 0x00CDBF70 / dtor 0x00CDBFB0 / operator[] 0x00CD3D2E,
//   movie ctor 0x006E7B60 / dtor 0x006E7B80, RunCutsceneMacro 0x00CBFB7D (__fastcall); CGameScriptInterface slots 7, 8,
//   11, 70, 72, 370, 371, 379, 512.
// Strings: "HeroDualAreaCave" 0x012ED198, "Thunder" 0x012ED164, "THUNDER" 0x012CC504, "HERO" 0x01255174, "" 0x0122D70E,
//   "CS_GIBBETHILL_HERODUEL_CAVESTART_1" 0x012ED3B0, "CS_GIBBETHILL_HERODUEL_CAVESTART_2" 0x012ED38C.
// Shape: the level wait is `while (!loaded && !MissionOver)` (the level-name temporary gets a conditional-destruction
//   flag); the cutscene follows the landed CQ_MinionCampScript::DoMission 0x00DA0230 template (`game` local in edi
//   for the pause pair); each arm's terminate exit unpauses first, and VC7.1 tail-merges the two RunCutsceneMacro calls.
// Layout (retail = Ego_r.pdb - 0x14): Game +0x40, MissionOver +0x4a, DuelStarted +0x4c, ThunderEnteredCaveFirst +0x4f.
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)

class CCPPointerInfo { public:                                  // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);           // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    void Release()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            Release();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class C3DVector { public: float X, Y, Z; };
extern C3DVector g_ZeroVector;                           // 0x0143E8E0
struct CScriptThingImp {                                 // the game-side thing behind a CScriptThing
    virtual void i0(); virtual void i1(); virtual void i2(); virtual void i3(); virtual void i4(); virtual void i5();
    virtual const C3DVector& GetPosition() const;        // slot 6 (+0x18)
};
class CScriptThing : public CBaseClass { public:                       // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThingImp> PImp;               // +0x04 Data, +0x08 Info
    CScriptThing() {}
    virtual ~CScriptThing();                             // 0x004AA840
    virtual void s01(); virtual void s02(); virtual void s03(); virtual void s04(); virtual void s05(); virtual void s06(); virtual void s07(); virtual void s08(); virtual void s09(); virtual void s10();
    virtual void s11(); virtual void s12(); virtual void s13(); virtual void s14(); virtual void s15(); virtual void s16(); virtual void s17(); virtual void s18(); virtual void s19(); virtual void s20();
    virtual void s21(); virtual void s22(); virtual void s23(); virtual void s24(); virtual void s25(); virtual void s26(); virtual void s27(); virtual void s28(); virtual void s29(); virtual void s30();
    virtual void s31(); virtual void s32(); virtual void s33(); virtual void s34(); virtual void s35(); virtual void s36(); virtual void s37(); virtual void s38(); virtual void s39(); virtual void s40();
    virtual void s41(); virtual void s42(); virtual void s43(); virtual void s44(); virtual void s45(); virtual void s46(); virtual void s47(); virtual void s48(); virtual void s49(); virtual void s50();
    virtual void s51(); virtual void s52(); virtual void s53(); virtual void s54(); virtual void s55(); virtual void s56(); virtual void s57(); virtual void s58(); virtual void s59(); virtual void s60();
    virtual void s61(); virtual void s62(); virtual void s63(); virtual void s64(); virtual void s65(); virtual void s66(); virtual void s67(); virtual void s68(); virtual void s69(); virtual void s70();
    virtual void s71(); virtual void s72(); virtual void s73(); virtual void s74();
    virtual bool IsAlive() const;                        // slot 75 (+0x12c) 0x004AB130
    const C3DVector& GetPosition() const { return !PImp.Data ? g_ZeroVector : PImp.Data->GetPosition(); }
    __forceinline CScriptThing& operator=(const CScriptThing& other)
    {
        PImp = other.PImp;
        return *this;
    }
};
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
class CScriptGameResourceObjectScriptedThingBase { public:     // vtable 0x0127094C, 0x10 bytes
    virtual void h000();
    unsigned char _body[0xc];
    CScriptGameResourceObjectScriptedThingBase();       // 0x007E72A0
    virtual ~CScriptGameResourceObjectScriptedThingBase();      // 0x007E74D0
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other);   // 0x008ABD10
    bool IsValid() const;                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
};
class CScriptGameResourceObjectMovieBase { public:             // vtable 0x01260EF4, 0x10 bytes
    virtual void m000();
    unsigned char _body[0xc];
    CScriptGameResourceObjectMovieBase();               // 0x006E7B60
    virtual ~CScriptGameResourceObjectMovieBase();              // 0x006E7B80
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
struct CV_HeroDuelScript : CScriptBase {
    CGameScriptInterface* Game;                          // +0x40
    void* PMasterData;                                   // +0x44
    bool MissionSucceeded;                               // +0x48
    bool MissionFailed;                                  // +0x49
    bool MissionOver;                                    // +0x4a
    bool GotToExecutionTree;                             // +0x4b
    bool DuelStarted;                                    // +0x4c
    bool ThunderDefeated;                                // +0x4d
    bool OkToShowHealthBar;                              // +0x4e
    bool ThunderEnteredCaveFirst;                        // +0x4f
    void WatchForHeroEnteringCave();
};

void CV_HeroDuelScript::WatchForHeroEnteringCave()
{
    while (!DuelStarted) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!Game->IsLevelLoaded("HeroDualAreaCave") && !MissionOver) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (MissionOver)
        return;
    if (IsActiveThreadTerminating())
        return;
    CScriptThing thunder = Game->GetThingWithScriptName("Thunder");
    CScriptGameResourceObjectScriptedThingBase thunderHandle;
    Game->StartScriptingEntity(thunder, thunderHandle, (EScriptAIPriority)4);
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
    cast["THUNDER"] = thunderHandle;
    cast["HERO"] = hero;
    CScriptGameResourceObjectMovieBase movie;
    Game->StartMovieSequence("", movie);
    CGameScriptInterface* game = Game;
    game->PauseAllNonScriptedEntities(true);
    Game->FixMovieSequenceCamera(true);
    Game->EntitySetCutsceneBehaviour(thunder, (ECutsceneBehaviour)2);
    if (ThunderEnteredCaveFirst) {
        if (IsActiveThreadTerminating()) {
            game->PauseAllNonScriptedEntities(false);
            return;
        }
        RunCutsceneMacro("CS_GIBBETHILL_HERODUEL_CAVESTART_1", cast, 0, 0, false, true);
    } else {
        if (IsActiveThreadTerminating()) {
            game->PauseAllNonScriptedEntities(false);
            return;
        }
        RunCutsceneMacro("CS_GIBBETHILL_HERODUEL_CAVESTART_2", cast, 0, 0, false, true);
    }
    Game->EntitySetCutsceneBehaviour(thunder, (ECutsceneBehaviour)2);
    Game->FixMovieSequenceCamera(false);
    game->PauseAllNonScriptedEntities(false);
}