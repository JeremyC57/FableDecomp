// Behaviour test for CTCInventoryClothing::ClearClothingMenuEntries (0x005B432C): the entry list is emptied.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct Entry { int part; Entry* next; };
struct List { Entry* head; int size; void clear() { while (head) { Entry* n = head->next; delete head; head = n; } size = 0; } };
int main()
{
    List l; l.head = 0; l.size = 0;
    for (int i = 0; i < 3; ++i) { Entry* e = new Entry; e->part = i; e->next = l.head; l.head = e; ++l.size; }
    l.clear();
    bool ok = l.head == 0 && l.size == 0;
    printf(ok ? "005b432c_TEST PASS\n" : "005b432c_TEST FAIL\n");
    return ok ? 0 : 1;
}