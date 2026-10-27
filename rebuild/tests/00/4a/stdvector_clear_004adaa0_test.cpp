// Behaviour test for 004adaa0 (plain C++98 model of the reconstructed logic).
#include <cstdio>
#include <new>
static int g_Dtors = 0;
class CTestQuest { public: int Id; CTestQuest() : Id(0) { } ~CTestQuest() { ++g_Dtors; } };
struct Vec { CTestQuest* Start; CTestQuest* Finish;
    CTestQuest* erase(CTestQuest* first, CTestQuest* last) { CTestQuest* i = first; for (CTestQuest* s = last; s != Finish; ++s, ++i) *i = *s;
        for (CTestQuest* d = i; d != Finish; ++d) d->~CTestQuest(); Finish = i; return first; }
    void clear() { erase(Start, Finish); } };
int main() {
    CTestQuest* raw = (CTestQuest*)::operator new(sizeof(CTestQuest) * 4);
    for (int i = 0; i < 4; ++i) { new (raw + i) CTestQuest(); raw[i].Id = i; }
    Vec v; v.Start = raw; v.Finish = raw + 4; v.clear();
    if (v.Finish != v.Start || g_Dtors != 4) { std::printf("004adaa0_TEST FAIL\n"); return 1; }
    ::operator delete(raw);
    { std::printf("004adaa0_TEST PASS\n"); return 0; }
}