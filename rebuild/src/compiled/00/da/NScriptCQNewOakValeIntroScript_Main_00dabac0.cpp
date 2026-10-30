// CQ_NewOakValeIntroScript::Main() -- the new Oakvale intro quest's main: bind the sixteen NOVI_* / OVI_DeadFather entity scripts, finish adding scripted entities, close the post-attack quest if the attack is already over, set the active quest's first objective, spawn StartBarrelTimer and run DoMission
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DABAC0, length 2018 bytes. Catalog label "CPersonalScript_GlobalThingsScript::Main"
//   is propagated: the vtable 0x012D7A30 slot holding this body and the NOVI_ strings / DoMission 0x00DBDE40 call are
//   CQ_NewOakValeIntroScript's.
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / CCharString(const CCharString&) 0x0099EC30 /
//   ~CCharString 0x0099EAE0, CScriptBase::IsActiveThreadTerminating 0x00CB7940 / AddEntityScriptBinding 0x00CB8230 /
//   PostAddScriptedEntities 0x00CB8930 / AddSpawnedFunction 0x00CB7E50, CSpawnedFuncBase ctor 0x00CDD450 (CSpawnedFunc vtable
//   0x012D7A3C), StartBarrelTimer 0x00DB4F70 (catalog label "Script_Guard_Duty" is propagated), DoMission 0x00DBDE40; the
//   binding allocators 0x00DAC2C0 .. 0x00DB81B0 (landed CreateCVtable bodies; binding vtable 0x012D8370); the game script
//   interface (+0x40) slots 64, 280, 296, 655.
// Layout (retail = Ego_r.pdb - 0x14): AttackOver +0x50.
// Shape: bindings / spawned function as the landed CQ_GuildTrainingWoodsDepartureScript::Main 0x00D61D20.
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
class CCPPointerInfo { public:                                 // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    __forceinline void Release()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    __forceinline CCountedPointer& operator=(const CCountedPointer& other)
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
class CScriptThing { public:                                   // 12 bytes, vtable 0x01238C8C
    virtual void t000();
    CCountedPointer<void> PImp;                         // +0x04 Data, +0x08 Info
    virtual ~CScriptThing();                                    // 0x004AA840
    __forceinline CScriptThing& operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
};
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
namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    void AddEntityScriptBinding(CEntityScriptBindingBase* binding);                   // 0x00CB8230
    void PostAddScriptedEntities();                                                   // 0x00CB8930
    __forceinline void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
public:
    void AddSpawnedFunction(CSpawnedFuncBase*, const CCharString&);   // 0x00CB7E50
    bool IsActiveThreadTerminating();   // 0x00CB7940
};
namespace NScript {
class CQ_NewOakValeIntroScript : public CScriptBase {
public:
#define ALLOC(Name) struct Name { static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_NewOakValeIntroScript& parent, CScriptDataBase* data, const CScriptThing& thing); };
    ALLOC(CLiveFather)         // 0x00DAC2C0
    ALLOC(CTheresa)            // 0x00DAC420
    ALLOC(CGuard)              // 0x00DAC580
    ALLOC(CVillager)           // 0x00DADE50
    ALLOC(CBully)              // 0x00DAEC60
    ALLOC(CVictim)             // 0x00DAEDE0
    ALLOC(CTeddyGirl)          // 0x00DAEF50
    ALLOC(CAffairMan)          // 0x00DB0880
    ALLOC(CAffairWoman)        // 0x00DB1DB0
    ALLOC(CAffairWife)         // 0x00DB29A0
    ALLOC(CBookTrader)         // 0x00DB3E30
    ALLOC(CBarrelMan)          // 0x00DB51B0
    ALLOC(CBarrelThug)         // 0x00DB6B40
    ALLOC(CBarrel)             // 0x00DB7D00
    ALLOC(CCreatedBeetle)      // 0x00DB7FF0
    ALLOC(CDeadFather)         // 0x00DB81B0
#undef ALLOC
    CGameScriptInterface* Game;                         // +0x40
    void*                 Data;                         // +0x44
    unsigned char         _pad48[0x50 - 0x48];          // VictimShake +0x48, StopTimeIndex +0x4c
    bool                  AttackOver;                   // +0x50
    void Main();
    void StartBarrelTimer();                            // 0x00DB4F70
    void DoMission();                                   // 0x00DBDE40
};
void CQ_NewOakValeIntroScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_NewOakValeIntroScript>("NOVI_LiveFather", this, &CLiveFather::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_NewOakValeIntroScript>("NOVI_Theresa", this, &CTheresa::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_NewOakValeIntroScript>("NOVI_Guard", this, &CGuard::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_NewOakValeIntroScript>("NOVI_Villager", this, &CVillager::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_NewOakValeIntroScript>("NOVI_Bully", this, &CBully::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_NewOakValeIntroScript>("NOVI_Victim", this, &CVictim::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_NewOakValeIntroScript>("NOVI_TeddyGirl", this, &CTeddyGirl::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_NewOakValeIntroScript>("NOVI_AffairMan", this, &CAffairMan::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_NewOakValeIntroScript>("NOVI_AffairWoman", this, &CAffairWoman::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_NewOakValeIntroScript>("NOVI_AffairWife", this, &CAffairWife::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_NewOakValeIntroScript>("NOVI_BookTrader", this, &CBookTrader::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_NewOakValeIntroScript>("NOVI_BarrelMan", this, &CBarrelMan::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_NewOakValeIntroScript>("NOVI_BarrelThug", this, &CBarrelThug::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_NewOakValeIntroScript>("NOVI_Barrel", this, &CBarrel::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_NewOakValeIntroScript>("NOVI_CreatedBeetle", this, &CCreatedBeetle::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_NewOakValeIntroScript>("OVI_DeadFather", this, &CDeadFather::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    if (AttackOver) {
        if (IsActiveThreadTerminating())
            return;
        Game->DeactivateQuest("Q__OakValeIntro_PostAttack", 0);
    }
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_01", "", "");
    SpawnFunction(new CSpawnedFunc<CQ_NewOakValeIntroScript>("StartBarrelTimer", 0, this, &CQ_NewOakValeIntroScript::StartBarrelTimer));
    DoMission();
}
}