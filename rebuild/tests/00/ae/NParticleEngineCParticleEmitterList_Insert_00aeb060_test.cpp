// Behaviour test for NParticleEngine::CParticleEmitterList::Insert @ 0x00AEB060: the same walk over a model circular list.
#include <cstdio>

struct Emitter { int priority; bool dead; int id; };
struct Node { Node* next; Node* prev; Emitter* data; };

struct List {
    Node sentinel;
    List() { sentinel.next = sentinel.prev = &sentinel; }
    Node* begin() { return sentinel.next; }
    Node* end() { return &sentinel; }
    int size() { int n = 0; for (Node* p = begin(); p != end(); p = p->next) ++n; return n; }
    Node* insert(Node* pos, Emitter* e) { Node* t = new Node; t->data = e; Node* p = pos->prev; t->next = pos; t->prev = p; p->next = t; pos->prev = t; return t; }
    void push_back(Emitter* e) { insert(end(), e); }
    void push_front(Emitter* e) { insert(begin(), e); }
    Node* erase(Node* n) { Node* nx = n->next; n->prev->next = nx; nx->prev = n->prev; delete n; return nx; }
};

struct CParticleEmitterList {
    List Active;
    long MaxActiveEmitters;
    bool Insert(Emitter* emitter)
    {
        if (Active.size() == 0) { Active.push_back(emitter); return true; }
        for (Node* it = Active.begin(); it != Active.end(); ) {
            if (it->data->dead)
                it = Active.erase(it);
            else {
                if (it->data->priority < emitter->priority) {
                    if (it == Active.begin()) { Active.push_front(emitter); return true; }
                    it = it->prev;
                    Active.insert(it, emitter);    // links before the previous node's position, as retail
                    return true;
                }
                it = it->next;
            }
        }
        if ((long)Active.size() < MaxActiveEmitters) { Active.push_back(emitter); return true; }
        return false;
    }
};

static int ids(CParticleEmitterList& l, int* out) { int n = 0; for (Node* p = l.Active.begin(); p != l.Active.end(); p = p->next) out[n++] = p->data->id; return n; }

int main()
{
    CParticleEmitterList l;
    l.MaxActiveEmitters = 3;
    Emitter a = { 3, false, 1 }, b = { 5, false, 2 }, c = { 1, false, 3 }, d = { 2, false, 4 }, e = { 0, false, 5 }, f = { 4, false, 6 };
    bool ok = true;
    ok = ok && l.Insert(&a);                 // empty -> [1]
    ok = ok && l.Insert(&b);                 // higher than front -> push_front [2,1]
    ok = ok && l.Insert(&c);                 // lowest, size 2 < 3 -> append [2,1,3]
    int o[8]; int n = ids(l, o);
    ok = ok && n == 3 && o[0] == 2 && o[1] == 1 && o[2] == 3;
    ok = ok && !l.Insert(&e);                // lowest, list full -> rejected
    // priority 2 goes before c (priority 1): the model inserts before the previous node's position (before `a`), as retail does
    ok = ok && l.Insert(&d);
    n = ids(l, o);
    ok = ok && n == 4 && o[0] == 2 && o[1] == 4 && o[2] == 1 && o[3] == 3;
    // dead emitters are erased on the way
    b.dead = true; a.dead = true;
    ok = ok && l.Insert(&f);                 // erases 2 (dead); 4's priority 2 < 4 and it is now the front -> push_front
    n = ids(l, o);
    ok = ok && n == 4 && o[0] == 6 && o[1] == 4 && o[2] == 1 && o[3] == 3;
    Emitter g = { 2, false, 7 };
    ok = ok && l.Insert(&g);                 // 6, 4 not lower; 1 is dead -> erased; 3 (priority 1) < 2 -> insert before prev (4)
    n = ids(l, o);
    ok = ok && n == 4 && o[0] == 6 && o[1] == 7 && o[2] == 4 && o[3] == 3;
    if (!ok) { std::printf("00aeb060_TEST FAIL\n"); return 1; }
    std::printf("00aeb060_TEST PASS\n");
    return 0;
}