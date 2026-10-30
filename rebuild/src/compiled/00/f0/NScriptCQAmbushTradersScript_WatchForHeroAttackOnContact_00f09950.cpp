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
struct CScriptThingImpl {
    virtual void i000(); virtual void i001(); virtual void i002(); virtual void i003(); virtual void i004(); virtual void i005(); virtual void i006(); virtual void i007();
    virtual void i008(); virtual void i009(); virtual void i010(); virtual void i011(); virtual void i012(); virtual void i013(); virtual void i014(); virtual void i015();
    virtual void i016(); virtual void i017(); virtual void i018(); virtual void i019(); virtual void i020();
    virtual bool MsgIsHitBy(const CCharString& attacker) const;   // slot 21 (+0x54)
    virtual void i022(); virtual void i023(); virtual void i024(); virtual void i025(); virtual void i026(); virtual void i027(); virtual void i028(); virtual void i029();
    virtual void i030(); virtual void i031(); virtual void i032(); virtual void i033(); virtual void i034(); virtual void i035(); virtual void i036(); virtual void i037();
    virtual void i038(); virtual void i039(); virtual void i040(); virtual void i041(); virtual void i042();
    virtual bool MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString& attacker) const;   // slot 43 (+0xac)
    virtual void i044(); virtual void i045(); virtual void i046(); virtual void i047(); virtual void i048(); virtual void i049(); virtual void i050(); virtual void i051();
    virtual void i052(); virtual void i053(); virtual void i054(); virtual void i055(); virtual void i056(); virtual void i057(); virtual void i058(); virtual void i059();
    virtual void i060(); virtual void i061(); virtual void i062(); virtual void i063(); virtual void i064(); virtual void i065(); virtual void i066(); virtual void i067();
    virtual void i068(); virtual void i069(); virtual void i070(); virtual void i071(); virtual void i072(); virtual void i073(); virtual void i074();
    virtual bool IsAlive() const;   // slot 75 (+0x12c)
};
class CScriptThing : public CBaseClass { public:                          // vtable 0x01238C8C, 12 bytes
    CCountedPointer<CScriptThingImpl> PImp;                 // +0x04 Data, +0x08 Info
    CScriptThing(const CScriptThing& other);                // 0x004ABE90
    __forceinline ~CScriptThing() {}                                      // out of line 0x004AA840
    bool MsgIsHitBy(const CCharString& attacker) const      // 0x004AAB30
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->MsgIsHitBy(attacker);
    }
    bool MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString& attacker) const   // 0x004AAD60
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->MsgIsHitByAnyAggressiveSpecialAbilityFrom(attacker);
    }
    bool IsAlive() const                                    // 0x004AB130
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->IsAlive();
    }
};
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