// Behaviour test for 0x004af6d0 (CQuestManager::SetQuestWorldMapOffset): the reconstructed logic run over a host model.
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
struct Quest { char Name[32]; long X, Y; };
struct QNode { QNode* next; Quest* q; };
static void SetOffset(QNode* head, const char* name, long x, long y)
{ for (QNode* n = head->next; n != head; n = n->next) if (std::strcmp(n->q->Name, name) == 0) { n->q->X = x; n->q->Y = y; return; } }

int main()
{
    Quest a = { "Q_A", 0, 0 }, b = { "Q_B", 0, 0 }; QNode head, na, nb; head.next = &na; na.next = &nb; nb.next = &head; na.q = &a; nb.q = &b;
    SetOffset(&head, "Q_B", 5, -6); CHECK(b.X == 5 && b.Y == -6 && a.X == 0);
    SetOffset(&head, "Q_C", 1, 1); CHECK(a.X == 0 && b.X == 5);
    if (g_fail) { std::printf("004af6d0_TEST FAIL\n"); return 1; }
    std::printf("004af6d0_TEST PASS\n");
    return 0;
}
