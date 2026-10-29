#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
// 00a50bf0 C3DAnimationStats::GetAllEvents
// Flags: /Gr
// Leaf: ?GetAllEvents@C3DAnimationStats@@QBEXAAV?$vector@U?$pair@VCCharString@@M@std@@V?$allocator@U?$pair@VCCharString@@M@std@@@2@@std@@@Z
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __declspec(x)
#endif
class CCharStringData;
class CCharString{public:CCharStringData*PStringData;CCharString():PStringData(0){}CCharString(const CCharString&);~CCharString();};
class CBaseClass{public:virtual~CBaseClass();};
class CBaseObject:public CBaseClass{unsigned char pass9_unread_pointer[4];};
#pragma pack(push,1)
class C3DVector{public:float X,Y,Z;};
#pragma pack(pop)
enum EAnimationType{};
namespace std{
template<class T>class allocator{};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
template<class A,class B>struct pair{A first;B second;pair(const A&a,const B&b):first(a),second(b){}pair(const pair&v):first(v.first),second(v.second){}template<class C,class D>pair(const pair<C,D>&v):first(v.first),second(v.second){}};
template<class T>struct less{};
template<class T>struct _Select1st{};
template<class T>struct _Nonconst_traits{typedef T&reference;typedef T*pointer;};
template<class T>struct _Const_traits{typedef const T&reference;typedef const T*pointer;};
struct input_iterator_tag{};
struct output_iterator_tag{};
struct forward_iterator_tag:input_iterator_tag{};
struct bidirectional_iterator_tag:forward_iterator_tag{};
template<class C,class T,class D,class P,class R>struct iterator{};
struct _Rb_tree_node_base{bool _M_color;_Rb_tree_node_base*_M_parent,*_M_left,*_M_right;};
template<class T>struct _Rb_tree_node:public _Rb_tree_node_base{T _M_value_field;_Rb_tree_node(const T&v):_M_value_field(v){};};
template<class T>class _Rb_global{public:static _Rb_tree_node_base*__fastcall _M_increment(_Rb_tree_node_base*);};
struct _Rb_tree_base_iterator{_Rb_tree_node_base*_M_node;bool operator!=(const _Rb_tree_base_iterator&x)const{return _M_node!=x._M_node;}};
template<class T,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator{
 typedef _Rb_tree_iterator<T,_Nonconst_traits<T> >iterator;
 _Rb_tree_iterator(_Rb_tree_node<T>*p){this->_M_node=p;}
 _Rb_tree_iterator(const iterator&x){this->_M_node=x._M_node;}
 typename Traits::reference operator*()const{return static_cast<_Rb_tree_node<T>*>(this->_M_node)->_M_value_field;}
 _Rb_tree_iterator&operator++(){this->_M_node=_Rb_global<bool>::_M_increment(this->_M_node);return *this;}
};
template<class T,class A>struct _Rb_tree_base{_STLP_alloc_proxy<_Rb_tree_node<T>*,_Rb_tree_node<T>,allocator<_Rb_tree_node<T> > >_M_header;};
template<class K,class V,class Key,class Compare,class A>class _Rb_tree:public _Rb_tree_base<V,A>{public:
 unsigned int _M_node_count;Compare _M_key_compare;
 typedef _Rb_tree_iterator<V,_Const_traits<V> >const_iterator;
 __forceinline const_iterator begin()const{return const_iterator(static_cast<_Rb_tree_node<V>*>(this->_M_header._M_data->_M_left));}
 __forceinline const_iterator end()const{return const_iterator(this->_M_header._M_data);}
 __forceinline unsigned int size()const{return _M_node_count;}
};
template<class K,class T,class C=less<K>,class A=allocator<pair<const K,T> > >class multimap{public:
 typedef pair<const K,T>value_type;typedef _Rb_tree<K,value_type,_Select1st<value_type>,C,A>_Rep_type;
 _Rep_type _M_t;
 __forceinline typename _Rep_type::const_iterator begin()const{return _M_t.begin();}
 __forceinline typename _Rep_type::const_iterator end()const{return _M_t.end();}
 __forceinline unsigned int size()const{return _M_t.size();}
};
template<class T,class A>class _Vector_base{public:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:typedef T value_type;void reserve(unsigned int);void push_back(const T&);};
template<class C>class back_insert_iterator:public iterator<output_iterator_tag,void,void,void,void>{protected:C*container;public:
 explicit back_insert_iterator(C&x):container(&x){}
 back_insert_iterator&operator=(const typename C::value_type&v){container->push_back(v);return *this;}
 back_insert_iterator&operator*(){return *this;}back_insert_iterator&operator++(){return *this;}
};
template<class C>__forceinline back_insert_iterator<C>back_inserter(C&x){return back_insert_iterator<C>(x);}
template<class I,class O,class D>__declspec(noinline) O __fastcall __copy(I first,I last,O out,const input_iterator_tag&,D*){for(;first!=last;++first,++out)*out=*first;return out;}
template<class I,class O>__forceinline O copy(I first,I last,O out){return __copy(first,last,out,bidirectional_iterator_tag(),static_cast<int*>(0));}
}
// Layout: RTTI CBaseObject8; AnimType8, Duration12, booleans16/17,
// original packed MovementVector18..29, then TimingEvents32 (retail12bytes).
class C3DAnimationStats:public CBaseObject{public:EAnimationType AnimType;float Duration;bool LoopFlag,MovementFlag;C3DVector MovementVector;std::multimap<CCharString,float>TimingEvents;void GetAllEvents(std::vector<std::pair<CCharString,float> >&)const;};
void C3DAnimationStats::GetAllEvents(std::vector<std::pair<CCharString,float> >&events)const{events.reserve(TimingEvents.size());std::copy(TimingEvents.begin(),TimingEvents.end(),std::back_inserter(events));}

