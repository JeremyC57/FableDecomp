// _STL::map<unsigned long, CCountedPointer<C3DAnimation2> >::operator[](const unsigned long& k) -- the animation instance slot for a key: lower_bound it, insert an empty counted pointer when absent, return the mapped pointer
// Grade: RELOCATION_MATCH (agent902, 4th grade) -- chunk-422 same-TU noinline insert_unique stand-in that reads v AND may write its counted pointer: a read-only stand-in lets VC7.1 fold the pair's release away (123), none keeps the escaped-pair scheduling (155).
// Leaf: ??A?$map@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2D810, 150 bytes (+ int3 padding), thiscall `ret 4`.
// Identity: C3DAnimationInfo::CAnimationData::Animations (Ego_r.pdb `map<unsigned long, CCountedPointer<C3DAnimation2> >`
//   at +0x1c of the 64-byte CAnimationData); the only caller is C3DAnimationInfo::BuildAnimationInstance (0x00A26BDE:
//   `lea ecx,[esi+1Ch]`). The catalog's "rbtree_lookup_or_insert" is descriptive.
// Callees: STLport `_Rb_tree::insert_unique(iterator, const value_type&)` @ 0x00A2CCF0 (iterator by value, result through a
//   hidden pointer), the count block's DeleteFunc (`mov ecx,[blk+8] ; call [blk+4]`), operator delete @ 0x00BFE9BC.
// Layout: node: color +0, parent +4, left +8, right +0xc, key +0x10, CCountedPointer {Data, Info} +0x14.
// Shape: STLport `iterator i = lower_bound(k); if (i == end() || key_comp()(k, (*i).first)) i = insert(i, value_type(k,
//   _Tp())); return (*i).second;` -- `_M_lower_bound` inlined (unsigned key, `jb` to the right child; `this` stays in ecx
//   for the call), the pair temporary in a 12-byte frame (key stored before the pushes, the empty pointer's two zero
//   stores after them), the iterator copy built in its argument slot, the insert result homed in the dead key slot; the
//   pair's counted pointer is released through a local block pointer (`dec [esi] ; jne`), its zero stores dead.
// Tried (chunk agent): nested / member-form / && releases through a local block pointer, a static Release helper
//   (155-161v150); the residue was the opaque insert_unique, which made the pair temporary escape.
void __cdecl operator delete(void* p);                  // 0x00BFE9BC

struct SCountBlock {
    long  RefCount;                                     // +0
    void (__fastcall* DeleteFunc)(void* data);          // +4, Data in ecx
    void* Data;                                         // +8
};

template <class T> class CCountedPointer {
public:
    T*           Data;                                  // +0
    SCountBlock* Info;                                  // +4
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer()
    {
        SCountBlock* info = Info;
        if (info && --info->RefCount == 0) {
            info->DeleteFunc(info->Data);
            delete info;
        }
    }
};

class C3DAnimation2;

namespace _STL {

struct _Rb_tree_node_base {
    unsigned char       _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};

template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};

template <class T> class allocator { };

template <class K, class T, class C = less<K>, class A = allocator<pair<const K, T> > > class map {
public:
    typedef K key_type;
    typedef pair<const K, T> value_type;
    struct _Node : _Rb_tree_node_base {
        value_type _M_value_field;                      // +0x10 key, +0x14 mapped
    };
    struct iterator {
        _Rb_tree_node_base* _M_node;
        iterator(_Rb_tree_node_base* n) : _M_node(n) { }
        iterator(const iterator& o) : _M_node(o._M_node) { }
        value_type& operator*() const { return static_cast<_Node*>(_M_node)->_M_value_field; }
        bool operator==(const iterator& o) const { return _M_node == o._M_node; }
    };
    _Rb_tree_node_base* _M_header;
    unsigned long       _M_node_count;
    C                   _M_key_compare;

    static const key_type& _S_key(_Rb_tree_node_base* x) { return static_cast<_Node*>(x)->_M_value_field.first; }
    _Rb_tree_node_base* _M_lower_bound(const key_type& k) const
    {
        _Rb_tree_node_base* y = _M_header;
        _Rb_tree_node_base* x = _M_header->_M_parent;
        while (x != 0)
            if (!_M_key_compare(_S_key(x), k))
                y = x, x = x->_M_left;
            else
                x = x->_M_right;
        return y;
    }
    iterator lower_bound(const key_type& k) { return iterator(_M_lower_bound(k)); }
    iterator end() { return iterator(_M_header); }
    C key_comp() const { return _M_key_compare; }
    iterator insert_unique(iterator position, const value_type& v);   // 0x00A2CCF0
    iterator insert(iterator position, const value_type& v) { return insert_unique(position, v); }

    T& operator[](const key_type& k)
    {
        iterator i = lower_bound(k);
        if (i == end() || key_comp()(k, (*i).first))
            i = insert(i, value_type(k, T()));
        return (*i).second;
    }
};
#pragma auto_inline(off)
// Same-TU stand-in for insert_unique(hint, v) 0x00A2CCF0: reads v and MAY write its counted pointer (so VC7.1 keeps the
// pair's release after the call), never keeps the address.
template <class K, class T, class C, class A>
__declspec(noinline) typename map<K, T, C, A>::iterator map<K, T, C, A>::insert_unique(iterator position, const value_type& v)
{
    _M_node_count += *(const unsigned char*)&v.first;
    if (position._M_node == 0)                          // never (the hint is a node or the header): a possible write to v
        const_cast<T&>(v.second) = T();
    return position;
}
#pragma auto_inline(on)


}

template CCountedPointer<C3DAnimation2>& _STL::map<unsigned long, CCountedPointer<C3DAnimation2> >::operator[](const unsigned long& k);