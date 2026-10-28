#include <cstdio>
#include <cstring>

// Behaviour model of the CWorld path builders (0x0049D610 / 0x0049D770 / 0x0049DDD0 / 0x0049DEC0):
// name = file name of the world path (SplitPath), then Data\Levels\ + name + extension.
static int g_live = 0;

class CWideString { public:
    char s[260];
    CWideString() { s[0] = 0; ++g_live; }
    CWideString(const char* p) { std::strcpy(s, p); ++g_live; }
    CWideString(const CWideString& o) { std::strcpy(s, o.s); ++g_live; }
    ~CWideString() { --g_live; }
    const CWideString& operator=(const CWideString& o) { std::strcpy(s, o.s); return *this; }
};

static CWideString operator+(const CWideString& a, const CWideString& b) { CWideString r(a); std::strcat(r.s, b.s); return r; }
static CWideString operator+(const CWideString& a, const char* b) { CWideString r(a); std::strcat(r.s, b); return r; }

static void SplitPath(const CWideString& path, CWideString* drive, CWideString* dir, CWideString* name, CWideString* ext)
{
    const char* p = path.s;
    const char* slash = 0;
    for (const char* q = p; *q; ++q) if (*q == '\\' || *q == '/') slash = q;
    const char* base = slash ? slash + 1 : p;
    const char* dot = std::strrchr(base, '.');
    int n = dot ? (int)(dot - base) : (int)std::strlen(base);
    if (name) { std::memcpy(name->s, base, n); name->s[n] = 0; }
    if (drive) drive->s[0] = 0;
    if (dir) dir->s[0] = 0;
    if (ext) ext->s[0] = 0;
}

static CWideString GetLevelsDirectory() { return CWideString("Data\\Levels\\"); }

static bool g_bRuntimeLevelFiles = false;

class CWorld { public:
    int m_dummy;
    CWideString ConvertWorldToStaticMapPath(const CWideString& world, bool xbox, bool runtime) const
    {
        CWideString name;
        SplitPath(world, 0, 0, &name, 0);
        if (runtime) {
            if (xbox) name = GetLevelsDirectory() + name + "_RT_XB.stb";
            else      name = GetLevelsDirectory() + name + "_RT.stb";
        }
        else {
            if (xbox) name = GetLevelsDirectory() + name + "_XB.stb";
            else      name = GetLevelsDirectory() + name + ".stb";
        }
        return name;
    }
    CWideString ConvertWorldToStaticMapPath(const CWideString& world) const { return ConvertWorldToStaticMapPath(world, false, g_bRuntimeLevelFiles); }
    CWideString ConvertWorldToISMThingyPath(const CWideString& world, bool xbox, bool runtime) const
    {
        CWideString name;
        SplitPath(world, 0, 0, &name, 0);
        if (runtime) {
            if (xbox) name = GetLevelsDirectory() + name + "_RT_XB.ism";
            else      name = GetLevelsDirectory() + name + "_RT.ism";
        }
        else {
            if (xbox) name = GetLevelsDirectory() + name + "_XB.ism";
            else      name = GetLevelsDirectory() + name + ".ism";
        }
        return name;
    }
    CWideString ConvertWorldToISMThingyPath(const CWideString& world) const { return ConvertWorldToISMThingyPath(world, false, g_bRuntimeLevelFiles); }
    CWideString ConvertWorldToLevelWADPath(const CWideString& world) const
    {
        CWideString name;
        SplitPath(world, 0, 0, &name, 0);
        name = GetLevelsDirectory() + name + ".wad";
        return name;
    }
    static CWideString ConvertWorldToQuestPath(const CWideString& world)
    {
        CWideString name;
        SplitPath(world, 0, 0, &name, 0);
        name = GetLevelsDirectory() + name + ".qst";
        return name;
    }
};
int main()
{
    CWorld world;
    int live = g_live;
    CWideString path("Data\\Worlds\\FinalAlbion.wld");
    g_bRuntimeLevelFiles = false;
    {
        CWideString r = world.ConvertWorldToStaticMapPath(path);
        if (std::strcmp(r.s, "Data\\Levels\\FinalAlbion.stb") != 0) { std::printf("0049ddd0_TEST FAIL\n"); return 1; }
    }
    g_bRuntimeLevelFiles = true;
    {
        CWideString r = world.ConvertWorldToStaticMapPath(path);
        if (std::strcmp(r.s, "Data\\Levels\\FinalAlbion_RT.stb") != 0) { std::printf("0049ddd0_TEST FAIL\n"); return 1; }
        CWideString bare = world.ConvertWorldToStaticMapPath(CWideString("Hero"));
        if (std::strcmp(bare.s, "Data\\Levels\\Hero_RT.stb") != 0) { std::printf("0049ddd0_TEST FAIL\n"); return 1; }
    }
    if (g_live != live + 1) { std::printf("0049ddd0_TEST FAIL\n"); return 1; }   // every temporary destroyed (only path lives)
    std::printf("0049ddd0_TEST PASS\n");
    return 0;
}