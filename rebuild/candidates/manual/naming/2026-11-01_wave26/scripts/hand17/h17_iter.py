# h17_iter.py: naming wave 17, family stl -- 0x004FE7F0 calls set<unsigned long>'s insert_unique 0x00514C10, whose STLport 4.6 result is
# pair<_Rb_tree_iterator<unsigned long, _Nonconst_traits<unsigned long> >, bool> (c675); the row's iterator model lacked the _Traits
# parameter. The same model serves its _Rb_tree<char>::insert_unique 0x00514B50. Name-only.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit
a = '004fe7f0'
edit(a, 'src', '''template <class _Value> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value> _Self;
    _Rb_tree_iterator() { _M_node = 0; }
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Self& __it) { _M_node = __it._M_node; }''', '''template <class _Tp> struct _Nonconst_traits { };   // STLport 4.6 iterator traits tag (naming wave 17: c675 insert_unique result type)
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    _Rb_tree_iterator() { _M_node = 0; }
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> >& __it) { _M_node = __it._M_node; }''')
edit(a, 'src', '    typedef _Rb_tree_iterator<_Value> iterator;', '    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;')