// Fixture-owned string token; no CCharStringData layout is asserted.
struct FixtureToken{int id,refs;};
static unsigned int reserveCalls,pushCalls;
static void*expectedOutput;
static unsigned int expectedCount;
CBaseClass::~CBaseClass(){}
CCharString::CCharString(const CCharString&v):PStringData(v.PStringData){if(PStringData)++reinterpret_cast<FixtureToken*>(PStringData)->refs;}
CCharString::~CCharString(){if(PStringData)--reinterpret_cast<FixtureToken*>(PStringData)->refs;}
namespace std{
template<>_Rb_tree_node_base*__fastcall _Rb_global<bool>::_M_increment(_Rb_tree_node_base*x){
 if(x->_M_right){x=x->_M_right;while(x->_M_left)x=x->_M_left;}
 else{_Rb_tree_node_base*y=x->_M_parent;while(x==y->_M_right){x=y;y=y->_M_parent;}if(x->_M_right!=y)x=y;}
 return x;
}
template<class T,class A>void vector<T,A>::reserve(unsigned int n){if(this!=expectedOutput||n!=expectedCount)abort();++reserveCalls;}
template<class T,class A>void vector<T,A>::push_back(const T&v){if(this!=expectedOutput||this->_M_finish==this->_M_end_of_storage._M_data)abort();new(this->_M_finish)T(v);++this->_M_finish;++pushCalls;}
}
typedef std::pair<const CCharString,float>MapValue;
typedef std::_Rb_tree_node<MapValue>Node;
static std::_Rb_tree_node_base*build(Node**nodes,int a,int b,std::_Rb_tree_node_base*parent,int depth,int maxDepth){
 if(a==b)return 0;int mid=(a+b)/2;Node*x=nodes[mid];x->_M_parent=parent;x->_M_color=(depth!=maxDepth||depth==0);x->_M_left=build(nodes,a,mid,x,depth+1,maxDepth);x->_M_right=build(nodes,mid+1,b,x,depth+1,maxDepth);return x;
}
int main(){
 typedef std::pair<CCharString,float>OutValue;
 unsigned int cases=0;
 for(int n=0;n<=32;++n)for(int prefix=0;prefix<=3;++prefix)for(int duplicate=0;duplicate<2;++duplicate){
  FixtureToken tokens[34];for(int i=0;i<34;++i){tokens[i].id=i;tokens[i].refs=0;}
  CCharString empty;Node*header=new(malloc(sizeof(Node)))Node(MapValue(empty,0.0f));Node*nodes[32];
  for(int i=0;i<n;++i){CCharString name;name.PStringData=reinterpret_cast<CCharStringData*>(&tokens[duplicate?i/3:i]);++reinterpret_cast<FixtureToken*>(name.PStringData)->refs;nodes[i]=new(malloc(sizeof(Node)))Node(MapValue(name,(float)i/4.0f));}
  int maxDepth=0;for(int c=n;c>1;c/=2)++maxDepth;
  header->_M_color=false;header->_M_parent=build(nodes,0,n,header,0,maxDepth);header->_M_left=n?nodes[0]:header;header->_M_right=n?nodes[n-1]:header;
  C3DAnimationStats stats;stats.TimingEvents._M_t._M_header._M_data=header;stats.TimingEvents._M_t._M_node_count=n;
  std::vector<OutValue>out;OutValue*storage=static_cast<OutValue*>(malloc((n+prefix+2)*sizeof(OutValue)));
  for(int i=0;i<prefix;++i)new(storage+i)OutValue(empty,-1.0f);
  out._M_start=storage;out._M_finish=storage+prefix;out._M_end_of_storage._M_data=storage+n+prefix+2;
  expectedOutput=&out;expectedCount=n;reserveCalls=pushCalls=0;
  stats.GetAllEvents(out);
  if(reserveCalls!=1||pushCalls!=(unsigned int)n||out._M_finish!=storage+n+prefix||out._M_start!=storage)return 1;
  for(int i=0;i<prefix;++i)if(storage[i].first.PStringData||storage[i].second!=-1.0f)return 2;
  for(int i=0;i<n;++i){if(storage[i+prefix].first.PStringData!=nodes[i]->_M_value_field.first.PStringData||storage[i+prefix].second!=(float)i/4.0f)return 3;}
  for(int i=0;i<n+prefix;++i)storage[i].~OutValue();free(storage);
  for(int i=0;i<n;++i)nodes[i]->~Node();
  for(int i=0;i<34;++i)if(tokens[i].refs!=0)return 4;
  for(int i=0;i<n;++i)free(nodes[i]);header->~Node();free(header);++cases;
 }
 if(cases!=264)return 5;puts("00a50bf0_TEST PASS");return 0;
}