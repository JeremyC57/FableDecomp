// NScript::CQ_CinemaTestScript::WatchForLeaving() -- watch thread: frame by frame while the arena level stays loaded and the hero alive, and the mission is neither failed nor succeeded; then mark the mission failed unless it succeeded
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F02030, length 193 bytes, plain `ret`.
// Identity: catalog CQ_CinemaTestScript::WatchForLeaving, consistent with its landed neighbours EndMission 0x00F02100 and
// PhaseOne 0x00F02170 (same 0x48-byte quest base, same "ArenaHallOfHeroes" level name).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CGameScriptInterface slots 7 NewScriptFrame (+0x1C),
// 11 IsLevelLoaded(const CCharString&) (+0x2C), 70 GetHero (+0x118, CScriptThing&); CScriptThing slot 75 IsAlive
// (+0x12C); CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0. String: "ArenaHallOfHeroes" @ 0x012C7820.
// Layout: retail quest base 0x48 bytes (CScriptBase 0x40, GSI +0x40, owner +0x44); MissionSucceeded +0x48,
// MissionFailed +0x49 (Ego_r.pdb +0x5C/+0x5D less the 0x14 debug-container shift).
// Shape: a top-tested wait loop whose `&&` condition starts with the level name built by an implicit conversion; the
// temporary is conditional for VC, so its destruction flag lives in a frame word (`or eax,1` load/store) and the
// condition rides in bl across the destructor; the loop body re-tests termination at the bottom (rotated loop).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {

class CQ_CinemaTestScript : public CScriptBase { public:
    CGameScriptInterface* GSI;                          // +0x40
    void*                 Owner;                        // +0x44
    bool MissionSucceeded;                              // +0x48
    bool MissionFailed;                                 // +0x49
    bool MissionOver;                                   // +0x4a
    void WatchForLeaving();
};

void CQ_CinemaTestScript::WatchForLeaving()
{
    while (GSI->IsLevelLoaded("ArenaHallOfHeroes") && GSI->GetHero().IsAlive() && !MissionFailed && !MissionSucceeded) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (!IsActiveThreadTerminating() && !MissionSucceeded)
        MissionFailed = true;
}
}