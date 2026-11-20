#include <stdio.h>
#include <string.h>
// Behaviour test for 0x00B3B060 CEngineResourceManager::GetIndexBufferUsage (links the row object).
// NF152 (2026-11-20, q4562 / q4729): rewritten on the shared headers the row now uses (engine/CEngineResourceManager.h retail branch,
//   engine/CDisplayManager.h); the checks and the PassPattern are unchanged. CDisplayManager::GetVertexShaderVersion and
//   NEngineInternal::GPDisplayManager are test stand-ins returning a settable version; the resource manager is zeroed storage.
#include "fable_compiler.h"
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#include "engine/Win32Types.h"
#include "engine/Direct3D9Types.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CDisplayManager.h"

static unsigned long g_version = 0;
unsigned long CDisplayManager::GetVertexShaderVersion() const { return g_version; }
namespace NEngineInternal { CDisplayManager* GPDisplayManager = 0; }

static double g_manager[(sizeof(CEngineResourceManager) + 7) / 8];
static double g_display[(sizeof(CDisplayManager) + 7) / 8];

int main()
{
    memset(g_manager, 0, sizeof g_manager);
    memset(g_display, 0, sizeof g_display);
    NEngineInternal::GPDisplayManager = (CDisplayManager*)g_display;
    CEngineResourceManager& manager = *(CEngineResourceManager*)g_manager;
    g_version = 0x44b; if (manager.GetIndexBufferUsage() != 24) { printf("00b3b060_TEST FAIL\n"); return 1; }
    g_version = 0x44c; if (manager.GetIndexBufferUsage() != 8) { printf("00b3b060_TEST FAIL\n"); return 1; }
    g_version = 0xffffffffUL; if (manager.GetIndexBufferUsage() != 8) { printf("00b3b060_TEST FAIL\n"); return 1; }
    printf("00b3b060_TEST PASS\n"); return 0;
}
