#include <cstdio>
#include <cstdlib>
// Behaviour model of CTCOverheadDisplay::CTCOverheadDisplay (0x007B7BC0): three self-linked list sentinels, null def, empty vector.
struct Node { Node* next; Node* prev; long first; float second; };
struct List { Node* head; List() : head(0) { Node* s = (Node*)std::malloc(sizeof(Node)); s->next = s; s->prev = s; head = s; } ~List() { std::free(head); } };
struct Overhead { void* parent; List Damage, Repair, Renown; const void* PDef; void* v0; void* v1; void* v2;
                  Overhead(void* p) : parent(p), PDef(0), v0(0), v1(0), v2(0) {} };
int main()
{
    int t; Overhead o(&t);
    if (o.parent != &t || o.Damage.head->next != o.Damage.head || o.Renown.head->prev != o.Renown.head || o.PDef || o.v0 || o.v2) { std::printf("007b7bc0_TEST FAIL\n"); return 1; }
    std::printf("007b7bc0_TEST PASS\n");
    return 0;
}