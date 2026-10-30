// CQ_WizardBattleScript::PhaseOne() -- mark the end-game barrier on the minimap and wait for the hero to reach it; have the hero chant and wait for the line to end; remove the barrier and its quest, then wait for the hero at the intro kick-off point, play the battle-intro cut-scene, bind Maze, switch the music and objectives over to the Hook Coast phase
// Leaf: PhaseOne
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E1CFD0, 1638 bytes (int3 padded to 1648).
// Callees: CCharString 0x0099EBF0 / ~ 0x0099EAE0, IsActiveThreadTerminating 0x00CB7940, IsDistanceBetweenThingsUnder
//   0x00CBE2FF, ~CScriptThing 0x004AA840 (and its inline body), CScriptThing::operator= 0x008AB980, handle 0x007E72A0 /
//   0x007E74D0 / operator= 0x008ABD10, cast map 0x00CDBF70 / 0x00CDBFB0 / operator[] 0x00CD3D2E, movie 0x006E7B60 /
//   0x006E7B80, RunCutsceneMacro 0x00CBFB7D; Game slots 7, 8, 70, 72, 108, 280, 296, 321, 350, 364, 366, 368, 370, 379,
//   565, 655, 696.
// Layout (retail = Ego_r.pdb - 0x14): VillagersTurnOnHero +0x48, Maze +0x5c.
// Shape: only the first wait loop's return keeps the inline ~CScriptThing bodies; `#pragma inline_depth(0)` around the
//   other returns and over the tail reproduces the out-of-line 0x004AA840 calls.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const char* text);           // 0x0099EFE0
};
class CWideString { public:
    void* PStringData;
    CWideString(const unsigned short* text);            // 0x0099B6B0
    ~CWideString();                                     // 0x0099B510
};
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class C3DVector { public: float X, Y, Z; };
struct CScriptThingImpl;
class CScriptThing : public CBaseClass { public:                      // 12 bytes, vtable 0x01238C8C
    virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005();
    virtual const C3DVector& GetPos() const;            // slot 6 (+0x18) 0x004AA980
    virtual void t007(); virtual void t008(); virtual void t009(); virtual void t010(); virtual void t011(); virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015(); virtual void t016(); virtual void t017(); virtual void t018(); virtual void t019(); virtual void t020(); virtual bool MsgIsHitBy(const CCharString& by) const; virtual void t022(); virtual void t023(); virtual void t024(); virtual void t025(); virtual void t026(); virtual void t027(); virtual void t028(); virtual void t029(); virtual void t030(); virtual void t031(); virtual void t032(); virtual void t033(); virtual void t034(); virtual void t035(); virtual void t036(); virtual void t037(); virtual void t038(); virtual void t039(); virtual void t040(); virtual bool MsgIsHitBySpecialAbilityFrom(long ability, const CCharString& by) const; virtual bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& by) const; virtual void t043(); virtual void t044(); virtual void t045(); virtual void t046(); virtual void t047(); virtual void t048(); virtual void t049(); virtual void t050(); virtual void t051(); virtual void t052(); virtual void t053(); virtual void t054(); virtual void t055(); virtual void t056(); virtual void t057(); virtual void t058(); virtual void t059(); virtual void t060(); virtual void t061(); virtual void t062(); virtual void t063(); virtual void t064(); virtual void t065(); virtual void t066(); virtual void t067(); virtual void t068(); virtual void t069(); virtual void t070(); virtual void t071(); virtual void t072(); virtual void t073(); virtual void t074();
    virtual bool IsAlive() const;                       // slot 75 (+0x12c) 0x004AB130
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04 Data, +0x08 Info
#ifdef THING_INLINE_COPY
    CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) {}
