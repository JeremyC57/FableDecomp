// Behaviour test for 0x009F0500 CCodeSectionManager::GetHandleFromLongName
#include <cstdio>
#include <cstring>
static int g_live = 0;
struct CCharString {
    const char* PStringData;
    CCharString() : PStringData("") { ++g_live; }
    CCharString(const char* s) : PStringData(s) { ++g_live; }
    CCharString(const CCharString& o) : PStringData(o.PStringData) { ++g_live; }
    ~CCharString() { --g_live; }
};
struct Node { CCharString first; unsigned long second; };
struct CRetailMap {
    Node* nodes[3];
    long count;
    Node header;
    Node* find(const CCharString& key)
    {
        for (long i = 0; i < count; ++i)
            if (std::strcmp(nodes[i]->first.PStringData, key.PStringData) == 0) return nodes[i];
        return &header;
    }
    Node* end() { return &header; }
};
struct CCodeSectionManager {
    CRetailMap LongNameToHandle;
    unsigned long GetHandleFromLongName(CCharString longName)
    {
        Node* it = LongNameToHandle.find(longName);
        if (it == LongNameToHandle.end())
            return 0xFFFFFFFF;
        return it->second;
    }
};
int main()
{
    Node a = { CCharString("Albion"), 3 };
    Node b = { CCharString("Oakvale"), 7 };
    Node h = { CCharString(""), 0 };
    CCodeSectionManager m;
    m.LongNameToHandle.nodes[0] = &a; m.LongNameToHandle.nodes[1] = &b; m.LongNameToHandle.count = 2;
    m.LongNameToHandle.header = h;
    int before = g_live;
    int ok = m.GetHandleFromLongName(CCharString("Oakvale")) == 7;
    ok = ok && m.GetHandleFromLongName(CCharString("Albion")) == 3;
    ok = ok && m.GetHandleFromLongName(CCharString("Bowerstone")) == 0xFFFFFFFF;
    ok = ok && g_live == before;
    if (!ok) { std::printf("009f0500_TEST FAIL\n"); return 1; }
    std::printf("009f0500_TEST PASS\n");
    return 0;
}