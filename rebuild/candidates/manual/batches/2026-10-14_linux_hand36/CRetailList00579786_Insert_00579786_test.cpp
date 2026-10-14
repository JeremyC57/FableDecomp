#include <cstdio>
#include <cstdlib>
#include <cstring>
static void* g_where = 0; static void* g_first = 0; static void* g_last = 0; static int g_calls = 0;
struct STag_00579786 { unsigned char m_tag; };
struct SIterator_00579786 { void* m_pNode; SIterator_00579786(void* n) : m_pNode(n) { } SIterator_00579786(const SIterator_00579786& o) : m_pNode(o.m_pNode) { } };
struct CRetailList_00579786 { void* m_pHead; void InsertRange(SIterator_00579786 w, void* f, void* l, const STag_00579786&) { g_where = w.m_pNode; g_first = f; g_last = l; ++g_calls; }
    void Insert(SIterator_00579786 w, void* f, void* l) { InsertRange(w, f, l, STag_00579786()); } };
int main() {
    CRetailList_00579786 l; int node = 0, f = 0, e = 0; SIterator_00579786 where(&node); l.Insert(where, &f, &e); if (g_where != &node || g_first != &f || g_last != &e || g_calls != 1) { std::printf("00579786_TEST FAIL\n"); return 1; }
    std::printf("00579786_TEST PASS\n"); return 0;
}
