// C3DMeshObject::SetMesh(C3DMeshInfo*) -- stores the mesh info and takes its counted mesh-stats pointer (assigned from the by-value result, which is then released)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A36010, length 128 bytes.
// Leaf: SetMesh
// Callees: 0x00A25530 C3DMeshInfo::GetMeshStats() (CCountedPointer<const C3DMeshStats> by hidden pointer into
// an 8-byte frame temporary), the counted block's destroy thunk (`call [info+4]`, data in ecx) and 0x00BFE9BC
// operator delete for the block when its count reaches zero.
// Layout (PDB C3DMeshObject): MeshInfo +4, Mesh (CCountedPointer: Data +8, Info +0xc).
// Shape: `Mesh = MeshInfo->GetMeshStats()`-style assignment through the landed CCountedPointer::operator=
// (0x00754F10 shape, inlined: both source fields read before the block compare) followed by the temporary's
// inline destructor (count decremented in place and re-read from the frame slot, block re-read for delete).
class CCPPointerInfo { public:                                  // PDB: RefCount +0, DeleteFunc +4, Data +8
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

template<class T> class CCountedPointer { public:
    T*              Data;                                // +0
    CCPPointerInfo* Info;                                // +4
    ~CCountedPointer()
    {
        if (Info != 0) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                operator delete(Info);
            }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        T* data = other.Data;
        CCPPointerInfo* info = other.Info;
        if (Info != info) {
            if (Info != 0) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    operator delete(Info);
                }
            }
            Data = data;
            Info = info;
            if (info != 0)
                ++info->RefCount;
        }
        return *this;
    }
};

class C3DMeshStats;
class C3DMeshInfo { public:
    CCountedPointer<const C3DMeshStats> GetMeshStats();  // 0x00A25530
};

class C3DMeshObject { public:
    virtual void v00();
    C3DMeshInfo*                         MeshInfo;       // +0x04
    CCountedPointer<const C3DMeshStats>  Mesh;           // +0x08
    void SetMesh(C3DMeshInfo* meshInfo);
};

void C3DMeshObject::SetMesh(C3DMeshInfo* meshInfo)
{
    MeshInfo = meshInfo;
    Mesh = meshInfo->GetMeshStats();
}