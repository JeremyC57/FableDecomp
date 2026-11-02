// NCreatureAction::CActionEventExplosion::CActionEventExplosion() -- the inlined abstract CActionEventBase (CBaseClass, Finished cleared), this class's vtable, then the AttachmentObject string
// Leaf: ??0CActionEventExplosion@NCreatureAction@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00692230, length 30 bytes, thiscall, returns this.
// Identity: the vtable stored is 0x0125C154 (RTTI NCreatureAction::CActionEventExplosion : NCreatureAction::CActionEventBase :
//   CBaseClass, ghidra_out/rtti_bases.tsv). Ego_r lists the class's GetHook / Alloc / SetDataFromString / Dispatch and the
//   abstract base's CCountedPointer<CActionEventBase> containers; the constructor itself is inline in the donor builds.
// Callees: CBaseClass::CBaseClass @ 0x0099A2D0 (engine/CBaseClass.h), CCharString::CCharString() @ 0x0099E4B0 (fable_string.h).
// Layout (Ego_r, no containers, so retail offsets are the PDB's): CActionEventBase {vptr, Finished +4, ActivationFrame +8,
//   PThing +0xc, PCameraManager +0x10}; CActionEventExplosion AttachmentObject (CCharString) +0x14, ExplosionDefIndex +0x18,
//   Dummy +0x1c, UseActionHelper +0x1d. The constructor sets only Finished and the string.
// Shape: the Finished store sits BEFORE this class's vtable store, so it belongs to the inlined base constructor, whose own
//   vtable store is dead (the abstract base is `__declspec(novtable)`).
// Repair (chunk 705): the landed source had no `// Leaf:` line. engine/CBaseClass.h's virtual ~CBaseClass gives this class an
//   implicit virtual dtor, so the TU now also emits ??_GCActionEventExplosion (40 bytes) through the vtable; with no leaf the
//   verifier grades the largest symbol containing the class name -- that ??_G -- hence DIFFER(40v30). The constructor's own
//   bytes never changed (no EH frame: the base flags have no /EHsc, the object holds no __ehhandler). Retail has the same
//   virtual dtor chain (the derived ??_G exists in Ego_r for the sibling CActionEventRemoveObject).
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

class CActionEventExplosion : public CActionEventBase { public:   // vtable 0x0125C154
    CCharString AttachmentObject;                     // +0x14
    long        ExplosionDefIndex;                    // +0x18
    bool        Dummy;                                // +0x1c
    bool        UseActionHelper;                      // +0x1d
    CActionEventExplosion();
};

CActionEventExplosion::CActionEventExplosion()
{
}
}