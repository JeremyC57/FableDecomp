// Behaviour test for CTCAtmosPlayer::OnSerialise (0x007E36A0).
#include <cstdio>
#include <cstring>

static int g_strings = 0;
struct CCharString {
    char Text[32];
    CCharString() { Text[0] = 0; ++g_strings; }
    CCharString(const CCharString& o) { std::strcpy(Text, o.Text); ++g_strings; }
    ~CCharString() { --g_strings; }
};
inline CCharString EmptyString() { CCharString empty; return empty; }
struct CDefString { long m_tablePos; CDefString() : m_tablePos(0) {} };
struct CDefinitionManager {
    static CDefinitionManager& Get() { static CDefinitionManager m; return m; }
    CDefString GetDefNameFromGlobalIndex(long index) { CDefString s; s.m_tablePos = index * 10; return s; }
    long GetDefGlobalIndexFromName(const CCharString& name) { return std::strcmp(name.Text, "ATMOS_CAVE") == 0 ? 7 : -5; }
};
static long g_savedLong = 0, g_longDefault = 0;
static const char* g_lastName = 0;
static bool g_stringDefaultEmpty = false;
struct CPersistContext {
    long Mode;
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void Transfer(const char* name, long& value, const long& defaultValue) { g_lastName = name; g_savedLong = value; g_longDefault = defaultValue; }
    void Transfer(const char* name, CCharString& value, const CCharString& defaultValue)
    { g_lastName = name; g_stringDefaultEmpty = defaultValue.Text[0] == 0; std::strcpy(value.Text, "ATMOS_CAVE"); }
};
class CTCAtmosPlayer {
public:
    void* PSoundEmitter; long AtmosDefIndex;
    void OnSerialise(CPersistContext& context, bool)
    {
        if (context.IsSaving()) {
            CDefString name = CDefinitionManager::Get().GetDefNameFromGlobalIndex(AtmosDefIndex);
            context.Transfer("AtmosName", name.m_tablePos, -1);
        } else {
            CCharString name;
            context.Transfer("AtmosName", name, EmptyString());
            AtmosDefIndex = CDefinitionManager::Get().GetDefGlobalIndexFromName(name);
        }
    }
};

int main()
{
    CTCAtmosPlayer p; p.PSoundEmitter = 0; p.AtmosDefIndex = 3;
    CPersistContext save; save.Mode = 1;
    p.OnSerialise(save, false);
    bool ok = g_savedLong == 30 && g_longDefault == -1 && std::strcmp(g_lastName, "AtmosName") == 0 && p.AtmosDefIndex == 3;
    CPersistContext load; load.Mode = 0;
    p.OnSerialise(load, false);
    ok = ok && p.AtmosDefIndex == 7 && g_stringDefaultEmpty && g_strings == 0;
    if (!ok) { std::printf("007e36a0_TEST FAIL\n"); return 1; }
    std::printf("007e36a0_TEST PASS\n");
    return 0;
}