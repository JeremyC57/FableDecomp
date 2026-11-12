// std::_Rb_tree_iterator::operator++(int) -- Return the old iterator and advance to its successor.
// Leaf: ??E?$_Rb_tree_iterator@KU?$_Const_traits@K@std@@@std@@QAE?AU01@H@Z
// Types (proven, agentFAM11, 2026-11-12): set<unsigned long>, the const iterator's post-increment. Sources: (1) retail: the landed real-typed
//   QuadricOptimiserInternals::COptimisedPrimitive::ComputeVertexCost 0x00AC8C80 calls it and its relocation binds exactly this spelling
//   (rebuild/oracles/reloc_bindings.tsv); (2) FableWin.pdb QuadricOptimiserInternals::CIndexList::Begin / End return the const iterator of
//   set<unsigned long> (_Tset_traits<unsigned long>).
//   Was the placeholder ??E?$_Rb_tree_iterator@VCUnprovenValue_00ad0a70@@U?$_Nonconst_traits@VCUnprovenValue_00ad0a70@@@std@@@std@@QAE?AU01@H@Z (_Nonconst_traits);
//   the caller binds the const iterator, so the traits are _Const_traits; same body.
namespace std {
struct _Rb_tree_node_base {
    bool _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template<class Dummy> class _Rb_global {
public:
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* x) {
        if(x->_M_right) {
            x=x->_M_right;
            while(x->_M_left) x=x->_M_left;
        } else {
            _Rb_tree_node_base* y=x->_M_parent;
            while(x==y->_M_right) { x=y; y=y->_M_parent; }
            if(x->_M_right!=y) x=y;
        }
        return x;
    }
};
template<class Value> struct _Const_traits {};
struct _Rb_tree_base_iterator { _Rb_tree_node_base* _M_node; };
template<class Value,class Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<Value,Traits> _Self;
    _Rb_tree_iterator() { _M_node=0; }
    _Rb_tree_iterator(const _Self& x) { _M_node=x._M_node; }
    _Self operator++(int) {
        _Self old=*this;
        _M_node=_Rb_global<bool>::_M_increment(_M_node);
        return old;
    }
};
template _Rb_tree_iterator<unsigned long,_Const_traits<unsigned long> > _Rb_tree_iterator<unsigned long,_Const_traits<unsigned long> >::operator++(int);
}