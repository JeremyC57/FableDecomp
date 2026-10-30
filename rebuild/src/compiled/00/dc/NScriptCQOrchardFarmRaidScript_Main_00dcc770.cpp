// CQ_OrchardFarmRaidScript::Main() -- the Orchard Farm raid quest's main: bind the team-spawn / artefact / Whisper / team-member / rear-entrance / intro-marker entity scripts, make sure the barricade quest is active, wait for OrchardFarm, fetch both teams' crate drop points, then spawn the game-rules thread for the hero's team (evil or good), the cutscene thread and the external-deactivation watcher
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DCC770, length 1981 bytes. Catalog label "CQ_GuildTrainingScript::Main" is
//   propagated: the body uses the CQ_OrchardFarmRaidScript strings and spawns the landed
//   CQ_OrchardFarmRaidScript::WatchForExternalScriptDeactivation 0x00DCCF30.
// Callees: operator new 0x00BFEA1A, operator delete 0x00BFE9BC, CCharString(const char*, long) 0x0099EBF0 /
//   CCharString(const CCharString&) 0x0099EC30 / ~CCharString 0x0099EAE0, ~CScriptThing 0x004AA840,
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940 / AddEntityScriptBinding 0x00CB8230 / PostAddScriptedEntities
//   0x00CB8930 / AddSpawnedFunction 0x00CB7E50, CSpawnedFuncBase ctor 0x00CDD450 (CSpawnedFunc vtable 0x012DB13C);
//   binding allocators (binding vtable 0x012DB2BC): CTeamSpawn 0x00DCD0E0 (GuardTeamSpawn, BanditTeamSpawn), CArtefact
//   0x00DCDB80, COrchardFarmWhisper 0x00DCEF50, CCrateTeamMember 0x00DCDE20 (GuardTeamMember, BanditTeamMember),
//   CFarmRearEntrance 0x00DCF3D0, CWhisperFarmRaidIntro 0x00DCF540, CWhisperMarker 0x00DCF610; spawned functions
//   ProcessGameRulesEvil 0x00DD03D0, ProcessGameRulesGood 0x00DD0F60, DoCutsceneIfRequired 0x00DCFA60,
//   WatchForExternalScriptDeactivation 0x00DCCF30; the game script interface (+0x40) slots 7, 12, 64, 72, 276, 284.
// Layout (retail = Ego_r.pdb - 0x14, and - 0x18 past the 12-byte retail STLport CrateList vector): HeroTeam +0x78,
//   Teams[2] (CCrateTeamManager, 0x40 bytes each) +0x8c; CCrateTeamManager::CrateDropPos +0x24.
// Shape: bindings / spawned functions as the landed CQ_GuildTrainingWoodsDepartureScript::Main 0x00D61D20; the crate
//   drop assignment is CScriptThing::operator= over the counted pointer (as the landed CCrateTeamMember::Init 0x00DCDF60)
//   with the returned temporary's ~CScriptThing out of line.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
__forceinline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess
class CScriptDataBase;
class CEntityScriptBase;
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
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CCrateTeamManager {                              // 0x40 bytes
    long  StateCounter[6];                              // +0x00
    long  MemberCount;                                  // +0x18
    long  TeamReinforcementsTimer;                      // +0x1c (CTimer)
    long  MaxTeamMembers;                               // +0x20
    CScriptThing CrateDropPos;                          // +0x24
    CScriptThing TeamCrateCarrier;                      // +0x30
    CCrateTeamManager* EnemyTeam;                       // +0x3c
};
namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_OrchardFarmRaidScript : public CScriptBase {
public:
#define ALLOC(Name, Addr) struct Name { static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_OrchardFarmRaidScript& parent, CScriptDataBase* data, const CScriptThing& thing); };
    ALLOC(CTeamSpawn, 0x00DCD0E0)
    ALLOC(CArtefact, 0x00DCDB80)
    ALLOC(COrchardFarmWhisper, 0x00DCEF50)
    ALLOC(CCrateTeamMember, 0x00DCDE20)
    ALLOC(CFarmRearEntrance, 0x00DCF3D0)
    ALLOC(CWhisperFarmRaidIntro, 0x00DCF540)
    ALLOC(CWhisperMarker, 0x00DCF610)
#undef ALLOC
    CGameScriptInterface* Game;                         // +0x40
    void*                 Data;                         // +0x44
    unsigned char         _pad48[0x78 - 0x48];          // CrateCount +0x48 .. MissionFailed +0x68, FailReasons +0x6c
    long                  HeroTeam;                     // +0x78
    unsigned char         _pad7c[0x8c - 0x7c];          // BanditWavesSpawned, RemindHeroOfObjectivesTimer, TextSystemScriptCode, flags
    CCrateTeamManager     Teams[2];                     // +0x8c
    void Main();
    void ProcessGameRulesEvil();                        // 0x00DD03D0
    void ProcessGameRulesGood();                        // 0x00DD0F60
    void DoCutsceneIfRequired();                        // 0x00DCFA60
    void WatchForExternalScriptDeactivation();          // 0x00DCCF30
};
void CQ_OrchardFarmRaidScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_OrchardFarmRaidScript>("GuardTeamSpawn", this, &CTeamSpawn::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_OrchardFarmRaidScript>("BanditTeamSpawn", this, &CTeamSpawn::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_OrchardFarmRaidScript>("Artefact", this, &CArtefact::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_OrchardFarmRaidScript>("OrchardFarmWhisper", this, &COrchardFarmWhisper::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_OrchardFarmRaidScript>("GuardTeamMember", this, &CCrateTeamMember::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_OrchardFarmRaidScript>("BanditTeamMember", this, &CCrateTeamMember::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_OrchardFarmRaidScript>("FarmRearEntrance", this, &CFarmRearEntrance::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_OrchardFarmRaidScript>("M_WhisperFarmRaidIntro", this, &CWhisperFarmRaidIntro::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_OrchardFarmRaidScript>("MK_OFI_GWLL_WHIS2", this, &CWhisperMarker::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    if (!Game->IsQuestActive("Q_OrchardFarm_Barricade"))
        Game->ActivateQuest("Q_OrchardFarm_Barricade");
    while (!Game->IsRegionLoaded("OrchardFarm")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Teams[1].CrateDropPos = Game->GetThingWithScriptName("BanditTeamCrateDrop");
    Teams[0].CrateDropPos = Game->GetThingWithScriptName("GuardTeamCrateDrop");
    if (HeroTeam == 1)
        AddSpawnedFunction(new CSpawnedFunc<CQ_OrchardFarmRaidScript>("ProcessGameRulesEvil", 0, this, &CQ_OrchardFarmRaidScript::ProcessGameRulesEvil), CCharString(""));
    else
        AddSpawnedFunction(new CSpawnedFunc<CQ_OrchardFarmRaidScript>("ProcessGameRulesGood", 0, this, &CQ_OrchardFarmRaidScript::ProcessGameRulesGood), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_OrchardFarmRaidScript>("DoCutsceneIfRequired", 0, this, &CQ_OrchardFarmRaidScript::DoCutsceneIfRequired), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_OrchardFarmRaidScript>("WatchForExternalScriptDeactivation", 0, this, &CQ_OrchardFarmRaidScript::WatchForExternalScriptDeactivation), CCharString(""));
}
}