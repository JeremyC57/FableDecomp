// behaviour test for vector<CMapInfo>::resize(n) (0x0051BE20): forwards (n, default CMapInfo) and destroys the temporary
#include <cstdio>
static int g_strings = 0, g_clears = 0;
struct CCharString { void* m_pData; CCharString() : m_pData(0) { ++g_strings; } ~CCharString() { --g_strings; } };
struct CCountedPointer { void* Data; void* m_pCount; CCountedPointer() : Data(0), m_pCount(0) {} };
struct CMapInfo {
    int Area[4]; CCountedPointer PMap; CCharString LevelName, LevelScriptName; unsigned int MapUID; bool Used; CCharString SerialiseScript; bool IsSea;
    CMapInfo() { Clear(); }
    void Clear() { ++g_clears; MapUID = 0; Used = false; IsSea = false; }
};
static unsigned int g_n = 0; static bool g_ok = false;
struct CMapInfoVector {
    void resize(unsigned int n, const CMapInfo& v) { g_n = n; g_ok = v.PMap.Data == 0 && v.PMap.m_pCount == 0 && v.MapUID == 0 && g_strings == 3; }
    void resize(unsigned int n) { resize(n, CMapInfo()); }
};
int main()
{
    CMapInfoVector v; v.resize(7);
    if (g_n != 7 || !g_ok || g_strings != 0 || g_clears != 1) { std::printf("0051be20_TEST FAIL\n"); return 1; }
    std::printf("0051be20_TEST PASS\n");
    return 0;
}
