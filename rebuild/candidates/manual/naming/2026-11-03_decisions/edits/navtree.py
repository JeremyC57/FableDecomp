P = "rebuild/src/compiled/00/a7/CNavQuadTreeNode_ConvertIndexesToPointers_00a77ca0.cpp"
EDITS = [(P, "template <class T> struct allocator { };", "template <class T> class allocator { };"),
 (P, """template <class V> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* n) : _M_node(n) {}
    _Rb_tree_iterator(const _Rb_tree_iterator& it) : _M_node(it._M_node) {}""", """template <class T> struct _Nonconst_traits { };
template <class V, class Traits> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* n) : _M_node(n) {}
    _Rb_tree_iterator(const _Rb_tree_iterator& it) : _M_node(it._M_node) {}"""),
 (P, """template <class K, class V, class KoV, class C, class A> struct _Rb_tree {
    typedef _Rb_tree_iterator<V> iterator;""", """template <class K, class V, class KoV, class C, class A> class _Rb_tree {
public:
    typedef _Rb_tree_iterator<V, _Nonconst_traits<V> > iterator;""")]
