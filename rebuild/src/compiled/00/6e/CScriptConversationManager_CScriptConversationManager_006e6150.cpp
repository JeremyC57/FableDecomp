// CScriptConversationManager::CScriptConversationManager() -- an empty conversation map (STLport tree: allocate the 0x40-byte header node, colour it red, null root, leftmost/rightmost = header), the next index 1, AlwaysShowText off
// Leaf: ??0CScriptConversationManager@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E6150, 65 bytes (80 with int3 padding), thiscall, returns this.
// Identity: Ego_r.pdb name set `??0CScriptConversationManager@@QAE@XZ`; the header node size 0x40 = the map node
//   {color, parent, left, right, key +0x10, CScriptConversation +0x14 (0x2C retail)}, the map/index/flag layout of the
//   landed manager members (0x006E5800/0x006E5950). The catalog's "List_Initialize_Empty" is descriptive.
// Callees: malloc @ 0x00BFEA0E (the allocator's allocate(1) for the header node).
// Layout (retail): Conversations +0 (STLport map, 12 bytes: header, node count, comparator), NextConversationIndex
//   +0x0C, AlwaysShowText +0x10.
// Shape: STLport `_Rb_tree_base(a)` nulls then allocates the header, `_Rb_tree()` sets _M_node_count(0) then runs
//   `_M_empty_initialize` (header re-read after each store through it); the member initialisers follow.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

namespace std {
template <class T> class allocator {
public:
    allocator() { }
    T* allocate(unsigned int n) { return (T*)malloc(n * sizeof(T)); }
};
template <class T> struct less { };
template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
    _STLP_alloc_proxy(const A& a, V p) : A(a), _M_data(p) { }
};
struct _Rb_tree_node_base {
    bool                _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0C
};
template <class V> struct _Rb_tree_node : public _Rb_tree_node_base {
    V _M_value_field;                                       // +0x10
};
template <class V, class A> class _Rb_tree_base {
public:
    typedef _Rb_tree_node<V> _Node;
    _STLP_alloc_proxy<_Node*, allocator<_Node> > _M_header;
    _Rb_tree_base(const A& a) : _M_header(allocator<_Node>(), (_Node*)0)
    {
        _M_header._M_data = _M_header.allocate(1);
    }
};
template <class K, class V, class Compare, class A> class _Rb_tree : public _Rb_tree_base<V, A> {
public:
    unsigned int _M_node_count;                             // +0x04
    Compare      _M_key_compare;                            // +0x08
    _Rb_tree() : _Rb_tree_base<V, A>(A()), _M_node_count(0) { _M_empty_initialize(); }
    private:
    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = false;          // red
        this->_M_header._M_data->_M_parent = 0;
        this->_M_header._M_data->_M_left = this->_M_header._M_data;
        this->_M_header._M_data->_M_right = this->_M_header._M_data;
    }
    public:
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template <class K, class T, class Compare = less<K>, class A = allocator<pair<const K, T> > > class map {
public:
    _Rb_tree<K, pair<const K, T>, Compare, A> _M_t;
};
}

class CScriptConversation {
public:
    unsigned char _data[0x2c];                              // retail STLport layout
};

class CScriptConversationManager {
public:
    std::map<long, CScriptConversation> Conversations;      // +0x00
    long NextConversationIndex;                             // +0x0C
    bool AlwaysShowText;                                    // +0x10
    CScriptConversationManager();
};

CScriptConversationManager::CScriptConversationManager()
    : NextConversationIndex(1), AlwaysShowText(false)
{
}