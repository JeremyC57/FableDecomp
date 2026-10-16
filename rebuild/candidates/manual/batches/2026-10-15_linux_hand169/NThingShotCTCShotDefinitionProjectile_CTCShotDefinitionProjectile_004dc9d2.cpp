// NThingShot::CTCShotDefinitionProjectile::~CTCShotDefinitionProjectile() -- destroy the Line1 CEnginePrimitiveHandle member (tell the primitive it loses its last handle, drop the counted pointer, CBaseClass base), then tail-call the CTCShotDefinitionBase destructor
// Leaf: ??1CTCShotDefinitionProjectile
// Retail: Fable.exe 0x004DC9D2, 104 bytes (size TU: odd start, `and dword ptr [m],0`; base destructor as `jmp`).
// Class: NThingShot::CTCShotDefinitionProjectile, vtable 0x01240CCC (rtti-vtables slot 0 = deleting dtor 0x004DEA77, which
// calls this body). The body stores no vtable of its own (only the handle's 0x0122F598), so the class is modelled
// `__declspec(novtable)` with an empty destructor.
// Callees: the primitive's virtual slot 3 (+0x0C, OnLastHandleReleased(handle)) and slot 1 (+0x04, Destroy on the last
// reference), CBaseClass::~CBaseClass @ 0x0099A2E0, CTCShotDefinitionBase::~CTCShotDefinitionBase @ 0x007BF2B0.
// Layout: retail Line1 (CEnginePrimitiveHandle: vptr +0, counted Data +4) at +0x9C (PDB 0xA0; the base sits 4 bytes
// below the PDB after HitThingsList). Handle model = the landed CLocalDetailPrimitiveMesh dtor 0x00BDFD80 with the size TU's
// __forceinline GetRefCount() ternary (`Object ? RefCount : 0` == 1) (graphics
// layer: CEnginePrimitiveHandle vtable 0x0122F598, CBaseClass vtable 0x01231710).
#pragma optimize("s", on)
class CEnginePrimitiveHandle;
class CEnginePersistentPrimitiveBase {
public:
    virtual void v000();
    virtual void Destroy();                                          // slot 1 (+0x04)
    virtual void v002();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle); // slot 3 (+0x0C)
    long RefCount;                                                   // +0x04
    __forceinline void Release()
    {
        if (--RefCount == 0)
            Destroy();
    }
};
class CIVCountedPointer {
public:
    CEnginePersistentPrimitiveBase* Object;
    CEnginePersistentPrimitiveBase* operator->() const { return Object; }
    __forceinline long GetRefCount() const { return Object ? Object->RefCount : 0; }
    __forceinline bool IsUnique() const { return GetRefCount() == 1; }
    __forceinline ~CIVCountedPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
    __forceinline CIVCountedPointer& operator=(int)
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
        Object = 0;
        return *this;
    }
};
class CBaseClass {                                 // vtable 0x01231710
public:
    virtual void v000();
    ~CBaseClass();                                 // 0x0099A2E0 (manifest "CBase::CBase" is propagated)
};
class CEnginePrimitiveHandle : public CBaseClass { // vtable 0x0122F598
public:
    CIVCountedPointer Data;                        // +0x04
    virtual void v001();
    __forceinline ~CEnginePrimitiveHandle()
    {
        if (Data.IsUnique())
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
};
namespace NThingShot {
struct CTCShotDefinitionBase {
    virtual ~CTCShotDefinitionBase();              // 0x007BF2B0
    unsigned char _pad04[0x9c - 0x04];
};
struct __declspec(novtable) CTCShotDefinitionProjectile : CTCShotDefinitionBase {   // vtable 0x01240CCC
    CEnginePrimitiveHandle Line1;                  // +0x9c
    ~CTCShotDefinitionProjectile();
};

CTCShotDefinitionProjectile::~CTCShotDefinitionProjectile()
{
}
}
