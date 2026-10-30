// NScript::CV_IntroductionToTrophiesScript::Main() -- the Introduction-to-Trophies quest main: bind the BriarRose, TrophiesIntroWatcher, TrophiesBriarTrophy and TrophiesIntroTable entity scripts, spawn the WatchForInsideTavern / WatchForCrowdVolume / WatchForQuestComplete threads, wait for QuestComplete, then activate V_TalentlessBard and deactivate the active quest
// Leaf: ?Main@CV_IntroductionToTrophiesScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E9C4B0, 926 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Main) of the quest-script vtable 0x012ED558 [dtor 0x00E9FBA0, RegisterMain 0x00E9C410 (landed
//   as NScript::CV_IntroductionToTrophiesScript::RegisterMain), Main 0x00E9C4B0, Init 0x00E9C3B0 (landed ...::Init),
//   OnPersist 0x00E9E110]; Ego_r.pdb 0x00F05240 ?Main@CV_IntroductionToTrophiesScript@NScript@@UAEXXZ. The four binding
//   factories are the class's landed Alloc statics (CBriarRose 0x00E9D9D0, CTrophiesIntroWatcher 0x00E9CEE0,
//   CTrophiesBriarTrophy 0x00E9D700, CTrophiesIntroTable 0x00E9D8E0) and the three spawned threads are the PDB members
//   WatchForInsideTavern 0x00E9CB20 (landed), WatchForCrowdVolume 0x00E9C8D0 and WatchForQuestComplete 0x00E9C850 (each
//   paired with its own name string). Catalog label: sub_00e9c4b0 (gap scan).
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0,
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940 / AddSpawnedFunction 0x00CB7E50 / AddEntityScriptBinding 0x00CB8230 /
//   PostAddScriptedEntities 0x00CB8930, CSpawnedFuncBase ctor 0x00CDD450; vtables CEntityScriptBinding<T> 0x012ED5F4,
//   CSpawnedFunc<T> 0x012ED56C; CGameScriptInterface slots 7 NewScriptFrame, 64 PostAddScriptedEntities, 276 ActivateQuest,
//   281 DeactivateQuestLater, 655 GetActiveQuestName (ghidra_out/cgsi_slots_aligned.tsv).
// Literals: "BriarRose" 0x012D42EC, "TrophiesIntroWatcher" 0x012ED5DC, "TrophiesBriarTrophy" 0x012ED5C8, "TrophiesIntroTable"
//   0x012ED5B4, "WatchForInsideTavern" 0x012ED59C, "WatchForCrowdVolume" 0x012ED588, "WatchForQuestComplete" 0x012E3838,
//   "" 0x0122D70E, "V_TalentlessBard" 0x012C54A4.
// Layout (retail = Ego_r.pdb - 0x14; Ego_r CV_IntroductionToTrophiesScript is 0x80 bytes): Game +0x40, PMasterData +0x44,
//   DoneBriarIntroSpeech +0x48 .. AttackSpeechComplete +0x4D, HitByHeroCount +0x50, HitWarningComplete +0x54,
//   InsideTavern +0x55, QuestComplete +0x56 (Ego_r +0x6A, the flag this Main waits on), OpinionCurrent +0x58,
//   OpinionRequired +0x5C, CutsceneStarted +0x60 .. OutsideTavernTimerReset +0x62, OpinionTimer +0x64, OutsideTavernTimer +0x68.
// Model: the landed NScript::CV_RockTrollFirstEncounterScript::Main 0x00EC3D40 (CEntityScriptBinding / CSpawnedFunc /
//   SpawnFunction, polymorphic CScriptBase, CGameScriptInterfaceBase keeping its padding virtuals).
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int bytes);         // 0x00BFEA1A (MSVCR71)
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess
class CScriptThing;
class CScriptDataBase;
class CEntityScriptBase;
template <class T> class CCountedPointer {
public:
    T* Data;
    void* Info;
};
class __declspec(novtable) CEntityScriptBindingBase {
public:
    virtual const CCharString& GetEntityScriptName() const = 0;
};
template <class T> class CEntityScriptBinding : public CEntityScriptBindingBase {   // 0x1c bytes
public:
    typedef CCountedPointer<CEntityScriptBase> (__fastcall *TAllocFunc)(T& parent, CScriptDataBase* data, const CScriptThing& thing);
    CCharString     EntityScriptName;                   // +0x04
    T*              PParentScript;                      // +0x08
    void (T::*Func)();                                  // +0x0c
    TAllocFunc      ClassAllocFunc;                     // +0x10
    bool            ClassAllocValid;                    // +0x14
    unsigned long   Flags;                              // +0x18
    CEntityScriptBinding(const CCharString& name, T* parent, TAllocFunc alloc, unsigned long flags)
        : EntityScriptName(name), PParentScript(parent), ClassAllocFunc(alloc), ClassAllocValid(true), Flags(flags) { }
    virtual const CCharString& GetEntityScriptName() const { return EntityScriptName; }
};
// CGameScriptInterfaceBase / CGameScriptInterface are the retail script interface from the shared headers
//   (FABLE_RETAIL_NSCRIPT opt-in: CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase {                                     // retail 0x40 bytes
public:
    virtual ~CScriptBase();
    virtual void RegisterMain();                        // slot 1
    virtual void Main() = 0;                            // slot 2
    virtual void Init();                                // slot 3
    unsigned char _body[0x3c];                          // +0x04 .. +0x40
    bool IsActiveThreadTerminating();                                                 // 0x00CB7940
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
    void AddEntityScriptBinding(CEntityScriptBindingBase* binding);                   // 0x00CB8230
    void PostAddScriptedEntities();                                                   // 0x00CB8930
    __forceinline void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
};
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
namespace NScript {
class CV_IntroductionToTrophiesScript : public CScriptBase {
public:
    class CBriarRose {
    public:
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_IntroductionToTrophiesScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E9D9D0
    };
    class CTrophiesIntroWatcher {
    public:
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_IntroductionToTrophiesScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E9CEE0
    };
    class CTrophiesBriarTrophy {
    public:
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_IntroductionToTrophiesScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E9D700
    };
    class CTrophiesIntroTable {
    public:
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_IntroductionToTrophiesScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E9D8E0
    };
    CGameScriptInterface* Game;                         // +0x40
    void* PMasterData;                                  // +0x44
    bool DoneBriarIntroSpeech;                          // +0x48
    bool DoneBriarTrophySpeech;                         // +0x49
    bool AttackedByHero;                                // +0x4a
    bool DoneApproachMessage1;                          // +0x4b
    bool DoneApproachMessage2;                          // +0x4c
    bool AttackSpeechComplete;                          // +0x4d
    long HitByHeroCount;                                // +0x50
    bool HitWarningComplete;                            // +0x54
    bool InsideTavern;                                  // +0x55
    bool QuestComplete;                                 // +0x56
    long OpinionCurrent;                                // +0x58 (BriarRoseOpinion)
    long OpinionRequired;                               // +0x5c (BriarRoseOpinion)
    bool CutsceneStarted;                               // +0x60
    bool OpinionTimerReset;                             // +0x61
    bool OutsideTavernTimerReset;                       // +0x62
    CTimer OpinionTimer;                                // +0x64
    CTimer OutsideTavernTimer;                          // +0x68
    virtual void Main();
    void WatchForQuestComplete();                       // 0x00E9C850
    void WatchForCrowdVolume();                         // 0x00E9C8D0
    void WatchForInsideTavern();                        // 0x00E9CB20
};

void CV_IntroductionToTrophiesScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CV_IntroductionToTrophiesScript>("BriarRose", this, &CBriarRose::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_IntroductionToTrophiesScript>("TrophiesIntroWatcher", this, &CTrophiesIntroWatcher::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_IntroductionToTrophiesScript>("TrophiesBriarTrophy", this, &CTrophiesBriarTrophy::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_IntroductionToTrophiesScript>("TrophiesIntroTable", this, &CTrophiesIntroTable::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    SpawnFunction(new CSpawnedFunc<CV_IntroductionToTrophiesScript>("WatchForInsideTavern", 0, this, &CV_IntroductionToTrophiesScript::WatchForInsideTavern));
    SpawnFunction(new CSpawnedFunc<CV_IntroductionToTrophiesScript>("WatchForCrowdVolume", 0, this, &CV_IntroductionToTrophiesScript::WatchForCrowdVolume));
    SpawnFunction(new CSpawnedFunc<CV_IntroductionToTrophiesScript>("WatchForQuestComplete", 0, this, &CV_IntroductionToTrophiesScript::WatchForQuestComplete));
    while (!QuestComplete) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->ActivateQuest("V_TalentlessBard");
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}
}   // namespace NScript