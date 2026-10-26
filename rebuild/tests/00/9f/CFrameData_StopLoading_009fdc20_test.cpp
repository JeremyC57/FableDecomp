// Behaviour test for 0x009FDC20 CFrameData::StopLoading.
#include <cstdio>
static int g_destroyed = 0, g_cleared = 0;
static void Destroy(void*) { ++g_destroyed; }
struct SCountBlock { int m_references; void (*m_pfnDestroy)(void*); void* m_pObject; };
class CBankFileAsyncData { public: bool finished; bool IsFinished() { return finished; } };
struct CCountedPointer {
    CBankFileAsyncData* m_pObject; SCountBlock* m_pCount;
    void Reset()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; }
        }
        m_pCount = 0; m_pObject = 0;
    }
};
class CGraphicFrameData { public: int Sizeof() { return 100; } };
class CManagedTexture { public:
    void* m_pData;
    bool HasData() const { return m_pData != 0; }
    int Sizeof() { return 1000; }
    void ClearHandle() { ++g_cleared; }
};
struct CFrameData {
    CGraphicFrameData* m_pGraphic; CManagedTexture LoadingManagedTexture; CCountedPointer FileData; unsigned int m_size;
    int Sizeof()
    {
        int size = (m_pGraphic ? m_pGraphic->Sizeof() + 4 : 4) + 0x44;
        if (LoadingManagedTexture.HasData()) size += LoadingManagedTexture.Sizeof();
        return size;
    }
    void SetResourceSize(unsigned int s) { m_size = s; }
    bool StopLoading()
    {
        if (FileData.m_pObject) {
            if (FileData.m_pObject->IsFinished()) { FileData.Reset(); LoadingManagedTexture.ClearHandle(); }
            else return false;
        }
        SetResourceSize(Sizeof());
        return true;
    }
};
static int Fail() { std::printf("009fdc20_TEST FAIL\n"); return 1; }
int main()
{
    CGraphicFrameData g; int data = 0;
    CBankFileAsyncData file; file.finished = false;
    SCountBlock* block = new SCountBlock; block->m_references = 1; block->m_pfnDestroy = Destroy; block->m_pObject = &file;
    CFrameData f; f.m_pGraphic = 0; f.LoadingManagedTexture.m_pData = 0; f.FileData.m_pObject = &file; f.FileData.m_pCount = block; f.m_size = 0;
    if (f.StopLoading() || f.m_size != 0 || g_destroyed != 0) return Fail();
    file.finished = true;
    if (!f.StopLoading() || f.FileData.m_pObject || f.FileData.m_pCount || g_destroyed != 1 || g_cleared != 1 || f.m_size != 4 + 0x44) return Fail();
    f.m_pGraphic = &g; f.LoadingManagedTexture.m_pData = &data;
    if (!f.StopLoading() || f.m_size != 104 + 0x44 + 1000 || g_cleared != 1) return Fail();
    std::printf("009fdc20_TEST PASS\n");
    return 0;
}