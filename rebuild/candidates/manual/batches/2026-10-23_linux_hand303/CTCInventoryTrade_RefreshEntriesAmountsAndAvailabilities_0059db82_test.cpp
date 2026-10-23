// Behaviour test for the trade-screen entry recount @ 0x0059db82 (PC=0): counting, recursion and arrow handling
#include <cstdio>
struct Node {
    int type; long amount; Node* kids[4]; int nkids; Node* mapped; int arrow; int shown; int hidden; int actionType;
    Node(int t, long a) : type(t), amount(a), nkids(0), mapped(0), arrow(0), shown(0), hidden(0), actionType(0) {}
    void Add(Node* k) { kids[nkids++] = k; }
};
struct Pair { unsigned long first, second; };
static const int kPC = 0;
static Pair Refresh(Node* c)
{
    Pair r; r.first = 0; r.second = 0;
    bool hasList = false;
    for (int i = 0; i < c->nkids; ++i) {
        if (c->kids[i]->type == 8) {
            if (c->mapped) {
                Node* list = c->mapped;
                for (int j = 0; j < list->nkids; ++j)
                    if (list->kids[j]->type == 7 && list->kids[j]->amount == 0) { ++r.first; ++r.second; }
            } else {
                Node* k = c->kids[i];
                for (int j = 0; j < k->nkids; ++j)
                    if (k->kids[j]->type == 7 && (!kPC || k->kids[j]->amount == 0)) {
                        Pair s = Refresh(k->kids[j]);
                        r.first += s.first + 1; r.second += s.second;
                    }
            }
            hasList = true;
        }
    }
    if (hasList && r.first != 0) {
        if (!c->arrow) { c->shown++; c->arrow = 1; }
    } else if (kPC || c->arrow) {
        c->hidden++;
        if (c->arrow) c->actionType = 1;
    }
    return r;
}
int main()
{
    int ok = 1;
    Node root(0, 0), listChild(8, 0), list(0, 0), e1(7, 0), e2(7, 3), e3(7, 0);
    list.Add(&e1); list.Add(&e2); list.Add(&e3);
    root.Add(&listChild); root.mapped = &list;
    Pair p = Refresh(&root);
    if (p.first != 2 || p.second != 2 || root.shown != 1 || root.arrow != 1) ok = 0;
    Node top(0, 0), branch(8, 0), sub(7, 0), sub2(7, 5), subList(8, 0), leafList(0, 0), leaf(7, 0);
    leafList.Add(&leaf); subList.Add(&leaf); sub.Add(&subList); sub.mapped = &leafList;
    branch.Add(&sub); branch.Add(&sub2); top.Add(&branch);
    Pair q = Refresh(&top);
    if (kPC) { if (q.first != 2 || q.second != 1) ok = 0; }
    else { if (q.first != 3 || q.second != 1) ok = 0; }
    Node empty(0, 0); empty.arrow = 1;
    Refresh(&empty);
    if (empty.hidden != 1 || empty.actionType != 1) ok = 0;
    Node none(0, 0);
    Refresh(&none);
    if (none.hidden != (kPC ? 1 : 0)) ok = 0;
    if (!ok) { std::printf("0059db82_TEST FAIL\n"); return 1; }
    std::printf("0059db82_TEST PASS\n");
    return 0;
}
