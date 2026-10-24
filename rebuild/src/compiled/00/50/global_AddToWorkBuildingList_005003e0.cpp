// AddToWorkBuildingList(CThingAICreature* creature, std::map<CThingBuilding*, std::list<CThingAICreature*> >& buildings, CThingBuilding* building) -- region population: give the building's work-place list one more villager while the building's sim definition still has room for it, else drop the full building from the candidate map
// Leaf: ?AddToWorkBuildingList@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005003E0, 164 bytes (padded to 176), `ret 4` (`__fastcall`: creature in ecx,
//   buildings in edx, building on the stack). PDB name ?AddToWorkBuildingList@@YIXPAVCThingAICreature@@AAV?$map@PAVCThingBuilding@@
//   V?$list@PAVCThingAICreature@@...@std@@...@std@@PAVCThingBuilding@@@Z (Ego_r.pdb name set). Twin of 0x00500490 (AddToHomeBuildingList, LiveSpaces +0x44):
//   the PDB pair is listed work-then-home and retail keeps that order with WorkSpaces read here.
//   Callers: the population loops of CWorldMap::PopulateRegionWithVillagers (0x00503941 .. 0x00504C9A, ten sites).
//   Catalog label "ListNode_InsertPointer" is descriptive.
// Callees: map::find 0x00513420 (out of line, iterator by hidden pointer; manifest "RBTree::FindPredecessorNode" is
//   descriptive), CThingBuilding::PeekPSimDefinition 0x0082D2D0 (CDefPointer<const CSimBuildingDef> by hidden pointer;
//   manifest "PeekPMaterial" is propagated), malloc 0x00BFEA0E, map::erase(iterator) 0x0051A2F0 (manifest
//   "ListNode_RemoveAndSort" is descriptive), the def's slot-1 destroy through the inline release.
// Layout: CSimBuildingDef (Ego_r.pdb) Property / WorkSpaces / LiveSpaces = retail +0x3C / +0x40 / +0x44 (the PDB's
//   +0x40.. less the 4 bytes retail CParentDefClassBase is smaller); map nodes {color, parent, left, right, key +0x10,
//   list +0x14}; STLport list = the sentinel pointer, size() = the distance walk.
// Model: same-TU `#pragma auto_inline(off)` bodies of map::find / map::erase (the instances this TU emits) let VC7.1 see
//   that the key reference and the iterator do not escape, which keeps the creature in ebp and the frame to one word.
// Shape: the building argument is dead after `find`, so the def pointer's hidden result reuses its stack slot and the
//   key is re-read from the node (`it->first`); find's result is not tested against end(); push_back is the inline
//   STLport insert(end(), x) (create node, prev read first); the full-building erase takes the iterator by value (the
//   user copy constructor builds it in its slot); the def pointer releases inline at the end (long form, as the landed
//   0x00942A00).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }

class CThingAICreature;

struct CRefCounted {                                // reference count at +4 of a def
    long Count;
    void DecRef() { --Count; }
    long GetRefCount() const { return Count; }
};

class CSimBuildingDef {                             // Ego_r.pdb names, retail offsets
public:
    virtual void v00();
    virtual void Destroy() const;                   // slot 1 (+0x04)
    mutable CRefCounted Ref;                        // +0x04
    unsigned char _pad08[0x3c - 0x08];              // CParentDefClassBase remainder
    long Property;                                  // +0x3C ESimBuildingProperty
    long WorkSpaces;                                // +0x40
    long LiveSpaces;                                // +0x44
    __forceinline void ReleaseTemporary() const { Ref.DecRef(); if (Ref.GetRefCount() == 0) Destroy(); }
};

template <class T> class CDefPointer {
public:
    T* Object;
    __forceinline ~CDefPointer() { if (Object) Object->ReleaseTemporary(); }
    T* operator->() const { return Object; }
};

class CThingBuilding {
public:
    CDefPointer<const CSimBuildingDef> PeekPSimDefinition() const;   // 0x0082D2D0
};

namespace std {

template <class T1, class T2>
inline void _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}

template <class T> class allocator {
public:
    allocator() { }
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};

template <class T> struct less {
    bool operator()(const T& a, const T& b) const { return a < b; }
};

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};

template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
};

struct _List_node_base {
    _List_node_base* _M_next;                       // +0
    _List_node_base* _M_prev;                       // +4
};

template <class T> struct _List_node : public _List_node_base {
    T _M_data;                                      // +8
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* x) : _M_node(x) { }
};

template <class T> struct _List_iterator : public _List_iterator_base {
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) { }
    _List_iterator(const _List_iterator& x) : _List_iterator_base(x._M_node) { }
};

