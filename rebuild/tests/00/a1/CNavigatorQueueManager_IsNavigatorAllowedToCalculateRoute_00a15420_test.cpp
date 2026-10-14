#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SQ { SQ* next; SQ* prev; long m8; void* nav; }; struct CQM { SQ* queue; bool Allowed(void* nav) { long i = 0; for (SQ* n = queue->next; n != queue; n = n->next, ++i) { if (i >= 2) break; if (n->nav == nav) return true; } return false; } };

int main() {
    SQ head, a, b, c; int n1, n2, n3; head.next = &a; a.next = &b; b.next = &c; c.next = &head; a.nav = &n1; b.nav = &n2; c.nav = &n3; CQM q; q.queue = &head; if (!q.Allowed(&n1) || !q.Allowed(&n2) || q.Allowed(&n3) || q.Allowed(0)) { std::printf("00a15420_TEST FAIL\n"); return 1; } head.next = &head; if (q.Allowed(&n1)) { std::printf("00a15420_TEST FAIL\n"); return 1; }
    std::printf("00a15420_TEST PASS\n"); return 0;
}