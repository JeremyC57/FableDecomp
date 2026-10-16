// behaviour test for 0x007293A0 CTCCreatureExpression::ForcePrimaryExpression
#include <cstdio>
struct State {
    long AnimIndex; float MaxWeight, Weight, OldWeight; bool Primary; unsigned char Stage;
    State() {}
    State(long a, float m) : AnimIndex(a), MaxWeight(m), Weight(0.0f), OldWeight(0.0f), Primary(true), Stage(0) {}
};
struct List {
    struct node { node* next; node* prev; State value; };
    node sentinel; node pool[8]; int used; node* head;
    List() : used(0) { head = &sentinel; sentinel.next = sentinel.prev = &sentinel; }
    bool empty() const { return head->next == head; }
    void push_front(const State& s) { node* n = &pool[used++]; n->value = s; n->next = head->next; n->prev = head; head->next->prev = n; head->next = n; }
};
struct Expr {
    List ExpressionStates;
    void ForcePrimaryExpression(long anim, float w)
    {
        if (!ExpressionStates.empty()) {
            List::node* f = ExpressionStates.head->next;
            if (f->value.AnimIndex == anim) { f->value.MaxWeight = w; return; }
            f->value.Primary = false;
        }
        if (anim) ExpressionStates.push_front(State(anim, w));
    }
};
int main()
{
    Expr e;
    e.ForcePrimaryExpression(0, 1.0f);
    bool ok = e.ExpressionStates.empty();
    e.ForcePrimaryExpression(5, 0.5f);
    List::node* f = e.ExpressionStates.head->next;
    ok = ok && f->value.AnimIndex == 5 && f->value.MaxWeight == 0.5f && f->value.Primary && f->value.Weight == 0.0f;
    e.ForcePrimaryExpression(5, 0.75f);
    ok = ok && e.ExpressionStates.used == 1 && f->value.MaxWeight == 0.75f;
    e.ForcePrimaryExpression(6, 1.0f);
    ok = ok && e.ExpressionStates.used == 2 && !f->value.Primary && e.ExpressionStates.head->next->value.AnimIndex == 6;
    e.ForcePrimaryExpression(0, 1.0f);
    ok = ok && e.ExpressionStates.used == 2 && !e.ExpressionStates.head->next->value.Primary;
    if (!ok) { std::printf("007293a0_TEST FAIL\n"); return 1; }
    std::printf("007293a0_TEST PASS\n");
    return 0;
}