// CEngineInternalPrimitive2DViewport::SetChildPrimitive(unsigned long, const CEnginePrimitive&, unsigned long) -- grow the child-handle vector to cover `index`, have the primitive type's manager create the internal primitive, store it in the child handle, update it from the description, enable it unless flag 0x20 is set and recompute the viewport's render-layer mask
// Leaf: SetChildPrimitive
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BE23F0, 216 bytes (+ int3 padding), `ret 0Ch`.
// Callees: vector<CEnginePrimitiveHandle>::Resize(long) 0x00443F90 (on the child handles +0x94),
//   CEngineInternalPrimitive2DViewport::UpdateRenderLayerMask 0x00BE1D80 (landed); the primitive-type manager table
//   (global 0x01436E84, managers at +0x10 indexed by CEnginePrimitive::Type) slot 1 CreateInternalPrimitive(type); the new
//   primitive's slots 3 OnLastHandleReleased(handle), 1 Destroy, 5 Update(const CEnginePrimitive&, flags).
// Layout: CEngineInternalPrimitiveBase (root CIVCountedPointeeBase {vptr, RefCount +4}; Enable = bit 0 of the +0x34 bit-field
//   byte, Ego_r.pdb agrees); the viewport's child handles at +0x94 (engine/CEngineInternalPrimitive2DViewport.h: Primitives
//   +0x94 as a 16-byte CArray; retail reads a 12-byte first/last/end vector). The header flattens the primitive base, whose
//   virtuals and bit field this body needs, so `this` is a local model with the header's member name.
// Shape: the child handle is a reference local into the vector; storing the new primitive is the landed
//   CEnginePrimitiveHandle model (`Clear()` = last-handle notice + `Data = 0`, then `Data = p`, every helper inline, so
//   two release sequences precede the store); the handle reference is taken BEFORE the manager lookup (taken after it, the
//   table global lands in edx instead of ecx: the parked group twin 0x00BC65D0); the enable bit is a bool local
//   `(flags & 0x20) ? false : true` computed before the null test (bl).
struct CEnginePrimitive {
    long Type;                                         // +0x00
};

struct CEnginePrimitiveHandle;
struct CEngineInternalPrimitiveBase {
    virtual void v00();
    virtual void Destroy();                            // slot 1 (+0x04)
    virtual void v02();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle);   // slot 3 (+0x0c)
    virtual void v04();
    virtual void Update(const CEnginePrimitive& primitive, unsigned long flags);   // slot 5 (+0x14)
    long          RefCount;                            // +0x04
    unsigned char _pad08[0x34 - 8];
    unsigned char Enable : 1;                          // +0x34 bit 0
    unsigned char NonPersistent : 1;
    unsigned char OverrideFadeDistance : 1;
    unsigned char PostRenderEnable : 1;
    void Release()
    {
        if (--RefCount == 0)
            Destroy();
    }
};

class CIVCountedPointer {                              // CIVCountedPointer<CEngineInternalPrimitiveBase>
public:
    CEngineInternalPrimitiveBase* Object;
    CEngineInternalPrimitiveBase* operator->() const { return Object; }
    bool IsUnique() const { return Object != 0 && Object->RefCount == 1; }
    CIVCountedPointer& operator=(CEngineInternalPrimitiveBase* p)
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
        Object = p;
        return *this;
    }
};

struct CEnginePrimitiveHandle {                        // 8 bytes: vptr (0x0122F598) + counted Data
    virtual void v00();
    CIVCountedPointer Data;                            // +0x04
    void Clear()
    {
        if (Data.IsUnique())
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
};

struct CHandleVector {                                 // vector<CEnginePrimitiveHandle> (12 bytes)
    CEnginePrimitiveHandle* m_pFirst;
    CEnginePrimitiveHandle* m_pLast;
    CEnginePrimitiveHandle* m_pEnd;
    unsigned long size() const { return m_pLast - m_pFirst; }
    void Resize(long count);                           // 0x00443F90
    CEnginePrimitiveHandle& operator[](unsigned long index) { return m_pFirst[index]; }
};

struct CEnginePrimitiveManagerBase {
    virtual void v00();
    virtual CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type);   // slot 1 (+0x04)
};
struct CEnginePrimitiveManagerTable {
    unsigned char                _pad[0x10];
    CEnginePrimitiveManagerBase* Managers[1];          // +0x10, indexed by primitive type
};
extern CEnginePrimitiveManagerTable* g_pPrimitiveManagers;   // 0x01436E84

struct CEngineInternalPrimitive2DViewport : CEngineInternalPrimitiveBase {
    unsigned char _pad38[0x94 - 0x38];
    CHandleVector Primitives;                          // +0x94
    void UpdateRenderLayerMask();                      // 0x00BE1D80
    void SetChildPrimitive(unsigned long index, const CEnginePrimitive& primitive, unsigned long flags);
};

void CEngineInternalPrimitive2DViewport::SetChildPrimitive(unsigned long index, const CEnginePrimitive& primitive, unsigned long flags)
{
    if (index >= Primitives.size())
        Primitives.Resize(index + 1);
    CEnginePrimitiveHandle& handle = Primitives[index];
    CEnginePrimitiveManagerBase* manager = g_pPrimitiveManagers->Managers[primitive.Type];
    bool enable = (flags & 0x20) ? false : true;
    if (manager) {
        CEngineInternalPrimitiveBase* child = manager->CreateInternalPrimitive(primitive.Type);
        handle.Clear();
        handle.Data = child;
        child->Update(primitive, flags);
        child->Enable = enable;
        UpdateRenderLayerMask();
    }
}
