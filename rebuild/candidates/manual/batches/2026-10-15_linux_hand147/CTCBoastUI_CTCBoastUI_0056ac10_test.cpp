#include <cstdio>
#include <cstdlib>
static int g_reg = 0;
struct NodeBase { char color; NodeBase* parent; NodeBase* left; NodeBase* right; };
struct Node : NodeBase { void* first; unsigned char second[0x20]; };
struct Map { Node* header; unsigned int count;
    Map() : header(0) { header = (Node*)std::malloc(sizeof(Node)); count = 0; header->color = 0; header->parent = 0; header->left = header; header->right = header; } };
struct Vec { void** s; void** f; void** e; Vec() : s(0), f(0), e(0) {} };
struct TC { void* parent; TC(void* p) : parent(p) {} void AddToGlobalUpdateTCs() { ++g_reg; } };
struct Boast : TC { Map groups; bool active; Vec comps; bool quitting, allowed, requested, later;
    Boast(void* p) : TC(p), active(false), quitting(false), allowed(true), requested(false), later(false) { AddToGlobalUpdateTCs(); } };
int main()
{
    int thing; Boast b(&thing);
    if (b.parent != &thing || g_reg != 1 || b.groups.count != 0 || b.groups.header->left != b.groups.header || b.groups.header->right != b.groups.header
        || b.groups.header->parent != 0 || b.groups.header->color != 0 || b.active || b.comps.s != 0 || b.quitting || !b.allowed || b.requested || b.later)
    { std::printf("0056ac10_TEST FAIL\n"); return 1; }
    std::free(b.groups.header);
    std::printf("0056ac10_TEST PASS\n");
    return 0;
}
