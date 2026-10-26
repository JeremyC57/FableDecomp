// Behaviour test for CBankFile::EndUpdateEntries (0x009CEA70): retail mode only closes; otherwise the
// file information is written, an existing recovery file deleted and the flags cleared.
#include <cstdio>

static int g_closes = 0, g_infos = 0, g_deletes = 0; static bool g_recoveryExists = false;
class CBankFile { public:
    bool RetailMode, Solid, UpdatingFlag;
    void WriteFileInformation(bool) { ++g_infos; }
    void EndUpdateEntries()
    {
        if (RetailMode) { ++g_closes; UpdatingFlag = false; return; }
        WriteFileInformation(true);
        {
            if (g_recoveryExists) { ++g_deletes; g_recoveryExists = false; }
        }
        Solid = false;
        UpdatingFlag = false;
    }
};

int main()
{
    CBankFile r = { true, true, true };
    r.EndUpdateEntries();
    if (g_closes != 1 || g_infos != 0 || r.UpdatingFlag || !r.Solid) { std::printf("009cea70_TEST FAIL\n"); return 1; }
    CBankFile b = { false, true, true };
    g_recoveryExists = true;
    b.EndUpdateEntries();
    if (g_infos != 1 || g_deletes != 1 || b.Solid || b.UpdatingFlag) { std::printf("009cea70_TEST FAIL\n"); return 1; }
    b.EndUpdateEntries();
    if (g_infos != 2 || g_deletes != 1) { std::printf("009cea70_TEST FAIL\n"); return 1; }
    std::printf("009cea70_TEST PASS\n");
    return 0;
}