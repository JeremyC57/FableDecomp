// Levers: same for(;;) shape as 0x00F09260, but the hit block does not break: it sets ContactAttackedByHero and falls through to the frame (retail continues the loop); in-body `#pragma inline_depth(0)` around the hit-block terminate return gives the one out-of-line ~CScriptThing.
// CQ_AmbushTradersScript::WatchForHeroAttackOnContact(CScriptThing) -- spawned per contact: each frame, stop when the contact dies; when the hero hits it (melee or an aggressive special ability) set ContactAttackedByHero and stop
// Leaf: ?WatchForHeroAttackOnContact@CQ_AmbushTradersScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F09950, 437 bytes (448 with padding), `ret 0Ch` (the by-value CScriptThing).
// Spawned as "WatchForHeroAttackOnContact" (0x012F815C) by the setup at 0x00F09689. Twin of 0x00F09260 (Sentry), registers swapped.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 ("SCRIPT_NAME_HERO" 0x0125D1C8),
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940, ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC,
//   ~CScriptThing 0x004AA840 (out of line); through the interface (+0x40) NewScriptFrame (slot 7); the thing's
//   implementation slots 21 MsgIsHitBy (+0x54), 43 MsgIsHitByAnyAggressiveSpecialAbilityFrom (+0xac), 75 IsAlive
//   (+0x12c) behind the inlined null-safe wrappers.
// Layout (retail = Ego_r.pdb - 0x14): ContactAttackedByHero +0x56.
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
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
__forceinline CScriptThing::~CScriptThing() {}
inline bool CScriptThing::MsgIsHitBy(const CCharString& attacker) const { if (!PImp.Data) return false; return PImp.Data->MsgIsHitBy(attacker); }
inline bool CScriptThing::MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString& attacker) const { if (!PImp.Data) return false; return PImp.Data->MsgIsHitByAnyAggressiveSpecialAbilityFrom(attacker); }
inline bool CScriptThing::IsAlive() const { if (!PImp.Data) return false; return PImp.Data->IsAlive(); }
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
namespace NScript {
class CQ_AmbushTradersScript : public CQuestScriptBase {
public:
    unsigned char _pad48[0xe];                             // +0x48 QuestFailed .. ContactDead
    bool          ContactAttackedByHero;                     // +0x56
    void WatchForHeroAttackOnContact(CScriptThing contact);
};
} // namespace NScript
using NScript::CQ_AmbushTradersScript;

void NScript::CQ_AmbushTradersScript::WatchForHeroAttackOnContact(CScriptThing contact)
{
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (!contact.IsAlive()) {
            if (IsActiveThreadTerminating())
                return;
            break;
        }
        if (contact.MsgIsHitBy("SCRIPT_NAME_HERO") || contact.MsgIsHitByAnyAggressiveSpecialAbilityFrom("SCRIPT_NAME_HERO")) {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
            ContactAttackedByHero = true;
        }
        Game->NewScriptFrame();
    }
    if (IsActiveThreadTerminating())
        return;
}