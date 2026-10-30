// NScript::CV_TravellingHeroesScript::CTH_Maze::Main() -- Maze (the rival hero) after his scene: alive filter on Me, keep Me persistent (removed if the maze cutscene already ran), wait for the maze cutscene, and if another CREATURE_RIVAL_HERO_MAZE exists teleport it onto Me and remove Me; otherwise take scripted control and, while the hero stays within 20 units, answer his talk (or his hits) with an after-scene line inside a paused movie sequence; remove Me once he walks away
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE1E40, 1602 bytes (+ int3 padding); entity-script vtable slot 1 (Main);
//   PDB ?Main@CTH_Maze@CV_TravellingHeroesScript@NScript@@EAEXXZ.
// Callees: CScriptThing copy 0x004ABE90 / dtor 0x004AA840 / operator== 0x008AAE20, ~vector<CScriptThing> 0x008AC970,
//   ~CIsThingAlive 0x00E241B0 (predicate vtable 0x012C32F0), SetPredicate 0x00F35B10, IsActiveThreadTerminating
//   0x00F35B30, CBaseIntelligentPointer 0x0099A380 / 0x0099A430, scripted-thing handle IsValid 0x00CD23B9 / Release
//   0x00CD2770 / GetScriptThing 0x007E7490 / Speak 0x007E7390 / IsPerformingScriptTask 0x007E7450 / dtor 0x007E74D0, movie
//   handle out-of-line ctor 0x006E7B60 / dtor 0x006E7B80 (counted-pointer member dtor 0x006E7AB0), CCharString 0x0099EBF0 /
//   0x0099EAE0, IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall); CScriptThing slots 21, 27, 41, 42; GameCalls slots 7,
//   8, 70, 80 GetAllThingsWithDefName, 108, 264, 370, 379, 472, 477.
// Strings: "CREATURE_RIVAL_HERO_MAZE" 0x012CE65C, "SCRIPT_NAME_HERO" 0x0125D1C8, "" 0x0122D70E,
//   "TEXT_QST_B14_MAZE_AFTERSCENE_TALK" 0x012F44E8, "TEXT_QST_B14_MAZE_AFTERSCENE_HIT" 0x012F44C4.
// Layout: Ego_r.pdb CTH_Maze (0x1c): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18.
//   CV_TravellingHeroesScript sits 0x14 below the Ego_r.pdb offsets: MazeCutsceneDone +0x7b.
// Shape: the CTH_ThunderBoy row 0x00EDF3A0 (same TU) with a local scripted-thing handle (its virtual calls devirtualised);
//   the talk and hit blocks each run a paused movie sequence (VC7.1 expands the movie ctor/dtor inline only in the first).
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)
class C3DVector;
class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
struct CScriptThingImpl;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
template <class T> class CCountedPointer { public:             // {Data, Info}
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
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
class CScriptThing : public CBaseClass { public:                     // 12 bytes, vtable 0x01238C8C
    virtual void s001(); virtual void s002(); virtual void s003(); virtual void s004(); virtual void s005(); virtual void s006(); virtual void s007(); virtual void s008();
    virtual void s009(); virtual void s010(); virtual void s011(); virtual void s012(); virtual void s013(); virtual void s014(); virtual void s015(); virtual void s016();
    virtual void s017(); virtual void s018(); virtual void s019(); virtual void s020();
    virtual bool MsgIsHitBy(const CCharString& name) const;   // slot 21 (+0x54)
    virtual void s022(); virtual void s023(); virtual void s024(); virtual void s025(); virtual void s026();
    virtual bool MsgIsTalkedToBy(const CCharString& name) const;   // slot 27 (+0x6c)
    virtual void s028(); virtual void s029(); virtual void s030(); virtual void s031(); virtual void s032(); virtual void s033(); virtual void s034(); virtual void s035();
    virtual void s036(); virtual void s037(); virtual void s038(); virtual void s039(); virtual void s040();
    virtual bool MsgIsHitBySpecialAbilityFrom(long ability, const CCharString& name) const;   // slot 41 (+0xa4)
    virtual bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& name) const;   // slot 42 (+0xa8)
    CCountedPointer<CScriptThingImpl> PImp;            // +0x04 Data, +0x08 Info
    struct InlineCopy { };                             // tag: the inline copy retail expands at by-value call sites
    CScriptThing(const CScriptThing& other);           // 0x004ABE90 (the out-of-line copy)
    CScriptThing(const CScriptThing& other, InlineCopy) : CBaseClass(), PImp(other.PImp) { }
    ~CScriptThing() { }                                // out-of-line copy 0x004AA840
    bool operator==(const CScriptThing& other) const;  // 0x008AAE20
};
template <class T> struct vector {                     // STLport vector (retail 12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector();                                         // 0x008AC970 for CScriptThing
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) { return *(_M_start + n); }
};
namespace NScript {
class __declspec(novtable) CPredicateBase
{
public:
    virtual void v00();
};
}
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                                 // vtable 0x012C32F0
    CScriptThing m_thing;                              // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive();                                  // 0x00E241B0
};
}
using NScript::CIsThingAlive;
typedef long EScriptAIPriority;
typedef long EScriptEntityMoveType;
typedef long ETextGroupSelectionMethod;
typedef long EGameflowPosition;
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CScriptGameResourceObjectBase : public CBaseObject { public:   // vtable 0x0126008C
    CScriptGameResourceObjectBase() { }
    __forceinline ~CScriptGameResourceObjectBase() { }
};
struct CResourcePImp {                                 // the handles' counted implementation pointer
    void* Data;
    void* Info;
    CResourcePImp() : Data(0), Info(0) { }
    ~CResourcePImp();                                  // 0x006E7AB0
};
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:   // vtable 0x0127094C, 0x10 bytes
    virtual void h001(); virtual void h002(); virtual void h003(); virtual void h004();
    virtual void MoveToThing(const CScriptThing& thing, float speed, EScriptEntityMoveType moveType, bool a, bool b, bool c, bool d);   // slot 5 (+0x14)
    virtual void h006(); virtual void h007(); virtual void h008(); virtual void h009(); virtual void h010(); virtual void h011();
    virtual CScriptThing GetScriptThing() const;   // slot 12 (+0x30)
    virtual void Speak(const CScriptThing& listener, const char* text, ETextGroupSelectionMethod method, bool a, bool b, bool c);   // slot 13 (+0x34)
    virtual void h014(); virtual void h015(); virtual void h016(); virtual void h017(); virtual void h018(); virtual void h019(); virtual void h020(); virtual void h021();
    virtual void h022(); virtual void h023(); virtual void h024(); virtual void h025();
    virtual bool IsPerformingScriptTask() const;   // slot 26 (+0x68)
    CResourcePImp PImp;                                // +0x08
    CScriptGameResourceObjectScriptedThingBase() { }
    virtual ~CScriptGameResourceObjectScriptedThingBase();   // 0x007E74D0
    bool IsValid() const;                                // 0x00CD23B9
    void Release();                                    // 0x00CD2770
};
class CScriptGameResourceObjectMovieBase : public CScriptGameResourceObjectBase { public:   // vtable 0x01260EF4, 0x10 bytes
    CResourcePImp PImp;                                // +0x08
    CScriptGameResourceObjectMovieBase() { }
    // implicit destructor; out-of-line copies: ctor 0x006E7B60, dtor 0x006E7B80
};
struct GameCalls {                                     // CGameScriptInterface vtable 0x01260F0C (ghidra_out/fable_cgsi_slots.h)
    virtual void g000(); virtual void g001(); virtual void g002(); virtual void g003(); virtual void g004(); virtual void g005(); virtual void g006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& handle, EScriptAIPriority priority);   // slot 8 (+0x20)
    virtual void g009(); virtual void g010(); virtual void g011(); virtual void g012(); virtual void g013(); virtual void g014(); virtual void g015(); virtual void g016();
    virtual void g017(); virtual void g018(); virtual void g019(); virtual void g020(); virtual void g021(); virtual void g022(); virtual void g023(); virtual void g024();
    virtual void g025(); virtual void g026(); virtual void g027(); virtual void g028(); virtual void g029(); virtual void g030(); virtual void g031(); virtual void g032();
    virtual void g033(); virtual void g034(); virtual void g035(); virtual void g036(); virtual void g037(); virtual void g038(); virtual void g039(); virtual void g040();
    virtual void g041(); virtual void g042(); virtual void g043(); virtual void g044(); virtual void g045(); virtual void g046(); virtual void g047(); virtual void g048();
    virtual void g049(); virtual void g050(); virtual void g051(); virtual void g052(); virtual void g053(); virtual void g054(); virtual void g055(); virtual void g056();
    virtual void g057(); virtual void g058(); virtual void g059(); virtual void g060(); virtual void g061(); virtual void g062(); virtual void g063(); virtual void g064();
    virtual void g065(); virtual void g066(); virtual void g067(); virtual void g068(); virtual void g069();
    virtual CScriptThing& GetHero();   // slot 70 (+0x118)
    virtual void g071(); virtual void g072(); virtual void g073(); virtual void g074(); virtual void g075(); virtual void g076(); virtual void g077(); virtual void g078();
    virtual void g079();
    virtual long GetAllThingsWithDefName(const CCharString& defName, vector<CScriptThing>& things) const;   // slot 80 (+0x140)
    virtual void g081(); virtual void g082(); virtual void g083(); virtual void g084(); virtual void g085(); virtual void g086(); virtual void g087(); virtual void g088();
    virtual void g089(); virtual void g090(); virtual void g091(); virtual void g092(); virtual void g093(); virtual void g094(); virtual void g095(); virtual void g096();
    virtual void g097(); virtual void g098(); virtual void g099(); virtual void g100(); virtual void g101(); virtual void g102(); virtual void g103(); virtual void g104();
    virtual void g105(); virtual void g106(); virtual void g107();
    virtual void RemoveThing(const CScriptThing& thing, bool a, bool b) const;   // slot 108 (+0x1b0)
    virtual void g109(); virtual void g110(); virtual void g111(); virtual void g112(); virtual void g113(); virtual void g114(); virtual void g115(); virtual void g116();
    virtual void g117(); virtual void g118(); virtual void g119(); virtual void g120(); virtual void g121(); virtual void g122(); virtual void g123(); virtual void g124();
    virtual void g125(); virtual void g126(); virtual void g127(); virtual void g128(); virtual void g129(); virtual void g130(); virtual void g131(); virtual void g132();
    virtual void g133(); virtual void g134(); virtual void g135(); virtual void g136(); virtual void g137(); virtual void g138(); virtual void g139(); virtual void g140();
    virtual void g141(); virtual void g142(); virtual void g143(); virtual void g144(); virtual void g145(); virtual void g146(); virtual void g147(); virtual void g148();
    virtual void g149(); virtual void g150(); virtual void g151(); virtual void g152(); virtual void g153(); virtual void g154(); virtual void g155(); virtual void g156();
    virtual void g157(); virtual void g158(); virtual void g159(); virtual void g160(); virtual void g161(); virtual void g162(); virtual void g163(); virtual void g164();
    virtual void g165(); virtual void g166(); virtual void g167(); virtual void g168(); virtual void g169(); virtual void g170(); virtual void g171(); virtual void g172();
    virtual void g173(); virtual void g174(); virtual void g175(); virtual void g176(); virtual void g177(); virtual void g178(); virtual void g179(); virtual void g180();
    virtual void g181(); virtual void g182(); virtual void g183(); virtual void g184(); virtual void g185(); virtual void g186(); virtual void g187(); virtual void g188();
    virtual void g189(); virtual void g190(); virtual void g191(); virtual void g192(); virtual void g193(); virtual void g194(); virtual void g195(); virtual void g196();
    virtual void g197(); virtual void g198(); virtual void g199(); virtual void g200(); virtual void g201(); virtual void g202(); virtual void g203(); virtual void g204();
    virtual void g205(); virtual void g206(); virtual void g207(); virtual void g208(); virtual void g209(); virtual void g210(); virtual void g211(); virtual void g212();
    virtual void g213(); virtual void g214(); virtual void g215(); virtual void g216(); virtual void g217(); virtual void g218(); virtual void g219(); virtual void g220();
    virtual void g221(); virtual void g222(); virtual void g223(); virtual void g224(); virtual void g225(); virtual void g226(); virtual void g227(); virtual void g228();
    virtual void g229(); virtual void g230(); virtual void g231(); virtual void g232(); virtual void g233(); virtual void g234(); virtual void g235(); virtual void g236();
    virtual void g237(); virtual void g238(); virtual void g239(); virtual void g240(); virtual void g241(); virtual void g242(); virtual void g243(); virtual void g244();
    virtual void g245(); virtual void g246(); virtual void g247(); virtual void g248(); virtual void g249(); virtual void g250(); virtual void g251(); virtual void g252();
    virtual void g253(); virtual void g254(); virtual void g255(); virtual void g256(); virtual void g257(); virtual void g258(); virtual void g259(); virtual void g260();
    virtual void g261(); virtual void g262(); virtual void g263();
    virtual float GetHealth(const CScriptThing& thing) const;   // slot 264 (+0x420)
    virtual void g265(); virtual void g266(); virtual void g267(); virtual void g268(); virtual void g269(); virtual void g270(); virtual void g271(); virtual void g272();
    virtual void g273(); virtual void g274(); virtual void g275(); virtual void g276(); virtual void g277(); virtual void g278(); virtual void g279(); virtual void g280();
    virtual void g281(); virtual void g282(); virtual void g283(); virtual void g284(); virtual void g285(); virtual void g286(); virtual void g287(); virtual void g288();
    virtual void g289(); virtual void g290(); virtual void g291(); virtual void g292(); virtual void g293(); virtual void g294(); virtual void g295(); virtual void g296();
    virtual void g297(); virtual void g298(); virtual void g299(); virtual void g300(); virtual void g301(); virtual void g302(); virtual void g303(); virtual void g304();
    virtual void g305(); virtual void g306(); virtual void g307(); virtual void g308(); virtual void g309(); virtual void g310(); virtual void g311(); virtual void g312();
    virtual void g313(); virtual void g314(); virtual void g315(); virtual void g316(); virtual void g317(); virtual void g318(); virtual void g319(); virtual void g320();
    virtual void g321(); virtual void g322(); virtual void g323(); virtual void g324(); virtual void g325(); virtual void g326(); virtual void g327(); virtual void g328();
    virtual void g329(); virtual void g330(); virtual void g331(); virtual void g332(); virtual void g333(); virtual void g334(); virtual void g335(); virtual void g336();
    virtual void g337(); virtual void g338(); virtual void g339(); virtual void g340(); virtual void g341(); virtual void g342(); virtual void g343(); virtual void g344();
    virtual void g345(); virtual void g346(); virtual void g347(); virtual void g348(); virtual void g349(); virtual void g350(); virtual void g351(); virtual void g352();
    virtual void g353(); virtual void g354(); virtual void g355(); virtual void g356(); virtual void g357(); virtual void g358(); virtual void g359(); virtual void g360();
    virtual void g361(); virtual void g362(); virtual void g363(); virtual void g364(); virtual void g365(); virtual void g366(); virtual void g367(); virtual void g368();
    virtual void g369();
    virtual void StartMovieSequence(const CCharString& name, CScriptGameResourceObjectMovieBase& movie);   // slot 370 (+0x5c8)
    virtual void g371(); virtual void g372(); virtual void g373(); virtual void g374(); virtual void g375(); virtual void g376(); virtual void g377(); virtual void g378();
    virtual void PauseAllNonScriptedEntities(bool pause);   // slot 379 (+0x5ec)
    virtual void g380(); virtual void g381(); virtual void g382(); virtual void g383(); virtual void g384(); virtual void g385(); virtual void g386(); virtual void g387();
    virtual void g388(); virtual void g389(); virtual void g390(); virtual void g391(); virtual void g392(); virtual void g393(); virtual void g394(); virtual void g395();
    virtual void g396(); virtual void g397(); virtual void g398(); virtual void g399(); virtual void g400(); virtual void g401(); virtual void g402(); virtual void g403();
    virtual void g404(); virtual void g405(); virtual void g406(); virtual void g407(); virtual void g408(); virtual void g409(); virtual void g410(); virtual void g411();
    virtual void g412(); virtual void g413(); virtual void g414(); virtual void g415(); virtual void g416(); virtual void g417(); virtual void g418(); virtual void g419();
    virtual void g420(); virtual void g421(); virtual void g422(); virtual void g423(); virtual void g424(); virtual void g425(); virtual void g426(); virtual void g427();
    virtual void g428(); virtual void g429(); virtual void g430(); virtual void g431(); virtual void g432(); virtual void g433(); virtual void g434(); virtual void g435();
    virtual void g436(); virtual void g437(); virtual void g438(); virtual void g439(); virtual void g440(); virtual void g441(); virtual void g442(); virtual void g443();
    virtual void g444(); virtual void g445(); virtual void g446(); virtual void g447(); virtual void g448(); virtual void g449(); virtual void g450(); virtual void g451();
    virtual void g452(); virtual void g453(); virtual void g454(); virtual void g455(); virtual void g456(); virtual void g457(); virtual void g458(); virtual void g459();
    virtual void g460(); virtual void g461(); virtual void g462(); virtual void g463(); virtual void g464(); virtual void g465(); virtual void g466(); virtual void g467();
    virtual void g468(); virtual void g469(); virtual void g470(); virtual void g471();
    virtual void EntityTeleportToThing(const CScriptThing& thing, const CScriptThing& target, bool a) const;   // slot 472 (+0x760)
    virtual void g473(); virtual void g474(); virtual void g475(); virtual void g476();
    virtual void SetThingPersistent(const CScriptThing& thing, bool persistent) const;   // slot 477 (+0x774)
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF

