// 006bc380 CRegion::GetWorldMapNameGraphic
// Leaf: ?GetWorldMapNameGraphic@CRegion@@QBEKXZ
// Flags:
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
#include "engine/CParentDefClassBase.h"
#define CRegion CRegionLayout
#include "engine/CRegion.h"
#undef CRegion
// Layout: shared storage, with the original public const member restored.
class CRegion:public CRegionLayout {public:unsigned long GetWorldMapNameGraphic()const;};
enum EWorldMapNameGraphic {};
namespace std{
template<class A,class B>struct pair{typedef A first_type;typedef B second_type;A first;B second;pair():first(),second(){} pair(const A&a,const B&b):first(a),second(b){}};
template<class A,class B,class R>struct binary_function{};
template<class A,class R>struct unary_function{};
template<class T>struct less:binary_function<T,T,bool>{bool operator()(const T&a,const T&b)const{return a<b;}};
template<class P>struct _Select1st:unary_function<P,typename P::first_type>{const typename P::first_type&operator()(const P&p)const{return p.first;}};
template<class T>class allocator{};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
struct _Rb_tree_node_base{bool _M_color;_Rb_tree_node_base*_M_parent,*_M_left,*_M_right;};
template<class T>struct _Rb_tree_node:public _Rb_tree_node_base{T _M_value_field;};
struct _Rb_tree_base_iterator{_Rb_tree_node_base*_M_node;};
template<class T>struct _Const_traits{typedef const T*pointer;typedef const T&reference;};
template<class T,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator{_Rb_tree_iterator(){} _Rb_tree_iterator(_Rb_tree_node_base*p){_M_node=p;} typename Traits::pointer operator->()const{return &static_cast<_Rb_tree_node<T>*>(_M_node)->_M_value_field;}};
template<class V,class A>struct _Rb_tree_base{protected:_STLP_alloc_proxy<_Rb_tree_node<V>*,_Rb_tree_node<V>,allocator<_Rb_tree_node<V> > >_M_header;};
template<class K,class V,class Select,class Compare,class A>class _Rb_tree:public _Rb_tree_base<V,A>{protected:unsigned long _M_node_count;Compare _M_key_compare;public:typedef _Rb_tree_iterator<V,_Const_traits<V> >const_iterator;template<class _KT> __declspec(noinline)const_iterator find(const _KT&k)const{
 _Rb_tree_node_base*y=this->_M_header._M_data;_Rb_tree_node_base*x=y->_M_parent;
 while(x){if(!_M_key_compare(Select()(static_cast<_Rb_tree_node<V>*>(x)->_M_value_field),k)){y=x;x=x->_M_left;}else x=x->_M_right;}
 if(y==this->_M_header._M_data||_M_key_compare(k,Select()(static_cast<_Rb_tree_node<V>*>(y)->_M_value_field)))return end();
 return const_iterator(y);
 }const_iterator end()const{return const_iterator(this->_M_header._M_data);}};
template<class K,class V,class C=less<K>,class A=allocator<pair<const K,V> > >class map{typedef pair<const K,V>value_type;typedef _Rb_tree<K,value_type,_Select1st<value_type>,C,A>_Rep_type;_Rep_type _M_t;public:typedef typename _Rep_type::const_iterator const_iterator;const_iterator find(const K&k)const{return _M_t.find(k);}const_iterator end()const{return _M_t.end();}};
}
// Layout: original CParentDefClassBase is 0x39, next native member aligned at0x3c.
namespace NUISystem {class CUILocaleGraphicsDef:public CParentDefClassBase{public:std::map<EWorldMapNameGraphic,unsigned long>WorldMapRegionName;};}
// Layout: native NameGraphic at0xbc, donor0xc8; opaque preceding data is not accessed.
class CRegionDef:public CParentDefClassBase{unsigned char _pad_0x39[0xbc-0x39];public:EWorldMapNameGraphic NameGraphic;};
namespace NLocalisation {void __fastcall GetUIGraphics(CDefPointer<const NUISystem::CUILocaleGraphicsDef>&);}
unsigned long CRegion::GetWorldMapNameGraphic()const{
 CDefPointer<const NUISystem::CUILocaleGraphicsDef>graphics;
 NLocalisation::GetUIGraphics(graphics);
 if(PDef_Object){
  const std::map<EWorldMapNameGraphic,unsigned long>&names=graphics->WorldMapRegionName;
  std::map<EWorldMapNameGraphic,unsigned long>::const_iterator it=names.find(static_cast<const CRegionDef*>(PDef_Object)->NameGraphic);
  if(it._M_node!=names.end()._M_node){
 // The found path has already dereferenced the graphics definition.
 unsigned long value=it->second;
 CDefPointeeBase*object=graphics.InternalGetObject();
 if(object->Release()==0)object->Delete();
 graphics.Object=0;
 return value;
 }
 }
 return 0;
}