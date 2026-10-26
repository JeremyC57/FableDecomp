// Behaviour test for 0x004BB2E0 vector<CQuestManager::CActivateQuestInfo>::_M_insert_overflow.
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_live = 0;
class CCharString { public:
    int id;
    CCharString(int v) : id(v) { ++g_live; }
    CCharString(const CCharString& o) : id(o.id) { ++g_live; }
    ~CCharString() { --g_live; }
};
struct CActivateQuestInfo {
    CCharString QuestName;
    void*       PScriptInfo;
    bool        GameScript;
    bool        LoadResources;
    CActivateQuestInfo(int n, bool g, bool l) : QuestName(n), PScriptInfo(0), GameScript(g), LoadResources(l) { }
};

struct Vec {
    CActivateQuestInfo* start; CActivateQuestInfo* finish; CActivateQuestInfo* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    void insert_overflow(CActivateQuestInfo* pos, const CActivateQuestInfo& x, unsigned int fill_len, bool atend)
    {
        unsigned int old_size = size();
        unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        CActivateQuestInfo* ns = len ? (CActivateQuestInfo*)std::malloc(len * sizeof(CActivateQuestInfo)) : 0;
        CActivateQuestInfo* nf = ns;
        for (CActivateQuestInfo* p = start; p != pos; ++p, ++nf) new (nf) CActivateQuestInfo(*p);
        if (fill_len == 1) { new (nf) CActivateQuestInfo(x); ++nf; }
        else for (unsigned int n = fill_len; n > 0; --n, ++nf) new (nf) CActivateQuestInfo(x);
        if (!atend) for (CActivateQuestInfo* p = pos; p != finish; ++p, ++nf) new (nf) CActivateQuestInfo(*p);
        for (CActivateQuestInfo* p = start; p != finish; ++p) p->~CActivateQuestInfo();
        if (start) std::free(start);
        start = ns; finish = nf; eos = ns + len;
    }
};

int main()
{
    int ok = 1;
    Vec v; v.start = v.finish = v.eos = 0;
    CActivateQuestInfo a(1, true, false), b(2, false, true), c(3, true, true);
    v.insert_overflow(v.finish, a, 1, true);
    v.insert_overflow(v.finish, b, 1, true);
    v.insert_overflow(v.start + 1, c, 2, false);         // len = 2 + max(2,2) = 4
    if (v.size() != 4 || v.eos - v.start != 4) ok = 0;
    int ids[4] = { 1, 3, 3, 2 };
    for (int i = 0; i < 4; ++i) if (v.start[i].QuestName.id != ids[i]) ok = 0;
    if (!v.start[1].GameScript || !v.start[1].LoadResources || v.start[3].GameScript || !v.start[3].LoadResources) ok = 0;
    if (g_live != 3 + 4) ok = 0;
    for (CActivateQuestInfo* p = v.start; p != v.finish; ++p) p->~CActivateQuestInfo();
    std::free(v.start);
    if (g_live != 3) ok = 0;
    if (!ok) { std::printf("004bb2e0_TEST FAIL\n"); return 1; }
    std::printf("004bb2e0_TEST PASS\n");
    return 0;
}