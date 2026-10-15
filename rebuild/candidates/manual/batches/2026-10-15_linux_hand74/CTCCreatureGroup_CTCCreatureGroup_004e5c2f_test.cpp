// Behaviour test for CTCCreatureGroup::CTCCreatureGroup (0x004E5C2F): base gets the thing, counts zero, ShutdownOnEmpty true, map built.
#include <cstdio>
struct CThing { int m_id; };
struct CTCBase {
    CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs;
    CTCBase(CThing* thing) : ParentThing(thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) {}
    virtual ~CTCBase() {}
};
static int g_mapsBuilt = 0;
struct CGroupMemberMap { unsigned long _Mysize; CGroupMemberMap() : _Mysize(0) { ++g_mapsBuilt; } };
struct CTCCreatureGroup : CTCBase {
    unsigned long MemberCount; unsigned long ActiveMemberCount; bool ShutdownOnEmpty; CGroupMemberMap Members;
    CTCCreatureGroup(CThing* thing) : CTCBase(thing), MemberCount(0), ActiveMemberCount(0), ShutdownOnEmpty(true) {}
};
int main()
{
    CThing t; t.m_id = 2;
    CTCCreatureGroup g(&t);
    if (g.ParentThing != &t || g.MemberCount != 0 || g.ActiveMemberCount != 0 || !g.ShutdownOnEmpty || g_mapsBuilt != 1) { std::printf("004e5c2f_TEST FAIL\n"); return 1; }
    std::printf("004e5c2f_TEST PASS\n");
    return 0;
}
