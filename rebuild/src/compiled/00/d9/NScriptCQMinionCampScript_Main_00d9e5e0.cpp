// CQ_MinionCampScript::Main() -- the minion camp quest's main: switch off the Darkwood3 creature generators, bind the BriarRose, MC_MinionGuard, MC_SpawnPoint, MC_Minion and FadeinSummoner entity scripts, finish adding scripted entities and spawn the DoMission thread
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D9E5E0, length 701 bytes.
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / CCharString(const CCharString&) 0x0099EC30
//   / ~CCharString 0x0099EAE0, CScriptBase::AddEntityScriptBinding 0x00CB8230, CScriptBase::PostAddScriptedEntities
//   0x00CB8930, CSpawnedFuncBase::CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc vtable 0x012D6CAC),
//   CScriptBase::AddSpawnedFunction 0x00CB7E50, the binding allocators 0x00D9E8B0 / 0x00D9EA80 / 0x00D9ED50 /
//   0x00D9F0B0 / 0x00D9F5B0 (binding vtable 0x012D6D04), DoMission 0x00DA0230; the game script interface's
//   PostAddScriptedEntities (slot 64), SetCreatureGeneratorsEnabled (slot 558).
// Layout: CScriptBase is 0x40 bytes in retail; the interface pointer at +0x40 (landed ctor 0x00D9E210).
// Shape: the landed CQ_CinemaTestScript::Main 0x00F018B0 model (CEntityScriptBinding with its flags, one flag word in
//   ebx guarding the name temporaries).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess

class CScriptThing;
class CScriptDataBase;
class CEntityScriptBase;
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

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_MinionCampScript : public CScriptBase {
public:
    typedef CCountedPointer<CEntityScriptBase> (__fastcall *TAlloc)(CQ_MinionCampScript& parent, CScriptDataBase* data, const CScriptThing& thing);
    struct CBriarRose      { static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_MinionCampScript& parent, CScriptDataBase* data, const CScriptThing& thing); };   // 0x00D9E8B0
    struct CMCMinionGuard  { static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_MinionCampScript& parent, CScriptDataBase* data, const CScriptThing& thing); };   // 0x00D9EA80
    struct CMCSpawnPoint   { static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_MinionCampScript& parent, CScriptDataBase* data, const CScriptThing& thing); };   // 0x00D9ED50
    struct CMCMinion       { static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_MinionCampScript& parent, CScriptDataBase* data, const CScriptThing& thing); };   // 0x00D9F0B0
    struct CFadeinSummoner { static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_MinionCampScript& parent, CScriptDataBase* data, const CScriptThing& thing); };   // 0x00D9F5B0
    CGameScriptInterface* Game;                         // +0x40
    void*                 Data;                         // +0x44
    void Main();
    void DoMission();                                   // 0x00DA0230
};
void CQ_MinionCampScript::Main()
{
    Game->SetCreatureGeneratorsEnabled("Darkwood3", false);
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_MinionCampScript>("BriarRose", this, &CBriarRose::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_MinionCampScript>("MC_MinionGuard", this, &CMCMinionGuard::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_MinionCampScript>("MC_SpawnPoint", this, &CMCSpawnPoint::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_MinionCampScript>("MC_Minion", this, &CMCMinion::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_MinionCampScript>("FadeinSummoner", this, &CFadeinSummoner::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    AddSpawnedFunction(new CSpawnedFunc<CQ_MinionCampScript>("DoMission", 0, this, &CQ_MinionCampScript::DoMission), CCharString(""));
}
}