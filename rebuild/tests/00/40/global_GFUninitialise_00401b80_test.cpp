#include <cstdio>

// Model of 0x00401B80 GFUninitialise: shut the global services down in a fixed order (init base, file installer
// singleton, bank file manager, system manager, threaded file manager, streaming font cache, primitive manager).
static int g_order[16];
static int g_count = 0;
static void Mark(int id) { g_order[g_count++] = id; }
struct CInitBaseClass { void Uninitialise() { Mark(1); } };
struct CBankFileManager { void Close() { Mark(3); } };
struct CSystemManager { void Uninitialise() { Mark(4); } };
struct CThreadedFileManager { void Uninitialise() { Mark(5); } };
struct CEnginePrimitiveManager { void Uninitialise() { Mark(7); } };
static CInitBaseClass g_InitBase;
static CBankFileManager g_BankFileManager;
static CThreadedFileManager g_ThreadedFileManager;
static CSystemManager g_SystemManager;
static CEnginePrimitiveManager g_PrimitiveManager;
static void FileInstallerSingletonRelease() { Mark(2); }
static CSystemManager* GFGetSystemManager() { return &g_SystemManager; }
static void UninitializeStreamingFontTextureCache() { Mark(6); }
static CEnginePrimitiveManager* GetEnginePrimitiveManager() { return &g_PrimitiveManager; }

static void GFUninitialise()
{
    g_InitBase.Uninitialise();
    FileInstallerSingletonRelease();
    g_BankFileManager.Close();
    GFGetSystemManager()->Uninitialise();
    g_ThreadedFileManager.Uninitialise();
    UninitializeStreamingFontTextureCache();
    GetEnginePrimitiveManager()->Uninitialise();
}

int main()
{
    GFUninitialise();
    if (g_count != 7) {
        std::printf("00401b80_TEST FAIL\n");
        return 1;
    }
    for (int i = 0; i < 7; ++i)
        if (g_order[i] != i + 1) {
            std::printf("00401b80_TEST FAIL\n");
            return 1;
        }
    std::printf("00401b80_TEST PASS\n");
    return 0;
}