struct CEntityScriptBase {                             // retail 0x14 bytes
    virtual void e00();
    GameCalls*   Game;                                 // +0x04
    CScriptThing Me;                                   // +0x08
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_TravellingHeroesScript { public:
    unsigned char _pad[0x78];
    bool ThunderKidsTalking;                           // +0x78
    bool ThunderSceneStarted;                          // +0x79
    bool ThunderSceneDone;                             // +0x7a
    bool MazeCutsceneDone;                             // +0x7b
    class CTH_Maze;
};
class CV_TravellingHeroesScript::CTH_Maze : public CEntityScriptBase {
public:
    CV_TravellingHeroesScript* ParentClass;            // +0x14
    CQ_SunnyvaleMasterData*    PMasterData;            // +0x18
    void Main();
};
}

void NScript::CV_TravellingHeroesScript::CTH_Maze::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    Game->SetThingPersistent(Me, true);
    if (ParentClass->MazeCutsceneDone) {
        if (IsActiveThreadTerminating()) return;
        Game->RemoveThing(Me, false, true);
    }
    while (!ParentClass->MazeCutsceneDone) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    vector<CScriptThing> mazes;
    Game->GetAllThingsWithDefName("CREATURE_RIVAL_HERO_MAZE", mazes);
    if (mazes.size() > 1) {
        if (IsActiveThreadTerminating()) return;
        if (mazes[0] == Me) {
            if (IsActiveThreadTerminating()) return;
            Game->EntityTeleportToThing(mazes[1], Me, false);
        }
        else {
            if (IsActiveThreadTerminating()) return;
            Game->EntityTeleportToThing(mazes[0], Me, false);
        }
        Game->RemoveThing(Me, false, true);
    }
    CScriptGameResourceObjectScriptedThingBase scripted;
    if (scripted.IsValid())
        scripted.Release();
    while (!Game->StartScriptingEntity(Me, scripted, 4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    while (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 20.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            GameCalls* game = Game;
            game->PauseAllNonScriptedEntities(true);
            if (Game->GetHealth(scripted.GetScriptThing()) > 0.0f) {
                scripted.Speak(Game->GetHero(), "TEXT_QST_B14_MAZE_AFTERSCENE_TALK", 0, false, true, false);
                while (scripted.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) {
                        game->PauseAllNonScriptedEntities(false);
                        return;
                    }
                }
                if (IsActiveThreadTerminating()) {
                    game->PauseAllNonScriptedEntities(false);
                    return;
                }
            }
            game->PauseAllNonScriptedEntities(false);
        }
        else {
            bool hitByHero = Me.MsgIsHitBy("SCRIPT_NAME_HERO")
                          || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                              && !Me.MsgIsHitBySpecialAbilityFrom(14, "SCRIPT_NAME_HERO"));
            if (hitByHero) {
                if (IsActiveThreadTerminating()) return;
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("", movie);
                GameCalls* game = Game;
                game->PauseAllNonScriptedEntities(true);
                if (Game->GetHealth(scripted.GetScriptThing()) > 0.0f) {
                    scripted.Speak(Game->GetHero(), "TEXT_QST_B14_MAZE_AFTERSCENE_HIT", 0, false, true, false);
                    while (scripted.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) {
                            game->PauseAllNonScriptedEntities(false);
                            return;
                        }
                    }
                    if (IsActiveThreadTerminating()) {
                        game->PauseAllNonScriptedEntities(false);
                        return;
                    }
                }
                game->PauseAllNonScriptedEntities(false);
            }
        }
    }
    if (IsActiveThreadTerminating()) return;
    Game->RemoveThing(Me, false, true);
}