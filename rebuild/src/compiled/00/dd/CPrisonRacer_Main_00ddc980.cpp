// CPrisonRacer::Main() -- a prisoner in the prison race: wait for the race start, take scripted control and set its running speed (slower for each racer), then every frame: every fourth frame find the nearest WaypointSpeedCheck marker and remember its index; sprint ("SPRINT" comment, faster speed) while behind the hero's nearest race marker, slow down again ("SLOW") when the sprint timer runs out; when idle follow the route to the nearest RaceMarker; stop when the hero won, and flag RaceEnded on reaching the warden
// Leaf: ?Main@CPrisonRacer@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DDC980, length 1477 bytes (+ int3 padding), plain `ret`.
// Callees: IsActiveThreadTerminating 0x00F35B30, CBaseIntelligentPointer ctor 0x0099A380, scripted-thing handle IsValid
//   0x00CD23B9 / Release 0x00CD2770 / FollowPreCalculatedRoute 0x007E7310 / ClearCommands 0x007E7360 / IsPerformingScriptTask
//   0x007E7450 / dtor 0x007E74D0, CScriptThing::GetDataString 0x004AA900 (slot 3) / operator= 0x008AB980 / dtor 0x004AA840,
//   ~vector<CScriptThing> 0x008AC970, ~CTimer 0x00CD4470, GetDistanceBetweenThings 0x00CBE512 /
//   IsDistanceBetweenThingsUnder 0x00CBE2FF / GFCharStringToInt 0x0099E7F0 (all __fastcall), MakePrisonerComment 0x00DDCF50
//   (catalog class CQ_PrisonEscapeRescueMotherScript; called on the racer's CQ_PrisonRaceScript), CCharString 0x0099EBF0 /
//   0x0099EAE0; g_pGameScriptInterface 0x0143E8F8 slots 87-90 (two inline CTimers); GameCalls (+0x04) slots 7, 8, 72, 75, 77,
//   488, 653. Data: pGlobalScriptDef 0x0143E90C (+0xDC8 racer speed, +0xDCC sprint speed, +0xDD0 sprint time, +0xDD4 rest time).
// Strings: "PrisonRaceWarden" 0x012DC920, "RaceMarker" 0x012CD708, "WaypointSpeedCheck" 0x012DC8E8, "SPRINT" 0x012DCAA4,
//   "SLOW" 0x012DCA9C; 0.02f 0x012C26F8.
// Layout: Ego_r.pdb CPrisonRacer (32 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (CQ_PrisonRaceScript,
//   retail = PDB - 0x14: HeroWonRace +0x48, RaceStarted +0x49, RaceEnded +0x4a, RacerSpeed +0x4c, NearestRaceMarker +0x58),
//   PMasterData +0x18, RaceWaypointIndex +0x1c. Entity-script models of the landed CGuardOutsideWoods::Main 0x00D874F0.
// Shape: the timers' destructors (DeregisterTimer) are inline at the main loop's own exits and out of line (0x00CD4470) at
//   the others (in-body inline_depth(0)); `if (!sprinting && behind) {..}` / `if (sprinting && timer done) {..}` lets VC
//   thread the known flag value between the two tests (each flag store sits before its timer restart). The main loop is
//   `for (;;) { if (terminating) return; ... NewScriptFrame(); }` so every inline exit shares one expansion.
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
long __fastcall GFCharStringToInt(const CCharString& text);   // 0x0099E7F0
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
#include "engine/CCountedPointer.h"
#define SLOTS_2_74 \
    virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005(); virtual void t006(); virtual void t007(); \
    virtual void t008(); virtual void t009(); virtual void t010(); virtual void t011(); virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015(); \
    virtual void t016(); virtual void t017(); virtual void t018(); virtual void t019(); virtual void t020(); virtual void t021(); virtual void t022(); virtual void t023(); \
    virtual void t024(); virtual void t025(); virtual void t026(); virtual void t027(); virtual void t028(); virtual void t029(); virtual void t030(); virtual void t031(); \
    virtual void t032(); virtual void t033(); virtual void t034(); virtual void t035(); virtual void t036(); virtual void t037(); virtual void t038(); virtual void t039(); \
    virtual void t040(); virtual void t041(); virtual void t042(); virtual void t043(); virtual void t044(); virtual void t045(); virtual void t046(); virtual void t047(); \
    virtual void t048(); virtual void t049(); virtual void t050(); virtual void t051(); virtual void t052(); virtual void t053(); virtual void t054(); virtual void t055(); \
    virtual void t056(); virtual void t057(); virtual void t058(); virtual void t059(); virtual void t060(); virtual void t061(); virtual void t062(); virtual void t063(); \
    virtual void t064(); virtual void t065(); virtual void t066(); virtual void t067(); virtual void t068(); virtual void t069(); virtual void t070(); virtual void t071(); \
    virtual void t072(); virtual void t073(); virtual void t074();
