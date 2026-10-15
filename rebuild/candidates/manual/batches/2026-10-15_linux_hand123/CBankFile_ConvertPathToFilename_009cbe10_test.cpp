// Behaviour test for CBankFile::ConvertPathToFilename (0x009CBE10): the directory part is dropped,
// name + extension is returned.
#include <cstdio>
#include <cstring>

struct CWideString { char text[64]; CWideString() { text[0] = 0; } CWideString& operator+=(const CWideString& o) { std::strcat(text, o.text); return *this; } };

static void SplitPath(const char* path, CWideString* drive, CWideString* directory, CWideString* name, CWideString* extension)
{
    const char* slash = std::strrchr(path, '\\');
    const char* base = slash ? slash + 1 : path;
    const char* dot = std::strrchr(base, '.');
    if (drive) drive->text[0] = 0;
    if (directory) directory->text[0] = 0;
    if (name) { int n = dot ? (int)(dot - base) : (int)std::strlen(base); std::memcpy(name->text, base, n); name->text[n] = 0; }
    if (extension) std::strcpy(extension->text, dot ? dot : "");
}

static void ConvertPathToFilename(const char* path, char* out)
{
    CWideString name;
    CWideString extension;
    SplitPath(path, 0, 0, &name, &extension);
    name += extension;
    std::strcpy(out, name.text);
}

int main()
{
    char out[64];
    ConvertPathToFilename("Data\\Banks\\textures.big", out);
    if (std::strcmp(out, "textures.big") != 0) { std::printf("009cbe10_TEST FAIL\n"); return 1; }
    ConvertPathToFilename("plain", out);
    if (std::strcmp(out, "plain") != 0) { std::printf("009cbe10_TEST FAIL\n"); return 1; }
    std::printf("009cbe10_TEST PASS\n");
    return 0;
}
