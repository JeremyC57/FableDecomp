// Behaviour test for 0x0049D550 CWorld::ConvertWorldToStaticMapPath: directory + file name of the world path + the .stb variant.
#include <cstdio>
#include <cstring>
struct CWideString {               // narrow-character stand-in for the test
    char text[96];
    CWideString() { text[0] = 0; }
    CWideString(const char* t) { std::strcpy(text, t); }
};
static CWideString operator+(const CWideString& l, const CWideString& r) { CWideString s(l.text); std::strcat(s.text, r.text); return s; }
static CWideString operator+(const CWideString& l, const char* r) { CWideString s(l.text); std::strcat(s.text, r); return s; }
static void SplitPath(const CWideString& path, CWideString*, CWideString*, CWideString* name, CWideString*)
{
    const char* p = std::strrchr(path.text, '\\'); p = p ? p + 1 : path.text;
    std::strcpy(name->text, p); char* dot = std::strrchr(name->text, '.'); if (dot) *dot = 0;
}
static CWideString GetLevelsDirectory_0041a410() { return CWideString("Data\\Levels\\"); }
struct CWorld {
    CWideString ConvertWorldToStaticMapPath(const CWideString& world, bool xbox, bool runtime) const
    {
        CWideString name;
        SplitPath(world, 0, 0, &name, 0);
        if (runtime) {
            if (xbox) name = GetLevelsDirectory_0041a410() + name + "_RT_XB.stb";
            else name = GetLevelsDirectory_0041a410() + name + "_RT.stb";
        } else {
            if (xbox) name = GetLevelsDirectory_0041a410() + name + "_XB.stb";
            else name = GetLevelsDirectory_0041a410() + name + ".stb";
        }
        return name;
    }
};
int main()
{
    CWorld w; CWideString world("Data\\Levels\\FinalAlbion.wld");
    bool ok = std::strcmp(w.ConvertWorldToStaticMapPath(world, false, false).text, "Data\\Levels\\FinalAlbion.stb") == 0
           && std::strcmp(w.ConvertWorldToStaticMapPath(world, true, false).text, "Data\\Levels\\FinalAlbion_XB.stb") == 0
           && std::strcmp(w.ConvertWorldToStaticMapPath(world, false, true).text, "Data\\Levels\\FinalAlbion_RT.stb") == 0
           && std::strcmp(w.ConvertWorldToStaticMapPath(world, true, true).text, "Data\\Levels\\FinalAlbion_RT_XB.stb") == 0;
    if (!ok) { std::printf("0049d550_TEST FAIL\n"); return 1; }
    std::printf("0049d550_TEST PASS\n");
    return 0;
}