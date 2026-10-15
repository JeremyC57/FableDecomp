// CEngineSceneGridCell::~CEngineSceneGridCell() -- clear the cell's primitive list, then destroy the two group-primitive handles (last first): each notifies its primitive when it held the last reference and releases it
// Leaf: ??1CEngineSceneGridCell
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B93660, length 118 bytes (+ int3 padding).
// Callees: ClearPrimitiveList(CEngineInternalPrimitiveBase*) @ 0x00B8FEE0 (__fastcall, the first primitive in ecx),
// virtual slot 3 (last-handle notification, the handle as argument) and slot 1 (destroy) of the persistent primitive,
// CBaseClass::~CBaseClass @ 0x0099A2E0 per handle. Vtable 0x0122F598 = CEnginePrimitiveHandle.
// Layout: the PDB CEngineSceneGridCell (no engine header) with the landed CEnginePrimitiveHandle model; the handle
// destructor matches the retail CEnginePrimitiveHandle deleting destructor @ 0x0041C500: vtable store, `if
// (Data.GetRefCount() == 1) Data->OnLastHandleReleased(this)` through a NULL-SAFE count accessor (`m_p ? count : 0`: the
// pointer lands in eax and is copied to ecx; testing `Data.m_p && Data->m_refCount` loads ecx directly and rotates the loop), `Data = 0` (CIVCountedPointer release plus the unconditional
// store), then the member CIVCountedPointer's own inline release.
// Shape: the cell is not polymorphic (no vtable store of its own); the array is destroyed by an inlined counted loop from
// the end (`sub esi,8`, `dec ebx ; jne`); the zero lives in edi, so the second release's pointer is the known zero
// (`mov ecx,edi`) with its null test kept.
struct CBaseClass {                                // vtable 0x01231710
    virtual void v00();
    CBaseClass();                                  // 0x0099A2D0
    ~CBaseClass();                                 // 0x0099A2E0
};

struct CEnginePrimitiveHandle;

struct CEnginePersistentPrimitiveBase {
    virtual void v00();
    virtual void Destroy();                        // slot 1
    virtual void v02();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle);   // slot 3
    long m_refCount;                               // +0x04
};

template <class T> struct CIVCountedPointer {      // intrusive counted pointer (count at +4 of the object)
    T* m_p;
    CIVCountedPointer() : m_p(0) { }
    ~CIVCountedPointer() { Release(); }
    void Release()
    {
        T* p = m_p;
        if (p) {
            if (--p->m_refCount == 0)
                p->Destroy();
            m_p = 0;
        }
    }
    CIVCountedPointer& operator=(T* p) { Release(); m_p = p; return *this; }
    T* operator->() const { return m_p; }
    long GetRefCount() const { return m_p ? m_p->m_refCount : 0; }
};

struct CEnginePrimitiveHandle : CBaseClass {       // vtable 0x0122F598, 8 bytes
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;   // +0x04
    virtual void v00();
    CEnginePrimitiveHandle() { }
    ~CEnginePrimitiveHandle()
    {
        if (Data.GetRefCount() == 1)
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
};

struct CEngineInternalPrimitiveBase;

struct CEngineSceneGridCell {                      // 0x48 bytes (PDB layout, matches retail)
    CEngineSceneGridCell*         Parent;          // +0x00
    CEngineInternalPrimitiveBase* FirstPrimitive;  // +0x04
    float                         BoundingBox[6];  // +0x08 C3DBoundingBox
    float                         DrawDistance;    // +0x20
    CEnginePrimitiveHandle        GroupPrimitiveHandle[2];   // +0x24
    long                          LocalCount;      // +0x34
    unsigned long                 TotalCount;      // +0x38
    unsigned long                 RenderMask;      // +0x3c
    unsigned long                 OccludedLayers;  // +0x40
    unsigned char                 BoundingBoxValid : 1;     // +0x44 bit 0
    unsigned char                 TotalCountValid : 1;      //        bit 1
    unsigned char                 RenderMaskValid : 1;      //        bit 2
    unsigned char                 WasRendered : 1;          //        bit 3
    unsigned char                 HasOcclusionResults : 1;  //        bit 4
    unsigned char                 OccludedFrames;  // +0x45
    unsigned char                 PreloadFrames;   // +0x46
    ~CEngineSceneGridCell();
};

void __fastcall ClearPrimitiveList(CEngineInternalPrimitiveBase* first);   // 0x00B8FEE0

CEngineSceneGridCell::~CEngineSceneGridCell()
{
    ClearPrimitiveList(FirstPrimitive);
}
