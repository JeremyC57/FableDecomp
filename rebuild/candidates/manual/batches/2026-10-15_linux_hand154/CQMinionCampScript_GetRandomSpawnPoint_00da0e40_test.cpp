// Behaviour test for 0x00da0e40 (CQ_MinionCampScript::GetRandomSpawnPoint): the reconstructed logic run over a host model.
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
struct Node { Node* next; int value; };
struct List { Node head; unsigned int size() const { unsigned int n = 0; for (Node* p = head.next; p != &head; p = p->next) ++n; return n; } };
static bool g_term = false; static int g_rand = 0;
static int my_rand() { return g_rand; }
static void GetRandom(List& l, int& out)
{
    if (l.head.next == &l.head) return; if (g_term) return;
    unsigned int index = my_rand() % l.size(); unsigned int i = 0;
    for (Node* it = l.head.next; it != &l.head; it = it->next, ++i) { if (g_term) return; if (i == index) { if (g_term) return; out = it->value; return; } }
}

int main()
{
    Node n[3]; List l; l.head.next = &n[0]; n[0].next = &n[1]; n[1].next = &n[2]; n[2].next = &l.head; for (int i = 0; i < 3; ++i) n[i].value = 10 + i;
    int out = -1; g_rand = 7; GetRandom(l, out); CHECK(out == 11);
    g_rand = 3; GetRandom(l, out); CHECK(out == 10);
    out = -1; g_term = true; GetRandom(l, out); CHECK(out == -1); g_term = false;
    List e; e.head.next = &e.head; GetRandom(e, out); CHECK(out == -1);
    if (g_fail) { std::printf("00da0e40_TEST FAIL\n"); return 1; }
    std::printf("00da0e40_TEST PASS\n");
    return 0;
}
