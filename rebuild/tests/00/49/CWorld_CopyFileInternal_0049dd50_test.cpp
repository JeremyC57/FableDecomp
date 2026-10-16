#include <cstdio>
#include <cstring>
static char g_log[32]; static int g_n = 0;
struct File { int access; int flags; const char* name; File(const char* n, int a, int f) : access(a), flags(f), name(n) { g_log[g_n++] = (char)('0' + a); } ~File() { g_log[g_n++] = name[0]; } };
static void CopyFile(File& s, File& d) { if (s.access == 1 && d.access == 4 && s.flags == 2) g_log[g_n++] = 'C'; }
static void CopyFileInternal(const char* src, const char* dst) { File s(src, 1, 2); File d(dst, 4, 2); CopyFile(s, d); }
static bool Run() { CopyFileInternal("src", "dst"); g_log[g_n] = 0; return std::strcmp(g_log, "14Cds") == 0; }
int main()
{
    if (!Run()) { std::printf("0049dd50_TEST FAIL\n"); return 1; }
    std::printf("0049dd50_TEST PASS\n");
    return 0;
}