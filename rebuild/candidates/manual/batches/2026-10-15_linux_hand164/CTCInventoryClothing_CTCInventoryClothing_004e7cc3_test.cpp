#include <cstdio>
// Behaviour model of CTCInventoryClothing::CTCInventoryClothing (0x004E7CC3): members start empty / null / false.
struct Vec { void* a; void* b; void* c; Vec() : a(0), b(0), c(0) {} };
struct Map { int count; Map() : count(0) {} };
struct Clothing {
    void* thing; Vec entries; Map selected; Map morphs; void* mannequin; bool useMannequin; bool loaded; Map items; void* list;
    Clothing(void* t) : thing(t), mannequin(0), useMannequin(false), loaded(false), list(0) {}
};
int main()
{
    int dummy;
    Clothing c(&dummy);
    if (c.thing != &dummy || c.entries.a || c.entries.b || c.entries.c || c.mannequin || c.useMannequin || c.loaded || c.list
        || c.selected.count || c.morphs.count || c.items.count) { std::printf("004e7cc3_TEST FAIL\n"); return 1; }
    std::printf("004e7cc3_TEST PASS\n");
    return 0;
}
