// CV_MurderTwistScript::MagicRuneProcess() -- once the quest passes state 1, every time the timer has run out while no region has unloaded, restart it and rumble each magic rune within 15 units (squared distance) of the hero
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EA8C20, 0x221 bytes, plain `ret`.
// Callees: IsActiveThreadTerminating 0x00CB7940, CCharString() 0x0099E4B0 / (const char*, long) 0x0099EBF0 / ~ 0x0099EAE0,
//   GetDistanceBetweenThings 0x00CBE512 (__fastcall, as in the landed CGuardInWoods / NecropolisGate code),
//   ~CScriptThing 0x004AA840; the timer runs through the global interface g_pGameScriptInterface 0x0143E8F8 (slots 87
//   RegisterTimer / 88 DeregisterTimer / 89 SetTimer / 90 GetTimer); Game slots 7 NewScriptFrame, 23 MsgOnRegionUnloaded,
//   70 GetHero, 106 CreateRumble; CScriptThing virtual slot 6 (+0x18) the position accessor. 15.0f is 0x0124E640.
// Layout (retail = Ego_r.pdb - 0x14): Game +0x40, MurderTwistState +0x4c, MagicRuneRumbleList +0x54 (vector<CScriptThing>).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharStringData { public: const char* m_pText; unsigned long m_length; };
class CCharString { public:
    CCharStringData* PStringData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class C3DVector { public: float X, Y, Z; };
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
float __fastcall GetDistanceBetweenThings(const CScriptThing& a, const CScriptThing& b);   // 0x00CBE512
namespace std {
template<class T> class allocator { };
template<class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return _M_start[n]; }
};
}
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer::CTimer() : TimerIndex(g_pGameScriptInterface->RegisterTimer()) {}
inline CTimer::~CTimer() { g_pGameScriptInterface->DeregisterTimer(TimerIndex); }
inline long CTimer::GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
inline CTimer& CTimer::operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CV_MurderTwistScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void* PMasterData;                                  // +0x44
    long MurderTwistItemsFound;                         // +0x48
    long MurderTwistState;                              // +0x4c
    long MurderTwistEvent;                              // +0x50
    std::vector<CScriptThing> MagicRuneRumbleList;      // +0x54
    void MagicRuneProcess();
};
} // namespace NScript
using NScript::CV_MurderTwistScript;

void NScript::CV_MurderTwistScript::MagicRuneProcess()
{
    while (!(MurderTwistState > 1)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CTimer timer;
    CCharString region;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        while (!Game->MsgOnRegionUnloaded(region)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            if (timer.GetTime() == 0) {
                if (IsActiveThreadTerminating())
                    return;
                timer = 1;
                for (unsigned int i = 0; i < MagicRuneRumbleList.size(); ++i) {
                    if (IsActiveThreadTerminating())
                        return;
                    CScriptThing* start = MagicRuneRumbleList._M_start;
                    if (GetDistanceBetweenThings(Game->GetHero(), start[i]) < 15.0f)
                        Game->CreateRumble(MagicRuneRumbleList[i].GetPos(), 0.3f, 0.2f, "");
                }
                if (IsActiveThreadTerminating())
                    return;
            }
        }
        if (IsActiveThreadTerminating())
            return;
        Game->NewScriptFrame();
    }
}