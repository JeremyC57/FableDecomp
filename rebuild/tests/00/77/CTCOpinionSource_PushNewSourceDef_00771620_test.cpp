// Behaviour test for CTCOpinionSource::PushNewSourceDef @ 0x00771620: unknown names (index <= 0) are not pushed.
#include <cstdio>
#include <cstring>
class CCharString { public: const char* text; };
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name)
    { if (std::strcmp(name.text, "OPINION_SOURCE_A") == 0) return 7; if (std::strcmp(name.text, "FULL") == 0) return 9; return 0; }
};
class CWorld { public: CDefinitionManager* DefinitionManager; };
static CDefinitionManager g_defs;
static CWorld g_world = { &g_defs };
class CTCOpinionSource { public:
    long m_stack[4]; int m_count;
    const CWorld* GetWorld() const { return &g_world; }
    bool PushSourceDefIndex(long index) { if (index == 9) return false; m_stack[m_count++] = index; return true; }
    long PushNewSourceDef(const CCharString& name)
    {
        CDefinitionManager* defs = GetWorld()->DefinitionManager;
        long index = defs->GetDefGlobalIndexFromName(name);
        if (index > 0 && PushSourceDefIndex(index)) return 1;
        return 0;
    }
};
int main()
{
    CTCOpinionSource s; s.m_count = 0;
    CCharString a = { "OPINION_SOURCE_A" }, b = { "NOPE" }, c = { "FULL" };
    if (s.PushNewSourceDef(a) != 1 || s.m_count != 1 || s.m_stack[0] != 7) { std::printf("00771620_TEST FAIL\n"); return 1; }
    if (s.PushNewSourceDef(b) != 0 || s.PushNewSourceDef(c) != 0 || s.m_count != 1) { std::printf("00771620_TEST FAIL\n"); return 1; }
    std::printf("00771620_TEST PASS\n");
    return 0;
}