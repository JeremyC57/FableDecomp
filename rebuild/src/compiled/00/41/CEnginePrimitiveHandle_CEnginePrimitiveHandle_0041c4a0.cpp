// CEnginePrimitiveHandle::~CEnginePrimitiveHandle() -- tell the primitive when this was its last handle, drop the counted reference, then the CBaseClass base
// Leaf: ??1CEnginePrimitiveHandle@@UAE@XZ
// Retail: Fable.exe @ 0x0041C4A0, 93 bytes, tail `jmp` to ~CBaseClass 0x0099A2E0. Catalog label
// CLocalDetailPrimitiveMeshZSpriteBatch::CObject::~CObject is wrong: the body stores vptr 0x0122F598, whose COL is
// .?AVCEnginePrimitiveHandle@@ : CBaseClass (slots.py), and its slot 0 (0x0041C500) is the matching deleting destructor
// (Ego_r.pdb ??_GCEnginePrimitiveHandle@@UAEPAXI@Z).
// Body: the inline Clear() (Ego_r.pdb ?Clear@CEnginePrimitiveHandle@@QAEXXZ): if the counted primitive is unique,
// its slot 3 (+0xc) is told (this); Data = 0 (release + two zero stores); then the CIVCountedPointer destructor (a second,
// null-tested release), then the base destructor.
// Layout: Ego_r.pdb CEnginePrimitiveHandle (8 bytes): vptr, CIVCountedPointer<CEnginePersistentPrimitiveBase> Data +0x04,
// as the landed CPSCDecalRenderer::~CPSCDecalRenderer 0x00B1FFC0 model of the same inline destructor.
// Shape: /O2 (16-aligned entry).
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
    __forceinline ~CIVCountedPointer() { ReleaseRef(); }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CEnginePrimitiveHandle : public CBaseClass { // vtable 0x0122F598, 8 bytes
public:
    CIVCountedPointer Data;                        // +0x04
    __forceinline void Clear()
    {
        if (Data.IsUnique())
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
    virtual ~CEnginePrimitiveHandle();
};
CEnginePrimitiveHandle::~CEnginePrimitiveHandle()
{
    Clear();
}