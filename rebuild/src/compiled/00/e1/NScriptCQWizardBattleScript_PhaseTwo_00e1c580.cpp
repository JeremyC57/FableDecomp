// CQ_WizardBattleScript::PhaseTwo() -- start the maze-phase monitor, set Maze on the hero, show his health bar and make him killable; then while Maze lives, have him taunt the hero (on a hit, when the hero strays far away, or at random in battle) through a fresh conversation whenever none is running, until the mission fails
// Leaf: PhaseTwo
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E1C580, 1324 bytes (int3 padded to 1344).
// Callees: operator new 0x00BFEA1A, CSpawnedFuncBase 0x00CDD450 (spawn vtable 0x012E11CC, MonitorMazePhases 0x00E1CAE0),
//   AddSpawnedFunction 0x00CB7E50, CCharString 0x0099EBF0 / ~ 0x0099EAE0, IsActiveThreadTerminating 0x00CB7940,
//   rand 0x00BFEB16, IsDistanceBetweenThingsOver 0x00CBE3EA, ~CScriptThing 0x004AA840 (and inline), ~vector 0x008AC970,
//   ~CScriptGameResourceObjectScriptedThingBase 0x007E74D0 (and inline); CScriptThing slots 21, 41, 42, 75;
//   Game slots 7, 70, 264, 325, 364, 366, 368, 451, 517.
// Layout (retail = Ego_r.pdb - 0x14): MissionFailed +0x4c, MazeConversation +0x58, Maze +0x5c.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
#define VEC_DTOR
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
class CCPPointerInfo { public:                                 // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);          // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
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
class CBaseObject { public:                                    // vtable 0x0129A7D8
    void* IntelligentPointer;                           // +0x04
protected:
    CBaseObject();                                       // 0x0099A380
    virtual ~CBaseObject();                              // 0x0099A430
};
class CScriptGameResourceObjectBase : public CBaseObject { public:    // vtable 0x0126008C
    virtual ~CScriptGameResourceObjectBase() {}
};
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:   // vtable 0x0127094C
    CCountedPointer<void> PImp;                         // +0x08
    CScriptGameResourceObjectScriptedThingBase() {}
};
class CRGBColour { public:                                     // D3DCOLOR byte order
    unsigned char B, G, R, A;
    __forceinline CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
template <class T, class A> struct CSpawnedFunc1 : CSpawnedFuncBase {
    void (T::*Func)(A&);                                // +0x34
    T* PClass;                                          // +0x38
    A* Arg;                                             // +0x3c
    __forceinline CSpawnedFunc1(const CCharString& name, unsigned long id, T* pclass, void (T::*func)(A&), A& arg)
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass), Arg(&arg) {}
    virtual void SuspendableProcess() { (PClass->*Func)(*Arg); }
};
extern "C" int __cdecl rand();
bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE3EA
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
    __forceinline void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
};
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
    void PhaseTwo();
    void MonitorMazePhases(CScriptThing& maze);         // 0x00E1CAE0
};
} // namespace NScript
using NScript::CQ_WizardBattleScript;

void NScript::CQ_WizardBattleScript::PhaseTwo()
{
    SpawnFunction(new CSpawnedFunc1<CQ_WizardBattleScript, CScriptThing>("MonitorMazePhases", 0, this, &NScript::CQ_WizardBattleScript::MonitorMazePhases, Maze));
    CScriptThing thing;
    CScriptGameResourceObjectScriptedThingBase object;
    Game->GiveThingBestEnemyTarget(Maze, Game->GetHero());
    Game->AddQuestInfoBarHealth(Maze, CRGBColour(255, 0, 0, 255), "HUD_QUEST_ICON_MAZE", 1.0f);
    Game->EntitySetAsKillable(Maze, true, true);
    Game->GetHealth(Maze);
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    std::vector<CScriptThing> things;
#pragma inline_depth(0)
    while (Maze.IsAlive()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        bool hit = Maze.MsgIsHitBy("SCRIPT_NAME_HERO") || (Maze.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Maze.MsgIsHitBySpecialAbilityFrom(14, "SCRIPT_NAME_HERO"));
        if (hit) {
            if (IsActiveThreadTerminating())
                return;
            if (rand() % 10 == 0 && !Game->IsConversationActive(MazeConversation)) {
                if (IsActiveThreadTerminating())
                    return;
                MazeConversation = Game->AddNewConversation(Maze, false, false);
                Game->AddLineToConversation(MazeConversation, "TEXT_QST_075_MAZE_TAKE_HIT", false, Maze, Game->GetHero());
            }
        }
        if (IsDistanceBetweenThingsOver(Game->GetHero(), Maze, 30.0f)) {
            if (IsActiveThreadTerminating())
                return;
            if (rand() % 200 == 0 && !Game->IsConversationActive(MazeConversation)) {
                if (IsActiveThreadTerminating())
                    return;
                MazeConversation = Game->AddNewConversation(Maze, false, false);
                Game->AddLineToConversation(MazeConversation, "TEXT_QST_075_MAZE_LOCATION_TAUNT", false, Maze, Game->GetHero());
            }
        } else {
            if (rand() % 100 == 0 && !Game->IsConversationActive(MazeConversation)) {
                if (IsActiveThreadTerminating())
                    return;
                MazeConversation = Game->AddNewConversation(Maze, false, false);
                Game->AddLineToConversation(MazeConversation, "TEXT_QST_075_MAZE_BATTLE_TAUNT", false, Maze, Game->GetHero());
            }
        }
        if (MissionFailed) {
            if (IsActiveThreadTerminating())
                return;
            return;
        }
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    if (MissionFailed)
        return;
}
#pragma inline_depth()