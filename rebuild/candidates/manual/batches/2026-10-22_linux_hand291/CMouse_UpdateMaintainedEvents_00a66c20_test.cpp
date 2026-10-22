// Behaviour test for CMouse::UpdateMaintainedEvents (0x00A66C20): every held-button event is re-issued with the drag since its press.
#include <cstdio>

struct V2 { float X, Y; };
struct V3 { float X, Y, Z; };
struct Ev {
    long Type; V2 startPos; float StartTime;
    int lastCall; V2 pos; V3 move; double start, now; long button;
    void Set(int which, const V2& p, const V3& m, double s, double n, long b) { lastCall = which; pos = p; move = m; start = s; now = n; button = b; }
};
struct Node { Node* next; Ev ev; };

static double g_now = 0.0;

static void Update(Node* head, const V2& mousePos)
{
    V3 movement; movement.X = 0; movement.Y = 0; movement.Z = 0;
    V2 pos; pos.X = mousePos.X; pos.Y = mousePos.Y;
    for (Node* n = head->next; n != head; n = n->next) {
        Ev& e = n->ev;
        long t = e.Type;
        int which = 0;
        if (t == 5) which = 1; else if (t == 8) which = 2; else if (t == 11) which = 3; else if (t >= 27 && t <= 31) which = 4;
        if (!which) continue;
        V2 sp = e.startPos;
        movement.X = pos.X - sp.X;
        movement.Y = pos.Y - sp.Y;
        e.Set(which, pos, movement, e.StartTime, g_now, which == 4 ? t - 27 : -1);
    }
}

int main()
{
    Node head, a, b, c;
    head.next = &a; a.next = &b; b.next = &c; c.next = &head;
    a.ev.Type = 5;  a.ev.startPos.X = 1; a.ev.startPos.Y = 2; a.ev.StartTime = 0.5f; a.ev.lastCall = 0;
    b.ev.Type = 6;  b.ev.lastCall = 0;
    c.ev.Type = 29; c.ev.startPos.X = 10; c.ev.startPos.Y = 0; c.ev.StartTime = 1.0f; c.ev.lastCall = 0;
    V2 m; m.X = 4; m.Y = 6; g_now = 2.0;
    Update(&head, m);
    bool ok = a.ev.lastCall == 1 && a.ev.move.X == 3 && a.ev.move.Y == 4 && a.ev.move.Z == 0 && a.ev.start == 0.5 && a.ev.now == 2.0
           && b.ev.lastCall == 0
           && c.ev.lastCall == 4 && c.ev.button == 2 && c.ev.move.X == -6 && c.ev.move.Y == 6 && c.ev.pos.X == 4;
    if (!ok) { std::printf("00a66c20_TEST FAIL\n"); return 1; }
    std::printf("00a66c20_TEST PASS\n");
    return 0;
}
