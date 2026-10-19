#include <cstdio>
struct Node { Node* next; Node* prev; long frames; };
static void release(Node* n) { delete n; }
static void tick(Node* sentinel, float expiry) {
    Node* n=sentinel->next;
    while(n!=sentinel) { --n->frames; if((float)n->frames <= -expiry) { Node* next=n->next; Node* prev=n->prev; prev->next=next; next->prev=prev; release(n); n=next; } else n=n->next; }
}
int main() {
    Node s={&s,&s,0}; Node* a=new Node(); a->next=&s; a->prev=&s; a->frames=0; s.next=a; s.prev=a;
    tick(&s,1.0f);
    if(s.next!=&s || s.prev!=&s) { std::printf("007b7a70_TEST FAIL\n"); return 1; }
    std::printf("007b7a70_TEST PASS\n"); return 0;
}
