#include <cstdio>
#include <cstdlib>
#include <cstring>
static void* g_where = 0; static void* g_first = 0; static void* g_last = 0; static int g_calls = 0;
struct STag_0053cb02 { unsigned char m_tag; };
struct SIterator_0053cb02 { void* m_pNode; SIterator_0053cb02(void* n) : m_pNode(n) { } SIterator_0053cb02(const SIterator_0053cb02& o) : m_pNode(o.m_pNode) { } };
struct CRetailList_0053cb02 { void* m_pHead; void InsertRange(SIterator_0053cb02 w, void* f, void* l, const STag_0053cb02&) { g_where = w.m_pNode; g_first = f; g_last = l; ++g_calls; }
    void Insert(SIterator_0053cb02 w, void* f, void* l) { InsertRange(w, f, l, STag_0053cb02()); } };
int main() {
    CRetailList_0053cb02 l; int node = 0, f = 0, e = 0; SIterator_0053cb02 where(&node); l.Insert(where, &f, &e); if (g_where != &node || g_first != &f || g_last != &e || g_calls != 1) { std::printf("0053cb02_TEST FAIL\n"); return 1; }
    std::printf("0053cb02_TEST PASS\n"); return 0;
}