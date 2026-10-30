// CQ_WizardBattleScript::Main() -- the Hook Coast wizard (Maze) battle main: initialise, bind the screamer / Maze / EndGameBarrier entity scripts, cache music sets 0x33 and 0x34 and spawn WatchForTermination and DoMission
// Leaf: ?Main@CQ_WizardBattleScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E1A990, 0x397 bytes, plain `ret`.
// Label: manifest "CV_MurderTwistScript::Main" is propagated; the class is CQ_WizardBattleScript (vtable 0x012E0F54,
//   object size 0x80 = Ego_r 148 - 0x14, allocated at 0x00E1D9F0; HKC_* / Screamer / Maze binding names match its
//   Maze / NumberOfScreamersAlive members in Ego_r.pdb).
// Callees: Init 0x00E1AD30 (manifest "CV_ChickenKickingScript::Init" propagated), operator new 0x00BFEA1A, CCharString
//   0x0099EBF0 / 0x0099EC30 / ~ 0x0099EAE0, AddEntityScriptBinding 0x00CB8230 (binding vtable 0x012E0FCC),
//   CScriptBase::PostAddScriptedEntities 0x00CB8930, CSpawnedFuncBase 0x00CDD450 (spawn vtable 0x012E0F68),
//   AddSpawnedFunction 0x00CB7E50; threads WatchForTermination 0x00E1AD60, DoMission 0x00E1C0F0;
//   CGameScriptInterface slots 64 PostAddScriptedEntities, 698 CacheMusicSet.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
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
class CQ_WizardBattleScript : public CScriptBase {
public:
    struct CHKC_VillagerScreamer {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_WizardBattleScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E1B100
    };
    struct CHKC_ScreamerPoint {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_WizardBattleScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E1B660
    };
    struct CChasingScreamer {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_WizardBattleScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E1AE50
    };
    struct CInitialScreamerPoint {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_WizardBattleScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E1BB80
    };
    struct CHKC_Maze {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_WizardBattleScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E1B950
    };
    struct CEndGameBarrier {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_WizardBattleScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E1BA60
    };
    CGameScriptInterface* Game;                          // +0x40
    void* PMasterData;                                   // +0x44
    void Main();
    void Init();                                         // 0x00E1AD30
    void WatchForTermination();                          // 0x00E1AD60
    void DoMission();                                    // 0x00E1C0F0
};
} // namespace NScript
using NScript::CQ_WizardBattleScript;

void NScript::CQ_WizardBattleScript::Main()
{
    Init();
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_WizardBattleScript>("HKC_VillagerScreamer", this, &CHKC_VillagerScreamer::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_WizardBattleScript>("HKC_ScreamerPoint", this, &CHKC_ScreamerPoint::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_WizardBattleScript>("ChasingScreamer", this, &CChasingScreamer::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_WizardBattleScript>("InitialScreamerPoint", this, &CInitialScreamerPoint::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_WizardBattleScript>("HKC_Maze", this, &CHKC_Maze::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_WizardBattleScript>("EndGameBarrier", this, &CEndGameBarrier::Alloc, 1));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    Game->CacheMusicSet((EMusicSetType)0x33);
    Game->CacheMusicSet((EMusicSetType)0x34);
    SpawnFunction(new CSpawnedFunc<CQ_WizardBattleScript>("WatchForTermination", 0, this, &NScript::CQ_WizardBattleScript::WatchForTermination));
    SpawnFunction(new CSpawnedFunc<CQ_WizardBattleScript>("DoMission", 0, this, &NScript::CQ_WizardBattleScript::DoMission));
}