#else
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
#endif
    CScriptThing() {}
    CScriptThing& operator=(const CScriptThing& other);   // 0x008AB980
    ~CScriptThing() {}
};
namespace std {
template<class T> inline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}
template<class T> class allocator { };
template<class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
#ifndef VEC_DTOR
#define VEC_DTOR __forceinline
#endif
    VEC_DTOR ~vector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
    T* erase(T* first, T* last);                        // 0x008AC9D0 for CScriptThing
    void clear() { erase(_M_start, _M_finish); }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return _M_start[n]; }
};
}
class CScriptGameResourceObjectScriptedThingBase { public:    // vtable 0x0127094C, 0x10 bytes
    void* _body[4];
    void Construct();                                  // 0x007E72A0 (out-of-line ctor)
    void Destroy();                                    // 0x007E74D0 (out-of-line dtor)
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other);   // 0x008ABD10
};
class CScriptGameResourceObjectMovieBase { public:            // vtable 0x01260EF4, 0x10 bytes
    void* _body[4];
    void Construct();                                  // 0x006E7B60
    void Destroy();                                    // 0x006E7B80
};
struct CCutsceneCastMap {                              // map<CCharString, CScriptGameResourceObjectScriptedThingBase>
    void* _body[3];
    void Construct();                                  // 0x00CDBF70
    void Destroy();                                    // 0x00CDBFB0
    CScriptGameResourceObjectScriptedThingBase& operator[](const CCharString& key);   // 0x00CD3D2E
};
struct CCutsceneFlagMap;
struct CCutsceneArgMap;
void __fastcall RunCutsceneMacro(const CCharString& name, CCutsceneCastMap& cast, CCutsceneFlagMap* flags, CCutsceneArgMap* args, bool a5, bool a6);   // 0x00CBFB7D
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
namespace NScript {
class CQ_WizardBattleScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void*                 PMasterData;                  // +0x44
    bool VillagersTurnOnHero;                           // +0x48
    bool VillagersChased;                               // +0x49
    bool FightersStart;                                 // +0x4a
    bool MissionSucceeded;                              // +0x4b
    bool MissionFailed;                                 // +0x4c
    bool MissionOver;                                   // +0x4d
    bool QuestStartScreenShown;                         // +0x4e
    bool MazeDefeated;                                  // +0x4f
    long MazePhase;                                     // +0x50
    long NumberOfScreamersAlive;                        // +0x54
    long MazeConversation;                              // +0x58
    CScriptThing Maze;                                  // +0x5c
    CScriptThing TopOfLighthouse;                       // +0x68
    CScriptThing MiddleOfLighthouse;                    // +0x74
    void PhaseOne();
};
} // namespace NScript
using NScript::CQ_WizardBattleScript;

void NScript::CQ_WizardBattleScript::PhaseOne()
{
    CScriptThing ruinStairs = Game->GetThingWithScriptName("HKC_RuinStairs");
    CScriptThing statueMarker = Game->GetThingWithScriptName("HKC_StatueMarker");
    CScriptThing spawnPoint = Game->GetThingWithScriptName("HKC_AbbeyHiddenSpawnPoint");
    CScriptThing barrier = Game->GetThingWithScriptName("EndGameBarrier");
    Game->MiniMapAddMarker(barrier, "HUD_ORB_QUEST_CORE");
    while (!IsDistanceBetweenThingsUnder(Game->GetHero(), barrier, 5.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
    {
        return;
    }
    long conversation = Game->AddNewConversation(Game->GetHero(), false, false);
    Game->AddLineToConversation(conversation, "TEXT_CS_075_GM_CHANT", false, Game->GetHero(), Game->GetHero());
    while (Game->IsConversationActive(conversation)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
        {
            return;
        }
    }
    if (IsActiveThreadTerminating())
    {
        return;
    }
    CScriptThing endBarrier = Game->GetThingWithScriptName("EndGameBarrier");
    Game->RemoveThing(endBarrier, false, true);
    Game->DeactivateQuest("Q_EndGame_Barriers", 0);
    VillagersTurnOnHero = true;
    while (!IsDistanceBetweenThingsUnder(Game->GetHero(), Game->GetThingWithScriptName("WIZARD_INTRO_KICKOFF"), 8.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
        {
            return;
        }
    }
    if (IsActiveThreadTerminating())
        return;
    {
    CScriptGameResourceObjectScriptedThingBase hero;
    hero.Construct();
    Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4);
    CCutsceneCastMap cast;
    cast.Construct();
    cast["Hero"] = hero;
    CScriptGameResourceObjectMovieBase movie;
    movie.Construct();
    Game->StartMovieSequence("", movie);
    CGameScriptInterface* game = Game;
    game->PauseAllNonScriptedEntities(true);
    RunCutsceneMacro("CS_WIZARD_BATTLE_INTRO", cast, 0, 0, false, true);
    game->PauseAllNonScriptedEntities(false);
    movie.Destroy();
    cast.Destroy();
    hero.Destroy();
    }
    Maze = Game->GetThingWithScriptName("HKC_Maze");
    Game->OverrideMusic((EMusicSetType)0x17, false, false);
    Game->SetTeleportingAsActive(false);
    Game->DisplayQuestInfo(true);
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_WIZARD_BATTLE_OBJECTIVE_02", "HookCoast", "HookCoast");
}