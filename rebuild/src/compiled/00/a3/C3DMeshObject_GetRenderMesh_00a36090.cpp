// C3DMeshObject::GetRenderMesh(unsigned long lod, unsigned long flags) const -- fetch the render mesh of a LOD from the mesh info (always with flag 1 set) into a local counted pointer and return a copy of it
// Leaf: GetRenderMesh
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A36090, 112 bytes, `ret 0Ch` (hidden result + two arguments).
// Identity: Ego_r.pdb `?GetRenderMesh@C3DMeshObject@@QBE?AV?$CCountedPointer@$$CBVC3DMesh2@@@@KK@Z`. The catalog's
//   "C3DMeshInfo_GetMesh" names the callee. No direct callers (reached through the graphic appearance code).
// Callees: C3DMeshInfo::GetMesh(CCountedPointer<const C3DMesh2>& out, unsigned long lod, unsigned long flags) const
//   @ 0x00A25E70 (landed, returns bool, ignored here), the count block's destroy function (`mov ecx,[blk+8] ; call [blk+4]`),
//   operator delete @ 0x00BFE9BC.
// Layout (Ego_r.pdb C3DMeshObject): MeshInfo +4 (`mov ecx,[ecx+4]`). Count block: RefCount +0, DeleteFunc +4, Data +8.
// Shape: the local is default-constructed (both zero stores sit between the argument pushes and the call -- the inline
//   constructor runs right before its address is used), the result is copy-constructed from it, then the local's
//   destructor releases it: VC7.1 has no NRVO, and the add-ref and the release share one null test of the block (the
//   block re-read from the address-taken local at every use); the destructor's trailing zero stores are dead.
void __cdecl operator delete(void* p);                  // 0x00BFE9BC

struct SCountBlock {
    long  RefCount;                                     // +0
    void (__fastcall* DeleteFunc)(void* data);          // +4, Data in ecx
    void* Data;                                         // +8
};

template <class T> struct CCountedPointer {
    T*           Data;                                  // +0
    SCountBlock* Info;                                  // +4
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};

struct C3DMesh2;

struct C3DMeshInfo {
    bool GetMesh(CCountedPointer<const C3DMesh2>& out, unsigned long lod, unsigned long flags) const;   // 0x00A25E70
};

struct C3DMeshObject {
    virtual void v00();
    C3DMeshInfo* MeshInfo;                              // +0x04
    CCountedPointer<const C3DMesh2> GetRenderMesh(unsigned long lod, unsigned long flags) const;
};

CCountedPointer<const C3DMesh2> C3DMeshObject::GetRenderMesh(unsigned long lod, unsigned long flags) const
{
    CCountedPointer<const C3DMesh2> mesh;
    MeshInfo->GetMesh(mesh, lod, flags | 1);
    return mesh;
}