// 006eba60 CCombatManager_RebuildCombatLists
// Flags: /O2 /Oy /W3
// Leaf: CCombatManager_RebuildCombatLists
// Rebuild the ranged and melee combatant lists from the combatant tree: clear both STLport lists, walk the tree in
// order, append each creature to the ranged list when its combat type prefers the outer ring, else to the melee
// list, then sort both by distance from the position.
// Callees: free 0x00BFEA14, malloc 0x00BFEA0E, CThingCreatureBase::GetCombatManager 0x00661EC0,
// CThingCreatureBase::GetCombatTypeDef 0x00666D10, list::sort(CCompareByDistance) 0x006F3A30.
#include <stdlib.h>
#include "engine/CThingCreatureBase.h"
#include "engine/CCombatTypeDef.h"
namespace CombatLists {
struct CCombatManager;
// Layout: engine/CCombatTypeDef.h with the retail 12-byte containers (PreferOuterRing +0x68).
struct CThingCreatureBase : ::CThingCreatureBase {
    CCombatManager* GetCombatManager();          // 0x00661EC0
    ::CCombatTypeDef* GetCombatTypeDef();        // 0x00666D10
};
struct C3DVector { float X, Y, Z; C3DVector() {} __forceinline C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} __forceinline C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {} };
struct CCompareByDistance {
    C3DVector Position;
    __forceinline CCompareByDistance(const C3DVector& position) : Position(position) {}
};
// Layout: tree header +0, node parent +4, left +8, right +0xc, creature +0x18; list node next/prev/value +0/+4/+8.
struct _Rb_tree_node { unsigned color; _Rb_tree_node *parent, *left, *right; unsigned key, pad; CThingCreatureBase* creature; };
struct _Rb_tree { _Rb_tree_node* header; };
struct _List_node { _List_node *next, *prev; CThingCreatureBase* creature; };
struct list {
    _List_node* header;
    __forceinline void clear()
    {
        _List_node* p = header->next;
        while (p != header) { _List_node* old = p; p = p->next; free(old); }
        header->next = header;
        header->prev = header;
    }
    __forceinline void push_back(CThingCreatureBase* const& value)
    {
        _List_node* pos = header;
        _List_node* n = (_List_node*)malloc(sizeof(_List_node));
        if (&n->creature) n->creature = value;
        _List_node* previous = pos->prev;
        n->next = pos;
        n->prev = previous;
        previous->next = n;
        pos->prev = n;
    }
    void sort(CCompareByDistance);               // 0x006F3A30
};
void __fastcall CCombatManager_RebuildCombatLists(_Rb_tree* tree, const C3DVector& position, list* ranged, list* melee)
{
    ranged->clear();
    melee->clear();
    for (_Rb_tree_node* node = tree->header->left; node != tree->header; ) {
        CThingCreatureBase* creature = node->creature;
        creature->GetCombatManager();
        if (creature->GetCombatTypeDef()->PreferOuterRing)
            ranged->push_back(creature);
        else
            melee->push_back(creature);
        _Rb_tree_node* x = node;
        if (x->right) { x = x->right; while (x->left) x = x->left; }
        else { _Rb_tree_node* p = x->parent; while (x == p->right) { x = p; p = p->parent; } if (x->right != p) x = p; }
        node = x;
    }
    ranged->sort(CCompareByDistance(C3DVector(position.X, position.Y, position.Z)));
    melee->sort(CCompareByDistance(C3DVector(position.X, position.Y, position.Z)));
}
}