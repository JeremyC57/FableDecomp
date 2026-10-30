// CEngineScene::RenderSceneOcclusion(EEngineRenderLayer layer, const CEngineCamera& camera, EEngineOcclusionLayer occlusion_layer) -- render one layer into the occlusion pass: set the current occlusion mask from the occlusion layer, let every scene map's top grid level render its cells clipped to the camera (when the layer has grid-held primitives), then render the moving and non-persistent primitive lists, all into the layer's render stats
// Leaf: ?RenderSceneOcclusion@CEngineScene@@QAEXW4EEngineRenderLayer@@ABVCEngineCamera@@W4EEngineOcclusionLayer@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B94180, 283 bytes, thiscall `ret 0Ch`. No code or data reference reaches it in the
//   retail image (dead in TLC; it survives in the CEngineScene TU between its siblings).
// Identity: Ego_r.pdb names CEngineScene::RenderSceneOcclusion(EEngineRenderLayer, const CEngineCamera&, EEngineOcclusionLayer)
//   -- three arguments (`ret 0Ch`) -- and it is the only occlusion member that calls CEngineSceneGrid::
//   RenderCellClippedOcclusion (PDB: bool (unsigned long, unsigned long, EEngineRenderLayer, unsigned long, const
//   CEngineCamera&, bool, CEngineSceneRenderStats&); the unsigned long is CurrentOcclusionMask). Source order of the PDB
//   names: RenderScene 0x00B93F40, RenderSceneShadowBuffer 0x00B94060, this row, RenderSceneAndCheckVisibility 0x00B942A0,
//   RenderEntireScene 0x00B943F0 (the two siblings are landed as RenderLayerCells / RenderLayerCellsShadow; NAME_FIXES.tsv).
// Callees: map<EEngineRenderLayer, CEngineSceneRenderStats>::operator[] 0x00B97B60 (RenderStats, out of line),
//   CEngineSceneGrid::RenderCellClippedOcclusion 0x00B92790, CEngineScene::RenderPrimitiveList 0x00B91760 (twice).
// Layout (Ego_r.pdb names; the PDB's 16-byte Dinkumware maps are 12-byte STLport maps in retail, so members after MapList
//   sit 4 below the PDB, as the landed ctor 0x00B94A80 documents): CBaseClassNonCopyable vptr +0x00, MapList +0x04,
//   MapsRenderLayerMask +0x1C, FirstMovingPrimitive +0x20, FirstNonPersistentPrimitive +0x24, RenderStats +0x2C,
//   CurrentOcclusionMask +0x44. CEngineSceneMap: GridLevels (CArray<CCountedPointer<CEngineSceneGrid> >) +0x18.
//   No engine header exists for CEngineScene / CEngineSceneMap / CEngineSceneGrid (local models).
// Shape: two nested STLport map walks with the tree increment inlined (STLport 4.6 _Rb_global<bool>::_M_increment body
//   visible in the TU, as the landed 0x00AD94F0); CurrentOcclusionMask is re-read for every cell call. The out-of-line
//   map operator[] keeps its STLport body in the TU under __declspec(noinline) (the landed 0x00B97B60 shape): VC7.1 then
//   knows the key reference is only read, keeps `layer` in esi on the no-grid path and caches the camera in ebx for the
//   list renders (declared only, the layer was reloaded into other registers: 287v283); only RenderSceneOcclusion is graded.
#include "engine/CBaseClassNonCopyable.h"

enum EEngineRenderLayer { ENGINE_RENDER_LAYER_DUMMY = 0 };
enum EEngineOcclusionLayer { ENGINE_OCCLUSION_LAYER_DUMMY = 0 };

