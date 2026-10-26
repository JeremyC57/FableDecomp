// Behaviour test for 0x00A36090 C3DMeshObject::GetRenderMesh: GetMesh(local, lod, flags | 1), return a copy, release the local.
#include <cstdio>

static int g_deleted = 0;
struct SCountBlock {
    long  RefCount;
    void (*DeleteFunc)(void* data);
    void* Data;
};
static void DeleteMesh(void* data) { ++g_deleted; (void)data; }

template <class T> class CCountedPointer { public:
    T* Data;
    SCountBlock* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; }
        }
        Data = 0; Info = 0;
    }
};
class C3DMesh2 { public: int m_id; };
static unsigned long g_lod = 0, g_flags = 0;
class C3DMeshInfo { public:
    C3DMesh2* m_mesh;
    SCountBlock* m_block;
    bool GetMesh(CCountedPointer<const C3DMesh2>& out, unsigned long lod, unsigned long flags) const
    {
        g_lod = lod; g_flags = flags;
        if (!m_mesh) return false;
        out.Data = m_mesh; out.Info = m_block; ++m_block->RefCount;
        return true;
    }
};
class C3DMeshObject { public:
    C3DMeshInfo* MeshInfo;
    CCountedPointer<const C3DMesh2> GetRenderMesh(unsigned long lod, unsigned long flags) const
    {
        CCountedPointer<const C3DMesh2> mesh;
        MeshInfo->GetMesh(mesh, lod, flags | 1);
        return mesh;
    }
};

int main()
{
    C3DMesh2 mesh2; mesh2.m_id = 5;
    SCountBlock* block = new SCountBlock;
    block->RefCount = 1; block->DeleteFunc = DeleteMesh; block->Data = &mesh2;
    C3DMeshInfo info; info.m_mesh = &mesh2; info.m_block = block;
    C3DMeshObject obj; obj.MeshInfo = &info;
    {
        CCountedPointer<const C3DMesh2> got = obj.GetRenderMesh(2, 4);
        if (got.Data != &mesh2 || got.Info != block || block->RefCount != 2 || g_lod != 2 || g_flags != 5) {
            std::printf("00a36090_TEST FAIL\n"); return 1;
        }
    }
    if (block->RefCount != 1 || g_deleted != 0) { std::printf("00a36090_TEST FAIL\n"); return 1; }
    info.m_mesh = 0;
    {
        CCountedPointer<const C3DMesh2> got = obj.GetRenderMesh(0, 0);
        if (got.Data != 0 || got.Info != 0 || g_flags != 1) { std::printf("00a36090_TEST FAIL\n"); return 1; }
    }
    std::printf("00a36090_TEST PASS\n");
    return 0;
}