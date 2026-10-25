// Behaviour test for 0x00CB5C90 CScriptInfoManager::AddScript: a non-empty section is registered with the code-section
// manager (short name = the section, long name = the script's name, both bools false) and appended to
// CodeSectionNames; every script is appended to the Scripts map's vector (key = its name) and marks the map dirty.
#include <cstdio>
#include <cstring>

class CCharString {
public:
    const char* PData;
    CCharString(const char* s = 0) : PData(s) {}
    int GetLength() const { return PData ? (int)std::strlen(PData) : 0; }
};
class CScriptInfo { public: void* AllocFunc; void* MasterScriptDataAlloc; CCharString Name; int ID; bool MasterScript; };
struct SPair { CCharString first; CScriptInfo second; SPair() {} SPair(const CCharString& a, const CScriptInfo& b) : first(a), second(b) {} };

class CCodeSectionManager {
public:
    int registered;
    const char* lastShort;
    const char* lastLong;
    bool lastLoad, lastUnload;
    unsigned int RegisterSection(CCharString shortName, CCharString longName, bool loadNow, bool canUnload)
    {
        ++registered; lastShort = shortName.PData; lastLong = longName.PData; lastLoad = loadNow; lastUnload = canUnload;
        return (unsigned int)registered;
    }
};
static CCodeSectionManager g_manager;
static CCodeSectionManager& GetManager() { return g_manager; }

template <class T> class CSmallVector {                    // push_back with a fixed capacity of 4 (overflow grows)
public:
    T  items[8];
    int count, capacity;
    CSmallVector() : count(0), capacity(4) {}
    void push_back(const T& x)
    {
        if (count != capacity) items[count++] = x;
        else { capacity = 8; items[count++] = x; }  // the _M_insert_overflow path
    }
};

class CScriptMap : public CSmallVector<SPair> {
public:
    bool Dirty;
    CScriptMap() : Dirty(false) {}
    void Add(const CCharString& key, const CScriptInfo& value) { push_back(SPair(key, value)); Dirty = true; }
};

class CScriptInfoManager {
public:
    int CurrentScriptID;
    CScriptMap Scripts;
    CSmallVector<CCharString> CodeSectionNames;
    void AddScript(const CScriptInfo& info, CCharString section)
    {
        if (section.GetLength() > 0) {
            GetManager().RegisterSection(section, info.Name, false, false);
            CodeSectionNames.push_back(section);
        }
        Scripts.Add(info.Name, info);
    }
};

int main()
{
    bool ok = true;
    CScriptInfoManager m;
    CScriptInfo a; a.AllocFunc = 0; a.MasterScriptDataAlloc = 0; a.Name = CCharString("Q_Arena"); a.ID = 7; a.MasterScript = true;
    m.AddScript(a, CCharString("ARENA"));
    ok = ok && g_manager.registered == 1 && std::strcmp(g_manager.lastShort, "ARENA") == 0 &&
         std::strcmp(g_manager.lastLong, "Q_Arena") == 0 && !g_manager.lastLoad && !g_manager.lastUnload;
    ok = ok && m.CodeSectionNames.count == 1 && std::strcmp(m.CodeSectionNames.items[0].PData, "ARENA") == 0;
    ok = ok && m.Scripts.count == 1 && m.Scripts.Dirty && m.Scripts.items[0].first.PData == a.Name.PData &&
         m.Scripts.items[0].second.ID == 7 && m.Scripts.items[0].second.MasterScript;
    CScriptInfo b = a; b.Name = CCharString("Q_Bandits"); b.ID = 8;
    m.AddScript(b, CCharString(""));                       // empty section: not registered
    m.AddScript(b, CCharString());                         // NULL section: not registered
    ok = ok && g_manager.registered == 1 && m.CodeSectionNames.count == 1 && m.Scripts.count == 3;
    for (int i = 0; i < 4; ++i)
        m.AddScript(a, CCharString("S"));
    ok = ok && g_manager.registered == 5 && m.CodeSectionNames.count == 5 && m.CodeSectionNames.capacity == 8;
    if (!ok) { std::printf("00cb5c90_TEST FAIL\n"); return 1; }
    std::printf("00cb5c90_TEST PASS\n");
    return 0;
}