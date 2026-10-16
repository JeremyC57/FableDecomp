#include <cstdio>
#include <cstdlib>
// Behaviour model of CModeHoverBase::CModeHoverBase (0x008EAC40): empty vector, empty name, an empty list whose
// sentinel links to itself, a null particle effect.
struct Node { Node* next; Node* prev; void* a; void* b; };
struct Hover {
    void* first; void* last; void* end; const char* name; Node* queue; void* effect;
    Hover() : first(0), last(0), end(0), name(""), queue(0), effect(0)
    {
        Node* s = (Node*)std::malloc(sizeof(Node));
        s->next = s; s->prev = s; queue = s;
    }
};
int main()
{
    Hover h;
    if (h.first || h.last || h.end || h.name[0] != 0 || h.queue->next != h.queue || h.queue->prev != h.queue || h.effect) { std::printf("008eac40_TEST FAIL\n"); return 1; }
    std::free(h.queue);
    std::printf("008eac40_TEST PASS\n");
    return 0;
}
