// Test for CCRCSymbolMap::LoadFromFile(const CWideString&, const CCharString&) -- clear the map, remember the file name, read the whole
// file into a zero-terminated buffer, strip // and /* */ comments, parse, re-sort, forget the file name; true when the parse succeeded.
#include <cstdio>
#include <cstring>
static const char* g_fileText = "ENUM_A = 1; // first\r\n/* block */ENUM_B = 2;\n";
static char g_current[64];
class CCRCSymbolMap { public:
    int cleared, updated; char parsed[128];
    CCRCSymbolMap() : cleared(0), updated(0) { parsed[0] = 0; }
    void Clear() { ++cleared; }
    void UpdateDirty() { ++updated; }
    bool ParseBuffer(const char* buffer) { std::strcpy(parsed, buffer); return std::strstr(buffer, "ENUM_B") != 0; }
    bool LoadFromFile(const char* filename);
};
static void GFRemoveComments(char* text, const char* open, const char* close)
{
    char* s;
    while ((s = std::strstr(text, open)) != 0) {
        char* e = std::strstr(s + std::strlen(open), close);
        if (!e) { *s = 0; return; }
        if (std::strcmp(open, "/*") == 0) e += std::strlen(close);
        std::memmove(s, e, std::strlen(e) + 1);
    }
}
bool CCRCSymbolMap::LoadFromFile(const char* filename)
{
    bool result = false;
    Clear();
    std::strcpy(g_current, filename);
    unsigned int length = (unsigned int)std::strlen(g_fileText);
    char buffer[128];
    buffer[length] = 0;
    std::memcpy(buffer, g_fileText, length);
    GFRemoveComments(buffer, "//", "\n");
    GFRemoveComments(buffer, "//", "\r");
    GFRemoveComments(buffer, "/*", "*/");
    if (ParseBuffer(buffer))
        result = true;
    UpdateDirty();
    g_current[0] = 0;
    return result;
}
int main()
{
    CCRCSymbolMap map;
    bool ok = map.LoadFromFile("symbols.h");
    if (!ok || map.cleared != 1 || map.updated != 1 || g_current[0] != 0 || std::strstr(map.parsed, "first") || std::strstr(map.parsed, "block"))
    { std::printf("00a39010_TEST FAIL\n"); return 1; }
    std::printf("00a39010_TEST PASS\n");
    return 0;
}