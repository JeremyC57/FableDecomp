#include <cstdio>
#include <cstdlib>
// Behaviour model of NUISystem::CTable::CTable: base built with the owner, the Sprites map header initialised empty
// (color 0, no root, left/right pointing at itself), both separation pointers null, then GetDataFromDef.
struct Node { char color; Node* parent; Node* left; Node* right; };
struct Table {
    long owner; Node* header; unsigned long count; void* h; void* v; int dataCalls;
    Table(long o) : owner(o), header(0), count(0), h(0), v(0), dataCalls(0)
    {
        header = (Node*)std::malloc(0x1c);
        header->color = 0; header->parent = 0; header->left = header; header->right = header;
        ++dataCalls;
    }
};
int main()
{
    Table t(42);
    if (t.owner != 42 || !t.header || t.header->left != t.header || t.header->right != t.header || t.header->parent || t.count || t.h || t.v || t.dataCalls != 1) {
        std::printf("005517e0_TEST FAIL\n"); return 1;
    }
    std::free(t.header);
    std::printf("005517e0_TEST PASS\n");
    return 0;
}