struct CScriptThingImpl {                               // the game-side object behind a CScriptThing (PImp)
    virtual void t000(); virtual void t001(); SLOTS_2_74
    virtual bool IsAlive() const;                       // slot 75 (+0x12c)
};
struct CBase {                                          // vtable 0x01231710
    virtual ~CBase();                                   // 0x0099A2E0
};
class CScriptThing : public CBase { public:                           // vtable 0x01238C8C, 12 bytes
    virtual void s001() const; virtual void s002() const;
    virtual CCharString GetDataString() const;          // slot 3 (+0x0c) 0x004AA900
    virtual void s004() const; virtual void s005() const; virtual void s006() const; virtual void s007() const;
    virtual void s008() const; virtual void s009() const; virtual void s010() const; virtual void s011() const;
    virtual void s012() const; virtual void s013() const; virtual void s014() const; virtual void s015() const;
    virtual void s016() const; virtual void s017() const; virtual void s018() const; virtual void s019() const;
    virtual void s020() const; virtual void s021() const; virtual void s022() const; virtual void s023() const;
    virtual void s024() const; virtual void s025() const; virtual void s026() const; virtual void s027() const;
    virtual void s028() const; virtual void s029() const; virtual void s030() const; virtual void s031() const;
    virtual void s032() const; virtual void s033() const; virtual void s034() const; virtual void s035() const;
    virtual void s036() const; virtual void s037() const; virtual void s038() const; virtual void s039() const;
    virtual void s040() const; virtual void s041() const; virtual void s042() const; virtual void s043() const;
    virtual void s044() const; virtual void s045() const; virtual void s046() const; virtual void s047() const;
    virtual void s048() const; virtual void s049() const; virtual void s050() const; virtual void s051() const;
    virtual void s052() const; virtual void s053() const; virtual void s054() const; virtual void s055() const;
    virtual void s056() const; virtual void s057() const; virtual void s058() const; virtual void s059() const;
    virtual void s060() const; virtual void s061() const; virtual void s062() const; virtual void s063() const;
    virtual void s064() const; virtual void s065() const; virtual void s066() const; virtual void s067() const;
    virtual void s068() const; virtual void s069() const; virtual void s070() const; virtual void s071() const;
    virtual void s072() const; virtual void s073() const; virtual void s074() const;
    virtual bool IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }   // slot 75 (+0x12c), out of line 0x004AB130
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    virtual ~CScriptThing();                                    // 0x004AA840
    CScriptThing& operator=(const CScriptThing& other); // 0x008AB980
};
namespace std {
template <class T> class allocator;
template <class T, class A = allocator<T> > class vector {
public:                      // STLport vector (retail 12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector();                                          // 0x008AC970
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) { return _M_start[n]; }
};
}
using std::vector;
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CScriptGameResourceObjectBase : public CBaseObject { public:   // vtable 0x0126008C
    CScriptGameResourceObjectBase() { }
    __forceinline ~CScriptGameResourceObjectBase() { }
};
enum EScriptEntityMoveType { SCRIPT_ENTITY_MOVE_TYPE_NONE };
struct CCountedVoidPointer { void* Data; void* Info; CCountedVoidPointer() : Data(0), Info(0) { } };
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:   // vtable 0x0127094C, 0x10 bytes
    CCountedVoidPointer PImp;                           // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectScriptedThingBase() { }
    ~CScriptGameResourceObjectScriptedThingBase();      // 0x007E74D0
    bool IsValid() const;                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
    void FollowPreCalculatedRoute(const CScriptThing& thing, EScriptEntityMoveType moveType, bool a, bool b);   // 0x007E7310
    void ClearCommands();                               // 0x007E7360
    bool IsPerformingScriptTask() const;                // 0x007E7450
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
class CScriptDef { public:
    unsigned char _pad000[0xdc8];
    float PrisonRacerSpeed;                             // +0xdc8
    float PrisonRacerSprintSpeed;                       // +0xdcc
    long  PrisonRacerSprintTime;                        // +0xdd0
    long  PrisonRacerRestTime;                          // +0xdd4
};
extern CScriptDef* pGlobalScriptDef;                        // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
float __fastcall GetDistanceBetweenThings(const CScriptThing& a, const CScriptThing& b);   // 0x00CBE512
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer::CTimer() { TimerIndex = g_pGameScriptInterface->RegisterTimer(); }
inline CTimer::~CTimer() { g_pGameScriptInterface->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
struct CQ_PrisonRaceScript {
    unsigned char _pad[0x48];
    bool HeroWonRace;                                   // +0x48 (PDB +0x5c)
    bool RaceStarted;                                   // +0x49
    bool RaceEnded;                                     // +0x4a
    long RacerSpeed;                                    // +0x4c
    unsigned char _pad50[0x58 - 0x50];
    long NearestRaceMarker;                             // +0x58
    bool MakePrisonerComment(const CScriptThing& prisoner, const CCharString& line);   // 0x00DDCF50
};
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
};
struct CPrisonRacer : CEntityScriptBase {
    CQ_PrisonRaceScript* ParentClass;                   // +0x14
    void* PMasterData;                                  // +0x18
    long RaceWaypointIndex;                             // +0x1c
    void Main();
};
void CPrisonRacer::Main()
{
    while (!ParentClass->RaceStarted) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase racer;
    if (racer.IsValid())
        racer.Release();
    while (!Game->StartScriptingEntity(Me, racer, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->EntitySetMaxRunningSpeed(Me, pGlobalScriptDef->PrisonRacerSpeed - ParentClass->RacerSpeed * 0.02f);
    ParentClass->RacerSpeed++;
    CScriptThing warden = Game->GetThingWithScriptName("PrisonRaceWarden");
    bool sprinting = false;
    CTimer sprintTimer;
    CTimer restTimer;
    restTimer = 5;
    CScriptThing marker = Game->GetNearestWithScriptName(Me, "RaceMarker");
    vector<CScriptThing> speedChecks;
    Game->GetAllThingsWithScriptName("WaypointSpeedCheck", speedChecks);
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (Game->GetWorldFrame() % 4 == 0) {
            if (IsActiveThreadTerminating())
                return;
            float nearest = 1000.0f;
            for (unsigned long i = 0; i < speedChecks.size(); ++i) {
                float distance = GetDistanceBetweenThings(Me, speedChecks[i]);
                if (distance < nearest) {
                    nearest = distance;
                    RaceWaypointIndex = GFCharStringToInt(speedChecks[i].GetDataString());
                }
            }
        }
        if (!sprinting && ParentClass->NearestRaceMarker > RaceWaypointIndex) {
            if (IsActiveThreadTerminating())
                {
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
            sprinting = true;
            sprintTimer = pGlobalScriptDef->PrisonRacerSprintTime;
            Game->EntitySetMaxRunningSpeed(Me, pGlobalScriptDef->PrisonRacerSprintSpeed - ParentClass->RacerSpeed * 0.02f);
            ParentClass->MakePrisonerComment(Me, "SPRINT");
        }
        if (sprinting && sprintTimer.GetTime() == 0) {
            if (IsActiveThreadTerminating())
                {
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
            sprinting = false;
            restTimer = pGlobalScriptDef->PrisonRacerRestTime;
            Game->EntitySetMaxRunningSpeed(Me, pGlobalScriptDef->PrisonRacerSpeed - ParentClass->RacerSpeed * 0.02f);
            ParentClass->MakePrisonerComment(Me, "SLOW");
        }
        if (marker.IsAlive()) {
            if (IsActiveThreadTerminating())
                return;
            if (!racer.IsPerformingScriptTask()) {
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                marker = Game->GetNearestWithScriptName(Me, "RaceMarker");
                CScriptThing speedCheck = Game->GetNearestWithScriptName(Me, "WaypointSpeedCheck");
                RaceWaypointIndex = GFCharStringToInt(speedCheck.GetDataString());
                racer.FollowPreCalculatedRoute(marker, (EScriptEntityMoveType)1, false, true);
            }
        }
        if (ParentClass->HeroWonRace) {
            if (IsActiveThreadTerminating())
                {
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
            racer.ClearCommands();
            do {
                Game->NewScriptFrame();
            } while (!IsActiveThreadTerminating());
            {
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            }
        }
        if (IsDistanceBetweenThingsUnder(warden, Me, 5.0f)) {
            if (IsActiveThreadTerminating())
                {
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
            ParentClass->RaceEnded = true;
        }
        Game->NewScriptFrame();
    }
}