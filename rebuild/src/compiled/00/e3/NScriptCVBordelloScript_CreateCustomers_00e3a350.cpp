// CV_BordelloScript::CreateCustomers() -- while the Bordello region is loaded, every 15 timer ticks spawn a new BordelloClient (female villagers once the bordello became a nunnery, male otherwise) at M_ClientSpawn while fewer than three clients are alive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E3A350, length 860 bytes, plain `ret`.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CCharString(const char*, long) 0x0099EBF0 / ~CCharString
//   0x0099EAE0; ~CBaseClass 0x0099A2E0 and operator delete 0x00BFE9BC (the inline ~CScriptThing); CScriptThing slot 6
//   GetPos (+0x18); CGameScriptInterface slots 7 NewScriptFrame, 12 IsRegionLoaded, 72 GetThingWithScriptName (by value), 91
//   CreateCreature (by value); the CTimer calls 87..90 through the global interface 0x0143E8F8. Strings "Bordello",
//   "BordelloClient", "M_ClientSpawn", "CREATURE_BS_VILLAGER_FEMALE", "CREATURE_BS_VILLAGER_MALE".
// Layout: Ego_r.pdb CV_BordelloScript minus 0x14 (retail CScriptBase): BecomeNunnery +0x4d, ClientsAlive +0x54.
// Shape: generated script code; CTimer has inline ctor and dtor; the names are implicit conversions (all three built before
//   the GetThingWithScriptName call); both temporaries' ~CScriptThing expand inline.
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
class C3DVector { public: float X, Y, Z; };
struct CScriptThingImpl;
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() { }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;                   // 0x0143E8F8
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer::CTimer() { TimerIndex = g_pGameScriptInterface->RegisterTimer(); }
inline CTimer::~CTimer() { g_pGameScriptInterface->DeregisterTimer(TimerIndex); }
inline CTimer& CTimer::operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }
inline long CTimer::GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CV_BordelloScript : public CScriptBase {
public:
    CGameScriptInterface*    Game;                                     // +0x40
    void*         Owner;                                    // +0x44
    unsigned char _pad48[0x4d - 0x48];
    bool          BecomeNunnery;                            // +0x4d
    unsigned char _pad4e[0x54 - 0x4e];
    long          ClientsAlive;                             // +0x54
    void CreateCustomers();
};
} // namespace NScript
using NScript::CV_BordelloScript;

void NScript::CV_BordelloScript::CreateCustomers()
{
    CTimer timer;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        while (Game->IsRegionLoaded("Bordello")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            if (timer.GetTime() == 0 && ClientsAlive < 3) {
                if (IsActiveThreadTerminating())
                    return;
                timer = 15;
                if (BecomeNunnery) {
                    if (IsActiveThreadTerminating())
                        return;
                    Game->CreateCreature("CREATURE_BS_VILLAGER_FEMALE", Game->GetThingWithScriptName("M_ClientSpawn").GetPos(), "BordelloClient", false);
                } else {
                    if (IsActiveThreadTerminating())
                        return;
                    Game->CreateCreature("CREATURE_BS_VILLAGER_MALE", Game->GetThingWithScriptName("M_ClientSpawn").GetPos(), "BordelloClient", false);
                }
            }
        }
        if (IsActiveThreadTerminating())
            return;
        Game->NewScriptFrame();
    }
}