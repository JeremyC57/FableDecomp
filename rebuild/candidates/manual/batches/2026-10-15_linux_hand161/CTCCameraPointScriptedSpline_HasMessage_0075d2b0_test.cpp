// behaviour test for 0x0075D2B0 CTCCameraPointScriptedSpline::HasMessage
#include <cstdio>
struct Data { const char* p; unsigned int len; };
static int Compare(const char* a, const char* b)
{
    for (;;) { char ca = *a; if (ca == 0 && *b == 0) return 0; char cb = *b; if (ca < cb) return -1; if (ca > cb) return 1; ++a; ++b; }
}
struct Str {
    Data* d;
    bool operator==(const Str& o) const
    {
        if (d == o.d) return true;
        if (d && o.d && d->len == o.d->len) return Compare(d->p, o.d->p) ? false : true;
        return false;
    }
};
struct Node { Node* next; Node* prev; Str value; };
static int g_freed = 0;
struct SS {
    Node* head;
    bool HasMessage(const Str& m)
    {
        for (Node* n = head->next; n != head; n = n->next)
            if (n->value == m) { Node* nx = n->next; Node* pv = n->prev; pv->next = nx; nx->prev = pv; ++g_freed; return true; }
        return false;
    }
};
int main()
{
    Data da = { "Alpha", 5 }, db = { "Bravo", 5 }, dq = { "Bravo", 5 }, dx = { "Brav", 4 };
    Node h, a, b;
    h.next = &a; a.next = &b; b.next = &h; h.prev = &b; b.prev = &a; a.prev = &h;
    a.value.d = &da; b.value.d = &db;
    SS s; s.head = &h;
    Str q; q.d = &dx;
    bool ok = !s.HasMessage(q);
    q.d = &dq;
    ok = ok && s.HasMessage(q) && g_freed == 1 && a.next == &h && h.prev == &a;
    ok = ok && !s.HasMessage(q);
    q.d = &da;
    ok = ok && s.HasMessage(q) && h.next == &h;
    if (!ok) { std::printf("0075d2b0_TEST FAIL\n"); return 1; }
    std::printf("0075d2b0_TEST PASS\n");
    return 0;
}
