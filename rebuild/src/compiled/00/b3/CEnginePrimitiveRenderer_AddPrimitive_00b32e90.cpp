// CEnginePrimitiveRenderer::AddPrimitive(const CEnginePrimitive&) -- adds a one-off primitive: registers it as a persistent primitive under a temporary handle (flags 4, transient) and drops the handle
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B32E90, length 134 bytes (+ int3 padding).
// Signature from Ego_r.pdb: ?AddPrimitive@CEnginePrimitiveRenderer@@QAEXABVCEnginePrimitive@@@Z (`ret 4`).
// Callees: 0x0099A2D0 CBaseClass::CBaseClass(), 0x00B324A0 CEnginePrimitiveRenderer::SetPersistentPrimitive(
// CEnginePrimitiveHandle&, const CEnginePrimitive&, unsigned long, bool), 0x0099A2E0 CBaseClass::~CBaseClass();
// the primitive's virtual slot 3 (+0x0c, last handle released, with the handle) and slot 1 (+0x04, destroy).
// Layout: CEnginePrimitiveHandle = CBaseClass vptr (own vtable 0x0122F598) + CIVCountedPointer Data (+4), an
// 8-byte frame local; the persistent primitive's reference count sits at +4.
// Shape: the handle's inline constructor (out-of-line base call, vtable store and null Data scheduled after the
// argument pushes) and its inline destructor: vtable store, `IsUnique()` -> slot 3 with the handle, then the counted
// pointer's destructor -- one null test (jump-threaded with the IsUnique test), the release through two inline
// ref-count accessors (`DecRef(); if (GetRefCount() == 0) Destroy();`: `mov esi,[ecx+4] ; lea eax,[ecx+4] ; dec esi ;
// mov edx,esi ; test edx,edx ; mov [eax],esi`), one unconditional zero store, then the out-of-line base destructor.
class CEnginePrimitiveHandle;

class CEnginePersistentPrimitiveBase {
public:
    virtual void v000();
    virtual void Destroy();                                          // slot 1 (+0x04)
    virtual void v002();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle); // slot 3 (+0x0C)
    long RefCount;                                                   // +0x04
    void DecRef() { --RefCount; }
    long GetRefCount() const { return RefCount; }
    void Release()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};

class CIVCountedPointer {
public:
    CEnginePersistentPrimitiveBase* Object;
    CIVCountedPointer() : Object(0) { }
    CEnginePersistentPrimitiveBase* operator->() const { return Object; }
    bool IsUnique() const { return Object != 0 && Object->RefCount == 1; }
    ~CIVCountedPointer()
    {
        if (Object)
            Object->Release();
        Object = 0;
    }
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy

class CEnginePrimitiveHandle : public CBaseClass { // vtable 0x0122F598
public:
    CIVCountedPointer Data;                        // +0x04
    virtual void v001();
    CEnginePrimitiveHandle() { }
    ~CEnginePrimitiveHandle()
    {
        if (Data.IsUnique())
            Data->OnLastHandleReleased(this);
    }
};

class CEnginePrimitive;
#include "engine/CEnginePrimitiveRenderer.h"

struct CEnginePrimitiveRenderer_Methods : CEnginePrimitiveRenderer {
    void SetPersistentPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive,
                                unsigned long flags, bool transient);    // 0x00B324A0
    void AddPrimitive(const CEnginePrimitive& primitive);
};

void CEnginePrimitiveRenderer_Methods::AddPrimitive(const CEnginePrimitive& primitive)
{
    CEnginePrimitiveHandle handle;
    SetPersistentPrimitive(handle, primitive, 4, true);
}