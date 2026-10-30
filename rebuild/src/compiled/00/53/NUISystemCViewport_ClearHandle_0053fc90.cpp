// NUISystem::CViewport::ClearHandle() -- release the viewport's engine primitive handle (telling the primitive when this was its last handle)
// Leaf: ClearHandle
// Retail: Fable.exe (TLC, VC7.1) @ 0x0053FC90, 64 bytes (plain `ret`).
// Callees: the primitive's virtual slots 3 (+0x0c, told about the last handle, the handle passed) and 1 (+0x04, destroy at
// the last reference).
// Label: the catalog "NUISystem::CScrollingViewport::vfunc_91" is inherited: the CViewport vtable 0x01249A5C holds the same
// body at slot 91; Ego_r.pdb names ?ClearHandle@CViewport@NUISystem@@UAEXXZ (and CComponent::ClearHandle).
// Layout: Ego_r.pdb CViewport Handle (CEnginePrimitiveHandle, 8 bytes) at PDB +0x18c = retail +0x15c (the class's own
// members follow the 0x15c-byte retail CChangingStateComponent); the handle is CBaseClass + a counted Data pointer at +4.
// Shape: the inline CEnginePrimitiveHandle::Clear() of the landed ~CPSCDecalRenderer (0x00B1FFC0): the uniqueness test,
// then `Data = 0` = the inline release (null test, `dec [Data+4]`, destroy at zero, zero store) plus the second
// unconditional zero store of operator=(0).
class CEnginePrimitiveHandle;
class CEnginePersistentPrimitiveBase {
public:
    virtual void v000();
    virtual void Destroy();                                          // slot 1 (+0x04)
    virtual void v002();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle); // slot 3 (+0x0C)
    long RefCount;                                                   // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
class CIVCountedPointer {                          // CIVCountedPointer<CEnginePersistentPrimitiveBase>
public:
    CEnginePersistentPrimitiveBase* Object;
    CEnginePersistentPrimitiveBase* operator->() const { return Object; }
    bool IsUnique() const { return Object != 0 && Object->RefCount == 1; }
    __forceinline void ReleaseRef() { if (Object) { Object->Release(); Object = 0; } }
    __forceinline CIVCountedPointer& operator=(int) { ReleaseRef(); Object = 0; return *this; }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CEnginePrimitiveHandle : public CBaseClass { // vtable 0x0122F598, 8 bytes
public:
    CIVCountedPointer Data;                        // +0x04
    __forceinline void Clear()
    {
        if (Data.IsUnique())
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
};

namespace NUISystem {
class CViewport {
public:
    virtual void v000();
    unsigned char _pad04[0x15c - 4];
    CEnginePrimitiveHandle Handle;                 // +0x15c
    void ClearHandle();
};

void CViewport::ClearHandle()
{
    Handle.Clear();
}
}