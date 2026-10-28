// C3DMeshLODInfo::UnloadResource() -- when this LOD holds the only reference to its mesh, remove the LOD from its mesh info (keeping it as the stats when it can) and report success
// Leaf: ?UnloadResource@C3DMeshLODInfo@@UAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A254F0, 51 bytes. Name: Ego_r/FableWin PDB ?UnloadResource@C3DMeshLODInfo@@UAE_NXZ
//   (public virtual; catalog row agrees). Callee: C3DMeshInfo::RemoveLOD(ulong, bool) const @ 0x00A249A0 (PDB
//   ?RemoveLOD@C3DMeshInfo@@ABEXK_N@Z, private: C3DMeshLODInfo is its friend here).
// Layout: engine/C3DMeshLODInfo.h: CResource base 0x1C, Mesh (CCountedPointer<C3DMesh2>: Data +0x1C, Info +0x20),
//   FileData +0x24, MeshInfo +0x2C, LOD +0x30; CCPPointerInfo (PDB) RefCount +0. Local polymorphic model.
// Shape: the inline private CanUnload() as two returns (`if (Mesh.IsNull()) return false; return refs == 1;` -- the
//   second result is materialised and re-tested, `sete al ; test al,al`; an `&&` folds it into a jne), GetReferenceCount
//   = `Info ? Info->RefCount : 0`; the LOD index widened with movzx.
class CCPPointerInfo {
public:
    unsigned long RefCount;            // +0x00
    void (__cdecl* DeleteFunc)(void*); // +0x04
    void*         Data;                // +0x08
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    bool IsNull() const { return Data == 0; }
    unsigned long GetReferenceCount() const { return Info ? Info->RefCount : 0; }
};
class CResourceList;
class CResource {
public:
    virtual ~CResource();
    unsigned long  IVRefCount;         // +0x04
    CResourceList* ResourceList;       // +0x08
    CResource*     PrevResource;       // +0x0c
    CResource*     NextResource;       // +0x10
    unsigned long  ResourceSize;       // +0x14
    unsigned long  LastUsedFrame;      // +0x18
    virtual bool UnloadResource();
};
class C3DMesh2;
class CBankFileAsyncData;
class C3DMeshLODInfo;
class C3DMeshInfo {
    friend class C3DMeshLODInfo;
    void RemoveLOD(unsigned long lod, bool keepAsMeshStats) const;   // 0x00A249A0
};
class C3DMeshLODInfo : public CResource {
public:
    CCountedPointer<C3DMesh2>           Mesh;            // +0x1c
    CCountedPointer<CBankFileAsyncData> FileData;        // +0x24
    C3DMeshInfo*                        MeshInfo;        // +0x2c
    unsigned char                       LOD;             // +0x30
    bool                                UsedMeshStats;   // +0x31
    virtual bool UnloadResource();
private:
    bool CanUnload() const
    {
        if (Mesh.IsNull())
            return false;
        return Mesh.GetReferenceCount() == 1;
    }
};

bool C3DMeshLODInfo::UnloadResource()
{
    if (CanUnload()) {
        MeshInfo->RemoveLOD(LOD, true);
        return true;
    }
    return false;
}