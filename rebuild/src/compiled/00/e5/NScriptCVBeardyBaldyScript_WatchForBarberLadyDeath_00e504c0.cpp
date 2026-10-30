// CV_BeardyBaldyScript::WatchForBarberLadyDeath() -- find the barber nearest the hero, wait until she is alive, then until she is killed (by anyone), and record BarberLadyKilled
// Levers: ~CScriptThing __forceinline; IsAlive in if-form; depth 0 around the post-first-loop terminating return; killed block `if (!Term) { if (!Term) BarberLadyKilled = true; }` then one depth-0 `return;` (the merged out-of-line exit)
// Leaf: WatchForBarberLadyDeath
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E504C0, 400 bytes.
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~ @ 0x0099EAE0, CScriptBase::IsActiveThreadTerminating
//          @ 0x00CB7940, ~CScriptThing @ 0x004AA840 / inline (counted release, operator delete @ 0x00BFE9BC, ~CBaseClass
//          @ 0x0099A2E0), CScriptThing::IsAlive / MsgIsKilledBy inline (game-side thing slots 75 +0x12c / 18 +0x48);
//          CGameScriptInterface slots 7 NewScriptFrame, 70 GetHero (+0x118), 77 GetNearestWithScriptName (+0x134).
// Layout: Ego_r.pdb CV_BeardyBaldyScript - 0x14: BarberLadyKilled +0x69. CScriptThing model as the landed
//          CQ_HangingTreeGoodScript (0x00D74F80); same TU as the landed WatchForBeardyBaldyDeath (0x00E503D0).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)

class CCPPointerInfo { public:                                  // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);           // destroys the pointee
    void* Data;
};

template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
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
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
__forceinline CScriptThing::~CScriptThing() {}
inline bool CScriptThing::MsgIsKilledBy(const CCharString& killer) const { if (!PImp.Data) return false; return PImp.Data->MsgIsKilledBy(killer); }
inline bool CScriptThing::IsAlive() const { if (!PImp.Data) return false; return PImp.Data->IsAlive(); }
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

namespace NScript {
class CV_BeardyBaldyScript : public CQuestScriptBase {
public:
    unsigned char _pad48[0x20];                          // +0x48 quest flags, phase, required hair strings
    bool BeardyBaldyKilled;                              // +0x68
    bool BarberLadyKilled;                               // +0x69
    void WatchForBarberLadyDeath();
};
} // namespace NScript
using NScript::CV_BeardyBaldyScript;

void NScript::CV_BeardyBaldyScript::WatchForBarberLadyDeath()
{
    CScriptThing barber = GSI->GetNearestWithScriptName(GSI->GetHero(), "CREATURE_BOWERSTONE_SLUMS_VILLAGER_FEMALE_BARBER");
    while (!barber.IsAlive()) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
#pragma inline_depth(0)
    if (IsActiveThreadTerminating())
        return;
#pragma inline_depth()
    while (!IsActiveThreadTerminating()) {
        if (barber.MsgIsKilledBy("")) {
            if (!IsActiveThreadTerminating()) {
                if (!IsActiveThreadTerminating())
                    BarberLadyKilled = true;
            }
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
        GSI->NewScriptFrame();
    }
}