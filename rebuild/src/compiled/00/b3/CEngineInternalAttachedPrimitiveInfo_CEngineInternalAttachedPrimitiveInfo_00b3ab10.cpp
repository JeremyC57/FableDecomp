// CEngineInternalAttachedPrimitiveInfo::~CEngineInternalAttachedPrimitiveInfo() -- compiler-generated destructor: the PrimitiveHandle member's inline virtual destructor (Clear: tell the primitive when this is its last handle, release it), then ~CBaseClass
// Leaf: ??1CEngineInternalAttachedPrimitiveInfo@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B3AB10, length 105 bytes, tail jump to ~CBaseClass.
// Identity (proven): the scalar deleting destructor 0x00B3AB80 (slot 0 of vtable 0x012A1AB8, RTTI
//   .?AVCEngineInternalAttachedPrimitiveInfo@@) calls it; the other caller, CEnginePrimitiveAttachmentManager::SetAttachedPrimitive
//   (0x00B3A180, call at 0x00B3A2B7), destroys a local it built inline (stores 0x012A1AB8 and the handle vtable 0x0122F598)
//   after copying it into the list. Ego_r.pdb / FableWin.pdb: ??1CEngineInternalAttachedPrimitiveInfo@@UAE@XZ.
//   The catalog label is right for this row (the landed ~CPSCLight 0x00B1AEB0 had carried the same propagated label).
// Callees: CBaseClass::~CBaseClass @ 0x0099A2E0 (the handle's root, then this class's root as the tail jump); virtual calls on the
//   primitive: slot 3 (+0x0C, NotifyReleased, with the handle) when the handle holds its last reference, slot 1 (+0x04, Delete)
//   on its last release. Vtable stored: 0x0122F598 (.?AVCEnginePrimitiveHandle@@).
// Layout (Ego_r.pdb, 12 bytes): CBaseClass vptr +0x00, PrimitiveHandle (CEnginePrimitiveHandle: vptr + CIVCountedPointer
//   <CEnginePersistentPrimitiveBase> Data) +0x04. CEnginePersistentPrimitiveBase : CIVCountedPointeeBase (IVRefCount +0x04;
//   engine/CIVCountedPointeeBase.h retail opt-in).
// Shape: no vtable store of the class itself = a compiler-generated destructor (VC7.1 drops the dead vptr reset; a user
//   `~T() {}` adds the store). The handle's virtual destructor is the FableWin-named inline Clear() (null-safe GetRefCount()
//   == 1 test, NotifyReleased(*this), Data.Reset(0) = Release + the second unconditional zero store) followed by the
//   CIVCountedPointer member's own Release, exactly the model of the landed ~CTCHeroReceiveItems 0x007CA8E0 / ~CPSCLight.
//   The implicit ??1 is emitted the way retail's TU emits it: the PDB default constructor defined here emits the vtable,
//   its ??_G and the out-of-line implicit destructor (chunk 664 lesson); retail's own constructor is inline at its use site.
//   Slot 3's name (NotifyReleased) is the landed spelling, not PDB-attested (descriptive; see NOTES.md).
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0)

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CIVCountedPointeeBase.h"              // retail opt-in: vptr, slot 1 Delete() const, IVRefCount +0x04
class CEnginePrimitiveHandle;
class CEnginePersistentPrimitiveBase : public CIVCountedPointeeBase {
    friend class CEnginePrimitiveHandle;
protected:
    virtual void CUnprovenSlot2_012a3d60() = 0;             // slot 2: _purecall in the vtable 0x012A3D60 (no PDB name found)
    virtual void NotifyReleased(const CEnginePrimitiveHandle& handle);   // slot 3 (+0x0C)
    virtual bool WasRendered() const;                       // slot 4
};
template <class T> class CIVCountedPointer { public:
    CIVCountedPointeeBase* Object;
    CIVCountedPointer() : Object(0) { }
    T* operator->() const { return static_cast<T*>(Object); }
    unsigned long GetRefCount() const { return Object ? Object->IVRefCount : 0; }
    __forceinline void Release()
    {
        CIVCountedPointeeBase* object = Object;
        if (object) {
            if (--object->IVRefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
    __forceinline void Reset(T* object)
    {
        Release();
        Object = object;
        if (object)
            ++object->IVRefCount;
    }
    __forceinline ~CIVCountedPointer() { Release(); }
};
class CEnginePrimitiveHandle : public CBaseClass { public:  // vtable 0x0122F598, 8 bytes
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data; // +0x04
    CEnginePrimitiveHandle() { }
    unsigned long GetReferenceCount() const { return Data.GetRefCount(); }
    __forceinline void Clear()
    {
        if (GetReferenceCount() == 1)
            Data->NotifyReleased(*this);
        Data.Reset(0);
    }
    __forceinline virtual ~CEnginePrimitiveHandle() { Clear(); }
};

class CEngineInternalAttachedPrimitiveInfo : public CBaseClass { public:   // vtable 0x012A1AB8, 12 bytes
    CEngineInternalAttachedPrimitiveInfo();
    CEnginePrimitiveHandle PrimitiveHandle;                 // +0x04
};

CEngineInternalAttachedPrimitiveInfo::CEngineInternalAttachedPrimitiveInfo()
{
}