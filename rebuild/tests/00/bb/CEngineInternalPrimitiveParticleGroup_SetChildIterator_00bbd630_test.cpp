// Behaviour test (0x00bbd630): CEngineInternalPrimitiveParticleGroup::SetChildIterator stores a valid iterator for one primitive id
#include <cstdio>
struct CListNode { CListNode* Next; CListNode* Prev; };
struct CListIterator { CListNode* _M_node; CListIterator(CListNode* n) : _M_node(n) { } CListIterator(const CListIterator& o) : _M_node(o._M_node) { } };
class CPrimitiveIterator {
public:
    bool          Valid;
    CListIterator Iterator;
    CPrimitiveIterator() : Valid(false), Iterator(0) { }
    CPrimitiveIterator(CListIterator it) : Valid(true), Iterator(it) { }
};
class CIteratorArray {
public:
    CPrimitiveIterator* _M_start; CPrimitiveIterator* _M_finish; CPrimitiveIterator* _M_end_of_storage;
    CPrimitiveIterator& operator[](unsigned int i) { return *(_M_start + i); }
};
class CEngineInternalPrimitiveParticleGroup {
public:
    CIteratorArray PrimitiveIDToIterator;
    void SetChildIterator(unsigned int id, CListIterator it) { PrimitiveIDToIterator[id] = CPrimitiveIterator(it); }
};
int main()
{
    CListNode nodes[2];
    CPrimitiveIterator slots[4];
    CEngineInternalPrimitiveParticleGroup g;
    g.PrimitiveIDToIterator._M_start = slots; g.PrimitiveIDToIterator._M_finish = slots + 4; g.PrimitiveIDToIterator._M_end_of_storage = slots + 4;
    g.SetChildIterator(3, CListIterator(&nodes[1]));
    if (!slots[3].Valid || slots[3].Iterator._M_node != &nodes[1] || slots[0].Valid || slots[2].Valid) { std::printf("00bbd630_TEST FAIL\n"); return 1; }
    g.SetChildIterator(3, CListIterator(&nodes[0]));
    if (!slots[3].Valid || slots[3].Iterator._M_node != &nodes[0]) { std::printf("00bbd630_TEST FAIL\n"); return 1; }
    std::printf("00bbd630_TEST PASS\n");
    return 0;
}