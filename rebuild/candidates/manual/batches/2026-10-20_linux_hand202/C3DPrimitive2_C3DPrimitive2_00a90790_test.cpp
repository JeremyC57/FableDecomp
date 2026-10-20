// Behaviour test for C3DPrimitive2::~C3DPrimitive2 (0x00A90790): FreeData runs first, then the members go in reverse
// order -- mirrored list freed, partition tree released (block destroyed at count 0), partition index buffer freed,
// index/vertex/data objects deleted, cloth range destroyed and its block freed; null members are skipped. The free
// and block-delete spies record only while armed and never free (all blocks are static).
#include <cstdio>
#include <cstring>
static char g_log[512];
static int g_armed;
static void Log(const char* s) { if (g_armed) { std::strcat(g_log, s); std::strcat(g_log, ";"); } }
static void free_spy(void* p) { char b[32]; std::sprintf(b, "free%d", *(int*)p); Log(b); }
static void delete_block_spy(void*) { Log("deleteblock"); }
struct CCPPointerInfo { long RefCount; void (*DeleteFunc)(void* data); void* Data; };
static void DestroyTree(void*) { Log("tree"); }
template<class T> struct CCountedPointer {
    T* Data; CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete_block_spy(Info); }
        }
        Data = 0; Info = 0;
    }
};
struct C3DClothPrimitive; struct C3DMeshPartitionElement;
static void DestroyRange_CountedPointer_00a96940(CCountedPointer<C3DClothPrimitive>* first, CCountedPointer<C3DClothPrimitive>* last)
{ char b[32]; std::sprintf(b, "cloth%d", (int)(last - first)); Log(b); }
inline void DestroyRange(unsigned short*, unsigned short*) {}
inline void DestroyRange(unsigned long*, unsigned long*) {}
inline void DestroyRange(CCountedPointer<C3DClothPrimitive>* first, CCountedPointer<C3DClothPrimitive>* last)
{ DestroyRange_CountedPointer_00a96940(first, last); }
template<class T> struct CArray {
    T* m_begin; T* m_end; T* m_capacity;
    ~CArray() { DestroyRange(m_begin, m_end); if (m_begin) free_spy(m_begin); }
};
struct CPolymorphicResource { const char* Name; virtual ~CPolymorphicResource() { Log(Name); } };
template<class T> struct scoped_ptr { T* m_p; ~scoped_ptr() { if (m_p) m_p->~T(); } };   // the test objects are static: run the dtor only
struct CMovableResource { virtual ~CMovableResource() { Log("base"); } };
struct C3DPrimitive2 : CMovableResource {
    CArray<CCountedPointer<C3DClothPrimitive> > ClothPrimitives;
    scoped_ptr<CPolymorphicResource> Data, VertexBuffer, IndexBuffer;
    CArray<unsigned short> PartitionedMeshSourceIndexBuffer;
    CCountedPointer<C3DMeshPartitionElement> PartitionTree;
    CArray<unsigned long> MirroredTextureVertexIndexList;
    void FreeData() { Log("FreeData"); }
    virtual ~C3DPrimitive2();
};
C3DPrimitive2::~C3DPrimitive2()
{
    FreeData();
}
int main()
{
    static int clothBlock[4] = { 1 }, partBlock[2] = { 2 }, mirrorBlock[2] = { 3 };
    static CPolymorphicResource data, vb, ib;
    data.Name = "data"; vb.Name = "vb"; ib.Name = "ib";
    static CCPPointerInfo info = { 2, DestroyTree, 0 };
    int bad = 0;
    {
        C3DPrimitive2 p;
        p.ClothPrimitives.m_begin = (CCountedPointer<C3DClothPrimitive>*)clothBlock; p.ClothPrimitives.m_end = p.ClothPrimitives.m_begin + 2;
        p.Data.m_p = &data; p.VertexBuffer.m_p = &vb; p.IndexBuffer.m_p = &ib;
        p.PartitionedMeshSourceIndexBuffer.m_begin = (unsigned short*)partBlock; p.PartitionedMeshSourceIndexBuffer.m_end = 0;
        p.PartitionTree.Data = 0; p.PartitionTree.Info = &info;
        p.MirroredTextureVertexIndexList.m_begin = (unsigned long*)mirrorBlock; p.MirroredTextureVertexIndexList.m_end = 0;
        g_armed = 1;
    }
    g_armed = 0;
    if (std::strcmp(g_log, "FreeData;free3;free2;ib;vb;data;cloth2;free1;base;") != 0 || info.RefCount != 1) bad |= 1;
    g_log[0] = 0; info.RefCount = 1;
    {
        C3DPrimitive2 q;
        q.ClothPrimitives.m_begin = 0; q.ClothPrimitives.m_end = 0;
        q.Data.m_p = 0; q.VertexBuffer.m_p = 0; q.IndexBuffer.m_p = 0;
        q.PartitionedMeshSourceIndexBuffer.m_begin = 0; q.PartitionedMeshSourceIndexBuffer.m_end = 0;
        q.PartitionTree.Data = 0; q.PartitionTree.Info = &info;
        q.MirroredTextureVertexIndexList.m_begin = 0; q.MirroredTextureVertexIndexList.m_end = 0;
        g_armed = 1;
    }
    g_armed = 0;
    if (std::strcmp(g_log, "FreeData;tree;deleteblock;cloth0;base;") != 0 || info.RefCount != 0) bad |= 2;
    if (bad) { std::printf("00a90790_TEST FAIL\n"); return 1; }
    std::printf("00a90790_TEST PASS\n");
    return 0;
}
