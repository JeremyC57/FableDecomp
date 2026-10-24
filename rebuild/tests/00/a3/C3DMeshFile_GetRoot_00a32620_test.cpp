// Behaviour test for 0x00A32620 C3DMeshFile::GetRoot() const: the super-chunk pointer converted to const and static-cast
// into a fresh pointer; a null count block gives an empty result even when the data pointer is set; the reference count
// is balanced (one extra reference held by the returned pointer).
#include <cstdio>

static int g_deleted = 0;
struct CCPPointerInfo {
    long  RefCount;
    void (*DeleteFunc)(void* data);
    void* Data;
};
static void DeleteChunk(void* data) { ++g_deleted; (void)data; }

template <class T> class CCountedPointer {
public:
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    template <class U> CCountedPointer(const CCountedPointer<U>& other) : Data(other.Data), Info(other.Info)
    {
        if (Info) ++Info->RefCount;
    }
    ~CCountedPointer() { ReleaseData(); }
    void ReleaseData()
    {
        if (Info) {
            if (--Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; }
        }
    }
    void ShareData(T* data, CCPPointerInfo* info)
    {
        if (Info != info) {
            ReleaseData();
            Data = data;
            Info = info;
            if (info) ++info->RefCount;
        }
    }
    template <class U> void SetStaticCast(const CCountedPointer<U>& other) { ShareData(static_cast<T*>(other.Data), other.Info); }
};

class CChunkedFileSuperChunk { public: int m_tag; };
class C3DMeshFileRootChunk : public CChunkedFileSuperChunk { };

class C3DMeshFile {
public:
    CCountedPointer<CChunkedFileSuperChunk> RootChunk;
    unsigned int Version;
    CCountedPointer<const C3DMeshFileRootChunk> GetRoot() const
    {
        CCountedPointer<const C3DMeshFileRootChunk> root;
        root.SetStaticCast(CCountedPointer<const CChunkedFileSuperChunk>(RootChunk));
        return root;
    }
};

static int Fail() { std::printf("00a32620_TEST FAIL\n"); return 1; }

int main()
{
    C3DMeshFileRootChunk chunk; chunk.m_tag = 7;
    C3DMeshFile file;
    file.Version = 3;
    // empty block: the result stays empty even though Data is set
    file.RootChunk.Data = &chunk;
    file.RootChunk.Info = 0;
    {
        CCountedPointer<const C3DMeshFileRootChunk> r = file.GetRoot();
        if (r.Data != 0 || r.Info != 0) return Fail();
    }
    CCPPointerInfo* block = new CCPPointerInfo;
    block->RefCount = 1; block->DeleteFunc = DeleteChunk; block->Data = &chunk;
    file.RootChunk.Info = block;
    {
        CCountedPointer<const C3DMeshFileRootChunk> r = file.GetRoot();
        if (r.Data != &chunk || r.Info != block || block->RefCount != 2 || r.Data->m_tag != 7) return Fail();
        {
            CCountedPointer<const C3DMeshFileRootChunk> r2 = file.GetRoot();
            if (block->RefCount != 3) return Fail();
        }
        if (block->RefCount != 2) return Fail();
    }
    if (block->RefCount != 1 || g_deleted != 0) return Fail();
    file.RootChunk.ReleaseData();
    file.RootChunk.Info = 0;
    file.RootChunk.Data = 0;
    if (g_deleted != 1) return Fail();
    std::printf("00a32620_TEST PASS\n");
    return 0;
}