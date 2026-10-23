// 00a792e0 behaviour test: CNavQuadTree::ClearNodes (match.cpp body; the node array is released by swapping with an empty
// temporary through _STL::swap), host stand-ins. C++03, g++ -w; also VC7.1 /Od.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
static int g_frees = 0;
static void* g_lastFreed = 0;
static void host_free(void* p) { ++g_frees; g_lastFreed = p; free(p); }

namespace _STL { template <class T> inline void swap(T& a, T& b) { T tmp = a; a = b; b = tmp; } }
template <class T> struct CArray {
    T* m_first;
    T* m_last;
    T* m_end;
    static T* CopyTrivial(const T* first, const T* last, T* result)
    {
        if (first == last)
            return result;
        unsigned int n = (unsigned int)((const char*)last - (const char*)first);
        return (T*)((char*)memmove(result, first, n) + n);
    }
    unsigned long size() const { return (unsigned long)(m_last - m_first); }
    unsigned long capacity() const { return (unsigned long)(m_end - m_first); }
    T* begin() { return m_first; }
    T* end() { return m_last; }
    T* erase(T* first, T* last) { T* i = CopyTrivial(last, m_last, first); m_last = i; return first; }
    void clear() { erase(begin(), end()); }
    CArray() : m_first(0), m_last(0), m_end(0) {}
    ~CArray() { if (m_first) host_free(m_first); }
    void swap(CArray& other)
    {
        _STL::swap(m_first, other.m_first);
        _STL::swap(m_last, other.m_last);
        _STL::swap(m_end, other.m_end);
    }
    void ClearAndFree()
    {
        if (capacity() > 0) {
            clear();
            CArray empty;
            swap(empty);
        }
    }
};
template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) { return *(_M_start + n); }
};
static int g_deleted = 0;
class CANavQuadTreeNode {
public:
    virtual ~CANavQuadTreeNode() { ++g_deleted; }
};
class CNavBlockedQuadTreeNode : public CANavQuadTreeNode { };
static CNavBlockedQuadTreeNode g_BlockedNavNode;
static int g_mapClears = 0;
struct CNavConnectorNodeMap { void clear() { ++g_mapClears; } };
class CNavQuadTree {
public:
    struct CNavigationLayer { CArray<CANavQuadTreeNode*> Nodes; };
    vector<CNavigationLayer> NavigationLayers;
    CNavConnectorNodeMap ConnectorNodes;
    void ClearNodes();
};
void CNavQuadTree::ClearNodes()
{
    ConnectorNodes.clear();
    for (unsigned long i = 0; i < NavigationLayers.size(); ++i) {
        CNavigationLayer& layer = NavigationLayers[i];
        CANavQuadTreeNode** end = layer.Nodes.end();
        for (CANavQuadTreeNode** it = layer.Nodes.begin(); it != end; ++it) {
            CANavQuadTreeNode* node = *it;
            if (node && node != &g_BlockedNavNode)
                delete node;
        }
        layer.Nodes.ClearAndFree();
    }
}

int main()
{
    CNavQuadTree::CNavigationLayer layers[3];
    // layer 0: three real nodes, one null, one blocked; capacity 6
    CANavQuadTreeNode** a = (CANavQuadTreeNode**)malloc(6 * sizeof(CANavQuadTreeNode*));
    a[0] = new CANavQuadTreeNode; a[1] = 0; a[2] = &g_BlockedNavNode; a[3] = new CANavQuadTreeNode; a[4] = new CANavQuadTreeNode;
    layers[0].Nodes.m_first = a; layers[0].Nodes.m_last = a + 5; layers[0].Nodes.m_end = a + 6;
    // layer 1: empty with no storage (capacity 0: nothing freed)
    // layer 2: storage but no nodes
    CANavQuadTreeNode** c = (CANavQuadTreeNode**)malloc(2 * sizeof(CANavQuadTreeNode*));
    layers[2].Nodes.m_first = c; layers[2].Nodes.m_last = c; layers[2].Nodes.m_end = c + 2;
    CNavQuadTree t;
    t.NavigationLayers._M_start = layers; t.NavigationLayers._M_finish = layers + 3; t.NavigationLayers._M_end_of_storage = layers + 3;
    t.ClearNodes();
    CHECK(g_mapClears == 1);
    CHECK(g_deleted == 3);
    CHECK(g_frees == 2);
    CHECK(g_lastFreed == (void*)c);
    for (int i = 0; i < 3; ++i) {
        CHECK(layers[i].Nodes.m_first == 0);
        CHECK(layers[i].Nodes.m_last == 0);
        CHECK(layers[i].Nodes.m_end == 0);
    }
    t.NavigationLayers._M_finish = t.NavigationLayers._M_start;   // no layers: only the map is cleared
    t.ClearNodes();
    CHECK(g_mapClears == 2);
    CHECK(g_frees == 2);
    if (!g_fail) printf("00a792e0_TEST PASS\n");
    return g_fail;
}