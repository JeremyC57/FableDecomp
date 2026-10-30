// Levers: for(;;) with the terminating return first; in-body `#pragma inline_depth(0)` around the hit-block terminate return gives the out-of-line ~CScriptThing, the __forceinline dtor expands at the other exits.
// CQ_HobbeCaveScript::CheckJamesKilled(CScriptThing james) -- watches James: once he is killed the mission fails; the first time he is hit and loses health, clears the quest data's +0x114 flag
// Leaf: ?CheckJamesKilled@CQ_HobbeCaveScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D97170, 491 bytes, `ret 0Ch` (the by-value CScriptThing).
// Callees: interface slot 264 GetHealth (+0x420) then _ftol 0x00BFEA70, CCharString("" 0x0122D70E) 0x0099EBF0 / ~ 0x0099EAE0,
//   the thing's implementation slots 18 (+0x48, taken as MsgIsKilledBy: same const CCharString& shape, it fails the mission)
//   and 21 MsgIsHitBy (+0x54) behind null-safe wrappers, IsActiveThreadTerminating 0x00CB7940, NewScriptFrame (slot 7),
//   ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC, ~CScriptThing 0x004AA840.
// Layout (retail = Ego_r.pdb - 0x14): MissionFailed +0x4C; the quest data (+0x44) flag at +0x114 is unnamed.
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
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
__forceinline CScriptThing::~CScriptThing() {}
inline bool CScriptThing::MsgIsHitBy(const CCharString& attacker) const { if (!PImp.Data) return false; return PImp.Data->MsgIsHitBy(attacker); }
inline bool CScriptThing::MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString& attacker) const { if (!PImp.Data) return false; return PImp.Data->MsgIsHitByAnyAggressiveSpecialAbilityFrom(attacker); }
inline bool CScriptThing::MsgIsKilledBy(const CCharString& killer) const { if (!PImp.Data) return false; return PImp.Data->MsgIsKilledBy(killer); }
inline bool CScriptThing::IsAlive() const { if (!PImp.Data) return false; return PImp.Data->IsAlive(); }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct CHobbeCaveData {
    unsigned char _pad00[0x114];
    bool          Flag114;                                  // +0x114
};
struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* Game;                             // +0x40
    CHobbeCaveData*       Data;                             // +0x44
};
namespace NScript {
class CQ_HobbeCaveScript : public CQuestScriptBase {
public:
    unsigned char _pad48[4];
    bool          MissionFailed;                            // +0x4c
    void CheckJamesKilled(CScriptThing james);
};
} // namespace NScript
using NScript::CQ_HobbeCaveScript;

void NScript::CQ_HobbeCaveScript::CheckJamesKilled(CScriptThing james)
{
    bool hit = false;
    long health = (long)Game->GetHealth(james);
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (james.MsgIsKilledBy("")) {
            if (IsActiveThreadTerminating())
                return;
            MissionFailed = true;
            if (IsActiveThreadTerminating())
                return;
            return;
        }
        if (!hit && james.MsgIsHitBy("") && Game->GetHealth(james) < health) {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
            Data->Flag114 = false;
            hit = true;
        }
        Game->NewScriptFrame();
    }
}