// NScript::CV_RandomPopulationSimScript::Main() -- the random population sim's main: bind the "RandomPopulationVillager", "RandomPopulationGuard" and "RandomPopulationBandit" entity scripts (one shared Alloc), finish adding scripted entities, then spawn the ProcessAlbionPopulationSim and HighDetailPopulationSim threads
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EC1910, 592 bytes. Catalog label "CQ_TentacleKrakenBossFightScript::Main" is
//   propagated: the row's only reference is the vtable at 0x012F12E0, its spawned threads are
//   CV_RandomPopulationSimScript::ProcessAlbionPopulationSim 0x00EC2240 and HighDetailPopulationSim 0x00EC29F0.
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / CCharString(const CCharString&)
//   0x0099EC30 / ~CCharString 0x0099EAE0, CScriptBase::AddEntityScriptBinding 0x00CB8230, CScriptBase::
//   PostAddScriptedEntities 0x00CB8930, CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc vtable 0x012F12EC),
//   CScriptBase::AddSpawnedFunction 0x00CB7E50; the shared entity Alloc 0x00EC1B70 (manifest "CTriangleList" is
//   propagated); the game script interface's PostAddScriptedEntities (slot 64, +0x100).
// Layout: CEntityScriptBinding<CV_RandomPopulationSimScript> (0x1C bytes, vtable 0x012F136C) as the landed
//   CQ_DragonBossFightScript::Main 0x00D255E0.
// Shape: each name string is a temporary guarded by its own bit of one flag word (ebx: 1, 2, 4, 8, 0x10).
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
class CV_RandomPopulationSimScript : public CScriptBase { public:
    struct CRandomPopulationCreature {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_RandomPopulationSimScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00EC1B70
    };
    CGameScriptInterface* GSI;                          // +0x40
    void*                 Data;                         // +0x44
    void Main();
    void ProcessAlbionPopulationSim();                  // 0x00EC2240
    void HighDetailPopulationSim();                     // 0x00EC29F0
};
void CV_RandomPopulationSimScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CV_RandomPopulationSimScript>("RandomPopulationVillager", this, &CRandomPopulationCreature::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_RandomPopulationSimScript>("RandomPopulationGuard", this, &CRandomPopulationCreature::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_RandomPopulationSimScript>("RandomPopulationBandit", this, &CRandomPopulationCreature::Alloc, 0));
    PostAddScriptedEntities();
    GSI->PostAddScriptedEntities();
    AddSpawnedFunction(new CSpawnedFunc<CV_RandomPopulationSimScript>("ProcessAlbionPopulationSim", 0, this, &CV_RandomPopulationSimScript::ProcessAlbionPopulationSim), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_RandomPopulationSimScript>("HighDetailPopulationSim", 0, this, &CV_RandomPopulationSimScript::HighDetailPopulationSim), CCharString(""));
}
}