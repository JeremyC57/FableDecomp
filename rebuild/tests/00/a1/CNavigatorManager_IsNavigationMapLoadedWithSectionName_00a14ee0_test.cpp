#include <cstdio>
#include <cstdlib>
#include <cstring>
class CCharStringData { public: const char* m_pChars; unsigned long m_length; }; class CCharString { public: CCharStringData* m_pStringData; }; static long g_cmp = 0; static int Compare(const char* a, const char* b) { ++g_cmp; return std::strcmp(a, b); }
static bool operator==(const CCharString& sa, const CCharString& sb) { CCharStringData* a = sa.m_pStringData; CCharStringData* b = sb.m_pStringData; if (a == b) return true; if (!a || !b) return false; if (a->m_length != b->m_length) return false; return Compare(a->m_pChars, b->m_pChars) == 0; }
struct SNode { SNode* next; SNode* prev; unsigned char p[8]; long id; unsigned char q[0x10]; CCharString name; }; struct CMgr { unsigned char _pad[0x24]; SNode* maps; bool IsLoaded(long id, const CCharString* n) { for (SNode* x = maps->next; x != maps; x = x->next) if (x->id == id) return x->name == *n; return false; } };

int main() {
    SNode head, n1; CCharStringData da = {"abc", 3}, db = {"abc", 3}, dc = {"abd", 3}, dd = {"ab", 2}; head.next = &n1; n1.next = &head; n1.id = 7; n1.name.m_pStringData = &da; CMgr* m = new CMgr; m->maps = &head; CCharString s; s.m_pStringData = &da;
    if (!m->IsLoaded(7, &s) || g_cmp != 0) { std::printf("00a14ee0_TEST FAIL\n"); return 1; } s.m_pStringData = &db; if (!m->IsLoaded(7, &s) || g_cmp != 1) { std::printf("00a14ee0_TEST FAIL\n"); return 1; } s.m_pStringData = &dc; if (m->IsLoaded(7, &s) || g_cmp != 2) { std::printf("00a14ee0_TEST FAIL\n"); return 1; } s.m_pStringData = &dd; if (m->IsLoaded(7, &s) || g_cmp != 2) { std::printf("00a14ee0_TEST FAIL\n"); return 1; } s.m_pStringData = 0; if (m->IsLoaded(7, &s) || m->IsLoaded(8, &s)) { std::printf("00a14ee0_TEST FAIL\n"); return 1; } delete m;
    std::printf("00a14ee0_TEST PASS\n"); return 0;
}