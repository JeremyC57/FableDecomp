// CEngineScene::CEngineScene(float mapSize, unsigned long maxMapLevels, float maxPrimitiveCellSizeRatio, float maxAveragePrimitivesPerCell, const CCharString& name) -- construct an engine scene: empty map list and render-stats map, zeroed update stats, the map size and its reciprocal, the level count, the cell-size limits, the default occlusion grouping and the scene name
// Leaf: ??0CEngineScene@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B94A80, 196 bytes (+12 int3 padding), `ret 14h`.
// Name: FableWin.pdb ??0CEngineScene@@QAE@MKMMABVCCharString@@@Z. The catalog label NHeroInformationScreens::CBase::Init is a
// propagated label: the body stores vtable 0x012A3E68 = RTTI CEngineScene (rtti-vtables.tsv) and takes the PDB ctor's five
// arguments -- see NOTES label corrections.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0, malloc @ 0x00BFEA0E (the two map header nodes:
// 0x20 bytes for map<long, map<long, CCountedPointer<CEngineSceneMap> > >, 0x30 for map<EEngineRenderLayer,
// CEngineSceneRenderStats>), CCharString::CCharString() @ 0x0099E4B0, CCharString::operator=(const CCharString&) @ 0x0099EFB0.
// Constant: 0x0122DED8 = 1.0f (OneOverMapSize = 1.0f / mapSize).
// Layout (Ego_r.pdb names; the PDB's 16-byte maps are 12 in retail, so members after MapList sit 4 below the PDB):
// MapList +0x04, MapSize +0x10, OneOverMapSize +0x14, MaxMapLevels +0x18, MapsRenderLayerMask +0x1C (not set here),
// FirstMovingPrimitive +0x20, FirstNonPersistentPrimitive +0x24, Name +0x28, RenderStats +0x2C, UpdateStats +0x38 (12 bytes),
// CurrentOcclusionMask +0x44 (not set), OcclusionGroupDelay +0x48, OcclusionMinGroupSize +0x4C, MaxPrimitiveCellSizeRatio
// +0x50, MaxAveragePrimitivesPerCell +0x54, PreloadFramesMoving +0x58 (not set).
// Shape: the two maps are the STLport red-black tree of the landed CShaderRenderManager ctor 0x0098AB00 (the base nulls
// the header, allocates it, the tree zeroes its count, `_M_empty_initialize` colours it red and links it to itself through
// re-reads of the header member); the scalars are body assignments, the name assigned last; the statement order is
// load-bearing (MaxAveragePrimitivesPerCell after the occlusion defaults puts its store after theirs).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

class CCharStringData;
class CCharString {
public:
    CCharString();                                           // 0x0099E4B0
    ~CCharString();
    CCharString& operator=(const CCharString& other);        // 0x0099EFB0
private:
    CCharStringData* PStringData;
};

struct CBaseClass { virtual ~CBaseClass(); };
struct CBaseClassNonCopyable : CBaseClass {
    CBaseClassNonCopyable();                                 // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();
};

template <class T> struct CAllocator {                      // STLport allocator<T>: empty
    CAllocator() { }
};
template <class T> struct CAllocProxy : CAllocator<T> {     // STLport _STLP_alloc_proxy
    T* m_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), m_data(p) { }
};

template <int ValueSize> struct SRbTreeNode {               // _Rb_tree_node<value_type>
    bool         _M_color;                                  // +0x00 (false = red)
    SRbTreeNode* _M_parent;                                 // +0x04
    SRbTreeNode* _M_left;                                   // +0x08
    SRbTreeNode* _M_right;                                  // +0x0c
    unsigned char _M_value_field[ValueSize];                // +0x10
};
template <int ValueSize> struct CRbTreeBase {               // STLport _Rb_tree_base
    CAllocProxy<SRbTreeNode<ValueSize> > _M_header;
    CRbTreeBase(const CAllocator<SRbTreeNode<ValueSize> >& alloc) : _M_header(alloc, 0)
    {
        _M_header.m_data = (SRbTreeNode<ValueSize>*)malloc(sizeof(SRbTreeNode<ValueSize>));
    }
};
struct CLess { CLess() { } };                               // std::less<>: empty
template <int ValueSize> struct CRbTree : CRbTreeBase<ValueSize> {   // STLport _Rb_tree / map (12 bytes)
    unsigned long _M_node_count;
    CLess         _M_key_compare;
    CRbTree() : CRbTreeBase<ValueSize>(CAllocator<SRbTreeNode<ValueSize> >()), _M_node_count(0)
    {
        this->_M_header.m_data->_M_color = false;
        this->_M_header.m_data->_M_parent = 0;
        this->_M_header.m_data->_M_left = this->_M_header.m_data;
        this->_M_header.m_data->_M_right = this->_M_header.m_data;
    }
};

struct CEngineSceneUpdateStats {                            // 12 bytes
    unsigned long StaticPrimitivesAdded;
    unsigned long MovingPrimitivesAdded;
    unsigned long NonPersistentPrimitives;
    CEngineSceneUpdateStats() : StaticPrimitivesAdded(0), MovingPrimitivesAdded(0), NonPersistentPrimitives(0) { }
};
struct CEngineInternalPrimitiveBase;

class CEngineScene : public CBaseClassNonCopyable {
public:
    CEngineScene(float mapSize, unsigned long maxMapLevels, float maxPrimitiveCellSizeRatio,
                 float maxAveragePrimitivesPerCell, const CCharString& name);
    virtual ~CEngineScene();
    CRbTree<0x10>                 MapList;                  // +0x04 map<long, map<long, CCountedPointer<CEngineSceneMap> > >
    float                         MapSize;                  // +0x10
    float                         OneOverMapSize;           // +0x14
    unsigned long                 MaxMapLevels;             // +0x18
    unsigned long                 MapsRenderLayerMask;      // +0x1c
    CEngineInternalPrimitiveBase* FirstMovingPrimitive;     // +0x20
    CEngineInternalPrimitiveBase* FirstNonPersistentPrimitive;  // +0x24
    CCharString                   Name;                     // +0x28
    CRbTree<0x20>                 RenderStats;              // +0x2c map<EEngineRenderLayer, CEngineSceneRenderStats>
    CEngineSceneUpdateStats       UpdateStats;              // +0x38
    unsigned long                 CurrentOcclusionMask;     // +0x44
    unsigned long                 OcclusionGroupDelay;      // +0x48
    unsigned long                 OcclusionMinGroupSize;    // +0x4c
    float                         MaxPrimitiveCellSizeRatio;    // +0x50
    float                         MaxAveragePrimitivesPerCell;  // +0x54
    unsigned long                 PreloadFramesMoving;      // +0x58
};

CEngineScene::CEngineScene(float mapSize, unsigned long maxMapLevels, float maxPrimitiveCellSizeRatio,
                           float maxAveragePrimitivesPerCell, const CCharString& name)
{
    MapSize = mapSize;
    OneOverMapSize = 1.0f / mapSize;
    MaxMapLevels = maxMapLevels;
    MaxPrimitiveCellSizeRatio = maxPrimitiveCellSizeRatio;
    FirstMovingPrimitive = 0;
    FirstNonPersistentPrimitive = 0;
    OcclusionGroupDelay = 2;
    OcclusionMinGroupSize = 1;
    MaxAveragePrimitivesPerCell = maxAveragePrimitivesPerCell;
    Name = name;
}