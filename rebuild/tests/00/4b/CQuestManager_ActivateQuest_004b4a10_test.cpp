#include <cstdio>
#include <cstdlib>
#include <new>

// Behaviour model of CQuestManager::ActivateQuest (0x004B4A10): wrap the name in a one-element array (push_back on an
// empty vector -> the growth path), call ActivateMultipleQuests with the two flags, destroy the array, return its result.
static int g_alive = 0;
struct CCharString {
    const char* p;
    CCharString(const char* s) : p(s) { ++g_alive; }
    CCharString(const CCharString& o) : p(o.p) { ++g_alive; }
    ~CCharString() { --g_alive; }
};
struct CArray {
    CCharString* start; CCharString* finish; CCharString* eos;
    CArray() : start(0), finish(0), eos(0) { }
    ~CArray() { for (CCharString* q = start; q != finish; ++q) q->~CCharString(); if (start) std::free(start); }
    void push_back(const CCharString& x)
    {
        if (finish != eos) { new (finish) CCharString(x); ++finish; return; }
        unsigned int old = (unsigned int)(finish - start), len = old + (old < 1 ? 1 : old);
        CCharString* ns = (CCharString*)std::malloc(len * sizeof(CCharString));
        CCharString* nf = ns;
        for (CCharString* q = start; q != finish; ++q, ++nf) new (nf) CCharString(*q);
        new (nf) CCharString(x); ++nf;
        for (CCharString* d = start; d != finish; ++d) d->~CCharString();
        if (start) std::free(start);
        start = ns; finish = nf; eos = ns + len;
    }
};
struct CQuestManager {
    int calls; const char* lastName; int lastCount; bool lastFlag, lastLoad, answer;
    bool ActivateMultipleQuests(const CArray& names, bool flag, bool load)
    {
        ++calls; lastCount = (int)(names.finish - names.start); lastName = names.start[0].p; lastFlag = flag; lastLoad = load;
        return answer;
    }
    bool ActivateQuest(const CCharString& name, bool flag, bool load)
    {
        CArray quests;
        quests.push_back(name);
        return ActivateMultipleQuests(quests, flag, load);
    }
};

int main()
{
    CQuestManager qm; qm.calls = 0; qm.answer = true;
    CCharString name("Q_Arena");
    bool r = qm.ActivateQuest(name, true, false);
    if (!r || qm.calls != 1 || qm.lastCount != 1 || qm.lastName != name.p || !qm.lastFlag || qm.lastLoad) { std::printf("004b4a10_TEST FAIL\n"); return 1; }
    qm.answer = false;
    r = qm.ActivateQuest(name, false, true);
    if (r || qm.calls != 2 || qm.lastFlag || !qm.lastLoad) { std::printf("004b4a10_TEST FAIL\n"); return 1; }
    if (g_alive != 1) { std::printf("004b4a10_TEST FAIL\n"); return 1; }                       // the arrays' copies are gone
    std::printf("004b4a10_TEST PASS\n");
    return 0;
}