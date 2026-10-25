// Test for 0x006C20A0 CLevelLoader::IsBusy: the source object must report false while the object list's sentinel points
// to itself and true once a node is linked in.
#include <stdio.h>
#include <string.h>
struct Node { Node* next; Node* prev; void* data[2]; };
class CLevelLoader {
public:
    bool IsBusy() const;
};
int main()
{
    unsigned char loader[0x18];
    Node sentinel, item;
    memset(loader, 0, sizeof(loader));
    Node* head = &sentinel;
    memcpy(loader + 0x14, &head, 4);
    sentinel.next = sentinel.prev = &sentinel;
    const CLevelLoader* l = (const CLevelLoader*)loader;
    bool ok = !l->IsBusy();
    sentinel.next = sentinel.prev = &item; item.next = item.prev = &sentinel;
    ok = ok && l->IsBusy();
    if (!ok) { puts("006c20a0_TEST FAIL"); return 1; }
    puts("006c20a0_TEST PASS");
    return 0;
}
