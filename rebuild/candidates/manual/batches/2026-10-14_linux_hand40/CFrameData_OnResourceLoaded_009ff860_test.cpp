#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_upload = 0; static bool g_uploadOk = false; static int g_deleted = 0;
struct CIVCountedPointeeBase { virtual void Destroy(int) { } virtual void Delete() { ++g_deleted; } unsigned long IVRefCount; };
struct CIVCountedPointer { CIVCountedPointeeBase* Object; void Release() { CIVCountedPointeeBase* old = Object; if (old) { if (--old->IVRefCount == 0) old->Delete(); Object = 0; } }
    CIVCountedPointer& operator=(CIVCountedPointeeBase* o) { Release(); Object = o; if (o) ++o->IVRefCount; return *this; } };
struct CFrameOwner_009ff860 { unsigned char _pad[8]; CIVCountedPointer m_data; };
struct CFrameData { unsigned char _pad[0x1c]; void* m_pGraphic; unsigned char _pad20[0x10]; void* m_pAsyncRead; unsigned char _pad34[0xc]; CFrameOwner_009ff860* m_pOwner;
    bool UploadFrame(long) { ++g_upload; return g_uploadOk; }
    void OnResourceLoaded() { if (m_pAsyncRead) { if (!UploadFrame(0)) { if (!m_pGraphic) m_pOwner->m_data = 0; } } } };
int main() {
    CIVCountedPointeeBase obj; obj.IVRefCount = 1; CFrameOwner_009ff860 o; o.m_data.Object = &obj; CFrameData f; std::memset(&f, 0, sizeof f); f.m_pOwner = &o;
    f.OnResourceLoaded(); if (g_upload != 0 || o.m_data.Object != &obj) { std::printf("009ff860_TEST FAIL\n"); return 1; }
    int r; f.m_pAsyncRead = &r; g_uploadOk = true; f.OnResourceLoaded(); if (g_upload != 1 || o.m_data.Object != &obj) { std::printf("009ff860_TEST FAIL\n"); return 1; }
    g_uploadOk = false; f.m_pGraphic = &r; f.OnResourceLoaded(); if (g_upload != 2 || o.m_data.Object != &obj) { std::printf("009ff860_TEST FAIL\n"); return 1; }
    f.m_pGraphic = 0; f.OnResourceLoaded(); if (g_upload != 3 || o.m_data.Object != 0 || g_deleted != 1 || obj.IVRefCount != 0) { std::printf("009ff860_TEST FAIL\n"); return 1; }
    std::printf("009ff860_TEST PASS\n"); return 0;
}
