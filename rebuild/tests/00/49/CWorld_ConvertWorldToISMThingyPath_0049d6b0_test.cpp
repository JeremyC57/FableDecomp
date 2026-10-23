// Behaviour test for 0x0049D6B0 CWorld::ConvertWorldToISMThingyPath: directory + file name of the world path + the .ism variant.
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
    CWideString ConvertWorldToISMThingyPath(const CWideString& world, bool xbox, bool runtime) const
    {
        CWideString name;
        SplitPath(world, 0, 0, &name, 0);
        if (runtime) {
            if (xbox) name = GetLevelsDirectory_0041a410() + name + "_RT_XB.ism";
            else name = GetLevelsDirectory_0041a410() + name + "_RT.ism";
        } else {
            if (xbox) name = GetLevelsDirectory_0041a410() + name + "_XB.ism";
            else name = GetLevelsDirectory_0041a410() + name + ".ism";
        }
        return name;
    }
};
int main()
{
    CWorld w; CWideString world("Data\\Levels\\FinalAlbion.wld");
    bool ok = std::strcmp(w.ConvertWorldToISMThingyPath(world, false, false).text, "Data\\Levels\\FinalAlbion.ism") == 0
           && std::strcmp(w.ConvertWorldToISMThingyPath(world, true, false).text, "Data\\Levels\\FinalAlbion_XB.ism") == 0
           && std::strcmp(w.ConvertWorldToISMThingyPath(world, false, true).text, "Data\\Levels\\FinalAlbion_RT.ism") == 0
           && std::strcmp(w.ConvertWorldToISMThingyPath(world, true, true).text, "Data\\Levels\\FinalAlbion_RT_XB.ism") == 0;
    if (!ok) { std::printf("0049d6b0_TEST FAIL\n"); return 1; }
    std::printf("0049d6b0_TEST PASS\n");
    return 0;
}