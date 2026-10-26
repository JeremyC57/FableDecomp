#include <cstdio>
class CAFile { public: int id; };
class CASuspendableProcess;
static long g_count = 0;
static CAFile* g_loadedFile = 0; static CASuspendableProcess* g_loadedProcess = (CASuspendableProcess*)1; static long g_loadedLength = 0;
static int g_unassigned = 0;
class CCharString {
public:
    void* PStringData;
    CCharString(CAFile& file, long length) : PStringData(0)
    {
        ++g_count;
        if (PStringData != 0) UnassignString();
        LoadFromFile(file, 0, length);
    }
    void UnassignString() { ++g_unassigned; }
    void LoadFromFile(CAFile& file, CASuspendableProcess* process, long length) { g_loadedFile = &file; g_loadedProcess = process; g_loadedLength = length; }
};
int main()
{
    CAFile file; file.id = 7;
    CCharString s(file, 123);
    if (g_count != 1 || g_unassigned != 0 || g_loadedFile != &file || g_loadedProcess != 0 || g_loadedLength != 123 || s.PStringData != 0) { std::printf("0099fa50_TEST FAIL\n"); return 1; }
    CCharString t(file, -1);
    if (g_count != 2 || g_loadedLength != -1) { std::printf("0099fa50_TEST FAIL\n"); return 1; }
    std::printf("0099fa50_TEST PASS\n"); return 0;
}