template <class T> class list {
public:
    typedef _List_node<T> _Node;
    typedef _List_iterator<T> iterator;
    _STLP_alloc_proxy<_Node*, allocator<_Node> > _M_node;    // +0
    ~list();                                        // 0x004AB4F0
    iterator begin() const { return iterator(_M_node._M_data->_M_next); }
    iterator end() const { return iterator(_M_node._M_data); }
    unsigned int size() const
    {
        unsigned int n = 0;
        for (iterator first = begin(), last = end(); first._M_node != last._M_node; first._M_node = first._M_node->_M_next)
            ++n;
        return n;
    }
    _Node* _M_create_node(const T& x)
    {
        _Node* p = _M_node.allocate(1);
        _Construct(&p->_M_data, x);
        return p;
    }
    iterator insert(iterator position, const T& x)
    {
        _Node* tmp = _M_create_node(x);
        _List_node_base* n = position._M_node;
        _List_node_base* p = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = p;
        p->_M_next = tmp;
        n->_M_prev = tmp;
        return tmp;
    }
    void push_back(const T& x) { insert(end(), x); }
};

struct _Rb_tree_node_base {
    bool _M_color;                                  // +0x00
    _Rb_tree_node_base* _M_parent;                  // +0x04
    _Rb_tree_node_base* _M_left;                    // +0x08
    _Rb_tree_node_base* _M_right;                   // +0x0C
};

template <class V> struct _Rb_tree_node : public _Rb_tree_node_base {
    V _M_value_field;                               // +0x10
};

template <class V> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) { }
    _Rb_tree_iterator(const _Rb_tree_iterator& it) : _M_node(it._M_node) { }
    V* operator->() const { return &static_cast<_Rb_tree_node<V>*>(_M_node)->_M_value_field; }
};

extern "C" void __cdecl free(void* p);             // 0x00BFEA14

template <class Dummy> class _Rb_global {
public:
    static _Rb_tree_node_base* __cdecl _Rebalance_for_erase(_Rb_tree_node_base* z, _Rb_tree_node_base*& root,
        _Rb_tree_node_base*& leftmost, _Rb_tree_node_base*& rightmost);   // 0x0042A34E (_STLP_CALL)
};

template <class T> __forceinline void _Destroy(T* p) { p->~T(); }

template <class K, class T, class Compare = less<K>, class A = allocator<pair<const K, T> > > class map {
public:
    typedef pair<const K, T> value_type;
    typedef _Rb_tree_node<value_type> _Node;
    typedef _Rb_tree_iterator<value_type> iterator;
    _STLP_alloc_proxy<_Node*, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                     // +0x04
    Compare _M_key_compare;                         // +0x08
    iterator end() { return iterator(_M_header._M_data); }
    iterator find(const K& k);                      // 0x00513420
    void erase(iterator position);                  // 0x0051A2F0
};

}

// Same-TU bodies of the two map members (the template instances this TU emits; graded only through their effect:
// VC7.1 sees that neither lets the key or the iterator escape).
#pragma auto_inline(off)
template <class K, class T, class Compare, class A>
typename std::map<K, T, Compare, A>::iterator std::map<K, T, Compare, A>::find(const K& k)
{
    std::_Rb_tree_node_base* y = _M_header._M_data;
    std::_Rb_tree_node_base* x = _M_header._M_data->_M_parent;
    while (x != 0) {
        if (!(static_cast<_Node*>(x)->_M_value_field.first < k)) { y = x; x = x->_M_left; }
        else x = x->_M_right;
    }
    if (y == _M_header._M_data || k < static_cast<_Node*>(y)->_M_value_field.first)
        return end();
    return iterator(y);
}

template <class K, class T, class Compare, class A>
void std::map<K, T, Compare, A>::erase(iterator position)
{
    _Node* y = (_Node*)std::_Rb_global<bool>::_Rebalance_for_erase(position._M_node, _M_header._M_data->_M_parent,
        _M_header._M_data->_M_left, _M_header._M_data->_M_right);
    std::_Destroy(&y->_M_value_field);
    if (y != 0) free(y);
    --_M_node_count;
}
#pragma auto_inline(on)

void __fastcall AddToWorkBuildingList(CThingAICreature* creature,
    std::map<CThingBuilding*, std::list<CThingAICreature*> >& buildings, CThingBuilding* building)
{
    std::map<CThingBuilding*, std::list<CThingAICreature*> >::iterator it = buildings.find(building);
    CDefPointer<const CSimBuildingDef> def = it->first->PeekPSimDefinition();
    if (def.Object) {
        if (it->second.size() + 1 < (unsigned int)def->WorkSpaces)
            it->second.push_back(creature);
        else
            buildings.erase(it);
    }
}