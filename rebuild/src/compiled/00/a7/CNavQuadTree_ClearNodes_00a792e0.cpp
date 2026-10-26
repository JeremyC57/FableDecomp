// CNavQuadTree::ClearNodes() -- empty the connector map, then for every navigation layer delete its quad-tree nodes (except the shared blocked node) and release the layer's node array storage
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A792E0, 238 bytes (+ int3 padding). __fastcall, `ret`.
// Callees: the connector map's clear 0x00A83FD0 (CVectorMap<long, CCountedPointer<CNavConnectorNode> >; manifest label
//   "Vector_DestroyAndClear_ParticleEmitter" is a propagated guess), each node's virtual deleting dtor (slot 0, `push 1`),
//   memmove 0x00BFEAE6 (the SGI erase of the empty tail), free 0x00BFEA14.
// Layout: Ego_r.pdb CNavQuadTree with STLport 12-byte vectors: NavigationLayers +0x1c (40-byte CNavQuadTree::CNavigationLayer:
//   CArray<CANavQuadTreeNode*> Nodes +0, LineLists +0xc, SwitchableLineLists +0x18), ..., Regions +0x74 (4-byte list),
//   ConnectorNodes +0x78. The shared blocked node is the static at 0x013D2D2C (never deleted; a dozen nav routines compare to it).
// Shape: indexed walk over the layers (size re-read each step, the byte offset strength-reduced into a frame slot); the node walk
//   caches end(); the array release is `if (capacity()) { clear(); free the block and null all three pointers }` with clear()
//   the landed CShaderDataBank::CloseShaders 0x00A62050 erase(begin(), end()).
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6
extern "C" void __cdecl free(void* block);                          // 0x00BFEA14
namespace std { template <class T> inline void swap(T& a, T& b) { T tmp = a; a = b; b = tmp; } }
template <class T> class CArray { public:                  // retail CArray (STLport vector): first / last / end
    T* m_first;
    T* m_last;
    T* m_end;
    static T* CopyTrivial(const T* first, const T* last, T* result)
    {
        if (first == last)
            return result;
        unsigned int n = (const char*)last - (const char*)first;
        return (T*)((char*)memmove(result, first, n) + n);
    }
    unsigned long size() const { return m_last - m_first; }
    unsigned long capacity() const { return m_end - m_first; }
    T* begin() { return m_first; }
    T* end() { return m_last; }
    T* erase(T* first, T* last)
    {
        T* i = CopyTrivial(last, m_last, first);
        m_last = i;
        return first;
    }
    void clear() { erase(begin(), end()); }
    CArray() : m_first(0), m_last(0), m_end(0) {}
    ~CArray() { if (m_first) free(m_first); }
    void swap(CArray& other)
    {
        std::swap(m_first, other.m_first);
        std::swap(m_last, other.m_last);
        std::swap(m_end, other.m_end);
    }
    void ClearAndFree()                             // clear, then swap with an empty temporary whose dtor frees the block
    {
        if (capacity() > 0) {
            clear();
            CArray empty;
            swap(empty);
        }
    }
};
template <class T> struct vector {                  // STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) { return *(_M_start + n); }
};
class CANavQuadTreeNode {
public:
    virtual ~CANavQuadTreeNode();                   // slot 0 (deleting destructor)
};
class CNavBlockedQuadTreeNode : public CANavQuadTreeNode {
};
extern CNavBlockedQuadTreeNode g_BlockedNavNode;    // 0x013D2D2C
struct CNavConnectorNodeMap {                       // CVectorMap<long, CCountedPointer<CNavConnectorNode> > (16 bytes)
    unsigned char _body[0x10];
    void clear();                                   // 0x00A83FD0
};
class CNavQuadTree {
public:
    class CNavigationLayer { public:                       // 40 bytes
        CArray<CANavQuadTreeNode*> Nodes;           // +0x00
        unsigned char LineLists[0xc];               // +0x0c vector<list<C2DLineF> >
        unsigned char SwitchableLineLists[0x10];    // +0x18 CVectorMap<unsigned __int64, list<C2DLineF> >
    };
    void*         __vftable;                        // +0x00
    bool          Initialised;                      // +0x04
    bool          Activated;                        // +0x05
    bool          Initialising;                     // +0x06
    unsigned char _pad07[0x15];                     // MapPos, MapWidth, MapHeight, MapIndex
    vector<CNavigationLayer> NavigationLayers;      // +0x1c
    unsigned char _pad28[0x50];                     // TransitionPoints .. Regions
    CNavConnectorNodeMap ConnectorNodes;            // +0x78
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