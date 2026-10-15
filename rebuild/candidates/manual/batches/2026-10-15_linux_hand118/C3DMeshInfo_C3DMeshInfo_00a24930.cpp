// C3DMeshInfo::~C3DMeshInfo() -- clears the four LOD-mesh counted pointers of the shared mesh data (when there is any), releases the mesh data itself, then the CMeshDataBankEntry base
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A24930, length 105 bytes.
// Leaf: ??1C3DMeshInfo
// Callees: the counted objects' virtual slot 1 (Destroy) when their count at +4 reaches zero.
// Vtables: 0x0129CD3C C3DMeshInfo (stored first), 0x0129CD28 CMeshDataBankEntry (the base's inline destructor, last).
// Layout: the 12-byte CMeshDataBankEntry root (vptr + two words) and MeshData (CIVCountedPointer<CMeshData>)
// at +0x0c, as the landed constructor 0x00A248F0 models the PDB C3DMeshInfo; CMeshData (PDB, 32 bytes) holds
// its own count at +4 and LODMeshes[4] at +8 (the loop runs the byte offset 8..0x18 with `jb`).
// Shape: each LOD slot is `if (slot) slot = CIVCountedPointer();` -- the assignment is the landed
// CIVCountedPointer operator= (Release with its own null test and inner zero store, then the copied null
// stored again), the MeshData pointer re-read every iteration; the MeshData member destructor is the bare
// Release (zero store inside the branch).
struct CCountedObject {                            // counted object: vptr, then the reference count
    virtual void v00();
    virtual void Destroy();                        // slot 1
    long m_refCount;                               // +0x04
    void Release()
    {
        if (--m_refCount == 0)
            Destroy();
    }
};

template <class T>
struct CIVCountedPointer {
    T* Object;
    CIVCountedPointer() : Object(0) { }
    ~CIVCountedPointer() { Release(); }
    void Release()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        Release();
        Object = other.Object;
        if (Object)
            ++Object->m_refCount;
        return *this;
    }
};

struct C3DMeshLODInfo : CCountedObject { };

struct CMeshDataBankEntry {                        // the 12-byte root: vptr + two words
    virtual void v00();
    long m_4;                                      // +0x04
    long m_8;                                      // +0x08
    ~CMeshDataBankEntry() { }
};

struct C3DMeshInfo : CMeshDataBankEntry {
    struct CMeshData : CCountedObject {            // PDB C3DMeshInfo::CMeshData (32 bytes)
        CIVCountedPointer<C3DMeshLODInfo> LODMeshes[4];   // +0x08
        CIVCountedPointer<C3DMeshLODInfo> MeshStats;      // +0x18
        unsigned long                     MeshID;         // +0x1c
    };
    CIVCountedPointer<CMeshData> MeshData;         // +0x0c
    virtual void v00();
    ~C3DMeshInfo();
};

C3DMeshInfo::~C3DMeshInfo()
{
    if (MeshData.Object) {
        for (unsigned long i = 0; i < 4; ++i) {
            if (MeshData.Object->LODMeshes[i].Object)
                MeshData.Object->LODMeshes[i] = CIVCountedPointer<C3DMeshLODInfo>();
        }
    }
}
