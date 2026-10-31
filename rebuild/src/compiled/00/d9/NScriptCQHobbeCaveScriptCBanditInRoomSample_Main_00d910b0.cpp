// NScript::CQ_HobbeCaveScript::CBanditInRoomSample::Main() -- trapped bandit sample: while the script runs, remove itself once the parent script reports the side-room hobbes killed; otherwise whenever its timer has run out and the hero is within 5m, restart the timer (HC_BanditCallsForHelpDelay) and say TEXT_QST_036_BANDIT_HEAR_HIM_TRAPPED to the hero in a fresh conversation
// Leaf: ?Main@CBanditInRoomSample@CQ_HobbeCaveScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D910B0, 369 bytes, plain `ret` (entity-script vtable slot 1).
// Name: Ego_r.pdb ?Main@CBanditInRoomSample@CQ_HobbeCaveScript@NScript@@EAEXXZ (private virtual; the PDB also has ??0, ??1,
//   ?Alloc@ (landed 0x00D90FF0), ?Init@ and ?GetParentScript@ of CBanditInRoomSample).
// Callees: CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30 (engine/CEntityScriptBase.h); IsDistanceBetweenThingsUnder
//   0x00CBE2FF (Ego_r ?IsDistanceBetweenThingsUnder@@YI_NABVCScriptThing@@0M@Z); CCharString(const char*, long) 0x0099EBF0 /
//   ~CCharString 0x0099EAE0; CGameScriptInterface slots 7 NewScriptFrame, 70 GetHero, 108 RemoveThing, 364
//   AddNewConversation, 365 AddPersonToConversation, 366 AddLineToConversation on Game, and 87 RegisterTimer / 88
//   DeregisterTimer / 89 SetTimer / 90 GetTimer on g_pGameScriptInterface 0x0143E8F8 (the CTimer bodies, engine/CTimer.h).
// Globals: g_pGameScriptInterface 0x0143E8F8, pGlobalScriptDef 0x0143E90C (the established bound spellings); 5.0f; the
//   string 0x012D5AD4.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8} (engine/CEntityScriptBase.h); Ego_r.pdb CBanditInRoomSample ParentClass
//   +0x14, PMasterData +0x18; CQ_HobbeCaveScript KilledSideRoomHobbes retail +0x4E (Ego_r.pdb +0x62: the quest-script base
//   is 0x14 smaller in retail); CScriptDef HC_BanditCallsForHelpDelay retail +0xCB0 (Ego_r.pdb +0xD60, the 0xB0 shift of
//   the landed CScriptDef rows). Both are local partial models (no shared headers).
// Shape: the timer is a CTimer local (engine/CTimer.h; ctor RegisterTimer, dtor DeregisterTimer, GetTime, operator=
//   expanded inline) -- its destructor runs at every exit, which gives retail's two DeregisterTimer copies. Generated
//   script style: `if (!KilledSideRoomHobbes) { ... } else { if (IsActiveThreadTerminating()) return; RemoveThing; return; }`
//   (the two returns of the else arm share one dtor copy; edx/eax as retail), and the conversation block's
//   `if (IsActiveThreadTerminating()) return;` makes its own dtor copy that VC7.1 hoists the global load out of and
//   cross-jumps into the loop exit's copy (`mov ecx,[g] ; jne exit+6`).
class CThing;
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
#include "engine/CGameScriptInterface.h"
#include "engine/CEntityScriptBase.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
#include "engine/CTimer.h"
inline CTimer::CTimer() : TimerIndex(g_pGameScriptInterface->RegisterTimer()) { }
inline CTimer::~CTimer() { g_pGameScriptInterface->DeregisterTimer(TimerIndex); }
inline long CTimer::GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
inline CTimer& CTimer::operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
class CScriptDef { public:
    unsigned char _pad[0xcb0];
    long HC_BanditCallsForHelpDelay;                    // +0xCB0 retail (Ego_r.pdb +0xD60)
};
extern CScriptDef* pGlobalScriptDef;                    // 0x0143E90C
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HobbeCaveScript { public:
    unsigned char _pad[0x4e];
    bool KilledSideRoomHobbes;                          // +0x4E retail (Ego_r.pdb +0x62)
    class CBanditInRoomSample : public CEntityScriptBase {
    private:
        virtual void Main();
    public:
        CQ_HobbeCaveScript*     ParentClass;            // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
    };
};

void CQ_HobbeCaveScript::CBanditInRoomSample::Main()
{
    CTimer timer;
    while (!IsActiveThreadTerminating()) {
        if (!ParentClass->KilledSideRoomHobbes) {
            if (timer.GetTime() == 0 && IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 5.0f)) {
                if (IsActiveThreadTerminating())
                    return;
                timer = pGlobalScriptDef->HC_BanditCallsForHelpDelay;
                long conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, "TEXT_QST_036_BANDIT_HEAR_HIM_TRAPPED", false, Me, Game->GetHero());
            }
            Game->NewScriptFrame();
        } else {
            if (IsActiveThreadTerminating())
                return;
            Game->RemoveThing(Me, false, true);
            return;
        }
    }
}
}