#include <cstdio>
#include <cstdlib>
#include <new>
// Behaviour model: copy-construct 16-byte {u64 key, intrusive circular list of lines} records into raw storage.
struct Node { Node* next; Node* prev; float x0, y0, x1, y1; };
struct Rec { unsigned int keylo, keyhi; Node* head; };
static int g_dispatch;
static void insert_range(Node* head, const Node* first, const Node* last)
{
    ++g_dispatch;
    for (; first != last; first = first->next) {
        Node* n = (Node*)std::malloc(sizeof(Node)); *n = *first;
        n->next = head; n->prev = head->prev; head->prev->next = n; head->prev = n;
    }
}
static Rec* uninit_copy(const Rec* first, const Rec* last, Rec* result)
{
    Rec* cur = result;
    for (; first != last; ++first, ++cur) {
        cur->keylo = first->keylo; cur->keyhi = first->keyhi;
        Node* h = (Node*)std::malloc(sizeof(Node)); h->next = h; h->prev = h; cur->head = h;
        insert_range(h, first->head->next, first->head);
    }
    return cur;
}
static int count(const Node* h) { int n = 0; for (const Node* p = h->next; p != h; p = p->next) ++n; return n; }
int main()
{
    Rec src[2];
    for (int r = 0; r < 2; ++r) {
        src[r].keylo = 100 + r; src[r].keyhi = 7; Node* h = (Node*)std::malloc(sizeof(Node)); h->next = h; h->prev = h; src[r].head = h;
    }
    Node seed[3]; for (int i = 0; i < 3; ++i) { seed[i].x0 = (float)i; seed[i].next = (i < 2) ? &seed[i + 1] : 0; }
    Node tmp; tmp.next = &seed[0];
    seed[2].next = &tmp; insert_range(src[1].head, &seed[0], &tmp);   // src[1] gets 3 lines
    g_dispatch = 0;
    Rec dst[2];
    Rec* e = uninit_copy(src, src + 2, dst);
    if (e != dst + 2 || g_dispatch != 2 || dst[0].keylo != 100 || dst[1].keylo != 101 || dst[1].keyhi != 7
        || count(dst[0].head) != 0 || count(dst[1].head) != 3 || dst[1].head == src[1].head || dst[1].head->next->x0 != 0.0f)
        { std::printf("0051ac00_TEST FAIL\n"); return 1; }
    if (uninit_copy(src, src, dst) != dst) { std::printf("0051ac00_TEST FAIL\n"); return 1; }
    std::printf("0051ac00_TEST PASS\n");
    return 0;
}