#include <cstdio>
// Behaviour test: DoEULAThings over stand-in loader functions: 0 on load/lookup failure or refusal, 1 (and a free) on acceptance.
static int g_loadOk = 1, g_procOk = 1, g_result = 1, g_freed = 0, g_strings = 0;
static const char* g_lastKey = 0;
static void* LoadLibraryW(const unsigned short*) { static int lib; return g_loadOk ? &lib : 0; }
typedef int (*EBUEulaProc)(const char*, const char*, const char*, int);
static int FakeEula(const char* key, const char*, const char*, int first) { g_lastKey = key; return first == 1 ? g_result : -1; }
static void* GetProcAddress(void*, const char*) { return g_procOk ? (void*)FakeEula : 0; }
static int FreeLibrary(void*) { ++g_freed; return 1; }
struct CCharStringData { const char* Text; unsigned int Length; };
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(CCharStringData* d) : PStringData(d) { ++g_strings; }
    CCharString(const CCharString& o) : PStringData(o.PStringData) { ++g_strings; }
    ~CCharString() { --g_strings; }
    const char* GetString() const { return PStringData == 0 ? "" : PStringData->Text; }
};
static CCharStringData g_keyData = { "Software\\Fable", 14 };
struct CWideString { CCharString ConvertToCharString() const { return CCharString(&g_keyData); } };
static CWideString g_RegistryKeyLocation;
static const char* g_EULAFileName = "eula.txt";
static const char* g_WarrantyFileName = "warranty.txt";
static int DoEULAThings()
{
    void* library = LoadLibraryW((const unsigned short*)0);
    if (library == 0)
        return 0;
    EBUEulaProc eula = (EBUEulaProc)GetProcAddress(library, "EBUEula");
    if (eula != 0)
    {
        bool accepted = eula(g_RegistryKeyLocation.ConvertToCharString().GetString(), g_EULAFileName, g_WarrantyFileName, 1) == 1;
        if (accepted)
        {
            FreeLibrary(library);
            return 1;
        }
    }
    return 0;
}
int main()
{
    if (DoEULAThings() != 1 || g_freed != 1 || g_strings != 0 || g_lastKey != g_keyData.Text) { std::printf("00401fe0_TEST FAIL\n"); return 1; }
    g_result = 0;
    if (DoEULAThings() != 0 || g_freed != 1) { std::printf("00401fe0_TEST FAIL\n"); return 1; }
    g_result = 1; g_procOk = 0;
    if (DoEULAThings() != 0 || g_freed != 1) { std::printf("00401fe0_TEST FAIL\n"); return 1; }
    g_procOk = 1; g_loadOk = 0;
    if (DoEULAThings() != 0) { std::printf("00401fe0_TEST FAIL\n"); return 1; }
    std::printf("00401fe0_TEST PASS\n");
    return 0;
}