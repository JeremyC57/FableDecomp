#include <cstdio>
#include <cstring>
class CCharStringData { public: const char* m_pChars; unsigned long m_length; };
long Compare_00411570(const char* a, const char* b) { return std::strcmp(a, b); }
class CCharString { public: CCharStringData* m_pData; };
static bool operator==(const CCharString& sa, const CCharString& sb)
{
    CCharStringData* a = sa.m_pData; CCharStringData* b = sb.m_pData;
    if (a == b) return true;
    if (!a || !b) return false;
    if (a->m_length != b->m_length) return false;
    return Compare_00411570(a->m_pChars, b->m_pChars) == 0;
}
class CQuestManager { public:
    CCharString* m_pNamesFirst; CCharString* m_pNamesLast;
    long GetIndexFromRegisteredQuest(const CCharString& name)
    {
        CCharString* last = m_pNamesLast; long index = -1; CCharString* it = m_pNamesFirst;
        if (it != last) { CCharString key = name; do { ++index; if (*it == key) return index; ++it; } while (it != last); }
        return -1;
    }
};
int main()
{
    CCharStringData d0 = { "hand", 4 }, d1 = { "foot", 4 }, d2 = { "head", 4 }, q = { "head", 4 }, r = { "tail", 4 }, s = { "heads", 5 };
    CCharString names[4]; names[0].m_pData = &d0; names[1].m_pData = 0; names[2].m_pData = &d1; names[3].m_pData = &d2;
    CQuestManager m; m.m_pNamesFirst = names; m.m_pNamesLast = names + 4;
    CCharString k; k.m_pData = &q;
    if (m.GetIndexFromRegisteredQuest(k) != 3) { std::printf("004af740_TEST FAIL\n"); return 1; }
    k.m_pData = &d1; if (m.GetIndexFromRegisteredQuest(k) != 2) { std::printf("004af740_TEST FAIL\n"); return 1; }
    k.m_pData = 0; if (m.GetIndexFromRegisteredQuest(k) != 1) { std::printf("004af740_TEST FAIL\n"); return 1; }
    k.m_pData = &r; if (m.GetIndexFromRegisteredQuest(k) != -1) { std::printf("004af740_TEST FAIL\n"); return 1; }
    k.m_pData = &s; if (m.GetIndexFromRegisteredQuest(k) != -1) { std::printf("004af740_TEST FAIL\n"); return 1; }
    m.m_pNamesLast = names; k.m_pData = &d0; if (m.GetIndexFromRegisteredQuest(k) != -1) { std::printf("004af740_TEST FAIL\n"); return 1; }
    std::printf("004af740_TEST PASS\n");
    return 0;
}