#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_destroyed = 0; static void* g_destroyedObj = 0; static int g_freed = 0; static void* g_freedBlock[4];
struct SCountBlock_00add5f0 { long m_references; void (*m_pfnDestroy)(void* object); void* m_pObject; };
static void DestroyObj(void* o) { ++g_destroyed; g_destroyedObj = o; }
struct CCountedPointer_00add5f0 { void* m_pObject; SCountBlock_00add5f0* m_pCount;
    ~CCountedPointer_00add5f0() { if (m_pCount) { --m_pCount->m_references; if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); g_freedBlock[g_freed++] = m_pCount; } } m_pObject = 0; m_pCount = 0; } };
static int g_fileDtor = 0;
namespace LHA { struct CFile { virtual ~CFile() { ++g_fileDtor; } }; }
struct CLHFile : LHA::CFile { CCountedPointer_00add5f0 m_data; virtual ~CLHFile() { } };
int main() {
    int obj; SCountBlock_00add5f0 blk; blk.m_references = 1; blk.m_pfnDestroy = DestroyObj; blk.m_pObject = &obj;
    CLHFile* f = new CLHFile; f->m_data.m_pObject = &obj; f->m_data.m_pCount = &blk; delete f; if (blk.m_references != 0 || g_destroyed != 1 || g_destroyedObj != &obj || g_freed != 1 || g_freedBlock[0] != &blk || g_fileDtor != 1) { std::printf("00add5f0_TEST FAIL\n"); return 1; }
    f = new CLHFile; f->m_data.m_pObject = 0; f->m_data.m_pCount = 0; delete f; if (g_destroyed != 1 || g_fileDtor != 2) { std::printf("00add5f0_TEST FAIL\n"); return 1; }
    std::printf("00add5f0_TEST PASS\n"); return 0;
}