// Behaviour test for 0x004C88E0 CThing::SerialiseThingInfo(bool saving, CCharString& data, CSerialiseInfo& info):
// saving points the persist context at the string; loading parses the string (kept alive in TempCharString) and opens
// the context on the parser; both run the context overload; only a load then reads an optional "DefinitionSubType"
// item into DefinitionType.
#include <cstdio>
#include <cstring>

class CCharString { public:
    char text[64];
    CCharString() { text[0] = 0; }
    CCharString(const char* s) { std::strcpy(text, s); }
    const char* c_str() const { return text; }
};
class CStringParser { public:
    const char* String;
    CCharString TempCharString;
    CStringParser() : String(0) {}
    void Init(const char* t, int, const CCharString&, int) { String = t; }
    void Init(const CCharString& t, const CCharString& filename = CCharString(""))
    {
        TempCharString = t;
        Init(t.c_str(), 0, filename, 0);
    }
    bool SkipToItem(const CCharString& name)
    {
        if (!String) return false;
        const char* p = std::strstr(String, name.c_str());
        if (!p) return false;
        String = p + std::strlen(name.c_str()) + 1;
        return true;
    }
    CCharString ReadAsString()
    {
        CCharString r; int n = 0;
        while (String[n] && String[n] != ';') { r.text[n] = String[n]; ++n; }
        r.text[n] = 0;
        return r;
    }
};
class CPersistContext { public:
    int mode;                       // 1 save, 2 load
    CCharString* saveTo;
    CStringParser* parser;
    void Init(CCharString& data, void*) { mode = 1; saveTo = &data; parser = 0; }
    void Init(CStringParser* p, int, bool, void*) { mode = 2; saveTo = 0; parser = p; }
};
struct CSerialiseInfo { unsigned int UIDLow, UIDHigh; CCharString DefinitionType; signed char Player; };

static int g_sharedCalls = 0, g_lastMode = 0;
static void SerialiseThingInfo(CPersistContext& context, CSerialiseInfo& info)
{
    ++g_sharedCalls; g_lastMode = context.mode;
    if (context.mode == 1) std::strcat(context.saveTo->text, "UID 5;");
    else info.UIDLow = 5;
}
static void SerialiseThingInfo(bool saving, CCharString& data, CSerialiseInfo& info)
{
    CStringParser parser;
    CPersistContext context;
    if (saving)
        context.Init(data, 0);
    else {
        parser.Init(data);
        context.Init(&parser, 0, true, 0);
    }
    SerialiseThingInfo(context, info);
    if (!saving) {
        if (parser.SkipToItem("DefinitionSubType"))
            info.DefinitionType = parser.ReadAsString();
    }
}

int main()
{
    int ok = 1;
    CSerialiseInfo info; info.UIDLow = 0; info.DefinitionType = CCharString("OBJECT_X");
    CCharString saved("");
    SerialiseThingInfo(true, saved, info);
    if (g_sharedCalls != 1 || g_lastMode != 1 || std::strcmp(saved.text, "UID 5;") != 0 ||
        std::strcmp(info.DefinitionType.text, "OBJECT_X") != 0) ok = 0;
    CCharString text("UID 5;DefinitionSubType CREATURE_Y;");
    SerialiseThingInfo(false, text, info);
    if (g_sharedCalls != 2 || g_lastMode != 2 || info.UIDLow != 5 || std::strcmp(info.DefinitionType.text, "CREATURE_Y") != 0) ok = 0;
    CCharString plain("UID 5;");
    SerialiseThingInfo(false, plain, info);                    // no sub-type item: DefinitionType untouched
    if (g_sharedCalls != 3 || std::strcmp(info.DefinitionType.text, "CREATURE_Y") != 0) ok = 0;
    if (!ok) { std::printf("004c88e0_TEST FAIL\n"); return 1; }
    std::printf("004c88e0_TEST PASS\n");
    return 0;
}