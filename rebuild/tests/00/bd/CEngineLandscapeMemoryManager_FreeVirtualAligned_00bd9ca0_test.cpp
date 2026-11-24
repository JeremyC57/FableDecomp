// Behaviour test for CEngineLandscapeMemoryManager::FreeVirtualAligned(void*) @ 0x00BD9CA0 on the shared header: links the row object.
// The manager is raw storage (its constructor is not linked); FreeVirtual 0x00BD9B90 is stubbed to record the raw block it receives.
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_FREEVIRTUALALIGNED
#include <stdio.h>
#include "engine/CEngineLandscapeMemoryManager.h"

static double g_manager_storage[sizeof(CEngineLandscapeMemoryManager) / sizeof(double) + 1];
static void* g_freed = 0;
static int g_frees = 0;
void CEngineLandscapeMemoryManager::FreeVirtual(void* block) { g_freed = block; ++g_frees; }

int main()
{
    CEngineLandscapeMemoryManager* manager = reinterpret_cast<CEngineLandscapeMemoryManager*>(g_manager_storage);
    long raw[16];
    char* base = reinterpret_cast<char*>(raw);
    char* aligned = base + 24;                   // AllocVirtualAligned stored (aligned - raw) just before the block
    reinterpret_cast<long*>(aligned)[-1] = 24;
    manager->FreeVirtualAligned(aligned);
    bool ok = g_frees == 1 && g_freed == base;
    manager->FreeVirtualAligned(0);              // a null block is ignored
    if (g_frees != 1) ok = false;
    aligned = base + 8;                          // a different stored offset
    reinterpret_cast<long*>(aligned)[-1] = 8;
    manager->FreeVirtualAligned(aligned);
    ok = ok && g_frees == 2 && g_freed == base;
    printf(ok ? "00bd9ca0_TEST PASS\n" : "00bd9ca0_TEST FAIL\n");
    return ok ? 0 : 1;
}
