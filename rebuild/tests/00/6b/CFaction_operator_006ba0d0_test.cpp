// Behaviour test for 0x006BA0D0 CFaction::operator= (implicit: name, then the allied and enemy sets).
#include <cstdio>
#include <cstring>

class CCharString {
public:
    const char* PData;
    static int s_copies;
    static int s_assigns;
    CCharString(const char* s) : PData(s) {}
    CCharString(const CCharString& o) : PData(o.PData) { ++s_copies; }
    const CCharString& operator=(const CCharString& o) { PData = o.PData; ++s_assigns; return *this; }
};
int CCharString::s_copies = 0;
int CCharString::s_assigns = 0;
static bool Same(const CCharString& s, const char* t) { return std::strcmp(s.PData, t) == 0; }

class CNameSet {                    // stands in for set<CCharString>: assignment replaces the contents
public:
    const char* Items[4];
    int Count;
    static int s_assigns;
    CNameSet() : Count(0) {}
    void Add(const char* s) { Items[Count++] = s; }
    CNameSet& operator=(const CNameSet& o)
    {
        Count = o.Count;
        for (int i = 0; i < Count; ++i) Items[i] = o.Items[i];
        ++s_assigns;
        return *this;
    }
};
int CNameSet::s_assigns = 0;

class CFaction {
public:
    CCharString Name;
    CNameSet    AlliedFactions;
    CNameSet    EnemyFactions;
    CFaction(const char* n) : Name(n) {}
};

int main()
{
    CFaction a("NONE");
    a.EnemyFactions.Add("X"); a.EnemyFactions.Add("Y");
    CFaction b("GUARDS");
    b.AlliedFactions.Add("TOWNSFOLK"); b.AlliedFactions.Add("HERO");
    b.EnemyFactions.Add("BANDITS");
    CCharString::s_assigns = 0;
    CFaction* r = &(a = b);
    if (!(r == &a && CCharString::s_assigns == 1 && CNameSet::s_assigns == 2 && Same(a.Name, "GUARDS") && a.AlliedFactions.Count == 2 && std::strcmp(a.AlliedFactions.Items[1], "HERO") == 0 && a.EnemyFactions.Count == 1 && std::strcmp(a.EnemyFactions.Items[0], "BANDITS") == 0)) {
        std::printf("006ba0d0_TEST FAIL\n");
        return 1;
    }
    std::printf("006ba0d0_TEST PASS\n");
    return 0;
}