#include <cstdio>
// MinionSitOnSeatBase::Cleanup: when the group uses a gather point and the owner is alive, RemoveUser(creature)
struct Thing { unsigned char flags91; bool hasGather; void* removed; void RemoveUser(void* p) { removed = p; } };
struct Ptr { Thing* item; Thing* GetPItem() const { return item; } };
struct Group {
    void* creature; Ptr owner; bool usesGather;
    void Cleanup()
    {
        if (usesGather) {
            if (owner.GetPItem() != 0 && !(owner.GetPItem()->flags91 & 1)) {
                if (owner.GetPItem()->hasGather) owner.GetPItem()->RemoveUser(creature);
            }
        }
    }
};
int main()
{
    int me; Thing t; t.flags91 = 0; t.hasGather = true; t.removed = 0; Group g; g.creature = &me; g.owner.item = &t; g.usesGather = true;
    g.Cleanup(); if (t.removed != &me) { std::printf("0090fa60_TEST FAIL\n"); return 1; }
    t.removed = 0; t.hasGather = false; g.Cleanup(); if (t.removed != 0) { std::printf("0090fa60_TEST FAIL\n"); return 1; }
    t.hasGather = true; t.flags91 = 1; g.Cleanup(); if (t.removed != 0) { std::printf("0090fa60_TEST FAIL\n"); return 1; }
    t.flags91 = 0; g.usesGather = false; g.Cleanup(); if (t.removed != 0) { std::printf("0090fa60_TEST FAIL\n"); return 1; }
    g.usesGather = true; g.owner.item = 0; g.Cleanup(); if (t.removed != 0) { std::printf("0090fa60_TEST FAIL\n"); return 1; }
    std::printf("0090fa60_TEST PASS\n"); return 0;
}