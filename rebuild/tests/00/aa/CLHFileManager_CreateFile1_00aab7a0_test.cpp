#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SFileSpec { int x; }; struct CLHFileManager; static void* g_manager = 0; static const SFileSpec* g_spec = 0;
struct CLHFile { unsigned char _body[0x14]; CLHFile(CLHFileManager* m, const SFileSpec& s) { g_manager = m; g_spec = &s; } };
struct CLHFileManager { virtual CLHFile* CreateFile0(const SFileSpec& s) { return new CLHFile(this, s); } virtual CLHFile* CreateFile1(const SFileSpec& s) { return new CLHFile(this, s); } };

int main()
{
    CLHFileManager m; SFileSpec spec; spec.x = 1; CLHFile* f = m.CreateFile1(spec); if (!f || g_manager != &m || g_spec != &spec) { std::printf("00aab7a0_TEST FAIL\n"); return 1; } delete f;
    std::printf("00aab7a0_TEST PASS\n");
    return 0;
}