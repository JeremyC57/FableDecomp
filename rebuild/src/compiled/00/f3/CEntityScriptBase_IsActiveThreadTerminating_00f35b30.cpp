// CEntityScriptBase::IsActiveThreadTerminating() const -- ask the parent script whether its active thread is terminating
// Leaf: ?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ
// Retail: Fable.exe @ 0x00F35B30, 12 bytes, tail `jmp`. Ego_r.pdb lists ?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ;
// the entity-script Main bodies call it between frames (landed CMagicBarrierGraveyard::Main 0x00CE0600, ...).
// Twin of SetPredicate 0x00F35B10 (slot 3 then the CScriptBase method).
// Callees: virtual GetParentScript() (slot 3, +0xC; pure in the CEntityScriptBase vtable 0x012C3224, derived bodies return
// ParentClass +0x14), CScriptBase::IsActiveThreadTerminating 0x00CB7940 (Ego_r ?IsActiveThreadTerminating@CScriptBase@@QAE_NXZ,
// non-const: `PActiveThread +0x2C ? PActiveThread->Terminating +5 : false`).
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"

class CEntityScriptBase {
public:
    virtual ~CEntityScriptBase();                                    // slot 0
    virtual void Main() = 0;                                         // slot 1
    virtual void Init();                                             // slot 2
    virtual CScriptBase* GetParentScript() const = 0;                // slot 3 (+0xC)
    bool IsActiveThreadTerminating() const;
private:
    void* Game;                                                      // +0x04 CGameScriptInterfaceBase*
    void* Me[3];                                                     // +0x08 CScriptThing
};

bool CEntityScriptBase::IsActiveThreadTerminating() const
{
    return GetParentScript()->IsActiveThreadTerminating();
}
