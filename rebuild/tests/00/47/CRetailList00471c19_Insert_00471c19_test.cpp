#include <cstdio>
#include <cstdlib>
#include <cstring>
static void* g_where = 0; static void* g_first = 0; static void* g_last = 0; static int g_calls = 0;
struct STag_00471c19 { unsigned char m_tag; };
struct SIterator_00471c19 { void* m_pNode; SIterator_00471c19(void* n) : m_pNode(n) { } SIterator_00471c19(const SIterator_00471c19& o) : m_pNode(o.m_pNode) { } };
struct CRetailList_00471c19 { void* m_pHead; void InsertRange(SIterator_00471c19 w, void* f, void* l, const STag_00471c19&) { g_where = w.m_pNode; g_first = f; g_last = l; ++g_calls; }
    void Insert(SIterator_00471c19 w, void* f, void* l) { InsertRange(w, f, l, STag_00471c19()); } };
int main() {
    CRetailList_00471c19 l; int node = 0, f = 0, e = 0; SIterator_00471c19 where(&node); l.Insert(where, &f, &e); if (g_where != &node || g_first != &f || g_last != &e || g_calls != 1) { std::printf("00471c19_TEST FAIL\n"); return 1; }
    std::printf("00471c19_TEST PASS\n"); return 0;
}