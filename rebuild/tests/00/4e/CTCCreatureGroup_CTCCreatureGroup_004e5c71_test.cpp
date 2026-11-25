// Behaviour model of ~CTCCreatureGroup 0x004E5C71 (implicit destructor): the Members map (+0x18) is destroyed first, then the CTCBase
// base destructor runs; nothing else is touched.
#include <cstdio>
#include <new>
static int g_order = 0;
static const void* g_members_this = 0;
static const void* g_base_this = 0;
class CTCBase { public:
    void* Owner; void* Next;
    CTCBase() : Owner(0), Next(0) {}
    virtual ~CTCBase() { g_order = g_order * 10 + 2; g_base_this = this; }
};
struct CGroupMemberMap { void* Head; void* Reserved; unsigned long Size;
    ~CGroupMemberMap() { g_order = g_order * 10 + 1; g_members_this = this; } };
class CTCCreatureGroup : public CTCBase { public:
    unsigned long MemberCount; unsigned long ActiveMemberCount; bool ShutdownOnEmpty;
    CGroupMemberMap Members;
};
int main()
{
    char buf[sizeof(CTCCreatureGroup)];
    CTCCreatureGroup* group = new (buf) CTCCreatureGroup;
    group->MemberCount = 7;
    group->~CTCCreatureGroup();
    if (g_order != 12) { std::printf("BAD_ORDER %d\n", g_order); return 1; }
    if (g_members_this != &group->Members) { std::printf("BAD_MEMBERS\n"); return 1; }
    if (g_base_this != (CTCBase*)group) { std::printf("BAD_BASE\n"); return 1; }
    if (sizeof(void*) == 4 && (const char*)g_members_this - (const char*)group != 0x18) { std::printf("BAD_OFFSET\n"); return 1; }
    std::printf("DESTVAL_004e5c71_OK\n");
    return 0;
}
