// 00ac8a60 QuadricOptimiserInternals::COptimisedPrimitive::UpdateCost
// Leaf: ?UpdateCost@COptimisedPrimitive@QuadricOptimiserInternals@@AAEXABV?$CCountedPointer@VCVertexPairContraction@QuadricOptimiserInternals@@@@@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Flags:
#include <stddef.h>
#include <new>
extern "C" void*__cdecl malloc(size_t);
#include "engine/CCountedPointer.h"
#define COptimisedPrimitive COptimisedPrimitiveFields
#include "engine/COptimisedPrimitive.h"
#undef COptimisedPrimitive
namespace std{
template<class T>class allocator{public:T*allocate(size_t n){return static_cast<T*>(malloc(n*sizeof(T)));}};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
template<class T,class A>class _Vector_base{protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{};
template<class A,class B>struct pair{typedef A first_type;typedef B second_type;A first;B second;pair(const A&a,const B&b):first(a),second(b){}};
template<class A,class R>struct unary_function{typedef A argument_type;typedef R result_type;};
template<class A,class B,class R>struct binary_function{typedef A first_argument_type;typedef B second_argument_type;typedef R result_type;};
template<class T>struct less:public binary_function<T,T,bool>{bool operator()(const T&a,const T&b)const{return a<b;}};
template<class P>struct _Select1st:public unary_function<P,typename P::first_type>{const typename P::first_type&operator()(const P&p)const{return p.first;}};
template<class T>struct _Nonconst_traits{typedef T*pointer;typedef T&reference;};
struct _Rb_tree_node_base{bool _M_color;_Rb_tree_node_base*_M_parent,*_M_left,*_M_right;};
template<class Dummy>class _Rb_global{public:static void __cdecl _Rebalance(_Rb_tree_node_base*,_Rb_tree_node_base*&);};
template<class T>struct _Rb_tree_node:public _Rb_tree_node_base{T _M_value_field;};
struct _Rb_tree_base_iterator{_Rb_tree_node_base*_M_node;};
template<class T,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator{_Rb_tree_iterator(){} _Rb_tree_iterator(_Rb_tree_node<T>*node){_M_node=node;} _Rb_tree_iterator(const _Rb_tree_iterator&x){_M_node=x._M_node;}};
template<class T,class A>struct _Rb_tree_base{protected:_STLP_alloc_proxy<_Rb_tree_node<T>*,_Rb_tree_node<T>,allocator<_Rb_tree_node<T> > >_M_header;};
template<class K,class V,class Select,class Compare,class A=allocator<V> >class _Rb_tree:public _Rb_tree_base<V,A>{protected:unsigned int _M_node_count;Compare _M_key_compare;public:typedef _Rb_tree_iterator<V,_Nonconst_traits<V> >iterator;void erase(iterator);__declspec(noinline) iterator insert_equal(const V&v){_Rb_tree_node<V>*y=this->_M_header._M_data;_Rb_tree_node<V>*x=static_cast<_Rb_tree_node<V>*>(y->_M_parent);while(x){y=x;x=static_cast<_Rb_tree_node<V>*>(_M_key_compare(Select()(v),Select()(x->_M_value_field))?x->_M_left:x->_M_right);}return _M_insert(x,y,v,0);}
private:
 __declspec(noinline) iterator _M_insert(_Rb_tree_node_base*x,_Rb_tree_node_base*y,const V&v,_Rb_tree_node_base*w){
  _Rb_tree_node<V>*z;
  if(y==this->_M_header._M_data||(w==0&&(x!=0||_M_key_compare(Select()(v),Select()(static_cast<_Rb_tree_node<V>*>(y)->_M_value_field))))){
   z=_M_create_node(v);y->_M_left=z;
   if(y==this->_M_header._M_data){this->_M_header._M_data->_M_parent=z;this->_M_header._M_data->_M_right=z;}else if(y==this->_M_header._M_data->_M_left)this->_M_header._M_data->_M_left=z;
  }else{z=_M_create_node(v);y->_M_right=z;if(y==this->_M_header._M_data->_M_right)this->_M_header._M_data->_M_right=z;}
  z->_M_parent=y;z->_M_left=z->_M_right=0;_Rb_global<bool>::_Rebalance(z,this->_M_header._M_data->_M_parent);++_M_node_count;return iterator(z);
 }
protected:
 _Rb_tree_node<V>*_M_create_node(const V&v){_Rb_tree_node<V>*n=this->_M_header.allocate(1);new(&n->_M_value_field)V(v);return n;}
};
template<class K,class V,class C=less<K>,class A=allocator<pair<const K,V> > >class multimap{typedef pair<const K,V>value_type;typedef _Rb_tree<K,value_type,_Select1st<value_type>,C,A>_Rep_type;_Rep_type _M_t;public:typedef typename _Rep_type::iterator iterator;void erase(iterator p){_M_t.erase(p);}iterator insert(const value_type&v){return _M_t.insert_equal(v);}};
}
template<class T>class CArray:public std::vector<T>{};
#include "engine/C3DVector.h"
namespace QuadricOptimiserInternals{
class CSubContraction;class CVertexPairContraction;
typedef std::multimap<float,CCountedPointer<CVertexPairContraction> > ContractionMap;
// Layout: original fields; retail CArray is12 bytes, reducing donor offsets by4.
class CVertexPairContraction{public:bool Exists,Unsplit;CArray<CSubContraction>SubContractions;float Weight2,Cost;C3DVector ResultingPosition;ContractionMap::iterator ListIterator;};
// Layout: shared flattened fields; original class namespace and private method restored.
class COptimisedPrimitive:public COptimisedPrimitiveFields{void UpdateCost(const CCountedPointer<CVertexPairContraction>&);};
void COptimisedPrimitive::UpdateCost(const CCountedPointer<CVertexPairContraction>&contraction){
 if(contraction->Exists){
  reinterpret_cast<ContractionMap&>(VertexPairContractions).erase(contraction->ListIterator);
  contraction->ListIterator=reinterpret_cast<ContractionMap&>(VertexPairContractions).insert(std::pair<const float,CCountedPointer<CVertexPairContraction> >(contraction->Cost,contraction));
 }
}
}