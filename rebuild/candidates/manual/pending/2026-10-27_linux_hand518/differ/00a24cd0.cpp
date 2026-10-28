// C3DMeshInfo::Reset() -- drop the mesh data and return the mesh info to its unloaded state (no radius, no physics mesh, no LOD errors or sizes)
// Grade: DIFFER(82v80) -- everything to the PhysicsMeshIndex store matches; the LODErrors clear: retail `xor ecx,ecx ; mov [esi+40h..4Ch],ecx` direct (with the LODDataSizes `lea eax,[esi+50h] ; xor edx,edx` scheduled inside it); a memset gives `lea edx,[esi+40h]`, a counted loop shares one edi zero with the MeshData stores (74 bytes)
// Leaf: ?Reset@C3DMeshInfo@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A24CD0, 80 bytes. Name: Ego_r/FableWin PDB ?Reset@C3DMeshInfo@@EAEXXZ (private
//   virtual; the catalog row carries this mangled name). Callee: the mesh data's virtual Delete (slot 1, +4) at count zero.
// Layout: as IsLoaded 0x00A23E40 (engine/C3DMeshInfo.h offsets: MeshData +0xC, SafeBoundingRadius +0x38,
//   PhysicsMeshIndex +0x3C, LODErrors[4] +0x40, LODDataSizes[4] +0x50); C3DBoundingSphere (PDB) Center +0, Radius +0xC.
// Shape: CIVCountedPointer's inline `= 0` (release through the object, clear, then the store -- retail keeps both zero
//   stores); BoundingSphere.Radius = -1 (+0x34); LODErrors and LODDataSizes by two memset intrinsics
//   (each materialises its own zero: `xor ecx,ecx`, then `lea eax,[esi+50h] ; xor edx,edx`).
#pragma intrinsic(memset)
extern "C" void* __cdecl memset(void* dest, int value, unsigned int count);
#pragma pack(push, 1)
class C3DVector { public: float X, Y, Z; };
#pragma pack(pop)
class C3DBoundingSphere {
public:
    C3DVector Center;   // +0x00
    float     Radius;   // +0x0c
};
class CResourceList;
class CResource {                                  // CIVCountedPointeeBase + resource list links (0x1C bytes)
public:
    virtual void Destroy(int flags);
    virtual void Delete();                         // slot 1 (+4): the last reference
    unsigned long  IVRefCount;                     // +0x04
    CResourceList* ResourceList;                   // +0x08
    CResource*     PrevResource;                   // +0x0c
    CResource*     NextResource;                   // +0x10
    unsigned long  ResourceSize;                   // +0x14
    unsigned long  LastUsedFrame;                  // +0x18
};
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
};
template <class T> class CIVCountedPointer {
public:
    T* Object;
    T* Get() const { return Object; }
    T* operator->() const { return Object; }
    bool IsNull() const { return Object == 0; }
    CIVCountedPointer& operator=(T* object)
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
        Object = object;
        return *this;
    }
};
class C3DMesh2;
class CBankFileAsyncData;
class C3DMeshInfo;
class C3DMeshLODInfo : public CResource {
public:
    CCountedPointer<C3DMesh2>           Mesh;          // +0x1c
    CCountedPointer<CBankFileAsyncData> FileData;      // +0x24
    C3DMeshInfo*                        MeshInfo;      // +0x2c
    unsigned char                       LOD;           // +0x30
    bool                                UsedMeshStats; // +0x31
};
class C3DMeshInfo {
public:
    class CMeshData {
    public:
        virtual void Destroy(int flags);
        virtual void Delete();                            // slot 1 (+4)
        void Release() { if (--IVRefCount == 0) Delete(); }
        unsigned long                     IVRefCount;     // +0x04
        CIVCountedPointer<C3DMeshLODInfo> LODMeshes[4];   // +0x08
        CIVCountedPointer<C3DMeshLODInfo> MeshStats;      // +0x18
        unsigned long                     MeshID;         // +0x1c
    };
    virtual unsigned long Sizeof() const;
    long                          f4;                     // +0x04 (retail-only, engine/C3DMeshInfo.h)
    long                          f8;                     // +0x08
    CIVCountedPointer<CMeshData>  MeshData;               // +0x0c
    unsigned char                 BoundingBox[0x18];      // +0x10 C3DBoundingBox
    C3DBoundingSphere             BoundingSphere;         // +0x28
    float                         SafeBoundingRadius;     // +0x38
    long                          PhysicsMeshIndex;       // +0x3c
    float                         LODErrors[4];           // +0x40
    unsigned long                 LODDataSizes[4];        // +0x50
    bool HasLOD(unsigned long lod) const { return lod < 4 && LODDataSizes[lod] != 0; }
    bool IsLoaded() const;
    bool IsLoadedStats() const;
private:
    virtual void Reset();
};

void C3DMeshInfo::Reset()
{
    MeshData = 0;
    BoundingSphere.Radius = -1.0f;
    PhysicsMeshIndex = -1;
    memset(LODErrors, 0, sizeof(LODErrors));
    memset(LODDataSizes, 0, sizeof(LODDataSizes));
}
