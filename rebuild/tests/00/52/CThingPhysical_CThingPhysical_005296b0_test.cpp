#include <cstdio>
#include <cstdlib>
// Behaviour model of CThingPhysical::CThingPhysical (0x005296B0).
struct Node { Node* next; Node* prev; };
struct Physical {
    unsigned int type; Node* head; void* lastCreator; void* originalCreator; long CanComeBetween;
    Physical(unsigned int t) : type(t), head(0), lastCreator(0), originalCreator(0), CanComeBetween(-1)
    { Node* s = (Node*)std::malloc(sizeof(Node)); s->next = s; s->prev = s; head = s; }
    ~Physical() { std::free(head); }
};
int main()
{
    Physical p(7);
    if (p.type != 7 || p.head->next != p.head || p.lastCreator || p.originalCreator || p.CanComeBetween != -1) { std::printf("005296b0_TEST FAIL\n"); return 1; }
    std::printf("005296b0_TEST PASS\n");
    return 0;
}