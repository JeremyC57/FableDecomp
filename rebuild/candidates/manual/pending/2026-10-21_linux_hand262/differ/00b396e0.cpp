// Grade: DIFFER(497v497) -- register only: &PrimitiveTransform (kept across GetTransform for RedoMatrixConversion) lands in ebp, retail ebx; every other byte identical
// CEngineInternalAttachedPrimitive::Update(const CEnginePrimitive& primitive, const CEnginePrimitiveAttachmentInfo& info,
//   const CEnginePrimitiveHandle& parent, long parentGroupIndex, unsigned long flags) -- copy the attachment info, invalidate the
//   child inverse transform, take the parent handle and group index; fail when the parent (or its group child) is missing or is
//   not a mesh type (9/11/13 -> parent type 2); (re)create the output primitive through the type's manager when absent or of
//   another type, update it from the description, disable it, fetch its transform and convert the units.
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B396E0, 497 bytes (+int3), `ret 0x14`.
// Callees: CCharString::operator= 0x0099EFB0 (x2, the compiler-generated attachment-info copy), CEnginePrimitiveManager
//   0x00BACDD0 (on the manager with (primitive, 0); the manifest's `_Cons_val<...CParticleMorphs...>` label is propagated),
//   CEngineInternalAttachedPrimitive::RedoMatrixConversion 0x00B38DB0; virtuals on the internal primitive: slots 1/3 (handle
//   release), 5 Update, 12 (+0x30) group child, 21 (+0x54) transform; manager slot 1 CreateInternalPrimitive; managers table 0x01436E84.
// Layout: Ego_r.pdb CEngineInternalAttachedPrimitive (PrimitiveTransform +0x48, ParentType +0x78, ParentHandle +0x80, ParentGroupIndex
//   +0x88, OutputPrimitive +0x8c, AttachmentInfo +0x94, ChildInverseTransformValid +0xd8; agrees with the landed ctor 0x00B3A910);
//   no engine/ header exists; handle model = landed SetChildPrimitive 0x00BBC310 / AddChildPrimitive 0x00BE2310.
// Tried: named transform pointer / reference (before or after Update), inverted final test, child unnamed, manager/output declared
//   at function scope -- the register stays ebp.
struct CMatrix3x4 { float E[12]; };
struct CEnginePrimitive {
    long Type;                                         // +0x00
};
class CCharString {
public:
    struct CCharStringData* PStringData;
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
};
struct CEnginePrimitiveAttachmentInfo {
    unsigned long Flags;                     // +0x00
    CCharString   ParentDummyName;           // +0x04
    long          ParentDummyIndex;          // +0x08
    CCharString   ChildDummyName;            // +0x0c
    long          ChildDummyIndex;           // +0x10
};

struct CEnginePrimitiveHandle;
struct CEngineInternalPrimitiveBase {
    virtual void v00();
    virtual void Destroy();                            // slot 1 (+0x04)
    virtual void v02();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle);   // slot 3 (+0x0c)
    virtual void v04();
    virtual void Update(const CEnginePrimitive& primitive, unsigned long flags);   // slot 5 (+0x14)
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CEngineInternalPrimitiveBase* GetGroupChild(long index);   // slot 12 (+0x30)
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19(); virtual void v20();
    virtual bool GetTransform(CMatrix3x4& transform);  // slot 21 (+0x54)
    long          RefCount;                            // +0x04
    long          Type;                                // +0x08
    unsigned char _pad0c[0x34 - 0xc];
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

class CIVCountedPointer {                          // CIVCountedPointer<CEngineInternalPrimitiveBase>
public:
    CEngineInternalPrimitiveBase* Object;
    CEngineInternalPrimitiveBase* operator->() const { return Object; }
    bool IsUnique() const { return Object != 0 && Object->RefCount == 1; }
    __forceinline CIVCountedPointer& operator=(CEngineInternalPrimitiveBase* p)
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
        Object = p;
        return *this;
    }
    __forceinline CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        CEngineInternalPrimitiveBase* old = Object;
        if (old != other.Object) {
            if (old) {
                old->Release();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->RefCount;
        }
        return *this;
    }
};

