// NScript::CQ_ArenaHoldingScriptScript::Main() -- Arena holding area: wait for the ArenaExterior region, bind the six entity scripts (entrance guards and the town fans, all flag 1), post the scripted entities, then make the ArenaExtToHallOfHeroes door unusable and persistent
// Leaf: ?Main@CQ_ArenaHoldingScriptScript@NScript@@UAEXXZ
// Naming wave 29 (family sfbh): engine/CSpawnedFuncBase.h now carries retail's implicit destructor and the real CCharString /
//   CCountedPointer<const NScript::CPredicateBase> members; header contract met here: engine/CCountedPointer.h included before it.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CF0790, 960 bytes, thiscall, plain `ret`.
// Identity: vtable slot 2 (Main) of 0x012C60A0 (reference .rdata 0x012C60A8), the vtable the landed ctor 0x00CF06A0
//   NScript::CQ_ArenaHoldingScriptScript::CQ_ArenaHoldingScriptScript stores (slot 1 RegisterMain 0x00CF06F0, slot 3 Init
//   0x00CF06D0, both landed); Ego_r.pdb ?Main@CQ_ArenaHoldingScriptScript@NScript@@UAEXXZ; the six bound names are the six
//   nested classes the PDB gives Alloc / Main for (CEntranceGuard, CEntranceGuard2, COakvaleFan, CBowerstoneFan,
//   CHookCoastFan, CFanMarker), their landed Allocs 0x00CF0B60 / 0x00CF0C40 / 0x00CF17F0 / 0x00CF2A90 / 0x00CF3C40 / 0x00CF4DF0.
// Callees: CTimer (inline: pDSTGame RegisterTimer slot 87 / DeregisterTimer slot 88; out-of-line copies 0x00CD4450 /
//   0x00CD4470), CCharString 0x0099EBF0 / 0x0099EC30 / ~ 0x0099EAE0, operator new 0x00BFEA1A, AddEntityScriptBinding
//   0x00CB8230 (binding vtable 0x012C6130), PostAddScriptedEntities 0x00CB8930, IsActiveThreadTerminating 0x00CB7940,
//   ~CScriptThing 0x004AA840; CGameScriptInterface slots 7 NewScriptFrame, 12 IsRegionLoaded, 64 PostAddScriptedEntities,
//   72 GetThingWithScriptName, 447 SetThingAsUsable, 477 SetThingPersistent.
// Layout: CLevelScriptBase<CQ_SunnyvaleMasterData> (engine/CLevelScriptBase.h: Game +0x40, MasterScriptData +0x44), the base the
//   landed ctor 0x00CF06A0 proves.
// Shape: the binding / wait models of the landed CQ_OakValeBanditRaidScript::Main 0x00DBF2D0; an unused function-scope CTimer
//   (its inline ctor / dtor bracket the body: RegisterTimer first, DeregisterTimer on every return); the bindings' name
//   temporaries are VC7.1's flag-guarded conditional temporaries of `new` (bits 1..0x20 in ebx).
#include "fable_compiler.h"
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
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
#include "engine/CCountedPointer.h"                     // CCountedPointer<T> for CSpawnedFuncBase::PPredicate (header contract)
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess

class CScriptThing;
class CScriptDataBase;
class CEntityScriptBase;
class CActiveEntityScriptBase;
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"                       // : CBaseClass, vtable 0x01238C8C; ~CScriptThing 0x004AA840 out of line

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
    FABLE_FORCEINLINE CEntityScriptBinding(const CCharString& name, T* parent, TAllocFunc alloc, unsigned long flags)
        : EntityScriptName(name), PParentScript(parent), ClassAllocFunc(alloc), ClassAllocValid(true), Flags(flags) {}
    virtual const CCharString& GetEntityScriptName() const { return EntityScriptName; }
};

#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
#include "engine/CLevelScriptBase.h"
namespace NScript { class CQ_SunnyvaleMasterData; }
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS)
#include "engine/CTimer.h"
// Retail expands the timer's constructor and destructor here (out-of-line copies 0x00CD4450 / 0x00CD4470): defined inline
//   after the header.
inline CTimer::CTimer() { TimerIndex = pDSTGame->RegisterTimer(); }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
namespace NScript {
class CQ_ArenaHoldingScriptScript : public CLevelScriptBase<NScript::CQ_SunnyvaleMasterData> {   // base: the landed ctor 0x00CF06A0
public:
    class CEntranceGuard { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_ArenaHoldingScriptScript& parent, CScriptDataBase* data, const CScriptThing& thing); };    // 0x00CF0B60
    class CEntranceGuard2 { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_ArenaHoldingScriptScript& parent, CScriptDataBase* data, const CScriptThing& thing); };   // 0x00CF0C40
    class COakvaleFan { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_ArenaHoldingScriptScript& parent, CScriptDataBase* data, const CScriptThing& thing); };       // 0x00CF17F0
    class CBowerstoneFan { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_ArenaHoldingScriptScript& parent, CScriptDataBase* data, const CScriptThing& thing); };    // 0x00CF2A90
    class CHookCoastFan { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_ArenaHoldingScriptScript& parent, CScriptDataBase* data, const CScriptThing& thing); };     // 0x00CF3C40
    class CFanMarker { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_ArenaHoldingScriptScript& parent, CScriptDataBase* data, const CScriptThing& thing); };        // 0x00CF4DF0
    virtual void Main();
};
} // namespace NScript
using NScript::CQ_ArenaHoldingScriptScript;

void NScript::CQ_ArenaHoldingScriptScript::Main()
{
    CTimer timer;
    while (!Game->IsRegionLoaded("ArenaExterior")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_ArenaHoldingScriptScript>("EntranceGuard", this, &CEntranceGuard::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_ArenaHoldingScriptScript>("EntranceGuard2", this, &CEntranceGuard2::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_ArenaHoldingScriptScript>("OakvaleFan", this, &COakvaleFan::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_ArenaHoldingScriptScript>("BowerstoneFan", this, &CBowerstoneFan::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_ArenaHoldingScriptScript>("HookCoastFan", this, &CHookCoastFan::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_ArenaHoldingScriptScript>("FanMarker", this, &CFanMarker::Alloc, 1));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    CScriptThing door = Game->GetThingWithScriptName("ArenaExtToHallOfHeroes");
    Game->SetThingAsUsable(door, false);
    Game->SetThingPersistent(door, true);
}