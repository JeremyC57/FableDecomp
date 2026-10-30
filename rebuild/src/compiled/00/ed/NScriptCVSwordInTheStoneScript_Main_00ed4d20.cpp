// CV_SwordInTheStoneScript::Main() -- the Sword in the Stone quest main: bind SwordOfStrength / SwordCrowd, wait for a Witchwood region to load, play the guildmaster intro once, spawn WatchForSwordPickup, wait for the hero to take the sword, complete the quest and deactivate it once Witchwood3 unloads
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ED4D20, 0x3f2 bytes, plain `ret`.
// Callees: operator new 0x00BFEA1A, CCharString() 0x0099E4B0 / (const char*, long) 0x0099EBF0 / copy 0x0099EC30 /
//   ~ 0x0099EAE0, strstr 0x00BFEB3A, IsActiveThreadTerminating 0x00CB7940, AddEntityScriptBinding 0x00CB8230,
//   CScriptBase::PostAddScriptedEntities 0x00CB8930, CSpawnedFuncBase 0x00CDD450 (spawn vtable 0x012F3030),
//   AddSpawnedFunction 0x00CB7E50; allocators 0x00ED5130 / 0x00ED5260; thread WatchForSwordPickup 0x00ED57E0;
//   CGameScriptInterface slots 7, 12, 22 MsgOnRegionLoaded, 64, 274 HeroReceiveMessageFromGuildMaster,
//   281 DeactivateQuestLater, 288 SetQuestAsCompleted, 655 GetActiveQuestName.
// Shape: the region-name test is the landed CV_ChapelOfEvilScript::GetVictimIsGuard 0x00E6E050 NameContains helper.
// Layout (retail = Ego_r.pdb - 0x14): Game +0x40, PMasterData +0x44, HeroHasTakenSword +0x52,
//   SwordInTheStoneIntroComplete +0x53; master data +0xc7 is an unnamed sword-in-the-stone-done flag.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharStringData { public: const char* m_pText; unsigned long m_length; };
class CCharString { public:
    CCharStringData* PStringData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const char* GetPString() const { return PStringData == 0 ? "" : PStringData->m_pText; }   // "" = 0x0129AAF4
};
extern "C" char* __cdecl strstr(const char* text, const char* pattern);   // 0x00BFEB3A
__forceinline bool NameContains(const CCharString& name, const char* text)
{
    CCharString pattern(text, -1);
    return strstr(name.GetPString(), pattern.GetPString()) ? true : false;
}
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
template <class T> struct CSpawnedFunc : CSpawnedFuncBase {
    void (T::*Func)();                                  // +0x34
    T* PClass;                                          // +0x38
    __forceinline CSpawnedFunc(const CCharString& name, unsigned long id, T* pclass, void (T::*func)())
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass) {}
    virtual void SuspendableProcess() { (PClass->*Func)(); }
};

class CScriptThing;
class CScriptDataBase;
class CEntityScriptBase;
class CActiveEntityScriptBase;
template <class T> class CCountedPointer { public: T* Data; void* Info; };

class __declspec(novtable) CEntityScriptBindingBase { public:
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

class CGameScriptInterface;
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
class CV_SwordInTheStoneScript : public CScriptBase {
public:
    struct CSwordOfStrength {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_SwordInTheStoneScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00ED5130
    };
    struct CSwordCrowd {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_SwordInTheStoneScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00ED5260
    };
    CGameScriptInterface* Game;                          // +0x40
    unsigned char* PMasterData;                          // +0x44
    long SwordRemovalAttemptCount;                       // +0x48
    bool SwordCutscene;                                  // +0x4c
    bool CrowdAttemptingToPullSword;                     // +0x4d
    bool CrowdSwordPullActive;                           // +0x4e
    bool HeroHasBeenToldSwordLegend;                     // +0x4f
    bool HeroCongratulated;                              // +0x50
    bool CrowdDisperse;                                  // +0x51
    bool HeroHasTakenSword;                              // +0x52
    bool SwordInTheStoneIntroComplete;                   // +0x53
    void Main();
    void WatchForSwordPickup();                          // 0x00ED57E0
};
} // namespace NScript
using NScript::CV_SwordInTheStoneScript;

void NScript::CV_SwordInTheStoneScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CV_SwordInTheStoneScript>("SwordOfStrength", this, &CSwordOfStrength::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_SwordInTheStoneScript>("SwordCrowd", this, &CSwordCrowd::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    CCharString region;
    if (IsActiveThreadTerminating())
        return;
    for (;;) {
        while (!Game->MsgOnRegionLoaded(region)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        if (NameContains(region, "Witchwood"))
            break;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())       // retail tests twice here (two translator statement guards back to back)
        return;
    if (IsActiveThreadTerminating())
        return;
    if (!SwordInTheStoneIntroComplete) {
        if (IsActiveThreadTerminating())
            return;
        Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_062_GM_INTRO", "", true, true);
        SwordInTheStoneIntroComplete = true;
    }
    SpawnFunction(new CSpawnedFunc<CV_SwordInTheStoneScript>("WatchForSwordPickup", 0, this, &NScript::CV_SwordInTheStoneScript::WatchForSwordPickup));
    while (!(HeroHasTakenSword == true)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    PMasterData[0xc7] = true;
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(), false, false, false);
    while (!!Game->IsRegionLoaded("Witchwood3")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}