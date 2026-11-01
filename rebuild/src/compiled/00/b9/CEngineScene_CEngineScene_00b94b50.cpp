// CEngineScene::~CEngineScene() -- engine scene destructor: clear the moving and non-persistent primitive lists, then the render-stats map, the scene name and the per-level map list, then the non-copyable base
// Leaf: ??1CEngineScene@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B94B50, 159 bytes, `jmp` to the base destructor.
// Identity (proven): the sole caller is the scalar deleting destructor 0x00B98A10, slot 0 of vtable 0x012A3E68 (RTTI
//   CEngineScene : CBaseClassNonCopyable); the body stores that vtable and ends in ~CBaseClassNonCopyable 0x0099A300 (direct
//   base, RTTI). Ego_r.pdb ??1CEngineScene@@UAE@XZ (0x00A47C40, beside its ctor ??0CEngineScene@@QAE@MKMMABVCCharString@@@Z
//   and ??_G 0x00A492C0), FableWin.pdb ??1CEngineScene@@UAE@XZ (0x02E1D720). The catalog label
//   CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip is a propagation.
// Callees: CEngineInternalPrimitiveBase::ClearPrimitiveList 0x00B8FEE0 (static __fastcall: Ego_r.pdb / FableWin.pdb
//   ?ClearPrimitiveList@CEngineInternalPrimitiveBase@@SIXPAV1@@Z) twice; std::_Rb_tree<EEngineRenderLayer, pair<const
//   EEngineRenderLayer, CEngineSceneRenderStats>, ...>::_M_erase 0x00B95EE0; ~CCharString 0x0099EAE0;
//   std::_Rb_tree<long, pair<const long, map<long, CCountedPointer<CEngineSceneMap> > >, ...>::_M_erase 0x00B98350; free
//   0x00BFEA14; CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300.
// Layout (Ego_r.pdb members at retail STLport sizes; no CEngineScene header; engine/CEngineInternalPrimitiveBase.h is a
//   flattened data struct without the static ClearPrimitiveList, so that callee's class is declared locally): MapList +0x04, MapSize +0x10, OneOverMapSize +0x14,
//   MaxMapLevels +0x18, MapsRenderLayerMask +0x1c, FirstMovingPrimitive +0x20, FirstNonPersistentPrimitive +0x24,
//   Name +0x28, RenderStats +0x2c, UpdateStats +0x38, CurrentOcclusionMask +0x44 .. PreloadFramesMoving +0x58 (0x5c bytes).
// Shape: user destructor (vtable store first) whose body clears the two primitive lists; then the members in reverse
//   order: ~map RenderStats, ~CCharString Name, ~map MapList (STLport clear() + header free, shared zero register).
extern "C" void __cdecl free(void* __p);                 // 0x00BFEA14 (allocator::deallocate)
namespace std {
template <class _Tp> class allocator {
public:
    typedef _Tp value_type;
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;                                // +0x00
    _Base_ptr   _M_parent;                               // +0x04
    _Base_ptr   _M_left;                                 // +0x08
    _Base_ptr   _M_right;                                // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                               // +0x10
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
protected:
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;   // +0x00
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
protected:
    typedef _Rb_tree_node<_Value> _Node;
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef unsigned int size_type;
protected:
    size_type _M_node_count;                             // +0x04
    _Compare  _M_key_compare;                            // +0x08
    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
private:
    void _M_erase(_Link_type __x);                       // out of line, one instantiation per tree type
public:
    ~_Rb_tree() { clear(); }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    _Rep_type _M_t;                                      // +0x00 (12 bytes)
};
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set {
public:
    typedef _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _Rep_type;
    _Rep_type _M_t;                                      // +0x00 (12 bytes)
};
} // namespace std

#include "engine/CBaseClassNonCopyable.h"               // ~CBaseClassNonCopyable 0x0099A300

class CCharStringData;
class CCharString {
public:
    ~CCharString();                                     // 0x0099EAE0
    CCharStringData* PStringData;                       // +0x00
};
#include "engine/CCountedPointer.h"                    // retail model (8 bytes: Data, Info)
class CEngineSceneMap;
enum EEngineRenderLayer { };
class CEngineSceneRenderStats {                         // Ego_r.pdb, 0x1C bytes
public:
    unsigned long StaticPrimitivesRendered;
    unsigned long MovingPrimitivesRendered;
    unsigned long StaticPrimitivesTouched;
    unsigned long MovingPrimitivesTouched;
    unsigned long VisibleCells;
    unsigned long BoundingBoxTests;
    unsigned long BoundingSphereTests;
};
class CEngineSceneUpdateStats {                         // Ego_r.pdb, 0xC bytes
public:
    unsigned long StaticPrimitivesAdded;
    unsigned long MovingPrimitivesAdded;
    unsigned long NonPersistentPrimitives;
};
class CEngineInternalPrimitiveBase {
public:
    static void __fastcall ClearPrimitiveList(CEngineInternalPrimitiveBase* first);   // 0x00B8FEE0
};

class CEngineScene : public CBaseClassNonCopyable {
public:
    CEngineScene(float mapSize, unsigned long maxMapLevels, float maxPrimitiveCellSizeRatio, float maxAveragePrimitivesPerCell, const CCharString& name);
    virtual ~CEngineScene();
    std::map<long, std::map<long, CCountedPointer<CEngineSceneMap> > > MapList;   // +0x04
    float                         MapSize;                      // +0x10
    float                         OneOverMapSize;               // +0x14
    unsigned long                 MaxMapLevels;                 // +0x18
    unsigned long                 MapsRenderLayerMask;          // +0x1c
    CEngineInternalPrimitiveBase* FirstMovingPrimitive;         // +0x20
    CEngineInternalPrimitiveBase* FirstNonPersistentPrimitive;  // +0x24
    CCharString                   Name;                         // +0x28
    std::map<EEngineRenderLayer, CEngineSceneRenderStats> RenderStats;   // +0x2c
    CEngineSceneUpdateStats       UpdateStats;                  // +0x38
    unsigned long                 CurrentOcclusionMask;         // +0x44
    unsigned long                 OcclusionGroupDelay;          // +0x48
    unsigned long                 OcclusionMinGroupSize;        // +0x4c
    float                         MaxPrimitiveCellSizeRatio;    // +0x50
    float                         MaxAveragePrimitivesPerCell;  // +0x54
    unsigned long                 PreloadFramesMoving;          // +0x58
};

CEngineScene::~CEngineScene()
{
    CEngineInternalPrimitiveBase::ClearPrimitiveList(FirstMovingPrimitive);
    CEngineInternalPrimitiveBase::ClearPrimitiveList(FirstNonPersistentPrimitive);
}