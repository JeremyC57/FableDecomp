// C3DMeshObject::operator=(const C3DMeshObject&) -- memberwise copy: mesh info, the counted mesh-stats pointer (same-block test, release, copy, add-ref), the position matrix, the inverse-matrix flag and matrix, the scale
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C3380, length 120 bytes.
// Leaf: ??4C3DMeshObject
// Callees: the counted block's destroy thunk (`call [info+4]` with the data in ecx, __fastcall) and 0x00BFE9BC
// operator delete for the block when the count reaches zero.
// Layout (PDB C3DMeshObject, 120 bytes; no engine header): vptr +0, MeshInfo +4, Mesh (CCountedPointer: Data +8,
// Info +0xc) , PositionMatrix +0x10 (48-byte POD: `rep movsd` x12), InversePositionMatrixSetup +0x40,
// InversePositionMatrix +0x44, Scale +0x74.
// Shape: the counted-pointer assignment is the landed CCountedPointer::operator= (0x00754F10) inlined: the
// source block read before the `this->Info != other.Info` compare, the source object between the compare and
// its branch, the count decremented in place and re-read through the member, the block re-read for delete.
struct CCPPointerInfo {                                  // PDB: RefCount +0, DeleteFunc +4, Data +8
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

template<class T> struct CCountedPointer {
    T*              Data;                                // +0
    CCPPointerInfo* Info;                                // +4
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

struct C3DVector  { float X, Y, Z; };
struct CMatrix3x4 { C3DVector XAxis, YAxis, ZAxis, Position; };   // 48-byte POD
struct C3DMeshInfo;
struct C3DMeshStats;

struct C3DMeshObject {
    virtual void v00();
    C3DMeshInfo*                         MeshInfo;                    // +0x04
    CCountedPointer<const C3DMeshStats>  Mesh;                        // +0x08
    CMatrix3x4                           PositionMatrix;              // +0x10
    bool                                 InversePositionMatrixSetup;  // +0x40
    CMatrix3x4                           InversePositionMatrix;       // +0x44
    float                                Scale;                       // +0x74   sizeof == 0x78
    C3DMeshObject& operator=(const C3DMeshObject& other);
};

C3DMeshObject& C3DMeshObject::operator=(const C3DMeshObject& other)
{
    MeshInfo = other.MeshInfo;
    Mesh = other.Mesh;
    PositionMatrix = other.PositionMatrix;
    InversePositionMatrixSetup = other.InversePositionMatrixSetup;
    InversePositionMatrix = other.InversePositionMatrix;
    Scale = other.Scale;
    return *this;
}
