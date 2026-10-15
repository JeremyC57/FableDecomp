// CLocalDetailPrimitiveMesh::~CLocalDetailPrimitiveMesh() -- destroy the mesh's CEnginePrimitiveHandle member (+0x6c: tell the primitive it is losing its last handle, drop the counted Data pointer, CBaseClass base), then the CMovableResource base's inline destructor stores its vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDFD80, length 105 bytes.
// Leaf: ??1CLocalDetailPrimitiveMesh
// Callees: the primitive's virtual slot 3 (+0x0C, with the handle) and slot 1 (+0x04, Destroy on the last
//          reference), CBaseClass::~CBaseClass @ 0x0099A2E0. Vtables: 0x0122F598 CEnginePrimitiveHandle,
//          0x0129B860 CMovableResource.
// Shape: the handle model of the landed 0x004C4160 (forceinline handle / counted-pointer destructors, IsUnique,
// `Data = 0` releasing then storing zero twice, the member destructor releasing again); the mesh class is
// `__declspec(novtable)` with an empty destructor, so only the base's inline destructor stores a vtable (after
// the members are destroyed).
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
    ~CBaseClass();                                 // 0x0099A2E0
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

struct CMovableResource {                          // vtable 0x0129B860
    virtual void v000();
    ~CMovableResource() { }
};
struct __declspec(novtable) CLocalDetailPrimitiveMesh : CMovableResource {
    unsigned char          _pad04[0x68];
    CEnginePrimitiveHandle m_handle;               // +0x6c
    ~CLocalDetailPrimitiveMesh();
};

CLocalDetailPrimitiveMesh::~CLocalDetailPrimitiveMesh()
{
}
