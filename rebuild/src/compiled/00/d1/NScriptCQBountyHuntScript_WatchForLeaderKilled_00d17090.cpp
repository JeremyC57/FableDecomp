// NScript::CQ_BountyHuntScript::WatchForLeaderKilled(CScriptThing leader) -- watch thread: frame by frame until the bandit leader is killed (by anyone) or decapitated, then (unless terminating) set LeaderKilled
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D17090, length 233 bytes, `ret 0Ch` (the 12-byte CScriptThing is passed
// by value and destroyed here).
// Identity: catalog CQ_BountyHuntScript::WatchForLeaderKilled; it sets the class's LeaderKilled flag and reads the
// Sunnyvale master data's BountyHuntDecapitation flag (Ego_r.pdb layouts).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CGameScriptInterface slot 7 NewScriptFrame (+0x1C);
// the leader's implementation object (CScriptThing slot 18 MsgIsKilledBy, +0x48) through the inline
// CScriptThing::MsgIsKilledBy; CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0; the by-value
// leader's destructor: inline (vtable 0x01238C8C store, counted release through the info block's destroy function
// and operator delete 0x00BFE9BC, CBaseClass::~CBaseClass 0x0099A2E0) on the terminating path, the out-of-line
// CScriptThing::~CScriptThing 0x004AA840 on the normal one. String: "" @ 0x0122D70E.
// Layout: retail quest base 0x48 bytes (GSI +0x40, PMasterData +0x44); LeaderKilled +0x4F (Ego_r.pdb +0x63 less the
// 0x14 debug-container shift); CQ_SunnyvaleMasterData::BountyHuntDecapitation +0xD9 (no shift). CScriptThing is
// modelled locally (engine/CScriptThing.h is a flat 12-byte view with no virtual interface).
// Shape: the wait condition is a named bool over one `||` chain whose first operand holds the "" conversion
// temporary (destruction flag in ebx, result in a frame byte). The by-value leader has THREE destructor sites (the
// terminating return inside the loop, a second `if (IsActiveThreadTerminating()) return;` after it, and the end):
// VC7.1 expands the inline destructor at the first site only and calls the out-of-line ~CScriptThing 0x004AA840 at
// the other two, which tail-merge (with two sites it expands both -- the parked 249-byte form).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

class CCPPointerInfo { public:                                 // counted-pointer block (Ego_r.pdb)
    unsigned long RefCount;                             // +0x00
    void (__fastcall* DeleteFunc)(void* data);          // +0x04
    void* Data;                                         // +0x08
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

class CScriptThing;
struct CCountedPointerScriptThing {                     // CCountedPointer<CScriptThing>
    CScriptThing*   Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
    ~CCountedPointerScriptThing()
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

class CScriptThing : public CBaseClass { public:                      // 12 bytes, vtable 0x01238C8C
    CCountedPointerScriptThing PImp;                    // +0x04
    virtual ~CScriptThing() {}                          // out of line: 0x004AA840
    virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010();
    virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual bool MsgIsKilledBy(const CCharString& killer) const     // slot 18 (+0x48)
    {
        if (PImp.Data)
            return PImp.Data->MsgIsKilledBy(killer);
        return false;
    }
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
class CQ_SunnyvaleMasterData { public:
    unsigned char _pad000[0xd9];
    bool          BountyHuntDecapitation;               // +0xd9
};

}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {

class CQ_BountyHuntScript : public CScriptBase { public:
    CGameScriptInterface*   GSI;                        // +0x40
    CQ_SunnyvaleMasterData* PMasterData;                // +0x44
    unsigned char _flags048[7];                         // +0x48 .. +0x4f (QuestFailed .. VillagerIntroDone)
    bool LeaderKilled;                                  // +0x4f
    void WatchForLeaderKilled(CScriptThing leader);
};

void CQ_BountyHuntScript::WatchForLeaderKilled(CScriptThing leader)
{
    for (;;) {
        bool waiting = !(leader.MsgIsKilledBy("") || PMasterData->BountyHuntDecapitation);
        if (!waiting)
            break;
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    LeaderKilled = true;
}
}