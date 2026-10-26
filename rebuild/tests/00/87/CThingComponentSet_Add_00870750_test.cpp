#include <cstdio>
#include <cstring>

class CCharString { public: const char* m_text; };
class CDefString { public: long TablePos; CDefString(const CCharString& n) : TablePos((long)std::strlen(n.m_text)) { } };
struct CRegisteredTCInfo { void* m_pfnAllocate; unsigned long m_type; CCharString m_name; };
static CRegisteredTCInfo g_infos[2];
static const CRegisteredTCInfo* FindRegisteredTCInfo(const CCharString& name)
{
    for (int i = 0; i < 2; ++i) if (std::strcmp(g_infos[i].m_name.m_text, name.m_text) == 0) return &g_infos[i];
    return 0;
}
struct CEntry {
    CDefString Name; unsigned long Type; bool Flag;
    CEntry(const CCharString& name, unsigned long type, bool flag) : Name(name), Type(type), Flag(flag) { }
};
struct CSmallVector_CEntry {
    long m_storage[8 * 3]; signed char Size;
    void push_back(const CEntry& e) { long* s = &m_storage[Size * 3]; s[0] = e.Name.TablePos; s[1] = (long)e.Type; s[2] = e.Flag ? 1 : 0; ++Size; }
};
class CThingComponentSet { public:
    CSmallVector_CEntry Entries;
    void Add(const CCharString& name, bool flag)
    {
        const CRegisteredTCInfo* info = FindRegisteredTCInfo(name);
        if (info)
            Entries.push_back(CEntry(info->m_name, info->m_type, flag));
    }
};
int main()
{
    g_infos[0].m_name.m_text = "CTCPhysicsStandard"; g_infos[0].m_type = 3;
    g_infos[1].m_name.m_text = "CTCAmbientBird"; g_infos[1].m_type = 9;
    CThingComponentSet set; set.Entries.Size = 0;
    CCharString bird = { "CTCAmbientBird" }; CCharString missing = { "CTCNothing" };
    set.Add(bird, true); set.Add(missing, true);
    if (set.Entries.Size != 1 || set.Entries.m_storage[0] != 14 || set.Entries.m_storage[1] != 9 || set.Entries.m_storage[2] != 1) { std::printf("00870750_TEST FAIL\n"); return 1; }
    CCharString phys = { "CTCPhysicsStandard" }; set.Add(phys, false);
    if (set.Entries.Size != 2 || set.Entries.m_storage[3] != 18 || set.Entries.m_storage[4] != 3 || set.Entries.m_storage[5] != 0) { std::printf("00870750_TEST FAIL\n"); return 1; }
    std::printf("00870750_TEST PASS\n");
    return 0;
}