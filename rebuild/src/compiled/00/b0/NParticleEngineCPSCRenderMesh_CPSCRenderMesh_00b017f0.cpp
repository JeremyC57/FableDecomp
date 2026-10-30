// NParticleEngine::CPSCRenderMesh::~CPSCRenderMesh() -- clear the mesh and trail primitive handles, then their member destructors and the base chain (CPSCBase's implicit destructor, ~CBaseClassNonCopyable tail call)
// Leaf: ??1CPSCRenderMesh
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B017F0, 272 bytes, tail `jmp`. Byte twin of ~CPSCRenderSprite 0x00AFBBD0 with the
//   handles 0x10 higher; stores vtable 0x0129F96C = NParticleEngine::CPSCRenderMesh (RTTI).
// Callees: the primitive's virtual slots 3 (+0x0C, OnLastHandleReleased(handle)) and 1 (+0x04, Destroy), ~CBaseClass
//   0x0099A2E0 (each handle's base), ~CBaseClassNonCopyable 0x0099A300 (the component base, tail call).
// Layout: Ego_r.pdb CPSCRenderMesh (0x64, retail offsets equal): CPSCBase (vptr, ParticleSystem +4, flag bits +8),
//   BankIndex +0x0c .. RenderSizeParam +0x48, PrimHandle +0x4c, ChildPrimIndex +0x54, TrailPrimHandle +0x58,
//   ChildTrailPrimIndex +0x60.
// Shape: the body is two inline `Clear()` calls (0x00AF70D0 form); each CEnginePrimitiveHandle member then runs its inline
//   destructor (vtable store, Clear, the counted pointer's own release, base). One shared zero register (ebx).
class CEnginePrimitiveHandle;
class CEnginePersistentPrimitiveBase {
public:
    virtual void v000();
    virtual void Destroy();                                          // slot 1 (+0x04)
    virtual void v002();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle); // slot 3 (+0x0C)
    long RefCount;                                                   // +0x04
    void Release()
    {
        if (--RefCount == 0)
            Destroy();
    }
};
class CIVCountedPointer {
public:
    CEnginePersistentPrimitiveBase* Object;
    CEnginePersistentPrimitiveBase* operator->() const { return Object; }
    bool IsUnique() const { return Object != 0 && Object->RefCount == 1; }
    __forceinline CIVCountedPointer& operator=(int)
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
        Object = 0;
        return *this;
    }
    __forceinline ~CIVCountedPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CEnginePrimitiveHandle : public CBaseClass { // vtable 0x0122F598
public:
    CIVCountedPointer Data;                        // +0x04
    __forceinline void Clear()
    {
        if (Data.IsUnique())
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
    __forceinline virtual ~CEnginePrimitiveHandle() { Clear(); }
};

class CBaseClassNonCopyable {                      // vtable 0x0129A7C4
protected:
    virtual ~CBaseClassNonCopyable();              // 0x0099A300 ??1CBaseClassNonCopyable@@MAE@XZ (protected)
};

namespace NParticleEngine {
class CParticleSystem;
class CPSCBase : public CBaseClassNonCopyable { public:
    CParticleSystem* ParticleSystem;               // +0x04
    bool UpdateStarted : 1;                        // +0x08
    bool Initialised : 1;
};
class CPSCRenderMesh : public CPSCBase { public:                 // vtable 0x0129F96C
    long                   BankIndex;              // +0x0c
    long                   TrailBankIndex;         // +0x10
    unsigned long          Colours[6];             // +0x14 trail / main CRGBColours
    unsigned long          Bits2c[7];              // +0x2c packed render settings
    unsigned long          RenderSizeParam;        // +0x48
    CEnginePrimitiveHandle PrimHandle;             // +0x4c
    long                   ChildPrimIndex;         // +0x54
    CEnginePrimitiveHandle TrailPrimHandle;        // +0x58
    long                   ChildTrailPrimIndex;    // +0x60
    virtual ~CPSCRenderMesh();
};

CPSCRenderMesh::~CPSCRenderMesh()
{
    PrimHandle.Clear();
    TrailPrimHandle.Clear();
}
}