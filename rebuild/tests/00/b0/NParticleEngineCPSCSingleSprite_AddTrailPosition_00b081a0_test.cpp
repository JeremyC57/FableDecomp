#include <cstdio>
#include <cstdlib>

// Model of CPSCSingleSprite::AddTrailPosition: push_back onto an intrusive circular list, trim the front past MaxTrailLength.
struct Node { Node* next; Node* prev; float x, y, z, w; };
struct Trail {
    Node* head;
    unsigned int size() const { unsigned int n = 0; for (Node* p = head->next; p != head; p = p->next) ++n; return n; }
    void push_back(float x, float y, float z, float w)
    {
        Node* n = (Node*)std::malloc(sizeof(Node));
        n->x = x; n->y = y; n->z = z; n->w = w;
        Node* p = head->prev;
        n->next = head; n->prev = p; p->next = n; head->prev = n;
    }
    void pop_front() { Node* n = head->next; n->prev->next = n->next; n->next->prev = n->prev; std::free(n); }
};
struct Sprite {
    float TrailWidth;
    long MaxTrailLength;
    Trail trail;
    void AddTrailPosition(float x, float y, float z, float width)
    {
        trail.push_back(x, y, z, width);
        while (trail.size() > (unsigned long)MaxTrailLength) trail.pop_front();
    }
};

int main()
{
    Node sentinel; sentinel.next = sentinel.prev = &sentinel;
    Sprite s; s.TrailWidth = 2.0f; s.MaxTrailLength = 3; s.trail.head = &sentinel;
    for (int i = 0; i < 5; ++i) s.AddTrailPosition((float)i, 0.0f, 0.0f, s.TrailWidth);
    if (s.trail.size() != 3 || sentinel.next->x != 2.0f || sentinel.prev->x != 4.0f || sentinel.prev->w != 2.0f) { std::printf("00b081a0_TEST FAIL\n"); return 1; }
    s.MaxTrailLength = 1;
    s.AddTrailPosition(9.0f, 1.0f, 2.0f, 0.5f);
    if (s.trail.size() != 1 || sentinel.next->x != 9.0f || sentinel.next->z != 2.0f || sentinel.next->w != 0.5f) { std::printf("00b081a0_TEST FAIL\n"); return 1; }
    s.trail.pop_front();
    std::printf("00b081a0_TEST PASS\n");
    return 0;
}