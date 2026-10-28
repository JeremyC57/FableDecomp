// Behaviour test for 0x009976D0 CAFile::SplitPath (CCharString): every non-null output receives its part of the path;
// null outputs are skipped.
#include <cstdio>
#include <cstring>
class CCharString { public: char buf[300]; int assigns; CCharString() : assigns(0) { buf[0] = 0; }
    const CCharString& operator=(const char* t) { std::strcpy(buf, t); ++assigns; return *this; } };
static void FakeSplit(const char* path, char* drive, char* dir, char* fname, char* ext)
{
    std::strncpy(drive, path, 2); drive[2] = 0;
    const char* slash = std::strrchr(path, '\\'); const char* dot = std::strrchr(path, '.');
    unsigned int n = (unsigned int)(slash + 1 - (path + 2)); std::strncpy(dir, path + 2, n); dir[n] = 0;
    n = (unsigned int)(dot - (slash + 1)); std::strncpy(fname, slash + 1, n); fname[n] = 0;
    std::strcpy(ext, dot);
}
static void SplitPath(const char* path, CCharString* drive, CCharString* dir, CCharString* fname, CCharString* ext)
{
    char d[3]; char di[256]; char f[256]; char e[256];
    FakeSplit(path, d, di, f, e);
    if (drive) *drive = d;
    if (dir) *dir = di;
    if (fname) *fname = f;
    if (ext) *ext = e;
}
int main()
{
    int fails = 0;
    CCharString a, b, c, d;
    SplitPath("C:\\Data\\Levels\\Town.wld", &a, &b, &c, &d);
    if (std::strcmp(a.buf, "C:") || std::strcmp(b.buf, "\\Data\\Levels\\") || std::strcmp(c.buf, "Town") ||
        std::strcmp(d.buf, ".wld")) ++fails;
    CCharString e;
    SplitPath("D:\\x\\y.z", 0, 0, &e, 0);
    if (std::strcmp(e.buf, "y") || e.assigns != 1) ++fails;
    std::printf(fails ? "009976d0_TEST FAIL\n" : "009976d0_TEST PASS\n");
    return fails ? 1 : 0;
}