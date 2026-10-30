// CQ_NewOakValeIntroScript::DoMission() -- wait for StartOakVale; on first run fade out, turn the hero into a child, spawn the barrel/gold/marker threads, set up the intro (music, pre-attack quest, unkillable hero, stopped noon clock, no sleeping, money bag, hero's old house unlocked, start screen, intro music) and wait for the intro flag; then run the attack and post-attack phases, restore the hero, complete and deactivate the quest
// Leaf: DoMission
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DBDE40, 1172 bytes (int3 padded to 1184).
// Callees: CCharString 0x0099EBF0 / ~ 0x0099EAE0, ~CScriptThing 0x004AA840, IsActiveThreadTerminating 0x00CB7940,
//   operator new 0x00BFEA1A, CSpawnedFuncBase 0x00CDD450 (spawn vtable 0x012D7A3C), AddSpawnedFunction 0x00CB7E50; threads
//   WatchBarrels 0x00DBE890, WatchForGotGold 0x00DBE2E0, ManageQuestCoreMarkers 0x00DBE4E0; AttackStuff 0x00DBE3C0,
//   PostAttackStuff 0x00DBEB20; CGameScriptInterface slots 7, 12, 70, 72, 94, 276, 280, 281, 288, 309, 341, 372, 427, 431, 517,
//   570, 646, 648, 655, 696, 698.
// Layout: Game +0x40, TimeStoppedToken +0x4c (long& for SetTimeAsStopped), IntroDone +0x50.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const char* text);           // 0x0099EFE0
};
class CScriptThing { public:                                   // 12 bytes {vptr, PImp.Data, PImp.Info}
    void* VTable;
    void* PImpData;
    void* PImpInfo;
    ~CScriptThing();                                    // 0x004AA840
};
class CWideString { public:
    void* PStringData;
    CWideString(const unsigned short* text);            // 0x0099B6B0
    ~CWideString();                                     // 0x0099B510
};
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess

class CScriptThing;
class CScriptDataBase;
class CEntityScriptBase;
class CActiveEntityScriptBase;
template <class T> class CCountedPointer { public: T* Data; void* Info; };

struct __declspec(novtable) CEntityScriptBindingBase {
    virtual const CCharString& GetEntityScriptName() const = 0;
};
template <class T> struct CEntityScriptBinding : CEntityScriptBindingBase {
    typedef CCountedPointer<CEntityScriptBase> (__fastcall *TAllocFunc)(T& parent, CScriptDataBase* data, const CScriptThing& thing);
    CCharString     EntityScriptName;                   // +0x04
    T*              PParentScript;                      // +0x08
    void (T::*Func)();                                  // +0x0c
    TAllocFunc      ClassAllocFunc;                     // +0x10
    bool            ClassAllocValid;                    // +0x14
    unsigned long   Flags;                              // +0x18
    CEntityScriptBinding(const CCharString& name, T* parent, TAllocFunc alloc, unsigned long flags)
        : EntityScriptName(name), PParentScript(parent), ClassAllocFunc(alloc), ClassAllocValid(true), Flags(flags) {}
    virtual const CCharString& GetEntityScriptName() const { return EntityScriptName; }
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase { public:                                     // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                                                 // 0x00CB7940
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
    void AddEntityScriptBinding(CEntityScriptBindingBase* binding);                   // 0x00CB8230
    void PostAddScriptedEntities();                                                   // 0x00CB8930
    __forceinline void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
};
namespace NScript {
class CQ_NewOakValeIntroScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void*                 PMasterData;                  // +0x44
    unsigned char         _pad48[4];
    long                  TimeStoppedToken;             // +0x4c
    bool                  IntroDone;                    // +0x50
    void DoMission();
    void WatchBarrels();                                // 0x00DBE890
    void WatchForGotGold();                             // 0x00DBE2E0
    void ManageQuestCoreMarkers();                      // 0x00DBE4E0
    void AttackStuff();                                 // 0x00DBE3C0
    void PostAttackStuff();                             // 0x00DBEB20
};
} // namespace NScript
using NScript::CQ_NewOakValeIntroScript;

void NScript::CQ_NewOakValeIntroScript::DoMission()
{
    while (!Game->IsRegionLoaded("StartOakVale")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (!IntroDone) {
        if (IsActiveThreadTerminating())
            return;
        Game->FadeScreenOutUntilNextCallToFadeScreenIn(2.0f, 0.0f);
        Game->TurnCreatureInto(Game->GetHero(), "CREATURE_HERO_CHILD");
        SpawnFunction(new CSpawnedFunc<CQ_NewOakValeIntroScript>("WatchBarrels", 0, this, &NScript::CQ_NewOakValeIntroScript::WatchBarrels));
        SpawnFunction(new CSpawnedFunc<CQ_NewOakValeIntroScript>("WatchForGotGold", 0, this, &NScript::CQ_NewOakValeIntroScript::WatchForGotGold));
        SpawnFunction(new CSpawnedFunc<CQ_NewOakValeIntroScript>("ManageQuestCoreMarkers", 0, this, &NScript::CQ_NewOakValeIntroScript::ManageQuestCoreMarkers));
        Game->CacheMusicSet((EMusicSetType)0x2e);
        Game->ActivateQuest("Q_NewOakValeIntro_PreAttack");
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        Game->EntitySetAsKillable(Game->GetHero(), false, false);
        Game->SetTimeAsStopped(true, TimeStoppedToken);
        Game->SetTimeOfDay(12.0f);
        Game->SetHeroSleepingAsEnabled(false);
        Game->DisplayMoneyBag(true);
        CScriptThing house = Game->GetThingWithScriptName("HerosOldHouse");
        Game->OverrideAutomaticHouseLocking(house, true);
        Game->OpenHouseDoors(house);
        Game->KickOffQuestStartScreen(Game->GetActiveQuestName(), false, true);
        Game->OverrideMusic((EMusicSetType)0x13, false, true);
    }
    while (!IntroDone) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    AttackStuff();
    PostAttackStuff();
    Game->FadeScreenOutUntilNextCallToFadeScreenIn(0.5f, 0.0f);
    Game->EntitySetAsKillable(Game->GetHero(), true, false);
    Game->SetHeroSleepingAsEnabled(true);
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(), false, false, false);
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}