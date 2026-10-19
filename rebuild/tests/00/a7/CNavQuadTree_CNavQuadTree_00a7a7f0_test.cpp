// Behaviour test for CNavQuadTree::~CNavQuadTree (0x00A7A7F0): ClearNodes, then the members in reverse order, then the base.
#include <cstdio>
#include <cstring>
static char g_log[512];
static void LOG(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static bool g_armed;
static void* g_freed[16]; static int g_nfreed;
static void spy_free(void* p) { if (g_armed) g_freed[g_nfreed++] = p; }   /* records only, never frees */
struct __false_type { __false_type() {} };
class CBaseClassNonCopyable { public: virtual ~CBaseClassNonCopyable() { LOG("base"); } };
struct CNavigationLayer { int x; };
struct CNavConnectorNodePair { int x; };
struct Pod { int x; };
void _Destroy_range(CNavigationLayer* first, CNavigationLayer* last, const __false_type&) { LOG(last - first == 2 ? "layers" : "layers?"); }
void _Destroy_range(CNavConnectorNodePair* first, CNavConnectorNodePair* last, const __false_type&) { LOG(last - first == 1 ? "connectors" : "connectors?"); }
template <class T> struct vector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    ~vector() { _Destroy_range(_M_start, _M_finish, __false_type()); if (_M_start) spy_free(_M_start); }
};
template <class T> struct pod_vector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    ~pod_vector() { if (_M_start) spy_free(_M_start); }
};
template <class V> struct CVectorMap { V Data; long Compare; };
struct CRetailList_ListOfCountedPointer {
    void* head;
    void clear() { LOG("regions"); }
    ~CRetailList_ListOfCountedPointer() { clear(); if (head) spy_free(head); }
};
class CNavQuadTree : public CBaseClassNonCopyable {
public:
    vector<CNavigationLayer> NavigationLayers;
    pod_vector<Pod> TransitionPoints;
    pod_vector<Pod> TopologyWeights;
    CVectorMap<pod_vector<Pod> > SwitchableNodes;
    pod_vector<Pod> ActionPoints;
    pod_vector<Pod> DetailedAreas;
    pod_vector<Pod> RegionSeeds;
    CRetailList_ListOfCountedPointer Regions;
    CVectorMap<vector<CNavConnectorNodePair> > ConnectorNodes;
    void ClearNodes() { LOG("clearnodes"); }
    virtual ~CNavQuadTree() { ClearNodes(); }
};
int main()
{
    static CNavigationLayer layers[2]; static CNavConnectorNodePair conn[1]; static Pod a[1], b[1], c[1], d[1], e[1], f[1]; static int head;
    {
        CNavQuadTree t;
        t.NavigationLayers._M_start = layers; t.NavigationLayers._M_finish = layers + 2;
        t.TransitionPoints._M_start = a; t.TopologyWeights._M_start = 0; t.SwitchableNodes.Data._M_start = b;
        t.ActionPoints._M_start = c; t.DetailedAreas._M_start = d; t.RegionSeeds._M_start = e;
        t.Regions.head = &head; t.ConnectorNodes.Data._M_start = conn; t.ConnectorNodes.Data._M_finish = conn + 1;
        (void)f;
        g_log[0] = 0; g_nfreed = 0; g_armed = true;
    }
    g_armed = false;
    if (std::strcmp(g_log, "clearnodes;connectors;regions;layers;base;") != 0) { std::printf("00a7a7f0_TEST FAIL\n"); return 1; }
    void* want[8] = { conn, &head, e, d, c, b, a, layers };
    if (g_nfreed != 8) { std::printf("00a7a7f0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 8; ++i) if (g_freed[i] != want[i]) { std::printf("00a7a7f0_TEST FAIL\n"); return 1; }
    std::printf("00a7a7f0_TEST PASS\n");
    return 0;
}