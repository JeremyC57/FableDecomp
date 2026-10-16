// Behaviour test for 0x00997800: ReplacePathExtension: drive + directory + name of the path with the new extension
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)

struct W { char s[64]; };
static void Split(const char* p, W* drive, W* dir, W* name) { const char* colon = std::strchr(p, ':'); const char* start = colon ? colon + 1 : p; drive->s[0] = 0; if (colon) { std::strncpy(drive->s, p, colon - p + 1); drive->s[colon - p + 1] = 0; } const char* slash = std::strrchr(start, '\\'); const char* file = slash ? slash + 1 : start; std::strncpy(dir->s, start, file - start); dir->s[file - start] = 0; const char* dot = std::strrchr(file, '.'); int n = dot ? (int)(dot - file) : (int)std::strlen(file); std::strncpy(name->s, file, n); name->s[n] = 0; }
static W Make(const W& drive, const W& dir, const W& name, const char* ext) { W r; std::strcpy(r.s, drive.s); std::strcat(r.s, dir.s); std::strcat(r.s, name.s); std::strcat(r.s, ext); return r; }
static W Replace(const char* path, const char* ext) { W drive, dir, name; Split(path, &drive, &dir, &name); return Make(drive, dir, name, ext); }
static void run() { W r = Replace("C:\\data\\levels\\map.wld", ".lev"); CHECK(!std::strcmp(r.s, "C:\\data\\levels\\map.lev")); W s = Replace("file", ".txt"); CHECK(!std::strcmp(s.s, "file.txt")); }

int main()
{
    run();
    if (g_fail) { std::printf("00997800_TEST FAIL\n"); return 1; }
    std::printf("00997800_TEST PASS\n");
    return 0;
}
