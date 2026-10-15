#include <cstdio>
// Behaviour model of 0x00CD3F00: the base definition manager is built over L"script.bin" with the script callback; the
// file-name temporary is destroyed right after the base constructor.
static int g_live = 0, g_built = 0; static bool g_sawName = false; static void (*g_callback)(int*) = 0;
struct CWideString {
    const unsigned short* m_pData;
    CWideString(const unsigned short* p) : m_pData(p) { ++g_live; }
    CWideString(const CWideString& o) : m_pData(o.m_pData) { ++g_live; }
    ~CWideString() { --g_live; }
};
static void OnScriptDefinitionLoaded(int*) {}
static const unsigned short kScriptBin[] = { 's','c','r','i','p','t','.','b','i','n',0 };
struct CDefinitionManager {
    virtual ~CDefinitionManager() {}
    CDefinitionManager(const CWideString& file, void (*cb)(int*))
    {
        g_sawName = file.m_pData[0] == 's' && file.m_pData[6] == '.' && file.m_pData[10] == 0; g_callback = cb; ++g_built;
    }
};
static CWideString ScriptDefinitionFileName() { return CWideString(kScriptBin); }
struct CScriptDefinitionManager : CDefinitionManager {
    CScriptDefinitionManager() : CDefinitionManager(ScriptDefinitionFileName(), &OnScriptDefinitionLoaded) {}
};
int main()
{
    CScriptDefinitionManager m;
    if (g_built != 1 || !g_sawName || g_callback != &OnScriptDefinitionLoaded || g_live != 0) { std::printf("00cd3f00_TEST FAIL\n"); return 1; }
    std::printf("00cd3f00_TEST PASS\n");
    return 0;
}