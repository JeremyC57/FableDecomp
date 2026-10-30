// CV_TourGuideScript::MoveToNextWaypoint(const CScriptThing&, CScriptGameResourceObjectScriptedThingBase&) -- send the guide to the given waypoint, or (if it is gone) step the waypoint counter round the 18-entry tour until a live waypoint marker is found and send him there
// Leaf: ?MoveToNextWaypoint@CV_TourGuideScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE6850, 485 bytes (496 with padding), `ret 8`.
// Callees: ~CScriptThing 0x004AA840 (out of line), ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC,
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940; CScriptThing slots 6 GetPos and 75 IsAlive (inline bodies through
//   PImp for the local), the handle's MoveToPosition (slot 4); through the interface (+0x40) NewScriptFrame (slot 7),
//   GetThingWithScriptName (slot 72, hidden result). The zero vector 0x0143E8E0.
// Shape: the local is an inline copy of the parameter; the lookup result is assigned through CCountedPointer's inline
//   operator= (source fields read first) and the temporary destroyed inline; the local's destructor is out of line.
//   GetPos is __forceinline: with a plain inline body VC7.1 calls it out of line at the final move.
// Layout (retail = Ego_r.pdb - 0x14): WaypointInfo[18] +0x48 (12-byte WaypointData, locMarker first),
//   WaypointCounter +0x164; interface pointer +0x40.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
class C3DVector { public: float X, Y, Z; };
extern C3DVector g_ZeroVector;                              // 0x0143E8E0
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
void operator delete(void* p);                              // 0x00BFE9BC (MSVCR71)
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing::~CScriptThing() { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
__forceinline const C3DVector& CScriptThing::GetPos() const { if (!PImp.Data) return g_ZeroVector; return PImp.Data->GetPos(); }
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
enum EScriptEntityMoveType { MOVE_TYPE_WALK = 0 };
class CScriptGameResourceObjectScriptedThingBase { public:         // vtable 0x0127094C, 0x10 bytes
    virtual ~CScriptGameResourceObjectScriptedThingBase();   // 0x007E74D0
    virtual void s001(); virtual void s002(); virtual void s003();
    virtual void MoveToPosition(const C3DVector& pos, float speed, EScriptEntityMoveType type, bool a, bool b); // slot 4 (+0x10)
    void* m_pIntelligentPointer;
    void* PImpData;
    void* PImpInfo;
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* Game;                             // +0x40
    void*                 Data;                             // +0x44
};
struct WaypointData {                                       // 12 bytes
    CCharString locMarker;
    CCharString locTextOverheard;
    CCharString locTextRequested;
};
namespace NScript {
class CV_TourGuideScript : public CQuestScriptBase {
public:
    WaypointData WaypointInfo[18];                          // +0x48
    bool TourGuideKilled;                                   // +0x120
    bool AllFollowersDead;                                  // +0x121
    bool QuestFinished;                                     // +0x122
    bool TourFinished;                                      // +0x123
    bool QuestActivated;                                    // +0x124
    bool Initialise;                                        // +0x125
    bool SpawnedQuestFinishThread;                          // +0x126
    CCharString RandomGuideResponse[5];                     // +0x128
    CCharString RandomFollowerResponseM[5];                 // +0x13c
    CCharString RandomFollowerResponseF[5];                 // +0x150
    long WaypointCounter;                                   // +0x164
    void MoveToNextWaypoint(const CScriptThing& waypoint, CScriptGameResourceObjectScriptedThingBase& guide);
};
} // namespace NScript
using NScript::CV_TourGuideScript;

void NScript::CV_TourGuideScript::MoveToNextWaypoint(const CScriptThing& waypoint, CScriptGameResourceObjectScriptedThingBase& guide)
{
    if (waypoint.IsAlive()) {
        if (IsActiveThreadTerminating())
            return;
        guide.MoveToPosition(waypoint.GetPos(), 1.0f, MOVE_TYPE_WALK, false, true);
        return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing next = waypoint;
    long start = WaypointCounter;
    while (!next.IsAlive()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        ++WaypointCounter;
        if (WaypointCounter >= 18) {
            if (IsActiveThreadTerminating())
                return;
            WaypointCounter = 0;
        } else if (WaypointCounter == start) {
            if (IsActiveThreadTerminating())
                return;
        }
        next = Game->GetThingWithScriptName(WaypointInfo[WaypointCounter].locMarker);
    }
    if (IsActiveThreadTerminating())
        return;
    guide.MoveToPosition(next.GetPos(), 1.0f, MOVE_TYPE_WALK, false, true);
}