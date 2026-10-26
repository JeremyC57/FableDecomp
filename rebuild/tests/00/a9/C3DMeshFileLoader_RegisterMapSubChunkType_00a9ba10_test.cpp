// Link test for 0x00A9BA10: the source object's C3DMeshFileLoader::RegisterMapSubChunkType passes the chunk type and
// MapChunkType->GetAsSuperChunk() to AddChunkType (supplied here) and returns its counted pointer.
#include <cstdio>
#include <cstddef>
class CCPPointerInfo { public: long RefCount; void (__fastcall* DeleteFunc)(void* data); void* Data; };
template <class T> class CCountedPointer {
public:
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer() { if (Info) --Info->RefCount; }
    T* operator->() const { return Data; }
};
class CChunkedFileSuperChunk { public: int id; };
class C3DMeshFileRootChunk;
class CChunkedFileChunk {
public:
    virtual ~CChunkedFileChunk() { }
    virtual void v01() { }
    virtual void v02() { }
    virtual void v03() { }
    virtual long IsUnknownType() const { return 0; }
    virtual void v05() { }
    virtual CChunkedFileSuperChunk* GetAsSuperChunk() { return 0; }
};
static CChunkedFileSuperChunk g_super;
static int g_superCalls = 0;
class CSuperOwner : public CChunkedFileChunk {
public:
    virtual CChunkedFileSuperChunk* GetAsSuperChunk() { ++g_superCalls; return &g_super; }
};
class C3DMeshFileLoader {                               // 0xC0 bytes (Ego_r.pdb)
public:
    unsigned char                         _pad00[0x10]; // +0x00 (vptr / base)
    CCountedPointer<C3DMeshFileRootChunk> RootChunkType;   // +0x10
    CCountedPointer<CChunkedFileChunk>    SubMeshChunkType;            // +0x18
    CCountedPointer<CChunkedFileChunk>    TransformationChunkType;     // +0x20
    CCountedPointer<CChunkedFileChunk>    BoneChunkType;               // +0x28
    CCountedPointer<CChunkedFileChunk>    PrimitiveChunkType;          // +0x30
    CCountedPointer<CChunkedFileChunk>    TrianglesChunkType;          // +0x38
    CCountedPointer<CChunkedFileChunk>    SmoothingGroupsChunkType;    // +0x40
    CCountedPointer<CChunkedFileChunk>    VerticesChunkType;           // +0x48
    CCountedPointer<CChunkedFileChunk>    UniqueVerticesChunkType;     // +0x50
    CCountedPointer<CChunkedFileChunk>    GroupChunkType;              // +0x58
    CCountedPointer<CChunkedFileChunk>    PrimitiveClothChunkType;     // +0x60
    CCountedPointer<CChunkedFileChunk>    SubMeshClothChunkType;       // +0x68
    CCountedPointer<CChunkedFileChunk>    MaterialListChunkType;       // +0x70
    CCountedPointer<CChunkedFileChunk>    MaterialChunkType;           // +0x78
    CCountedPointer<CChunkedFileChunk>    MaterialExtChunkType;        // +0x80
    CCountedPointer<CChunkedFileChunk>    MapChunkType;                // +0x88
    CCountedPointer<CChunkedFileChunk>    EngineMaterialChunkType;     // +0x90
    CCountedPointer<CChunkedFileChunk>    HelpersChunkType;            // +0x98
    CCountedPointer<CChunkedFileChunk>    HelperPointChunkType;        // +0xA0
    CCountedPointer<CChunkedFileChunk>    DummyObjectChunkType;        // +0xA8
    CCountedPointer<CChunkedFileChunk>    ConvexVolumeChunkType;       // +0xB0
    CCountedPointer<CChunkedFileChunk>    MirroredTextureChunkType;    // +0xB8
    CCountedPointer<CChunkedFileChunk> RegisterMapSubChunkType(const CChunkedFileChunk& chunkType);   // from the source object
private:
    CCountedPointer<CChunkedFileChunk> AddChunkType(const CChunkedFileChunk& chunkType, CChunkedFileSuperChunk* parent);   // 0x00A9AD00
};

static const CChunkedFileChunk* g_type = 0;
static CChunkedFileSuperChunk* g_parent = 0;
static CChunkedFileChunk g_proto;
static CCPPointerInfo g_info = { 1, 0, 0 };
CCountedPointer<CChunkedFileChunk> C3DMeshFileLoader::AddChunkType(const CChunkedFileChunk& chunkType, CChunkedFileSuperChunk* parent)
{
    g_type = &chunkType;
    g_parent = parent;
    CCountedPointer<CChunkedFileChunk> r;
    r.Data = &g_proto;
    r.Info = &g_info;
    ++g_info.RefCount;
    return r;
}
int main()
{
    CSuperOwner owner;
    CChunkedFileChunk type;
    C3DMeshFileLoader* loader = new C3DMeshFileLoader;
    loader->MapChunkType.Data = &owner;
    int ok = sizeof(C3DMeshFileLoader) == 0xC0 && offsetof(C3DMeshFileLoader, MapChunkType) == 0x88;
    {
        CCountedPointer<CChunkedFileChunk> p = loader->RegisterMapSubChunkType(type);
        ok = ok && g_superCalls == 1 && g_type == &type && g_parent == &g_super && p.Data == &g_proto && p.Info == &g_info && g_info.RefCount == 2;
    }
    ok = ok && g_info.RefCount == 1;
    loader->MapChunkType.Data = 0;
    delete loader;
    if (!ok) { std::printf("00a9ba10_TEST FAIL\n"); return 1; }
    std::printf("00a9ba10_TEST PASS\n");
    return 0;
}
