// Behaviour test for 0x004C8A30 CThing::GetSubTypeFromScript(const char*): the script is parsed up to "EndThing", the
// body loaded through SerialiseThingInfo, and the definition type mapped to its global index -- 0 for "NULL"; an empty
// (null-data) type is looked up like any other name (the inline operator== null arm compares against "").
#include <cstdio>
#include <cstring>

class CCharString { public:
    const char* PStringData;        // null = the empty string, as CCharString's null data block
    char buffer[64];
    CCharString() : PStringData(0) { buffer[0] = 0; }
    CCharString(const char* s) : PStringData(0) { Set(s); }
    CCharString(const CCharString& o) : PStringData(0) { if (o.PStringData) Set(o.PStringData); }
    CCharString& operator=(const CCharString& o) { if (o.PStringData) Set(o.PStringData); else PStringData = 0; return *this; }
    void Set(const char* s) { std::strcpy(buffer, s); PStringData = buffer; }
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (std::strcmp(text, "") == 0)
                return true;
            return false;
        }
        return std::strcmp(PStringData, text) == 0;
    }
};
class CStringParser { public:
    const char* String;
    CStringParser(const char* text) : String(text) {}
    CCharString ReadAsStringUntilString(const CCharString& end)
    {
        CCharString r;
        const char* p = std::strstr(String, end.PStringData);
        int n = p ? (int)(p - String) : (int)std::strlen(String);
        std::strncpy(r.buffer, String, n); r.buffer[n] = 0; r.PStringData = r.buffer;
        return r;
    }
};
struct CSerialiseInfo { unsigned int UIDLow, UIDHigh; CCharString DefinitionType; signed char Player; };
static int g_lookups = 0;
class CDefinitionManager { public:
    int GetDefGlobalIndexFromName(const CCharString& name) const
    {
        ++g_lookups;
        if (name.PStringData == 0) return -1;
        return (int)std::strlen(name.PStringData) * 10;
    }
};
static CDefinitionManager g_defs;
class CThingManager { public: CDefinitionManager* DefinitionManager; };
static CThingManager g_manager = { &g_defs };
static CThingManager* g_pThingManager = &g_manager;

static void SerialiseThingInfo(bool, CCharString& data, CSerialiseInfo& info)   // reads "TYPE <name>"
{
    const char* p = std::strstr(data.buffer, "TYPE ");
    if (p) { char t[64]; std::sscanf(p + 5, "%63s", t); info.DefinitionType = CCharString(t); }
}
static int GetSubTypeFromScript(const char* script)
{
    CStringParser parser(script);
    CCharString body = parser.ReadAsStringUntilString("EndThing");
    CSerialiseInfo info;
    SerialiseThingInfo(false, body, info);
    if (info.DefinitionType == "NULL")
        return 0;
    const CDefinitionManager* defs = g_pThingManager->DefinitionManager;
    return defs->GetDefGlobalIndexFromName(info.DefinitionType);
}

int main()
{
    int ok = 1;
    if (GetSubTypeFromScript("TYPE OBJECT_ABC EndThing TYPE IGNORED") != 100 || g_lookups != 1) ok = 0;
    if (GetSubTypeFromScript("TYPE NULL EndThing") != 0 || g_lookups != 1) ok = 0;
    if (GetSubTypeFromScript("NOTHING EndThing") != -1 || g_lookups != 2) ok = 0;
    if (!ok) { std::printf("004c8a30_TEST FAIL\n"); return 1; }
    std::printf("004c8a30_TEST PASS\n");
    return 0;
}