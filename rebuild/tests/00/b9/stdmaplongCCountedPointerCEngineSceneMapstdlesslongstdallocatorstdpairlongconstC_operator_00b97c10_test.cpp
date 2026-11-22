#define FABLE_RETAIL_STLP_RB_GLOBAL_ROTATE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_C
// std::map::operator[](const long&) -- finds or inserts an empty counted pointer at a scene-map coordinate.
// Leaf: ??A?$map@JV?$CCountedPointer@VCEngineSceneMap@@@@U?$less@J@std@@V?$allocator@U?$pair@$$CBJV?$CCountedPointer@VCEngineSceneMap@@@@@std@@@3@@std@@QAEAAV?$CCountedPointer@VCEngineSceneMap@@@@ABJ@Z
// Flags: default
// STLport 4.6 _map.h. Visible native insert bodies keep the temporary pair
// and counted-pointer release in the same form as the retail compiler.
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define FABLE_RETAIL_STLP_TREE_ITERATOR_TRAITS
#define FABLE_RETAIL_STLP_RB_GLOBAL_DECREMENT_SIZE_BODY
#define FABLE_RETAIL_STLP_RB_GLOBAL_INCREMENT_SIZE_BODY_PASS27
#include "engine/CCountedPointer.h"
#include "stlport/fable_map.h"
class CEngineSceneMap;
template CCountedPointer<CEngineSceneMap>& std::map<long, CCountedPointer<CEngineSceneMap> >::operator[](const long&);
#include "engine/CEngineSceneMap.h"
template CCountedPointer<CEngineSceneMap>& std::map<long, CCountedPointer<CEngineSceneMap> >::operator[](const long&);

#include <stdio.h>

namespace std {
static const bool _S_rb_tree_red = false;
static const bool _S_rb_tree_black = true;
template <class _Dummy>
void __cdecl _Rb_global<_Dummy>::_Rotate_left(_Rb_tree_node_base* x, _Rb_tree_node_base*& root)
{
    _Rb_tree_node_base* y = x->_M_right;
    x->_M_right = y->_M_left;
    if (y->_M_left != 0)
        y->_M_left->_M_parent = x;
    y->_M_parent = x->_M_parent;

    if (x == root)
        root = y;
    else if (x == x->_M_parent->_M_left)
        x->_M_parent->_M_left = y;
    else
        x->_M_parent->_M_right = y;
    y->_M_left = x;
    x->_M_parent = y;
}

template <class _Dummy>
void __cdecl _Rb_global<_Dummy>::_Rotate_right(_Rb_tree_node_base* x, _Rb_tree_node_base*& root)
{
    _Rb_tree_node_base* y = x->_M_left;
    x->_M_left = y->_M_right;
    if (y->_M_right != 0)
        y->_M_right->_M_parent = x;
    y->_M_parent = x->_M_parent;

    if (x == root)
        root = y;
    else if (x == x->_M_parent->_M_right)
        x->_M_parent->_M_right = y;
    else
        x->_M_parent->_M_left = y;
    y->_M_right = x;
    x->_M_parent = y;
}

template <class _Dummy> void __cdecl
_Rb_global<_Dummy>::_Rebalance(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root)
{
    __x->_M_color = _S_rb_tree_red;
    while (__x != __root && __x->_M_parent->_M_color == _S_rb_tree_red) {
        if (__x->_M_parent == __x->_M_parent->_M_parent->_M_left) {
            _Rb_tree_node_base* __y = __x->_M_parent->_M_parent->_M_right;
            if (__y && __y->_M_color == _S_rb_tree_red) {
                __x->_M_parent->_M_color = _S_rb_tree_black;
                __y->_M_color = _S_rb_tree_black;
                __x->_M_parent->_M_parent->_M_color = _S_rb_tree_red;
                __x = __x->_M_parent->_M_parent;
            }
            else {
                if (__x == __x->_M_parent->_M_right) {
                    __x = __x->_M_parent;
                    _Rotate_left(__x, __root);
                }
                __x->_M_parent->_M_color = _S_rb_tree_black;
                __x->_M_parent->_M_parent->_M_color = _S_rb_tree_red;
                _Rotate_right(__x->_M_parent->_M_parent, __root);
            }
        }
        else {
            _Rb_tree_node_base* __y = __x->_M_parent->_M_parent->_M_left;
            if (__y && __y->_M_color == _S_rb_tree_red) {
                __x->_M_parent->_M_color = _S_rb_tree_black;
                __y->_M_color = _S_rb_tree_black;
                __x->_M_parent->_M_parent->_M_color = _S_rb_tree_red;
                __x = __x->_M_parent->_M_parent;
            }
            else {
                if (__x == __x->_M_parent->_M_left) {
                    __x = __x->_M_parent;
                    _Rotate_right(__x, __root);
                }
                __x->_M_parent->_M_color = _S_rb_tree_black;
                __x->_M_parent->_M_parent->_M_color = _S_rb_tree_red;
                _Rotate_left(__x->_M_parent->_M_parent, __root);
            }
        }
    }
    __root->_M_color = _S_rb_tree_black;
}
}

static unsigned int destructions;
CEngineSceneMap::~CEngineSceneMap() { ++destructions; }

int main()
{
    CCountedPointer<CEngineSceneMap> retained(new CEngineSceneMap);
    std::map<long, CCountedPointer<CEngineSceneMap> > cells;
    CCountedPointer<CEngineSceneMap>* first = &cells[7];
    if (cells.size() != 1 || first->Get() != 0 || &cells[7] != first)
        return 1;
    *first = retained;
    if (cells[7].Get() != retained.Get() || cells.size() != 1 || retained.GetReferenceCount() != 2)
        return 2;
    if (cells[-2].Get() != 0 || cells[0].Get() != 0 || cells[14].Get() != 0 
        || cells.size() != 4 || &cells[7] != first)
        return 3;
    const long keys[] = {-2, 0, 7, 14};
    unsigned int i = 0;
    std::map<long, CCountedPointer<CEngineSceneMap> >::iterator it = cells.begin();
    for (; it != cells.end(); ++it, ++i) {
        if (i >= 4 || (*it).first != keys[i])
            return 4;
    }
    if (i != 4)
        return 5;
    first->Reset(0);
    if (destructions != 0 || retained.GetReferenceCount() != 1 || cells[7].Get() != 0)
        return 6;
    retained.Reset(0);
    if (destructions != 1)
        return 7;
    puts("00b97c10_TEST PASS");
    return 0;
}