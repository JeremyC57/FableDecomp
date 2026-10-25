// Behaviour test (0x00bbd600): CEngineInternalPrimitiveParticleGroup::ClearChildIterator resets one id's iterator slot to the invalid default
#include <cstdio>
struct CListNode { CListNode* Next; CListNode* Prev; };
class CPrimitiveIterator {
public:
    bool       Valid;
    CListNode* Node;
    CPrimitiveIterator() : Valid(false), Node(0) { }
    CPrimitiveIterator(CListNode* n) : Valid(true), Node(n) { }
};
class CIteratorArray {
public:
    CPrimitiveIterator* _M_start; CPrimitiveIterator* _M_finish; CPrimitiveIterator* _M_end_of_storage;
    CPrimitiveIterator& operator[](unsigned int i) { return *(_M_start + i); }
};
class CEngineInternalPrimitiveParticleGroup {
public:
    CIteratorArray PrimitiveIDToIterator;
    void ClearChildIterator(unsigned int id) { PrimitiveIDToIterator[id] = CPrimitiveIterator(); }
};
int main()
{
    CListNode nodes[3];
    CPrimitiveIterator slots[3];
    for (int i = 0; i < 3; ++i) slots[i] = CPrimitiveIterator(&nodes[i]);
    CEngineInternalPrimitiveParticleGroup g;
    g.PrimitiveIDToIterator._M_start = slots; g.PrimitiveIDToIterator._M_finish = slots + 3; g.PrimitiveIDToIterator._M_end_of_storage = slots + 3;
    g.ClearChildIterator(1);
    if (slots[1].Valid || !slots[0].Valid || !slots[2].Valid || slots[2].Node != &nodes[2]) { std::printf("00bbd600_TEST FAIL\n"); return 1; }
    std::printf("00bbd600_TEST PASS\n");
    return 0;
}