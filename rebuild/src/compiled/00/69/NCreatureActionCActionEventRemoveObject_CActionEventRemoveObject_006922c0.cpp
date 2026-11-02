// NCreatureAction::CActionEventRemoveObject::CActionEventRemoveObject() -- the inlined abstract CActionEventBase (CBaseClass, Finished cleared), this class's vtable, then the Hand string
// Leaf: ??0CActionEventRemoveObject@NCreatureAction@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006922C0, length 30 bytes (byte twin of CActionEventExplosion() 0x00692230 up to the vtable), thiscall, returns this.
// Identity: the vtable stored is 0x0125C1A0 (RTTI NCreatureAction::CActionEventRemoveObject : NCreatureAction::CActionEventBase :
//   CBaseClass, ghidra_out/rtti_bases.tsv). Ego_r lists the class's GetHook / Alloc / ??_G / SetDataFromString / Dispatch;
//   the constructor itself is inline in the donor builds.
// Callees: CBaseClass::CBaseClass @ 0x0099A2D0 (engine/CBaseClass.h), CCharString::CCharString() @ 0x0099E4B0 (fable_string.h).
// Layout (Ego_r, no containers, so retail offsets are the PDB's): CActionEventBase {vptr, Finished +4, ActivationFrame +8,
//   PThing +0xc, PCameraManager +0x10}; CActionEventRemoveObject Hand (CCharString) +0x14 (0x18 bytes). The constructor sets
//   only Finished and the string.
// Shape: the Finished store sits BEFORE this class's vtable store, so it belongs to the inlined base constructor, whose own
//   vtable store is dead (the abstract base is `__declspec(novtable)`).
// Repair (chunk 705): the landed source had no `// Leaf:` line. engine/CBaseClass.h's virtual ~CBaseClass gives this class an
//   implicit virtual dtor, so the TU now also emits ??_GCActionEventRemoveObject (40 bytes) through the vtable; with no leaf the
//   verifier grades the largest symbol containing the class name -- that ??_G -- hence DIFFER(40v30). The constructor's own
//   bytes never changed (no EH frame: the base flags have no /EHsc, the object holds no __ehhandler). Retail has the same
//   virtual dtor chain (Ego_r ??_GCActionEventRemoveObject@NCreatureAction@@UAEPAXI@Z).
#include "fable_string.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0)

class CThingCreatureBase;
class CGameCameraManager;

namespace NCreatureAction {
class __declspec(novtable) CActionEventBase : public CBaseClass { public:   // abstract
    bool                Finished;                     // +0x04
    long                ActivationFrame;              // +0x08
    CThingCreatureBase* PThing;                       // +0x0c
    CGameCameraManager* PCameraManager;               // +0x10
    CActionEventBase() : Finished(false) { }
};

class CActionEventRemoveObject : public CActionEventBase { public:   // vtable 0x0125C1A0
    CCharString Hand;                                 // +0x14
    CActionEventRemoveObject();
};

CActionEventRemoveObject::CActionEventRemoveObject()
{
}
}