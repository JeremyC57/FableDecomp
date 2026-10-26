#include <cstdio>
// behaviour test: dead or missing followers are erased from both lists (each erase of a plain follower refreshes the GUI), live ones
// get their emote icon re-added, order preserved
class CThing { public: bool Dead; };
struct Node { Node* next; Node* prev; CThing* item; };
struct List {
    Node head; List() { head.next = head.prev = &head; }
    void push(Node* n) { n->prev = head.prev; n->next = &head; head.prev->next = n; head.prev = n; }
    Node* erase(Node* n) { Node* next = n->next; n->prev->next = next; next->prev = n->prev; return next; }
    int size() { int c = 0; for (Node* n = head.next; n != &head; n = n->next) ++c; return c; }
};
static int g_refreshes = 0, g_icons = 0;
class CTCFollowed { public:
    List Followers, AIFollowers;
    void CullNullFollowers()
    {
        for (Node* it = Followers.head.next; it != &Followers.head; ) {
            CThing* f = it->item;
            if (!(f && !f->Dead)) { it = Followers.erase(it); ++g_refreshes; }
            else { ++g_icons; it = it->next; }
        }
        for (Node* it = AIFollowers.head.next; it != &AIFollowers.head; ) {
            CThing* f = it->item;
            if (!(f && !f->Dead)) it = AIFollowers.erase(it);
            else { ++g_icons; it = it->next; }
        }
    }
};
int main()
{
    CThing alive = { false }, dead = { true };
    Node a = { 0, 0, &alive }, b = { 0, 0, &dead }, c = { 0, 0, 0 }, d = { 0, 0, &alive }, e = { 0, 0, &dead };
    CTCFollowed f;
    f.Followers.push(&b); f.Followers.push(&a); f.Followers.push(&c);
    f.AIFollowers.push(&d); f.AIFollowers.push(&e);
    f.CullNullFollowers();
    bool ok = f.Followers.size() == 1 && f.Followers.head.next == &a && f.AIFollowers.size() == 1 && f.AIFollowers.head.next == &d &&
              g_refreshes == 2 && g_icons == 2;
    if (!ok) { std::printf("006d5260_TEST FAIL\n"); return 1; }
    std::printf("006d5260_TEST PASS\n");
    return 0;
}