// CEnginePrimitiveRenderer::SetPersistentPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive, unsigned long flags, bool nonPersistent) -- update the handle's internal primitive in place when it already has the right type, else release it, create a new internal primitive through the type's manager, initialise it and register it with the 2D renderer (flag 0x40) or its manager
// Grade: RELOCATION_MATCH -- the handle is the CEnginePrimitiveHandle model of the landed ??_G 0x0041C500: a counted pointer member with a null-safe `GetRefCount()`, Clear = `if (Data.GetRefCount() == 1) notify; Data = 0;` (operator= = release + a second zero store, retail's re-read and doubled store) and Set = Clear, the counted release again (the dead zero-register release), then the raw store of the new primitive
// Leaf: SetPersistentPrimitive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B324A0, length 368 bytes (with int3 padding; body 0x160), `ret 10h`.
// PDB: ?SetPersistentPrimitive@CEnginePrimitiveRenderer@@QAEXAAVCEnginePrimitiveHandle@@ABVCEnginePrimitive@@K_N@Z.
// Callees: the internal primitive's virtual slots 1 (Destroy), 3 (OnLastHandleReleased) and 5 (+0x14, the update from a
//   CEnginePrimitive and flags); the type manager's slot 1 (+4, CreateInternalPrimitive(type)) on the type table
//   @ 0x01436E84; __fastcall CEngineScene::UpdatePosition(CEngineInternalPrimitiveBase&, unsigned long) @ 0x00B91840;
//   CEnginePrimitiveRenderer2D::AddPrimitive @ 0x00B4A670 (global @ 0x01436E7C); the manager registration @ 0x00BACDD0.
// Layout: CEnginePrimitive: Type +0; CEngineInternalPrimitiveBase: RefCount +4, Type +8, GridPos.Level +0x24, Enable
//   (bit 0) / NonPersistent (bit 1) of +0x34; CEnginePrimitiveHandle {vptr, Data}.
// Shape: the handle helpers as the counted-pointer model above; `Enable = !(flags & 0x20)` through the 8-bit `not ; and 1` form.
struct CEnginePrimitive { long Type; };

class CEnginePrimitiveHandle;
class CEngineInternalPrimitiveBase {
public:
    virtual void v000();
    virtual void Destroy();                                          // slot 1
    virtual void v002();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle); // slot 3
    virtual void v004();
    virtual void Update(const CEnginePrimitive& primitive, unsigned long flags);   // slot 5 (+0x14)
    long          RefCount;                         // +0x04
    long          Type;                             // +0x08
    unsigned char _pad0c[0x18];
    signed char   GridLevel;                        // +0x24 (GridPos.Level)
    unsigned char _pad25[0x0f];
    unsigned char Enable : 1;                       // +0x34
    unsigned char NonPersistent : 1;
};

struct CIVCountedPointer {
    CEngineInternalPrimitiveBase* Object;
    long GetRefCount() const { return Object ? Object->RefCount : 0; }
    __forceinline void Release()
    {
        CEngineInternalPrimitiveBase* object = Object;
        if (object) {
            if (--object->RefCount == 0)
                object->Destroy();
            Object = 0;
        }
    }
    __forceinline CIVCountedPointer& operator=(CEngineInternalPrimitiveBase* object)
    {
        Release();
        Object = object;
        if (object)
            ++object->RefCount;
        return *this;
    }
};

class CEnginePrimitiveHandle {
public:
    void*             _vptr;
    CIVCountedPointer Data;                         // +0x04
    __forceinline void Clear()
    {
        if (Data.GetRefCount() == 1)
            Data.Object->OnLastHandleReleased(this);
        Data = 0;
    }
    __forceinline void Set(CEngineInternalPrimitiveBase* primitive)
    {
        Clear();
        Data.Release();
        Data.Object = primitive;
    }
};

class CEnginePrimitiveManager {
public:
    virtual void v00();
    virtual CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type);   // slot 1
    void RegisterPrimitive(CEngineInternalPrimitiveBase* primitive, unsigned long flags);   // 0x00BACDD0
};
struct CEnginePrimitiveTypeInfo {
    unsigned char            _pad[0x10];
    CEnginePrimitiveManager* PrimitiveManagers[47];   // +0x10
};
extern CEnginePrimitiveTypeInfo* g_pEnginePrimitiveTypeInfo;   // 0x01436E84

class CEnginePrimitiveRenderer2D {
public:
    void AddPrimitive(CEngineInternalPrimitiveBase* primitive);   // 0x00B4A670
};
extern CEnginePrimitiveRenderer2D* g_pEnginePrimitiveRenderer2D;   // 0x01436E7C

class CEngineScene {
public:
    static void __fastcall UpdatePosition(CEngineInternalPrimitiveBase& primitive, unsigned long flags);   // 0x00B91840
};

class CEnginePrimitiveRenderer {
public:
    void SetPersistentPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive, unsigned long flags, bool nonPersistent);
};

void CEnginePrimitiveRenderer::SetPersistentPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive,
                                                      unsigned long flags, bool nonPersistent)
{
    long type = primitive.Type;
    CEnginePrimitiveManager* manager = g_pEnginePrimitiveTypeInfo->PrimitiveManagers[type];
    unsigned char enable = (unsigned char)~(flags >> 5) & 1;
    CEngineInternalPrimitiveBase* current = handle.Data.Object;
    if (current) {
        if (current->Type == type) {
            current->Update(primitive, flags);
            current->Enable = enable;
            if (current->GridLevel >= 0)
                CEngineScene::UpdatePosition(*current, flags);
            return;
        }
        handle.Clear();
    }
    if (!manager)
        return;
    CEngineInternalPrimitiveBase* created = manager->CreateInternalPrimitive(type);
    handle.Set(created);
    created->NonPersistent = nonPersistent;
    created->Update(primitive, flags);
    created->Enable = enable;
    if (flags & 0x40)
        g_pEnginePrimitiveRenderer2D->AddPrimitive(created);
    else
        manager->RegisterPrimitive(created, flags);
}