#include <cstdio>
// Behaviour test for 00433693: the startup manager builds its CDefinitionManager base over the given file with the
// startup (no-op) error callback and ends up as the derived dynamic type.
class CWideString { public: int m_id; };
class CCharString { public: int m_id; };
static const CWideString* g_baseFile = 0;
class CDefinitionManager {
public:
    typedef void (*TErrorCallback)(CCharString* message);
    TErrorCallback m_errorCallback;
    CDefinitionManager(const CWideString& definitionsFile, TErrorCallback errorCallback)
        : m_errorCallback(errorCallback) { g_baseFile = &definitionsFile; }
    virtual ~CDefinitionManager() {}
    virtual int Kind() const { return 1; }
};
class CStartupDefinitionManager : public CDefinitionManager {
public:
    static void StartupErrorCallback(CCharString* message) { (void)message; }
    CStartupDefinitionManager(const CWideString& definitionsFile);
    virtual ~CStartupDefinitionManager() {}
    virtual int Kind() const { return 2; }
};
CStartupDefinitionManager::CStartupDefinitionManager(const CWideString& definitionsFile)
    : CDefinitionManager(definitionsFile, StartupErrorCallback)
{
}
int main()
{
    CWideString file; file.m_id = 7;
    CStartupDefinitionManager m(file);
    CDefinitionManager* base = &m;
    CCharString msg; msg.m_id = 1;
    if (g_baseFile != &file || base->m_errorCallback != &CStartupDefinitionManager::StartupErrorCallback || base->Kind() != 2) {
        std::printf("00433693_TEST FAIL\n"); return 1;
    }
    base->m_errorCallback(&msg);
    std::printf("00433693_TEST PASS\n");
    return 0;
}