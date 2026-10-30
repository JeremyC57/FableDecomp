// CV_BodyGuardScript::CV_BodyGuardScript(void* data, CGameScriptInterface* game) -- quest-script ctor: script base, interface/data pair, vtable 0x012E6E4C, the stood-still counter, four chat/commentary timers, the empty henchman list and the four respawn timers
// Leaf: ??0CV_BodyGuardScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E5D140, length 174 bytes (`ret 8`; 176 with padding).
// Callees: CScriptBase::CScriptBase @ 0x00CB8110, and through the global game script interface @ 0x0143E8F8:
// GetWorldFrame (slot 653, +0xa34) for the counter, RegisterTimer (slot 87, +0x15c) for every timer.
// Layout (retail = Ego_r.pdb CV_BodyGuardScript - 0x14, then - 0x18 after the 12-byte retail STLport vector):
// TotalWages +0x48, HeroCannotAffordHelp/HeroWieldingWeapon +0x4c/+0x4d (not initialised), HeroStoodStillCounter
// +0x50, HenchmanLastIdleChatTimer +0x54, HenchmanLastBoredChatTimer +0x58, CommentaryTimer +0x5c,
// HenchmanFreakedOutThePlayerTimer +0x60, HenchmanList +0x64, ConversationIndex +0x70 (not initialised),
// HenchmanDeadRespawnTimer[4] +0x74, HenchmanGlobalStatus[4] +0x84 (not initialised).
// Shape: the landed CV_ChickenKickingScript (0x00E62830) model: the two argument stores precede the vtable store (an
// inlined `__declspec(novtable)` intermediate base); the members are objects with inline constructors (CCounter from
// the world frame, CTimer from RegisterTimer, the STLport `_Vector_base` + alloc-proxy vector sharing one zero
// register); the timer array has a constructor and no destructor, so VC builds it with an inline counted loop.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;       // 0x0143E8F8

// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer::CTimer() : TimerIndex(g_pGameScriptInterface->RegisterTimer()) { }
class CCounter { public:                                           // a frame counter: the world frame it started at
    long StartFrame;
    CCounter() : StartFrame(g_pGameScriptInterface->GetWorldFrame()) { }
};
class CScriptThing;

// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct __declspec(novtable) CQuestScriptObject : CScriptBase {   // the argument-carrying intermediate base
    CGameScriptInterface* Game;                             // +0x40 (second argument)
    void*                 Data;                             // +0x44 (first argument, the script data)
    CQuestScriptObject(void* data, CGameScriptInterface* game) : CScriptBase(), Game(game), Data(data) { }
};

template <class T> struct allocator { allocator() { } };
template <class T, class A> struct _STLP_alloc_proxy : A {
    T _M_data;
    _STLP_alloc_proxy(const A& a, T p) : A(a), _M_data(p) { }
};
template <class T> struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, allocator<T> > _M_end_of_storage;
    _Vector_base(const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0) { }
};
template <class T> struct CRetailVector : _Vector_base<T> {   // STLport vector<T>: three pointers (12 bytes in retail)
    CRetailVector() : _Vector_base<T>(allocator<T>()) { }
};

namespace NScript {
class CV_BodyGuardScript : public CQuestScriptObject {
public:            // vtable 0x012E6E4C
    long     TotalWages;                                    // +0x48
    bool     HeroCannotAffordHelp;                          // +0x4c
    bool     HeroWieldingWeapon;                            // +0x4d
    CCounter HeroStoodStillCounter;                         // +0x50
    CTimer   HenchmanLastIdleChatTimer;                     // +0x54
    CTimer   HenchmanLastBoredChatTimer;                    // +0x58
    CTimer   CommentaryTimer;                               // +0x5c
    CTimer   HenchmanFreakedOutThePlayerTimer;              // +0x60
    CRetailVector<CScriptThing> HenchmanList;               // +0x64
    long     ConversationIndex;                             // +0x70
    CTimer   HenchmanDeadRespawnTimer[4];                   // +0x74
    long     HenchmanGlobalStatus[4];                       // +0x84
    CV_BodyGuardScript(void* data, CGameScriptInterface* game);
    virtual void v00();
};
} // namespace NScript
using NScript::CV_BodyGuardScript;

NScript::CV_BodyGuardScript::CV_BodyGuardScript(void* data, CGameScriptInterface* game)
    : CQuestScriptObject(data, game)
{
}