class CEngineCamera;
class CEngineSceneRenderStats {                                     // 28 bytes (Ego_r.pdb), as the landed operator[] 0x00B97B60
public:
    unsigned long StaticPrimitivesRendered;                          // +0x00
    unsigned long MovingPrimitivesRendered;                          // +0x04
    unsigned long StaticPrimitivesTouched;                           // +0x08
    unsigned long MovingPrimitivesTouched;                           // +0x0C
    unsigned long VisibleCells;                                      // +0x10
    unsigned long BoundingBoxTests;                                  // +0x14
    unsigned long BoundingSphereTests;                               // +0x18
    CEngineSceneRenderStats()
        : StaticPrimitivesRendered(0), MovingPrimitivesRendered(0), StaticPrimitivesTouched(0),
          MovingPrimitivesTouched(0), VisibleCells(0), BoundingBoxTests(0), BoundingSphereTests(0) { }
};
class CEngineInternalPrimitiveBase;
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;                                    // +0x00
};

namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; } };
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; typedef _Tp* pointer; };

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool _M_color;                                                   // +0x00
    _Base_ptr _M_parent;                                             // +0x04
    _Base_ptr _M_left;                                               // +0x08
    _Base_ptr _M_right;                                              // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                           // +0x10
};
template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static _Base_ptr _M_increment(_Base_ptr _M_node)
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        }
        else {
            _Base_ptr __y = _M_node->_M_parent;
            while (_M_node == __y->_M_right) {
                _M_node = __y;
                __y = __y->_M_parent;
            }
            if (_M_node->_M_right != __y)
                _M_node = __y;
        }
        return _M_node;
    }
};
typedef _Rb_global<bool> _Rb_global_inst;

struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    bool operator==(const _Rb_tree_base_iterator& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _Rb_tree_base_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) { _M_node = __it._M_node; }
    typename _Traits::reference operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    typename _Traits::pointer operator->() const { return &(operator*()); }
    _Rb_tree_iterator& operator++() { _M_node = _Rb_global_inst::_M_increment(_M_node); return *this; }
};

template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
protected:
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;  // +0x00
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
protected:
    unsigned int _M_node_count;                                      // +0x04
    _Compare _M_key_compare;                                         // +0x08
    static const _Key& _S_key(_Rb_tree_node_base* __x) { return _KeyOfValue()(((_Rb_tree_node<_Value>*)__x)->_M_value_field); }
    _Rb_tree_node_base*& _M_root() const { return this->_M_header._M_data->_M_parent; }
public:
    iterator begin() { return iterator(this->_M_header._M_data->_M_left); }
    iterator end() { return iterator(this->_M_header._M_data); }
    _Compare key_comp() const { return _M_key_compare; }
    iterator lower_bound(const _Key& __k)                           // _M_lower_bound inlined, as in the landed 0x00B97B60
    {
        _Rb_tree_node_base* __y = this->_M_header._M_data;          // last node which is not less than __k
        _Rb_tree_node_base* __x = _M_root();                        // current node
        while (__x != 0)
            if (!_M_key_compare(_S_key(__x), __k))
                __y = __x, __x = __x->_M_left;
            else
                __x = __x->_M_right;
        return iterator(__y);
    }
    iterator insert_unique(iterator __position, const _Value& __v);   // 0x00B97370 (RenderStats instance)
};

template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef _Key key_type;
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<key_type, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
private:
    _Rep_type _M_t;                                                  // 12 bytes
public:
    iterator begin() { return _M_t.begin(); }
    iterator end() { return _M_t.end(); }
    iterator lower_bound(const key_type& __k) { return _M_t.lower_bound(__k); }
    _Compare key_comp() const { return _M_t.key_comp(); }
    iterator insert(iterator __position, const value_type& __v) { return _M_t.insert_unique(__position, __v); }
    // Out of line in retail (0x00B97B60, 163 bytes). Its STLport body is kept in the TU (noinline) so that, as in retail,
    // VC7.1 sees the key reference is only read and keeps `layer` live in esi on the no-grid path.
    __declspec(noinline) _Tp& operator[](const key_type& __k)
    {
        iterator __i = lower_bound(__k);
        // __i->first is greater than or equivalent to __k.
        if (__i == end() || key_comp()(__k, (*__i).first))
            __i = insert(__i, value_type(__k, _Tp()));
        return (*__i).second;
    }
};
}

