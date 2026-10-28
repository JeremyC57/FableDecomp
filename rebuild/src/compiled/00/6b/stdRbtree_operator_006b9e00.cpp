// std::_Rb_tree::operator=(const Container& other) -- replace the destination tree with a deep copy.
// Leaf: ??4?$_Rb_tree@VCCharString@@V1@U?$_Identity@VCCharString@@@std@@U?$less@VCCharString@@@3@V?$allocator@VCCharString@@@3@@std@@QAEAAV01@ABV01@@Z
// Identity: proven; set<CCharString> callers copy two string sets; _M_erase0x4ABA70 proven string set, _M_copy0x513300 copies CCharString; current binding confirms real tree assignment.
class CCharString { public: void* PStringData; };
namespace std {
template<class T> class allocator {};
template<class T> struct less { bool operator()(const T& a,const T& b) const { return a<b; } };
template<class T> struct _Identity { const T& operator()(const T& v) const { return v; } };
template<class A,class B> struct pair { typedef A first_type; A first; B second; pair() : first(A()),second(B()) {} };
template<class P> struct _Select1st { const typename P::first_type& operator()(const P& p) const { return p.first; } };
struct _Rb_tree_node_base { bool _M_color; _Rb_tree_node_base* _M_parent; _Rb_tree_node_base* _M_left; _Rb_tree_node_base* _M_right; };
template<class V> struct _Rb_tree_node : _Rb_tree_node_base { V _M_value_field; };
template<class K,class V,class Extract,class Compare,class Alloc> class _Rb_tree {
public:
 typedef _Rb_tree_node<V> _Node; typedef _Node* _Link_type;
 _Link_type _M_header; unsigned int _M_node_count; Compare _M_key_compare;
 _Link_type& _M_root() const { return (_Link_type&)_M_header->_M_parent; }
 _Link_type& _M_leftmost() const { return (_Link_type&)_M_header->_M_left; }
 _Link_type& _M_rightmost() const { return (_Link_type&)_M_header->_M_right; }
 static _Link_type _S_minimum(_Link_type x) { while(x->_M_left) x=(_Link_type)x->_M_left; return x; }
 static _Link_type _S_maximum(_Link_type x) { while(x->_M_right) x=(_Link_type)x->_M_right; return x; }
 void clear() { if(_M_node_count!=0) { _M_erase(_M_root()); _M_leftmost()=_M_header; _M_root()=0; _M_rightmost()=_M_header; _M_node_count=0; } }
  _Rb_tree& operator=(const _Rb_tree& x) {
  if(this!=&x) { clear(); _M_node_count=0; _M_key_compare=x._M_key_compare;
   if(x._M_root()==0) { _M_root()=0; _M_leftmost()=_M_header; _M_rightmost()=_M_header; }
   else { _M_root()=_M_copy(x._M_root(),_M_header); _M_leftmost()=_S_minimum(_M_root()); _M_rightmost()=_S_maximum(_M_root()); _M_node_count=x._M_node_count; }
  } return *this;
 }
private: _Link_type _M_copy(_Link_type,_Link_type); void _M_erase(_Link_type);
};
template<class K,class V,class Compare=less<K>,class Alloc=allocator<pair<const K,V> > > class map {
public: typedef _Rb_tree<K,pair<const K,V>,_Select1st<pair<const K,V> >,Compare,Alloc> Tree; Tree _M_t;
 map& operator=(const map& other) { _M_t=other._M_t; return *this; }
};
}

template class std::_Rb_tree<CCharString,CCharString,std::_Identity<CCharString>,std::less<CCharString>,std::allocator<CCharString> >;