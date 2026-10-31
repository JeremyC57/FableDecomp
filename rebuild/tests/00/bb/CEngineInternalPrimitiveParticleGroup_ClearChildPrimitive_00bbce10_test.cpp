// Behaviour test for CEngineInternalPrimitiveParticleGroup::ClearChildPrimitive (0x00BBCE10): for an in-range, in-use id,
// erase its handle from the primitive list, mark the iterator slot unused, then remove the id from the free list and append
// it (so it appears exactly once, at the back); out-of-range or unused ids change nothing.
#include <stdio.h>
#include <list>
#include <vector>
struct Iter { bool used; std::list<int>::iterator node; };
struct Group {
    std::list<int> Primitives; std::vector<Iter> PrimitiveIDToIterator; std::list<long> FreePrimitiveIDs;
    void ClearChildPrimitive(unsigned long id)
    {
        if (id < PrimitiveIDToIterator.size()) {
            if (PrimitiveIDToIterator[id].used) {
                Primitives.erase(PrimitiveIDToIterator[id].node);
                PrimitiveIDToIterator[id].used = false;
                FreePrimitiveIDs.remove(id);
                FreePrimitiveIDs.push_back(id);
            }
        }
    }
};
int main()
{
    Group g; g.PrimitiveIDToIterator.resize(3);
    for (int i = 0; i < 3; ++i) { g.Primitives.push_back(i * 10); g.PrimitiveIDToIterator[i].used = true; g.PrimitiveIDToIterator[i].node = --g.Primitives.end(); }
    g.FreePrimitiveIDs.push_back(1); g.FreePrimitiveIDs.push_back(7);
    g.ClearChildPrimitive(1);
    if (g.Primitives.size() != 2 || g.PrimitiveIDToIterator[1].used || g.FreePrimitiveIDs.size() != 2 || g.FreePrimitiveIDs.back() != 1 || g.FreePrimitiveIDs.front() != 7) { printf("00bbce10_TEST FAIL\n"); return 1; }
    g.ClearChildPrimitive(1); g.ClearChildPrimitive(5);
    if (g.Primitives.size() != 2 || g.FreePrimitiveIDs.size() != 2) { printf("00bbce10_TEST FAIL\n"); return 1; }
    g.ClearChildPrimitive(2);
    if (g.Primitives.size() != 1 || g.Primitives.front() != 0 || g.FreePrimitiveIDs.back() != 2) { printf("00bbce10_TEST FAIL\n"); return 1; }
    printf("00bbce10_TEST PASS\n"); return 0;
}