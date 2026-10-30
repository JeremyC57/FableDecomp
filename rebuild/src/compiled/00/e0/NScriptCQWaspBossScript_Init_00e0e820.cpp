// CQ_WaspBossScript::Init() -- register the Wasp Boss quest region and map offset, clear the quest state and reset the wasp-helper timer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E0E820, length 192 bytes; vtable 0x012E00E4 slot 3 (factory 0x00E0E9F0,
// `push 5Ch` = Ego_r.pdb 0x70 - 0x14).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; the quest's interface (+0x40) AddQuestRegion
// (slot 271) and SetQuestWorldMapOffset (slot 272); g_pGameScriptInterface (0x0143E8F8) SetTimer (slot 89) via CTimer.
// Layout (Ego_r.pdb - 0x14): the eight flags +0x48..+0x4f, SavedVillagerCount +0x50, QuestStartScreened +0x54,
// ReachedWaspHelper (CTimer) +0x58.
// Shape: the quest-name temporary is built before the C2DCoordI(10, 10) temporary (an implicit conversion goes first),
// whose two words are stored after its address is pushed; the clears share one zero register in source order.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString();                                       // 0x0099E4B0
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)

#include "engine/CCountedPointer.h"


#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing() {}
inline CScriptThing::~CScriptThing() {}
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
class C2DCoordI { public:                                       // integer 2D coordinate
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) {}
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"

struct CQuestScriptBase : CScriptBase {                  // retail 0x48 bytes
    CGameScriptInterface* GSI;                           // +0x40
    void*                 Owner;                         // +0x44
};

extern CGameScriptInterface* g_pGameScriptInterface;     // 0x0143E8F8

// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer& CTimer::operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }

namespace NScript {
class CQ_WaspBossScript : public CQuestScriptBase {
public:
    bool MissionSucceeded;                               // +0x48
    bool MissionFailed;                                  // +0x49
    bool DronesCreated;                                  // +0x4a
    bool QueenHornetAttacks;                             // +0x4b
    bool CutsceneFinished;                               // +0x4c
    bool SaidLineOnce;                                   // +0x4d
    bool PanickedVillagersScene;                         // +0x4e
    bool StartChase;                                     // +0x4f
    long SavedVillagerCount;                             // +0x50
    bool QuestStartScreened;                             // +0x54
    CTimer ReachedWaspHelper;                            // +0x58
    void Init();
};
} // namespace NScript
using NScript::CQ_WaspBossScript;

void NScript::CQ_WaspBossScript::Init()
{
    GSI->AddQuestRegion("Q_WaspBoss", "PicnicArea");
    GSI->SetQuestWorldMapOffset("Q_WaspBoss", C2DCoordI(10, 10));
    PanickedVillagersScene = false;
    MissionSucceeded = false;
    MissionFailed = false;
    DronesCreated = false;
    SaidLineOnce = false;
    QueenHornetAttacks = false;
    CutsceneFinished = false;
    StartChase = false;
    QuestStartScreened = false;
    SavedVillagerCount = 0;
    ReachedWaspHelper = 0;
}