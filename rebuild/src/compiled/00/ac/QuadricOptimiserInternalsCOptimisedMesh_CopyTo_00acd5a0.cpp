// QuadricOptimiserInternals::COptimisedMesh::CopyTo(C3DMeshFile& file) -- write the optimised sub-meshes back into the mesh file: for each SUBM chunk in turn (paired with SubMeshes[i]) drop its PRIM children, then add a new primitive chunk for every primitive that still has a live face
// Leaf: ?CopyTo@COptimisedMesh@QuadricOptimiserInternals@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ACD5A0, 670 bytes (`ret 4`); the mesh-file counted-pointer temporaries release with
//   Data and Info cleared, the face count is read once per primitive.
// Callees (names from Ego_r.pdb): C3DMeshFile::GetRoot() 0x00AD1E80 (non-const; CCountedPointer<C3DMeshFileRootChunk> by
//   hidden return), C3DMeshFileRootChunk::FindFirstSubMesh() 0x00A9AEC0 ("SUBM" 0x0129E06C) and
//   FindNextSubMesh(const CCountedPointer<const C3DMeshFileSubMeshChunk>&) 0x00A9AF50, CChunkedFileSuperChunk::
//   RemoveSubChunksOfType(const char*) 0x00AA82C0 and AddSubChunk(const CCountedPointer<CChunkedFileChunk>&) 0x00AA84F0
//   (the sub-mesh chunk is a super chunk), operator new 0x00BFEA1A, C3DMeshFilePrimitiveChunk ctor 0x00ABD080, its CCountedPointer DeleteData
//   0x00ACF440, COptimisedPrimitive::CopyTo 0x00ACC490, operator delete 0x00BFE9BC; "PRIM" 0x0129DEE4.
// Layout (retail, STLport 12-byte vectors): COptimisedMesh {vptr, SubMeshes +4}; COptimisedSubMesh 0x18 {Primitives +0
//   (CCountedPointer, 8 bytes each), Bones +0xc}; COptimisedPrimitive Faces +0x1c (COptimisedFace 0x48 bytes, the
//   live flag at +0x3c); CCPPointerInfo {RefCount +0, DeleteFunc +4 (__fastcall), Data +8}.
struct CCPPointerInfo {
    unsigned long RefCount;                              // +0x00
    void (__fastcall* DeleteFunc)(void* data);           // +0x04
    void*         Data;                                  // +0x08
    CCPPointerInfo(void* data, void (__fastcall* deleteFunc)(void*)) : RefCount(1), DeleteFunc(deleteFunc), Data(data) {}
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                // +0x00
    CCPPointerInfo* Info;                                // +0x04
    static void __fastcall DeleteData(void* data);
    explicit CCountedPointer(T* data) : Data(data)
    {
        if (data)
            Info = new CCPPointerInfo(data, DeleteData);
        else
            Info = 0;
    }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    template <class U> CCountedPointer(const CCountedPointer<U>& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
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
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (Info) ++Info->RefCount;
        }
        return *this;
    }
};
template <class T> struct CVector {                      // STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int i) const { return _M_start[i]; }
};
class CChunkedFileChunk { };
class CChunkedFileSuperChunk : public CChunkedFileChunk {
public:
    void RemoveSubChunksOfType(const char* type);                        // 0x00AA82C0
    void AddSubChunk(const CCountedPointer<CChunkedFileChunk>& chunk);   // 0x00AA84F0
};
class C3DMeshFileSubMeshChunk : public CChunkedFileSuperChunk { };
class C3DMeshFilePrimitiveChunk : public CChunkedFileChunk {
public:
    unsigned char _body[0x38];
    C3DMeshFilePrimitiveChunk();                         // 0x00ABD080
};
class C3DMeshFileRootChunk : public CChunkedFileSuperChunk {
public:
    CCountedPointer<C3DMeshFileSubMeshChunk> FindFirstSubMesh();                                  // 0x00A9AEC0
    CCountedPointer<C3DMeshFileSubMeshChunk> FindNextSubMesh(const CCountedPointer<const C3DMeshFileSubMeshChunk>& after);   // 0x00A9AF50
};
class C3DMeshFile {
public:
    CCountedPointer<C3DMeshFileRootChunk> GetRoot();                                                // 0x00AD1E80
};
namespace QuadricOptimiserInternals {
class COptimisedFace {
public:
    unsigned char _pad00[0x3c];
    bool          Live;                                  // +0x3c
    unsigned char _pad3d[0xb];
};
class COptimisedPrimitive {
public:
    long                  MaterialIndex;                 // +0x00
    CVector<long>         UniqueVertices;                // +0x04
    CVector<long>         Vertices;                      // +0x10
    CVector<COptimisedFace> Faces;                       // +0x1c
    void CopyTo(C3DMeshFilePrimitiveChunk& chunk);       // 0x00ACC490
};
class COptimisedSubMesh {
public:
    CVector<CCountedPointer<COptimisedPrimitive> > Primitives;   // +0x00
    CVector<long>                                  Bones;        // +0x0c
};
class COptimisedMesh {
public:
    CVector<COptimisedSubMesh> SubMeshes;                // +0x04
    virtual void CopyTo(C3DMeshFile& file);
};

void COptimisedMesh::CopyTo(C3DMeshFile& file)
{
    C3DMeshFileRootChunk* root = file.GetRoot().Data;
    CCountedPointer<C3DMeshFileSubMeshChunk> subMesh = root->FindFirstSubMesh();
    if (subMesh.Data != 0) {
        unsigned int i = 0;
        do {
            COptimisedSubMesh& source = SubMeshes[i];
            subMesh.Data->RemoveSubChunksOfType("PRIM");
            for (unsigned int p = 0; p < source.Primitives.size(); ++p) {
                COptimisedPrimitive* primitive = source.Primitives[p].Data;
                unsigned int live = 0;
                unsigned int faceCount = primitive->Faces.size();
                for (unsigned int f = 0; f < faceCount; ++f)
                    if (primitive->Faces[f].Live)
                        ++live;
                if (live > 0) {
                    CCountedPointer<C3DMeshFilePrimitiveChunk> chunk(new C3DMeshFilePrimitiveChunk);
                    primitive->CopyTo(*chunk.Data);
                    subMesh.Data->AddSubChunk(chunk);
                }
            }
            subMesh = root->FindNextSubMesh(subMesh);
            ++i;
        } while (subMesh.Data != 0);
    }
}
}