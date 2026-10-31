// GetRelevantBuilding(map<CThingBuilding*, list<CThingAICreature*> >& buildings, ESimBuildingProperty property, bool checkWorkSpaces, bool checkLiveSpaces) -- the building with the given sim property and room left (work or live spaces) that has the fewest assigned creatures
// Grade: DIFFER(337v337) 4 difflines (was 6) -- sec1b 2026-10-30: moved onto engine/CDefPointer.h (FABLE_RETAIL_DEF_CLASS_BASE) and begin() declared before relevantBuilding, which fixes the head order of the xor; left: the end/relevant frame stores swap (retail relevant [esp+0Ch] before end [esp+18h]) and the `mov esi,[esp+10h]` reload is hoisted above the jae (retail duplicates it on both paths). Tried also: end-before-relevant order (same), uncached end (12). Next: the if-assignment shape of the fewer test.
// Leaf: ?GetRelevantBuilding@@YIPAVCThingBuilding@@AAV?$map@PAVCThingBuilding@@V?$list@PAVCThingAICreature@@V?$allocator@PAVCThingAICreature@@@std@@@std@@U?$less@PAVCThingBuilding@@@3@V?$allocator@U?$pair@QAVCThingBuilding@@V?$list@PAVCThingAICreature@@V?$allocator@PAVCThingAICreature@@@std@@@std@@@std@@@3@@std@@W4ESimBuildingProperty@@_N2@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005029A0, 337 bytes, __fastcall (ecx = map, edx = property), `ret 8` (the two bools).
// Identity (proven): Ego_r.pdb ?GetRelevantBuilding@@YIPAVCThingBuilding@@AAV?$map@...@W4ESimBuildingProperty@@_N2@Z (a
//   fastcall free function: map&, property, bool, bool -> CThingBuilding*), donor order GetRelevantBuilding <
//   AddToRelevantBuilding < CWorldMap::PopulateRegionWithVillagers, here 0x005029A0 < 0x00502B00 (its caller, passing the
//   property masks 4 / 8) < 0x00502E90; the body tests CSimBuildingDef::Property (retail +0x3C) and compares the creature
//   list size with WorkSpaces (+0x40) / LiveSpaces (+0x44) -- Ego_r.pdb +0x40 / +0x44 / +0x48 less 4.
// Callees: CThingBuilding::PeekPSimDefinition() const 0x0082D2D0 (CDefPointer<const CSimBuildingDef> by value),
//   map<CThingBuilding*, list<CThingAICreature*> >::operator[](const key_type&) 0x0051BF90 (twice); the CDefPointer release
//   (slot 1 Delete) and the STLport tree increment / list size walks are inline.
// Label correction: catalogued as the gap label "CBuilding_FindMostSpaceForNPCs".
// Parameter names are descriptive (the PDB carries none).
class CThingAICreature;
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
enum ESimBuildingProperty { };
class CSimBuildingDef : public CDefPointeeBase {
public:
    unsigned char _pad_0x1c[0x20];
    long Property;                                          // +0x3C
    long WorkSpaces;                                        // +0x40
    long LiveSpaces;                                        // +0x44
};
class CThingBuilding {
public:
    CDefPointer<const CSimBuildingDef> PeekPSimDefinition() const;   // 0x0082D2D0
};
namespace std {
template <class _Tp> class allocator { };
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Pair> struct _Select1st { };
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef unsigned int size_type;
    size_type size() const
    {
        size_type __result = 0;
        for (_List_node_base* __it = _M_node->_M_next; __it != _M_node; __it = __it->_M_next)
            ++__result;
        return __result;
    }
    _List_node_base* _M_node;
};
struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;
    _Base_ptr _M_parent;
    _Base_ptr _M_left;
    _Base_ptr _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static _Rb_tree_node_base* __cdecl _M_increment(_Rb_tree_node_base* _M_node)
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
template <class _Value> struct _Rb_tree_iterator {
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
    _Value& operator*() const { return _Link_type(_M_node)->_M_value_field; }
    _Value* operator->() const { return &(operator*()); }
    _Rb_tree_iterator& operator++() { _M_node = _Rb_global_inst::_M_increment(_M_node); return *this; }
    bool operator!=(const _Rb_tree_iterator& __y) const { return _M_node != __y._M_node; }
    _Rb_tree_node_base* _M_node;
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > > class map {
public:
    typedef _Key key_type;
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree_iterator<value_type> iterator;
    iterator begin() { return iterator(_M_header->_M_left); }
    iterator end() { return iterator(_M_header); }
    _Tp& operator[](const key_type& __k);                   // 0x0051BF90
    _Rb_tree_node_base* _M_header;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
};
}
typedef std::map<CThingBuilding*, std::list<CThingAICreature*> > TBuildingCreatureMap;

CThingBuilding* __fastcall GetRelevantBuilding(TBuildingCreatureMap& buildings, ESimBuildingProperty property,
                                               bool checkWorkSpaces, bool checkLiveSpaces)
{
    TBuildingCreatureMap::iterator it = buildings.begin();
    CThingBuilding* relevantBuilding = 0;
    TBuildingCreatureMap::iterator end = buildings.end();
    for (; it != end; ++it) {
        CDefPointer<const CSimBuildingDef> def = it->first->PeekPSimDefinition();
        if (!def.IsNull() && (def->Property & property)) {
            if (!(checkWorkSpaces || checkLiveSpaces) ||
                it->second.size() < (unsigned long)(checkWorkSpaces ? def->WorkSpaces : def->LiveSpaces)) {
                if (relevantBuilding == 0 || buildings[it->first].size() < buildings[relevantBuilding].size())
                    relevantBuilding = it->first;
            }
        }
    }
    return relevantBuilding;
}
