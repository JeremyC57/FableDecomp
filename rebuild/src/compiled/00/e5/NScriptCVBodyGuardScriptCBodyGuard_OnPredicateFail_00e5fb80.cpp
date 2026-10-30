// NScript::CV_BodyGuardScript::CBodyGuard::OnPredicateFail() -- a henchman is gone: drop him from the henchman list and his health bar, stop his wages if hired; when he was killed (or sacrificed) mark his type dead and start its 240-second respawn timer
// Leaf: ?OnPredicateFail@CBodyGuard@CV_BodyGuardScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E5FB80, 232 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 5).
// Identity (proven): slot 5 (OnPredicateFail) of the RTTI-less entity vtable 0x012E701C, which the landed
//   constructor 0x00E5DDE0 (manifest label CDarkwoodTrader::CDarkwoodTrader is propagated) of the "BodyGuard"
//   binding's Alloc 0x00E5DD60 (bound by NScript::CV_BodyGuardScript::Main 0x00E5D4F0 at 0x00E5D53C) stores (site 0x00E5DE01); Ego_r.pdb has ?OnPredicateFail@CBodyGuard@CV_BodyGuardScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CBodyGuard (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, members from +0x1c;
//   the parent level script's members sit at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: CScriptThing::operator== (Ego_r.pdb ??8CScriptThing@@QBE_NABV0@@Z, inline) through the implementation's slot 78
//   IsEqualTo; std::vector<CScriptThing>::erase(iterator) 0x00CD3152; interface slot 338 RemoveQuestInfoElement;
//   CBodyGuard::GetGoldCost 0x00E5FC70 (Ego_r.pdb ?GetGoldCost@CBodyGuard@CV_BodyGuardScript@NScript@@AAEJXZ, the only
//   long() member of the class; landed under the descriptive name GetHenchmanGoldCost); CScriptThing slot 18 MsgIsKilledBy
//   ("" 0x0122D70E temporary); CTimer::operator=(long) inline: g_pGameScriptInterface 0x0143E8F8 slot 89 SetTimer.
// Parent members: Ego_r.pdb CV_BodyGuardScript - 0x14 (HenchmanList is the one vector before the arrays).
class CCharString {
public:
    void* PData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CCounter {                                        // Ego_r.pdb: one long
public:
    long RelevantWorldFrame;
};
namespace NScript {
class CQ_SunnyvaleMasterData {                          // Ego_r.pdb layout (offsets equal retail at the head)
public:
    virtual ~CQ_SunnyvaleMasterData();
    long PostSavePosition;                              // +0x04 (EGameflowPosition)
    long HenchmanCommentaryState;                       // +0x08
    long HenchmanOverrideState;                         // +0x0c
    bool HenchmanSacrificedBodge;                       // +0x10
};
}
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
#include "engine/CCountedPointer.h"
class CScriptThing : public CBaseClass {                // 12 bytes, vtable 0x01238C8C
public:
    virtual ~CScriptThing();                            // slot 0
    virtual void t01(); virtual void t02(); virtual void t03(); virtual void t04(); virtual void t05(); virtual void t06(); virtual void t07(); virtual void t08();
    virtual void t09(); virtual void t10(); virtual void t11(); virtual void t12(); virtual void t13(); virtual void t14(); virtual void t15(); virtual void t16();
    virtual void t17();
    virtual bool MsgIsKilledBy(const CCharString& killer) const;   // slot 18 (+0x48)
    virtual void t19(); virtual void t20(); virtual void t21(); virtual void t22(); virtual void t23(); virtual void t24(); virtual void t25(); virtual void t26();
    virtual void t27(); virtual void t28(); virtual void t29(); virtual void t30(); virtual void t31(); virtual void t32(); virtual void t33(); virtual void t34();
    virtual void t35(); virtual void t36(); virtual void t37(); virtual void t38(); virtual void t39(); virtual void t40(); virtual void t41(); virtual void t42();
    virtual void t43(); virtual void t44(); virtual void t45(); virtual void t46(); virtual void t47(); virtual void t48(); virtual void t49(); virtual void t50();
    virtual void t51(); virtual void t52(); virtual void t53(); virtual void t54(); virtual void t55(); virtual void t56(); virtual void t57(); virtual void t58();
    virtual void t59(); virtual void t60(); virtual void t61(); virtual void t62(); virtual void t63(); virtual void t64(); virtual void t65(); virtual void t66();
    virtual void t67(); virtual void t68(); virtual void t69(); virtual void t70(); virtual void t71(); virtual void t72(); virtual void t73(); virtual void t74();
    virtual void t75(); virtual void t76(); virtual void t77();
    virtual bool IsEqualTo(const CScriptThing* other) const;   // slot 78 (+0x138) (on the implementation)
    bool operator==(const CScriptThing& other) const { return PImp->IsEqualTo(&*other.PImp); }
    CCountedPointer<CScriptThing> PImp;                 // +0x04 Data, +0x08 Info
};
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {   // STLport 4.6: start, finish, end of storage (0xC)
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    T* erase(T* position);                              // 0x00CD3152 for vector<CScriptThing>
};
}

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8 (the global script interface, bound name)
class CTimer {                                          // Ego_r.pdb: one long TimerIndex
public:
    long TimerIndex;
    CTimer& operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }
};
class CScriptBase;
class CPersistContext;
class CEntityScriptBase {                               // Ego_r.pdb: vptr, Game +0x04, Me +0x08 (20 bytes)
public:
    virtual ~CEntityScriptBase();                       // slot 0
    virtual void Main() = 0;                            // slot 1
    virtual void Init();                                // slot 2
    virtual CScriptBase* GetParentScript() const = 0;   // slot 3
    virtual void OnPersist(CPersistContext& context);   // slot 4
    virtual void OnPredicateFail();                     // slot 5
    virtual void OnInterrupted();                       // slot 6
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
};
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_BodyGuardScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    long TotalWages;                                    // +0x48
    bool HeroCannotAffordHelp;                          // +0x4c
    bool HeroWieldingWeapon;                            // +0x4d
    CCounter HeroStoodStillCounter;                     // +0x50
    CTimer HenchmanLastIdleChatTimer;                   // +0x54
    CTimer HenchmanLastBoredChatTimer;                  // +0x58
    CTimer CommentaryTimer;                             // +0x5c
    CTimer HenchmanFreakedOutThePlayerTimer;            // +0x60
    std::vector<CScriptThing> HenchmanList;             // +0x64
    long ConversationIndex;                             // +0x70
    CTimer HenchmanDeadRespawnTimer[4];                 // +0x74
    long HenchmanGlobalStatus[4];                       // +0x84
    class CBodyGuard;
};
class CV_BodyGuardScript::CBodyGuard : public CEntityScriptBase {
public:
    CV_BodyGuardScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;   // +0x18
    long HenchManType;                                  // +0x1c
    long BrainState;                                    // +0x20
    long CurrentHealthState;                            // +0x24
    long PreviousHealthState;                           // +0x28
    bool FreakedOut;                                    // +0x2c
    CTimer AngerTimer;                                  // +0x30
    CTimer AttackedBySummonersTimer;                    // +0x34
    long MyHealthBarID;                                 // +0x38
private:
    long GetGoldCost();                                 // 0x00E5FC70
private:
    virtual void OnPredicateFail();
};

void CV_BodyGuardScript::CBodyGuard::OnPredicateFail()
{
    for (CScriptThing* it = ParentClass->HenchmanList.begin(); it != ParentClass->HenchmanList.end(); ++it) {
        if (Me == *it) {
            ParentClass->HenchmanList.erase(it);
            break;
        }
    }
    Game->RemoveQuestInfoElement(MyHealthBarID);
    if (BrainState == 2)
        ParentClass->TotalWages -= GetGoldCost();
    BrainState = 1;
    if (Me.MsgIsKilledBy("") || PMasterData->HenchmanSacrificedBodge) {
        ParentClass->HenchmanGlobalStatus[HenchManType] = 1;
        ParentClass->HenchmanDeadRespawnTimer[HenchManType] = 240;
    }
}
}