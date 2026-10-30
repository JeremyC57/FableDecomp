// CV_ArcheryCompetitionScript::WatchForOwnerKilled() -- once a contest is running, fetch the AC_Owner and wait for him to be alive; then, until the contest reaches state 2, watch his health: when he dies, stop the game timer, finish the quest, reset the contest, enable teleporting, sheathe the hero's weapons and make his will usable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E331F0, length 714 bytes, plain `ret`.
// Callees: CBaseClass 0x0099A2D0 / ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC, ~CScriptThing 0x004AA840 (out of
//   line at the later returns), CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString(const char*, long)
//   0x0099EBF0 / ~CCharString 0x0099EAE0; the thing implementation's slot 75 IsAlive (+0x12c) behind the inlined null-safe
//   wrapper; SetTimer slot 89 through g_pGameScriptInterface 0x0143E8F8; CGameScriptInterface slots 7 NewScriptFrame, 70 GetHero, 72
//   GetThingWithScriptName (by value), 131 SetHeroWillAsUsable, 264 GetHealth, 506 EntitySheatheWeapons, 565
//   SetTeleportingAsActive. String "AC_Owner"; 0.0001f 0x0129BA3C.
// Layout: Ego_r.pdb CV_ArcheryCompetitionScript minus 0x14 (retail CScriptBase): QuestFinished +0x51, GameTimeLeft (CTimer)
//   +0x6c, ContestState +0xa0. CScriptThing as the landed CV_TalentlessBardScript::WatchForAttack 0x00ED9790, with the
//   counted-pointer operator= of CQ_HangingTreeGoodScript::UpdateWayPoint 0x00D74E80.
// Shape: generated script code; the outer loop is `for (;;)` with the terminate return first (a `while` loop tail-merges
//   the first wait loop's inline-destructor exit into the loop exit); IsAlive is the `Data && Data->IsAlive()` form (the
//   if/return form tests the inlined result with `cmp al,bl`); a function-scope CScriptThing reassigned from the by-value lookup; ~CScriptThing is
//   __forceinline with `#pragma inline_depth(0)` around the returns that call the out-of-line copy.
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
    CCountedPointer() : Data(0), Info(0) { }
    void Release()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    ~CCountedPointer()
    {
        Release();
        Data = 0;
        Info = 0;
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;                  // the source's fields are read first
        T* data = other.Data;
        if (Info != info) {
            Release();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
};
struct CScriptThingImpl {                                   // the game-side thing behind PImp.Data (CScriptThing vtable layout)
    virtual void i000(); virtual void i001(); virtual void i002(); virtual void i003(); virtual void i004(); virtual void i005(); virtual void i006(); virtual void i007();
    virtual void i008(); virtual void i009(); virtual void i010(); virtual void i011(); virtual void i012(); virtual void i013(); virtual void i014(); virtual void i015();
    virtual void i016(); virtual void i017();
    virtual bool MsgIsKilledBy(const CCharString& killer) const;             // slot 18 (+0x48)
    virtual void i019(); virtual void i020();
    virtual bool MsgIsHitBy(const CCharString& attacker) const;              // slot 21 (+0x54)
    virtual void i022(); virtual void i023(); virtual void i024(); virtual void i025(); virtual void i026(); virtual void i027(); virtual void i028(); virtual void i029();
    virtual void i030(); virtual void i031(); virtual void i032(); virtual void i033(); virtual void i034(); virtual void i035(); virtual void i036(); virtual void i037();
    virtual void i038(); virtual void i039(); virtual void i040(); virtual void i041(); virtual void i042();
    virtual bool MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString& attacker) const; // slot 43 (+0xac)
    virtual void i044();
    virtual void i045();
    virtual void i046();
    virtual void i047();
    virtual void i048();
    virtual void i049();
    virtual void i050();
    virtual void i051();
    virtual void i052();
    virtual void i053();
    virtual void i054();
    virtual void i055();
    virtual void i056();
    virtual void i057();
    virtual void i058();
    virtual void i059();
    virtual void i060();
    virtual void i061();
    virtual void i062();
    virtual void i063();
    virtual void i064();
    virtual void i065();
    virtual void i066();
    virtual void i067();
    virtual void i068();
    virtual void i069();
    virtual void i070();
    virtual void i071();
    virtual void i072();
    virtual void i073();
    virtual void i074();
    virtual bool IsAlive() const;                                                      // slot 75 (+0x12c)
};
class CScriptThing : public CBaseClass { public:                          // vtable 0x01238C8C, 12 bytes
    CCountedPointer<CScriptThingImpl> PImp;                 // +0x04 Data, +0x08 Info
    CScriptThing() { }
    CScriptThing(const CScriptThing& other);                // 0x004ABE90
    CScriptThing& operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
    __forceinline ~CScriptThing() {}                                      // out of line 0x004AA840
    bool IsAlive() const                                    // 0x004AB130 (the `&&` form: `test al,al` on the result)
    {
        return PImp.Data && PImp.Data->IsAlive();
    }
    bool MsgIsKilledBy(const CCharString& killer) const     // 0x004AAAF0
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->MsgIsKilledBy(killer);
    }
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
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;                   // 0x0143E8F8
class CTimer { public:                                             // a script timer: the interface's timer index
    long Index;
    void SetTime(long time) { g_pGameScriptInterface->SetTimer(Index, time); }
};
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CV_ArcheryCompetitionScript : public CScriptBase {
public:
    CGameScriptInterface*    Game;                                     // +0x40
    void*         Owner;                                    // +0x44
    unsigned char _pad48[0x51 - 0x48];
    bool          QuestFinished;                            // +0x51
    unsigned char _pad52[0x6c - 0x52];
    CTimer        GameTimeLeft;                             // +0x6c
    unsigned char _pad70[0xa0 - 0x70];
    long          ContestState;                             // +0xa0
    void WatchForOwnerKilled();
};
} // namespace NScript
using NScript::CV_ArcheryCompetitionScript;

void NScript::CV_ArcheryCompetitionScript::WatchForOwnerKilled()
{
    CScriptThing owner;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        while (ContestState != 1) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        owner = Game->GetThingWithScriptName("AC_Owner");
        while (!owner.IsAlive()) {
            Game->NewScriptFrame();
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
        }
#pragma inline_depth(0)
        if (IsActiveThreadTerminating())
            return;
#pragma inline_depth()
        for (;;) {
            if (IsActiveThreadTerminating())
                return;
            if (ContestState == 2) {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
                break;
            }
            if (Game->GetHealth(owner) < 0.0001f) {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
                GameTimeLeft.SetTime(0);
                ContestState = 0;
                QuestFinished = true;
                Game->SetTeleportingAsActive(true);
                Game->EntitySheatheWeapons(Game->GetHero(), false);
                Game->SetHeroWillAsUsable(true);
                break;
            }
            Game->NewScriptFrame();
        }
#pragma inline_depth(0)
        if (IsActiveThreadTerminating())
            return;
#pragma inline_depth()
        Game->NewScriptFrame();
    }
}