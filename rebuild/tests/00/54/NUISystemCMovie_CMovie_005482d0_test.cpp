#include <cstdio>
class CIEngine { public: int id; };
static int g_defReads = 0;
struct CDraggableInto { long Id; CDraggableInto(long id) : Id(id) { } CDraggableInto(const CDraggableInto& o) : Id(o.Id) { } };
struct CMovie : CDraggableInto {
    bool Playing; void* HandleData; bool AttempToPlay;
    static CIEngine* PEngine;
    CMovie(CIEngine& engine, long id) : CDraggableInto(id), Playing(false), HandleData(0), AttempToPlay(false) { PEngine = &engine; GetDataFromDef(); }
    CMovie(const CMovie& o) : CDraggableInto(o), Playing(false), HandleData(0), AttempToPlay(false) { GetDataFromDef(); }
    void GetDataFromDef() { ++g_defReads; }
};
CIEngine* CMovie::PEngine = 0;
int main()
{
    CIEngine e1, e2; e1.id = 1; e2.id = 2;
    CMovie a(e1, 7);
    bool fail = (CMovie::PEngine != &e1 || a.Id != 7 || a.Playing || a.AttempToPlay || g_defReads != 1);
    CMovie b(a);
    if (CMovie::PEngine != &e1 || b.Id != 7 || b.Playing || g_defReads != 2) fail = true;
    CMovie c(e2, 9);
    if (CMovie::PEngine != &e2 || c.Id != 9 || g_defReads != 3) fail = true;
    if (fail) { std::printf("005482d0_TEST FAIL\n"); return 1; }
    std::printf("005482d0_TEST PASS\n");
    return 0;
}