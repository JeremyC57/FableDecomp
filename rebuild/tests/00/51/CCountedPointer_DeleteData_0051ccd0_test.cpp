// Behaviour test for 0051ccd0 (plain C++98 model of the reconstructed logic).
#include <cstdio>
#include <new>
static int g_NodeReleases = 0;
struct CCountedPointerNode { int* Data; ~CCountedPointerNode() { ++g_NodeReleases; } };
struct Pair { int first; CCountedPointerNode second; };
class CRegionConnectionGraph { public: Pair* Start; Pair* Finish; Pair* End; bool Dirty;
    ~CRegionConnectionGraph() { for (Pair* p = Start; p != Finish; ++p) p->~Pair(); if (Start) ::operator delete(Start); } };
template <class T> class CCountedPointer { public: static void DeleteData(void* data) { delete (T*)data; } };
int main() {
    CRegionConnectionGraph* g = new CRegionConnectionGraph;
    g->Start = (Pair*)::operator new(sizeof(Pair) * 2); g->Finish = g->Start + 2; g->End = g->Finish;
    for (int i = 0; i < 2; ++i) new (g->Start + i) Pair();
    CCountedPointer<CRegionConnectionGraph>::DeleteData(g);
    if (g_NodeReleases != 2) { std::printf("0051ccd0_TEST FAIL\n"); return 1; }
    { std::printf("0051ccd0_TEST PASS\n"); return 0; }
}