#include <stdio.h>
#ifndef _MSC_VER
#define __forceinline inline
#endif
struct CUnprovenKey_00999e50 { long _M_unproven; };
struct CUnprovenValue_00999e50 { CUnprovenKey_00999e50 _M_key; };
struct CUnprovenKeyOfValue_00999e50 { const CUnprovenKey_00999e50& operator()(const CUnprovenValue_00999e50& v) const { return v._M_key; } };
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

template class std::_Rb_tree<CUnprovenKey_00999e50,CUnprovenValue_00999e50,CUnprovenKeyOfValue_00999e50,std::less<CUnprovenKey_00999e50>,std::allocator<CUnprovenValue_00999e50> >;

typedef std::_Rb_tree<CUnprovenKey_00999e50,CUnprovenValue_00999e50,CUnprovenKeyOfValue_00999e50,std::less<CUnprovenKey_00999e50>,std::allocator<CUnprovenValue_00999e50> > Container;
typedef Container Tree;
typedef Tree::_Link_type Node;
static int erases=0,copies=0,bad=0; static Node expectedOld,expectedSource,expectedParent; static Tree::_Node cloned[3];
namespace std {
template<class K,class V,class X,class C,class A> void _Rb_tree<K,V,X,C,A>::_M_erase(_Link_type node) { ++erases; if(node!=expectedOld) bad=1; }
template<class K,class V,class X,class C,class A> typename _Rb_tree<K,V,X,C,A>::_Link_type _Rb_tree<K,V,X,C,A>::_M_copy(_Link_type node,_Link_type parent) { ++copies; if(node!=expectedSource || parent!=expectedParent) bad=1; cloned[0]._M_parent=parent; cloned[0]._M_left=&cloned[1]; cloned[0]._M_right=&cloned[2]; cloned[1]._M_left=cloned[1]._M_right=cloned[2]._M_left=cloned[2]._M_right=0; return (_Link_type)&cloned[0]; }
}
static void empty(Tree& t,Node h) { t._M_header=h; t._M_node_count=0; h->_M_parent=0; h->_M_left=h->_M_right=h; }
int main() { Container dst,src; Tree& d=dst; Tree& s=src; Tree::_Node dh,sh,oldroot,sourceroot; empty(d,(Node)&dh); empty(s,(Node)&sh); d._M_node_count=2; dh._M_parent=&oldroot; s._M_node_count=3; sh._M_parent=&sourceroot; expectedOld=(Node)&oldroot; expectedSource=(Node)&sourceroot; expectedParent=(Node)&dh;
 if(&(dst=dst)!=&dst || erases || copies || d._M_node_count!=2) goto fail;
 if(&(dst=src)!=&dst || bad || erases!=1 || copies!=1 || d._M_node_count!=3 || dh._M_parent!=&cloned[0] || dh._M_left!=&cloned[1] || dh._M_right!=&cloned[2]) goto fail;
 expectedOld=(Node)&cloned[0]; empty(s,(Node)&sh); dst=src; if(bad || erases!=2 || copies!=1 || d._M_node_count || dh._M_parent || dh._M_left!=&dh || dh._M_right!=&dh) goto fail;
 s._M_node_count=3; sh._M_parent=&sourceroot; dst=src; if(bad || erases!=2 || copies!=2 || d._M_node_count!=3 || dh._M_parent!=&cloned[0]) goto fail;
 puts("0099a1f0_TEST PASS"); return 0;
fail: puts("0099a1f0_TEST FAIL"); return 1;
}