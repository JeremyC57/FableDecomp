// CV_MayorsInvitationScript::WatchExposeMayorProgress() -- once the bribe path is taken hand the hero the Mayor's Invitation quest card (once) and set its objective; if the hero duel has not been fought yet also start MonitorThunderHeroDuel
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB94A0, 0x264 bytes, plain `ret`. PDB ?WatchExposeMayorProgress@CV_MayorsInvitationScript@NScript@@QAEXXZ.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, IsActiveThreadTerminating 0x00CB7940,
//   operator new 0x00BFEA1A, CSpawnedFuncBase(const CCharString&, unsigned long) 0x00CDD450 (CSpawnedFunc<CV_MayorsInvitationScript>
//   vtable 0x012F0548, MonitorThunderHeroDuel 0x00EB70F0), AddSpawnedFunction 0x00CB7E50; CGameScriptInterface slots 7
//   NewScriptFrame, 295 GiveHeroQuestCardDirectly, 296 SetQuestCardObjective, 655 GetActiveQuestName.
// Shape: the spawn uses the inline `SpawnFunction(func, const char* region = "")` (region temporary built after new, the
//   thread name guarded by a flag in ebx); QuestCardGiven is set at the end of both arms.
// Layout (retail = Ego_r.pdb - 0x14): Game +0x40, the shared master data +0x44 (its +0x63 and +0xd5 are unnamed flags:
//   the hero duel is done / the bribe path was taken, cf. PhaseFive 0x00E7A740), QuestCardGiven +0x50.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)

class CCPPointerInfo { public:                                  // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);           // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
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
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
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
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class C3DVector { public: float X, Y, Z; };
extern C3DVector g_ZeroVector;                           // 0x0143E8E0
struct CScriptThingImp {                                 // the game-side thing behind a CScriptThing
    virtual void i0(); virtual void i1(); virtual void i2(); virtual void i3(); virtual void i4(); virtual void i5();
    virtual const C3DVector& GetPosition() const;        // slot 6 (+0x18)
};
class CScriptThing : public CBaseClass { public:                       // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThingImp> PImp;               // +0x04 Data, +0x08 Info
    CScriptThing() {}
    virtual ~CScriptThing();                             // 0x004AA840
    virtual void s01(); virtual void s02(); virtual void s03(); virtual void s04(); virtual void s05(); virtual void s06(); virtual void s07(); virtual void s08(); virtual void s09(); virtual void s10();
    virtual void s11(); virtual void s12(); virtual void s13(); virtual void s14(); virtual void s15(); virtual void s16(); virtual void s17(); virtual void s18(); virtual void s19(); virtual void s20();
    virtual void s21(); virtual void s22(); virtual void s23(); virtual void s24(); virtual void s25(); virtual void s26(); virtual void s27(); virtual void s28(); virtual void s29(); virtual void s30();
    virtual void s31(); virtual void s32(); virtual void s33(); virtual void s34(); virtual void s35(); virtual void s36(); virtual void s37(); virtual void s38(); virtual void s39(); virtual void s40();
    virtual void s41(); virtual void s42(); virtual void s43(); virtual void s44(); virtual void s45(); virtual void s46(); virtual void s47(); virtual void s48(); virtual void s49(); virtual void s50();
    virtual void s51(); virtual void s52(); virtual void s53(); virtual void s54(); virtual void s55(); virtual void s56(); virtual void s57(); virtual void s58(); virtual void s59(); virtual void s60();
    virtual void s61(); virtual void s62(); virtual void s63(); virtual void s64(); virtual void s65(); virtual void s66(); virtual void s67(); virtual void s68(); virtual void s69(); virtual void s70();
    virtual void s71(); virtual void s72(); virtual void s73(); virtual void s74();
    virtual bool IsAlive() const;                        // slot 75 (+0x12c) 0x004AB130
    const C3DVector& GetPosition() const { return !PImp.Data ? g_ZeroVector : PImp.Data->GetPosition(); }
    __forceinline CScriptThing& operator=(const CScriptThing& other)
    {
        PImp = other.PImp;
        return *this;
    }
};
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
template <class T> struct CSpawnedFunc : CSpawnedFuncBase {
    void (T::*Func)();                                   // +0x34
    T* PClass;                                           // +0x38
    __forceinline CSpawnedFunc(const CCharString& name, unsigned long id, T* pclass, void (T::*func)())
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass) {}
    virtual void SuspendableProcess() { (PClass->*Func)(); }
};
class CScriptBase { public:                                     // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                    // 0x00CB7940
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
    __forceinline void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CMasterScriptData {
    unsigned char _pad00[0x63];
    bool HeroDuelDone;                                   // +0x63 (unnamed)
    unsigned char _pad64[0xd5 - 0x64];
    bool MayorBribePathTaken;                            // +0xd5 (unnamed)
};
namespace NScript {
class CV_MayorsInvitationScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                          // +0x40
    CMasterScriptData* PMasterData;                      // +0x44
    bool MissionSucceeded;                               // +0x48
    bool MissionFailed;                                  // +0x49
    bool MissionOver;                                    // +0x4a
    bool HeroAbortedQuest;                               // +0x4b
    bool HeroLearnedSecret;                              // +0x4c
    bool HeroReturnedFromParanoidQuest;                  // +0x4d
    bool HeroHasDefeatedThunder;                         // +0x4e
    bool HeroHasLostToThunder;                           // +0x4f
    bool QuestCardGiven;                                 // +0x50
    void MonitorThunderHeroDuel();                       // 0x00EB70F0
    void WatchExposeMayorProgress();
};
} // namespace NScript
using NScript::CV_MayorsInvitationScript;

void NScript::CV_MayorsInvitationScript::WatchExposeMayorProgress()
{
    while (!PMasterData->MayorBribePathTaken) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (QuestCardGiven)
        return;
    if (IsActiveThreadTerminating())
        return;
    Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_MAYORS_INVITATION", Game->GetActiveQuestName(), false);
    if (PMasterData->HeroDuelDone) {
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_MAYORS_INVITATION_OBJECTIVE_09", "", "");
    } else {
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_MAYORS_INVITATION_OBJECTIVE_07", "", "");
        SpawnFunction(new CSpawnedFunc<CV_MayorsInvitationScript>("MonitorThunderHeroDuel", 0, this, &NScript::CV_MayorsInvitationScript::MonitorThunderHeroDuel));
    }
    QuestCardGiven = true;
}