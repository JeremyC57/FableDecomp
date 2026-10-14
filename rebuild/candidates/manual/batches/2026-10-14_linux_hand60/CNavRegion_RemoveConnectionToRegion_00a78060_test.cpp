#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CRegion; struct SConn { CRegion* a; CRegion* b; long refs; }; struct SNode { SNode* next; SNode* prev; SConn* conn; }; static SConn* g_last = 0;
struct CRegion { long m0; SNode* list; void Remove(SConn* c) { g_last = c; std::strcat(g_seq, "R"); }
  void RemoveTo(CRegion* other) { for (SNode* n = list->next; n != list; n = n->next) { SConn* c = n->conn; CRegion* far = this == c->a ? c->b : c->a; if (far == other) { SConn* s = n->conn; if (--s->refs == 0) { s->a->Remove(s); s->b->Remove(s); } return; } } } };

int main() {
    CRegion r1, r2, r3; SConn c12 = {&r1, &r2, 2}, c31 = {&r3, &r1, 1}; SNode head, n1, n2; head.next = &n1; n1.next = &n2; n2.next = &head; n1.conn = &c12; n2.conn = &c31; r1.list = &head;
    r1.RemoveTo(&r2); if (c12.refs != 1 || g_seq[0]) { std::printf("00a78060_TEST FAIL\n"); return 1; } r1.RemoveTo(&r3); if (c31.refs != 0 || std::strcmp(g_seq, "RR") || g_last != &c31) { std::printf("00a78060_TEST FAIL\n"); return 1; } r1.RemoveTo(&r2); if (c12.refs != 0 || std::strcmp(g_seq, "RRRR")) { std::printf("00a78060_TEST FAIL\n"); return 1; } r1.RemoveTo(&r2); if (std::strcmp(g_seq, "RRRR")) { std::printf("00a78060_TEST FAIL\n"); return 1; }
    std::printf("00a78060_TEST PASS\n"); return 0;
}
