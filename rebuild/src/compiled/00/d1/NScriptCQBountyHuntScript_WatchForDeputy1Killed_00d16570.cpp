// CQ_BountyHuntScript::WatchForDeputy1Killed(CScriptThing deputy) -- watch thread: frame by frame until the first bandit deputy is killed, or is hit by the hero and has (almost) no health left; then (unless terminating) set Deputy1Killed
// Leaf: WatchForDeputy1Killed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D16570, length 517 bytes (padded to 528), `ret 0Ch` (the 12-byte
// CScriptThing is passed by value and destroyed here).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CCharString(const char*, long) 0x0099EBF0 /
// ~CCharString 0x0099EAE0 ("" 0x0122D70E, "SCRIPT_NAME_HERO" 0x0125D1C8); the deputy's implementation slots 18
// MsgIsKilledBy (+0x48), 21 MsgIsHitBy (+0x54), 43 MsgIsHitByAnyAggressiveSpecialAbilityFrom (+0xac) behind the inline
// null-safe CScriptThing wrappers; through the interface (+0x40): NewScriptFrame (slot 7), GetHealth (slot 264,
// +0x420). Float 0.0001f 0x0129BA3C. The by-value deputy's destructor: inline (vtable 0x01238C8C, counted release,
// operator delete 0x00BFE9BC, ~CBaseClass 0x0099A2E0) at three exits, out of line ~CScriptThing 0x004AA840 at two.
// Layout (Ego_r.pdb - 0x14): Deputy1Killed +0x50.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
void operator delete(void* p);                              // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                                     // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);              // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    __forceinline ~CCountedPointer()
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
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
__forceinline CScriptThing::~CScriptThing() {}
inline bool CScriptThing::MsgIsKilledBy(const CCharString& killer) const { if (!PImp.Data) return false; return PImp.Data->MsgIsKilledBy(killer); }
inline bool CScriptThing::MsgIsHitBy(const CCharString& attacker) const { if (!PImp.Data) return false; return PImp.Data->MsgIsHitBy(attacker); }
inline bool CScriptThing::MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString& attacker) const { if (!PImp.Data) return false; return PImp.Data->MsgIsHitByAnyAggressiveSpecialAbilityFrom(attacker); }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_BountyHuntScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                             // +0x40
    void*                 Data;                             // +0x44
    unsigned char _flags048[8];                             // +0x48 .. +0x4f (QuestFailed .. LeaderKilled)
    bool Deputy1Killed;                                     // +0x50
    void WatchForDeputy1Killed(CScriptThing deputy);
};
} // namespace NScript
using NScript::CQ_BountyHuntScript;

void NScript::CQ_BountyHuntScript::WatchForDeputy1Killed(CScriptThing deputy)
{
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        bool killed = deputy.MsgIsKilledBy("");
        if (killed) {
            if (IsActiveThreadTerminating())
                return;
            break;
        }
        bool hit = deputy.MsgIsHitBy("SCRIPT_NAME_HERO") || deputy.MsgIsHitByAnyAggressiveSpecialAbilityFrom("SCRIPT_NAME_HERO");
        if (hit) {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
            if (Game->GetHealth(deputy) < 0.0001f) {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
                break;
            }
        }
        Game->NewScriptFrame();
    }
    if (IsActiveThreadTerminating())
        return;
    Deputy1Killed = true;
}