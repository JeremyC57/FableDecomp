// Behaviour test for 0x006B9460 CFaction::IsAlliedWithFaction(const CFaction&) const.
#include <cstdio>
#include <cstring>

class CCharStringData { public: const char* m_pData; unsigned int m_length; };

static int Compare(const char* a, const char* b)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0) return 0;
        char cb = *b;
        if (ca < cb) return -1;
        if (ca > cb) return 1;
        ++a; ++b;
    }
}

class CCharString { public:
    CCharStringData* PStringData;
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData) return true;
        if (PStringData && other.PStringData && PStringData->m_length == other.PStringData->m_length)
            return Compare(PStringData->m_pData, other.PStringData->m_pData) ? false : true;
        return false;
    }
};

struct Node { const char* key; };

struct CCharStringSet {                                 // stand-in: a flat array searched by name; header = "not found"
    Node* _M_header;
    Node* nodes;
    int   n;
    Node* _M_find(const CCharString& k) const
    {
        for (int i = 0; i < n; ++i)
            if (k.PStringData && std::strcmp(nodes[i].key, k.PStringData->m_pData) == 0) return &nodes[i];
        return _M_header;
    }
    unsigned int count(const CCharString& k) const { return _M_find(k) == _M_header ? 0 : 1; }
};

class CFaction { public:
    CCharString    Name;
    CCharStringSet AlliedFactions;
    bool IsAlliedWithFaction(const CFaction& faction) const
    {
        return faction.Name == Name || AlliedFactions.count(faction.Name) != 0;
    }
};

int main()
{
    CCharStringData hero = { "HERO", 4 }, hero2 = { "HERO", 4 }, guards = { "GUARDS", 6 }, bandits = { "BANDITS", 7 }, heron = { "HERON", 5 };
    Node header = { "" };
    Node allies[1] = { { "GUARDS" } };
    CFaction a; a.Name.PStringData = &hero; a.AlliedFactions._M_header = &header; a.AlliedFactions.nodes = allies; a.AlliedFactions.n = 1;
    CFaction b; b.Name.PStringData = &hero; b.AlliedFactions = a.AlliedFactions;        // same block
    CFaction c; c.Name.PStringData = &hero2; c.AlliedFactions = a.AlliedFactions;       // equal text, other block
    CFaction d; d.Name.PStringData = &guards; d.AlliedFactions = a.AlliedFactions;
    CFaction e; e.Name.PStringData = &bandits; e.AlliedFactions = a.AlliedFactions;
    CFaction f; f.Name.PStringData = &heron; f.AlliedFactions = a.AlliedFactions;
    CFaction g; g.Name.PStringData = 0; g.AlliedFactions = a.AlliedFactions;
    int ok = a.IsAlliedWithFaction(b) && a.IsAlliedWithFaction(c) && a.IsAlliedWithFaction(d) &&
             !a.IsAlliedWithFaction(e) && !a.IsAlliedWithFaction(f) && !a.IsAlliedWithFaction(g);
    if (!ok) { std::printf("006b9460_TEST FAIL\n"); return 1; }
    std::printf("006b9460_TEST PASS\n");
    return 0;
}