template <class T> class CCountedPointer {                          // Ego_r.pdb: Data +0, Info +4
public:
    T* Data;
    void* Info;
    T* operator->() const { return Data; }
};
namespace std {
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                                   // +0x00
    _Tp* _M_finish;                                                  // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;          // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    _Tp& operator[](unsigned int __n) { return *(this->_M_start + __n); }
};
}
template <class T> class CArray : public std::vector<T> {           // retail CArray<T>: an STLport vector (12 bytes)
public:
    T& operator[](unsigned int i) { return std::vector<T>::operator[](i); }
};

class CEngineSceneGrid {
public:
    bool RenderCellClippedOcclusion(unsigned long x, unsigned long y, EEngineRenderLayer layer, unsigned long occlusion_mask,
                                    const CEngineCamera& camera, bool clipped, CEngineSceneRenderStats& stats);   // 0x00B92790
};

class CEngineSceneMap {                                              // Ego_r.pdb names, retail offsets
public:
    unsigned char _pad00[0x18];                                      // vptr +0x00, Area +0x04, MaxLevels +0x14 (unused here)
    CArray<CCountedPointer<CEngineSceneGrid> > GridLevels;           // +0x18
};

class CEngineSceneUpdateStats {                                      // 12 bytes in retail (unused here)
public:
    unsigned long Stat[3];
};

class CEngineScene : public CBaseClassNonCopyable {
public:
    void RenderSceneOcclusion(EEngineRenderLayer layer, const CEngineCamera& camera, EEngineOcclusionLayer occlusion_layer);
private:
    void RenderPrimitiveList(CEngineInternalPrimitiveBase* list, EEngineRenderLayer layer, const CEngineCamera& camera,
                             CEngineSceneRenderStats& stats);        // 0x00B91760
    std::map<long, std::map<long, CCountedPointer<CEngineSceneMap> > > MapList;   // +0x04
    float MapSize;                                                   // +0x10
    float OneOverMapSize;                                            // +0x14
    unsigned long MaxMapLevels;                                      // +0x18
    unsigned long MapsRenderLayerMask;                               // +0x1C
    CEngineInternalPrimitiveBase* FirstMovingPrimitive;              // +0x20
    CEngineInternalPrimitiveBase* FirstNonPersistentPrimitive;       // +0x24
    CCharString Name;                                                // +0x28
    std::map<EEngineRenderLayer, CEngineSceneRenderStats> RenderStats;   // +0x2C
    CEngineSceneUpdateStats UpdateStats;                             // +0x38
    unsigned long CurrentOcclusionMask;                              // +0x44
};

void CEngineScene::RenderSceneOcclusion(EEngineRenderLayer layer, const CEngineCamera& camera,
                                        EEngineOcclusionLayer occlusion_layer)
{
    CEngineSceneRenderStats& stats = RenderStats[layer];
    CurrentOcclusionMask = 1 << (occlusion_layer + 1);
    if (MapsRenderLayerMask & layer)
    {
        for (std::map<long, std::map<long, CCountedPointer<CEngineSceneMap> > >::iterator it = MapList.begin();
             it != MapList.end(); ++it)
        {
            for (std::map<long, CCountedPointer<CEngineSceneMap> >::iterator map_it = it->second.begin();
                 map_it != it->second.end(); ++map_it)
            {
                map_it->second->GridLevels[0]->RenderCellClippedOcclusion(0, 0, layer, CurrentOcclusionMask, camera, false,
                                                                         stats);
            }
        }
    }
    RenderPrimitiveList(FirstMovingPrimitive, layer, camera, stats);
    RenderPrimitiveList(FirstNonPersistentPrimitive, layer, camera, stats);
}