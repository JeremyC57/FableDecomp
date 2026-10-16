#include <cstdio>
// Behaviour model of CTCCreatureOpinionOfHero::IsGuardOnLevel (0x006836D0).
struct Thing { bool dead; bool isGuard; };
struct Node { Node* next; Thing* value; };
struct Village { Node* head; };
struct Member { Village* village; Village* GetVillage() { return village; } };
struct Op {
    Member* member;
    bool IsGuardOnLevel()
    {
        if (member) {
            if (member->GetVillage()) {
                Village* v = member->GetVillage();
                for (Node* n = v->head->next; n != v->head; n = n->next)
                    if (n->value && !n->value->dead && n->value->isGuard) return true;
            }
        }
        return false;
    }
};
int main()
{
    Thing a = { false, false }, g = { true, true };
    Node h, n1, n2; h.next = &n1; n1.next = &n2; n2.next = &h; n1.value = &a; n2.value = &g; h.value = 0;
    Village v = { &h }; Member m = { &v }; Op o = { &m };
    if (o.IsGuardOnLevel()) { std::printf("006836d0_TEST FAIL\n"); return 1; }
    g.dead = false;
    if (!o.IsGuardOnLevel()) { std::printf("006836d0_TEST FAIL\n"); return 1; }
    m.village = 0;
    if (o.IsGuardOnLevel()) { std::printf("006836d0_TEST FAIL\n"); return 1; }
    o.member = 0;
    if (o.IsGuardOnLevel()) { std::printf("006836d0_TEST FAIL\n"); return 1; }
    std::printf("006836d0_TEST PASS\n");
    return 0;
}