#include <cstdio>
// Behaviour of CTCInventoryStats::~CTCInventoryStats (0x005CD2FE): the mannequin is killed (if any), then the members and bases go.
static long g_order[8]; static long g_count = 0;
class CThing { public: long m_kills; void Kill(bool immediate) { if (!immediate) ++m_kills; g_order[g_count++] = 1; } };
struct CTCInventoryBase_Retail { virtual ~CTCInventoryBase_Retail() { g_order[g_count++] = 4; } };
struct CBase { virtual long vfunc_1() { return 0; } ~CBase() { g_order[g_count++] = 3; } };
struct CCreatureKillMap { ~CCreatureKillMap() { g_order[g_count++] = 2; } };
class CTCInventoryStats : public CTCInventoryBase_Retail, public CBase { public:
    CCreatureKillMap CreaturesKilled; CThing* pMannequin; bool UseMannequin;
    virtual ~CTCInventoryStats() { if (pMannequin) pMannequin->Kill(false); }
};
int main()
{
    CThing mannequin; mannequin.m_kills = 0;
    CTCInventoryStats* s = new CTCInventoryStats; s->pMannequin = &mannequin;
    delete s;
    int bad = (mannequin.m_kills != 1 || g_count != 4 || g_order[0] != 1 || g_order[1] != 2 || g_order[2] != 3 || g_order[3] != 4);
    g_count = 0;
    CTCInventoryBase_Retail* b = new CTCInventoryStats; static_cast<CTCInventoryStats*>(b)->pMannequin = 0;
    delete b;
    bad |= (mannequin.m_kills != 1 || g_count != 3 || g_order[0] != 2);
    if (bad) { std::printf("005cd2fe_TEST FAIL\n"); return 1; }
    std::printf("005cd2fe_TEST PASS\n");
    return 0;
}