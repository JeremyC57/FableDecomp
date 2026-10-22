// Grade: DIFFER(593v678) -- vertical twin of 0x00A793D0 (byte-identical shape, y walk); same aligned and esp,-8 frame from the C2DVector GetNodeAt temporaries inside the nested loops (GOTCHAS chunk 232 lane gap under 3077).
// CNavQuadTree::ConnectVerticalMapEdge(float x, float neighbourX, const CNavigatorManager& manager) -- walk this map's vertical edge in every navigation layer and, where a free node of this map faces a free node of the neighbouring map, record a connector between them; the walk steps by the larger/smaller node size (blocked/free) or half a unit where the neighbour has no node
// Leaf: ?ConnectVerticalMapEdge@CNavQuadTree@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A79680, 678 bytes (`ret 0Ch`).
// Callees: CNavigatorManager::GetNodeAt 0x00A14F70, CNavQuadTree::GetNodeAt 0x00A770B0, the node's slot 5 GetID (+0x14),
//   operator new 0x00BFEA1A, CNavConnectorNode(const CANavQuadTreeNode*, const CANavQuadTreeNode*) 0x00A78A90, its
//   CCountedPointer DeleteData 0x00A82FA0, vector<pair<long, CCountedPointer<CNavConnectorNode> > >::_M_insert_overflow
//   0x00A838F0, operator delete 0x00BFE9BC; the node-size table 0x0129DCAC (32 .. 0.5 by level), 0.25f 0x0123078C,
//   0.5f 0x0122F59C.
// Layout (Ego_r.pdb, packed): CNavQuadTree MapPos +7, MapWidth +0x10, MapHeight +0x14, NavigationLayers +0x1c (STLport,
//   0x28-byte CNavigationLayer), ConnectorNodes +0x74 (CVectorMap: vptr, vector +4, Compare +0x10, Dirty +0x11).
struct CCPPointerInfo {
    unsigned long RefCount;                              // +0x00
    void (__fastcall* DeleteFunc)(void* data);           // +0x04
    void*         Data;                                  // +0x08
    CCPPointerInfo(void* data, void (__fastcall* deleteFunc)(void*)) : RefCount(1), DeleteFunc(deleteFunc), Data(data) {}
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                // +0x00
    CCPPointerInfo* Info;                                // +0x04
    static void __fastcall DeleteData(void* data);
    explicit CCountedPointer(T* data) : Data(data)
    {
        if (data)
            Info = new CCPPointerInfo(data, DeleteData);
        else
            Info = 0;
    }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
};
namespace std {
struct __true_type {};
struct __false_type {};
template <class T> inline const T& (min)(const T& a, const T& b) { return b < a ? b : a; }
template <class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) {}
};
}
inline void* operator new(unsigned int, void* p) { return p; }
template <class T> struct CVector {                      // STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_insert_overflow(T* position, const T& x, const std::__false_type&, unsigned int fillLength, bool atEnd);
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, std::__false_type(), 1, true);
    }
};
struct CANavQuadTreeNode {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual long GetID() const;                          // slot 5 (+0x14)
    C2DVector     Position;                              // +0x04
    unsigned char QuadTreeLevel;                         // +0x0c
    unsigned char NavLayer;                              // +0x0d
    bool          LeafNode;                              // +0x0e
    bool          Blocked;                               // +0x0f
};
extern float g_NavNodeSizes[];                           // 0x0129DCAC
struct CNavConnectorNode {
    unsigned char _body[8];
    CNavConnectorNode(const CANavQuadTreeNode* node, const CANavQuadTreeNode* neighbour);   // 0x00A78A90
};
struct CNavigatorManager {
    CANavQuadTreeNode* GetNodeAt(const C2DVector& position, long layer) const;   // 0x00A14F70
};
template <class K, class V> struct CVectorMap {
    void*                      _vptr;                    // +0x00
    CVector<std::pair<K, V> >  Data;                     // +0x04
    unsigned char              Compare;                  // +0x10
    bool                       Dirty;                    // +0x11
    void Add(const K& key, const V& value)
    {
        Data.push_back(std::pair<K, V>(key, value));
        Dirty = true;
    }
};
struct CNavigationLayer { unsigned char _body[0x28]; };
#pragma pack(push, 1)
struct CNavQuadTree {
    void*         _vptr;                                 // +0x00
    bool          Initialised;                           // +0x04
    bool          Activated;                             // +0x05
    bool          Initialising;                          // +0x06
    C2DVector     MapPos;                                // +0x07 (packed C2DVector)
    unsigned char _pad0f;                                // +0x0f
    float         MapWidth;                              // +0x10
    float         MapHeight;                             // +0x14
    long          MapIndex;                              // +0x18
    CVector<CNavigationLayer> NavigationLayers;          // +0x1c
    unsigned char _pad28[0x74 - 0x28];                   // TransitionPoints .. RegionSeeds, Regions
    CVectorMap<long, CCountedPointer<CNavConnectorNode> > ConnectorNodes;   // +0x74
    CANavQuadTreeNode* GetNodeAt(const C2DVector& position, long layer) const;   // 0x00A770B0
    void ConnectVerticalMapEdge(float x, float neighbourX, const CNavigatorManager& manager);
};
#pragma pack(pop)

void CNavQuadTree::ConnectVerticalMapEdge(float x, float neighbourX, const CNavigatorManager& manager)
{
    C2DVector start = MapPos;
    C2DVector end(MapPos.X + MapWidth, MapPos.Y + MapHeight);
    for (unsigned int layer = 0; layer < NavigationLayers.size(); ++layer) {
        float y = start.Y + 0.25f;
        while (y < end.Y) {
            const CANavQuadTreeNode* neighbour = manager.GetNodeAt(C2DVector(neighbourX, y), layer);
            if (neighbour != 0) {
                float neighbourSize = g_NavNodeSizes[neighbour->QuadTreeLevel];
                const CANavQuadTreeNode* node = GetNodeAt(C2DVector(x, y), layer);
                if (!neighbour->Blocked && !node->Blocked) {
                    long id = (neighbour->GetID() >> 16) & 0xffff;
                    CCountedPointer<CNavConnectorNode> connector(new CNavConnectorNode(node, neighbour));
                    ConnectorNodes.Add(id, connector);
                    y += std::min(neighbourSize, g_NavNodeSizes[node->QuadTreeLevel]);
                }
                else if (neighbour->Blocked && node->Blocked)
                    y += std::max(neighbourSize, g_NavNodeSizes[node->QuadTreeLevel]);
                else if (neighbour->Blocked)
                    y += neighbourSize;
                else
                    y += g_NavNodeSizes[node->QuadTreeLevel];
            }
            else
                y += 0.5f;
        }
    }
}