struct CEnginePrimitiveHandle {                    // 8 bytes: vptr (0x0122F598) + counted Data
    virtual void v00();
    CIVCountedPointer Data;                        // +0x04
    __forceinline void Clear()
    {
        if (Data.IsUnique())
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
    __forceinline void Set(CEngineInternalPrimitiveBase* p)
    {
        Clear();
        Data = p;
    }
    __forceinline CEnginePrimitiveHandle& operator=(const CEnginePrimitiveHandle& other)
    {
        Clear();
        Data = other.Data;
        return *this;
    }
};

struct CEnginePrimitiveManager {
    virtual void v00();
    virtual CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type);   // slot 1 (+0x04)
    void AddInternalPrimitive(CEngineInternalPrimitiveBase* primitive, bool flag);   // 0x00BACDD0
};
struct CEnginePrimitiveManagerTable {
    unsigned char             _pad[0x10];
    CEnginePrimitiveManager*  Managers[1];         // +0x10, indexed by primitive type
};
extern CEnginePrimitiveManagerTable* g_pPrimitiveManagers;   // 0x01436E84

enum EParentType { PARENT_TYPE_NONE = 0, PARENT_TYPE_MESH = 2 };

struct CEngineInternalPrimitiveBaseBody {
    virtual ~CEngineInternalPrimitiveBaseBody();
    unsigned char _body[0x48 - 4];
};

struct CEngineInternalAttachedPrimitive : CEngineInternalPrimitiveBaseBody {
    CMatrix3x4                     PrimitiveTransform;          // +0x48
    EParentType                    ParentType;                  // +0x78
    long                           ChildType;                   // +0x7c
    CEnginePrimitiveHandle         ParentHandle;                // +0x80
    long                           ParentGroupIndex;            // +0x88
    CEnginePrimitiveHandle         OutputPrimitive;             // +0x8c
    CEnginePrimitiveAttachmentInfo AttachmentInfo;              // +0x94
    CMatrix3x4                     ChildInverseTransform;       // +0xa8
    bool                           ChildInverseTransformValid;  // +0xd8

    CEngineInternalPrimitiveBase* GetParentPrimitive() const
    {
        if (ParentGroupIndex != -1)
            return ParentHandle.Data->GetGroupChild(ParentGroupIndex);
        return ParentHandle.Data.Object;
    }
    static EParentType GetParentType(long type)
    {
        switch (type) {
        case 9:
        case 11:
        case 13:
            return PARENT_TYPE_MESH;
        }
        return PARENT_TYPE_NONE;
    }
    void RedoMatrixConversion(CMatrix3x4* matrix);   // 0x00B38DB0
    bool Update(const CEnginePrimitive& primitive, const CEnginePrimitiveAttachmentInfo& info, const CEnginePrimitiveHandle& parent,
                long parentGroupIndex, unsigned long flags);
};

bool CEngineInternalAttachedPrimitive::Update(const CEnginePrimitive& primitive, const CEnginePrimitiveAttachmentInfo& info,
                                              const CEnginePrimitiveHandle& parent, long parentGroupIndex, unsigned long flags)
{
    AttachmentInfo = info;
    ChildInverseTransformValid = false;
    ParentHandle = parent;
    ParentGroupIndex = parentGroupIndex;
    if (!GetParentPrimitive())
        return false;
    ParentType = GetParentType(GetParentPrimitive()->Type);
    if (ParentType == PARENT_TYPE_NONE)
        return false;
    long type = primitive.Type;
    if (!OutputPrimitive.Data.Object || OutputPrimitive.Data->Type != type) {
        CEnginePrimitiveManager* manager = g_pPrimitiveManagers->Managers[type];
        if (!manager)
            return false;
        CEngineInternalPrimitiveBase* child = manager->CreateInternalPrimitive(type);
        OutputPrimitive.Set(child);
        manager->AddInternalPrimitive(child, false);
    }
    CEngineInternalPrimitiveBase* output = OutputPrimitive.Data.Object;
    output->Update(primitive, flags);
    output->Enable = false;
    if (!output->GetTransform(PrimitiveTransform))
        return false;
    RedoMatrixConversion(&PrimitiveTransform);
    return true;
}
