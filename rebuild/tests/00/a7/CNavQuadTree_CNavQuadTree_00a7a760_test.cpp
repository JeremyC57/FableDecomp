// behaviour test for 0x00A7A760 CNavQuadTree::CNavQuadTree
#include <cstdio>
#include <cstdlib>
struct CBase { int baseTag; CBase() : baseTag(1) {} };
template <class T> struct CRetailVector { T* s; T* f; T* e; CRetailVector() : s(0), f(0), e(0) {} };
template <class T> class CVectorMap { public: CRetailVector<T> m_entries; bool m_dirty; CVectorMap() : m_dirty(false) {} };
struct SNode { SNode* next; SNode* prev; int data[3]; };
struct CRetailList {
    SNode* m_node;
    CRetailList() : m_node(0) { SNode* head = (SNode*)std::malloc(sizeof(SNode)); head->next = head; head->prev = head; m_node = head; }
    ~CRetailList() { std::free(m_node); }
};
class CNavQuadTree : public CBase { public:
    bool Initialised, Activated, Initialising;
    CRetailVector<int> NavigationLayers, TransitionPoints; CRetailVector<unsigned char> TopologyWeights;
    CVectorMap<int> SwitchableNodes;
    CRetailVector<int> ActionPoints, DetailedAreas, RegionSeeds;
    CRetailList Regions;
    CVectorMap<int> ConnectorNodes;
    CNavQuadTree() : Initialised(false), Activated(false), Initialising(false) {}
};
int main()
{
    CNavQuadTree* t = new CNavQuadTree;
    int bad = t->baseTag != 1 || t->Initialised || t->Activated || t->Initialising || t->NavigationLayers.s != 0
        || t->TopologyWeights.e != 0 || t->SwitchableNodes.m_dirty || t->RegionSeeds.f != 0
        || t->Regions.m_node == 0 || t->Regions.m_node->next != t->Regions.m_node || t->Regions.m_node->prev != t->Regions.m_node
        || t->ConnectorNodes.m_dirty || t->ConnectorNodes.m_entries.s != 0;
    delete t;
    if (bad) { std::printf("00a7a760_TEST FAIL\n"); return 1; }
    std::printf("00a7a760_TEST PASS\n");
    return 0;
}