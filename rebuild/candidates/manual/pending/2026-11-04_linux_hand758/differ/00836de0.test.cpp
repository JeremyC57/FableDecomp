// Behaviour test for CThingTrackNode::FlipTrackDirection (0x00836DE0), g++ model: every node of the track has next/previous and start/end swapped.
#include <cstdio>
struct Node { Node* prev; Node* next; bool start, end; };
static void Flip(Node* n)
{
    if (!n) return;
    Node* t = n->prev; n->prev = n->next; n->next = t;
    if (n->start) { n->start = false; n->end = true; } else if (n->end) { n->end = false; n->start = true; }
}
static void FlipTrackDirection(Node* self)
{
    Node* previous = self->prev; Node* next = self->next;
    Flip(self);
    Node* forward = next; Node* backward = previous;
    while (forward || backward) {
        if (forward) { Node* n = forward; forward = n->next; Flip(n); }
        if (backward) { Node* n = backward; backward = n->prev; Flip(n); }
    }
}
int main()
{
    Node n[5];
    for (int i = 0; i < 5; ++i) { n[i].prev = i ? &n[i - 1] : 0; n[i].next = i < 4 ? &n[i + 1] : 0; n[i].start = i == 0; n[i].end = i == 4; }
    FlipTrackDirection(&n[2]);
    int ok = 1;
    for (int i = 0; i < 5; ++i) ok = ok && n[i].next == (i ? &n[i - 1] : 0) && n[i].prev == (i < 4 ? &n[i + 1] : 0);
    ok = ok && n[0].end && !n[0].start && n[4].start && !n[4].end && !n[2].start && !n[2].end;
    std::printf(ok ? "00836de0_TEST PASS\n" : "00836de0_TEST FAIL\n");
    return ok ? 0 : 1;
}
