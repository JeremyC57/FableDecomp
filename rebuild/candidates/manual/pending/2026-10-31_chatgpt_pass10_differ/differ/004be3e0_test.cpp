// Behavior-only fixture. Target and selected STLport vector/iterator bodies
// are pasted; unused receivers are reduced to exercised members. Every receiver,
// node and mesh is constructed; native offsets/dispatch remain separate proof.
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#ifndef _MSC_VER
#define __declspec(x)
#define __cdecl
#endif
static int cases,flipOnLoad,lookupCalls,componentCalls;
#define CHECK(x)do{if(!(x)){fprintf(stderr,"case %d line %d\n",cases,__LINE__);abort();}}while(0)
#include <new>
namespace std {
template<class T> class allocator {public:allocator(){}allocator(const allocator&){}~allocator(){}T*allocate(unsigned int n,const void* =0){return static_cast<T*>(malloc(n*sizeof(T)));}void deallocate(T*p,unsigned int){free(p);}};
template<class V,class T,class A> class _STLP_alloc_proxy : public A {public:V _M_data;_STLP_alloc_proxy(const A&a,V d):A(a),_M_data(d){} };
template<class A,class B> struct pair {typedef A first_type;typedef B second_type;A first;B second;pair():first(A()),second(B()){}pair(const A& a,const B& b):first(a),second(b){}template<class X,class Y>pair(const pair<X,Y>& p):first(p.first),second(p.second){}pair(const pair<A,B>& p):first(p.first),second(p.second){} };
template<class A,class R>struct unary_function {typedef A argument_type;typedef R result_type;};
template<class A,class B,class R>struct binary_function {typedef A first_argument_type;typedef B second_argument_type;typedef R result_type;};
template<class T>struct less:public binary_function<T,T,bool> {bool operator()(const T& x,const T& y)const{return x<y;}};
template<class T>struct _Select1st:public unary_function<T,typename T::first_type> {const typename T::first_type& operator()(const T& x)const{return x.first;}};
struct _Rb_tree_node_base {bool _M_color;_Rb_tree_node_base* _M_parent;_Rb_tree_node_base* _M_left;_Rb_tree_node_base* _M_right;};
template<class T> struct _Rb_tree_node : public _Rb_tree_node_base {T _M_value_field;};
template<class T,class A> struct _Rb_tree_base {public:_Rb_tree_base(const A&);public:_STLP_alloc_proxy<_Rb_tree_node<T>*,_Rb_tree_node<T>,allocator<_Rb_tree_node<T> > > _M_header;};
template<class K,class V,class Select,class Compare,class A> class _Rb_tree : public _Rb_tree_base<V,A> {public:_Rb_tree();public:unsigned int _M_node_count;Compare _M_key_compare;};
template<class K,class V,class Compare=less<K>,class A=allocator<pair<const K,V> > > class map {public:_Rb_tree<K,pair<const K,V>,_Select1st<pair<const K,V> >,Compare,A> _M_t;};
struct _List_node_base {_List_node_base* _M_next;_List_node_base* _M_prev;};
template<class T> struct _List_node : public _List_node_base {T _M_data;};
template<class T,class A> class _List_base {public:_List_base(const A&):_M_node(allocator<_List_node<T> >(),static_cast<_List_node<T>*>(0)){}_STLP_alloc_proxy<_List_node<T>*,_List_node<T>,allocator<_List_node<T> > > _M_node;};
struct _List_iterator_base {_List_node_base*_M_node;_List_iterator_base(_List_node_base*n):_M_node(n){}_List_iterator_base(){}void _M_incr(){_M_node=_M_node->_M_next;}bool operator!=(const _List_iterator_base&x)const{return _M_node!=x._M_node;}};
template<class T>struct _Nonconst_traits {typedef T value_type;typedef T*pointer;typedef T&reference;};template<class T>struct _Const_traits {typedef T value_type;typedef const T*pointer;typedef const T&reference;};
template<class T,class Traits>struct _List_iterator:public _List_iterator_base {typedef _List_iterator<T,_Nonconst_traits<T> >iterator;_List_iterator(_List_node<T>*n):_List_iterator_base(n){}_List_iterator(){}_List_iterator(const iterator&x):_List_iterator_base(x._M_node){}typename Traits::reference operator*()const{return static_cast<_List_node<T>*>(this->_M_node)->_M_data;}_List_iterator&operator++(){this->_M_incr();return *this;}};
template<class T,class A=allocator<T> > class list : public _List_base<T,A> {public:explicit list(const A&a=A()):_List_base<T,A>(a){}typedef _List_iterator<T,_Nonconst_traits<T> >iterator;typedef _List_iterator<T,_Const_traits<T> >const_iterator;const_iterator begin()const{return const_iterator(static_cast<_List_node<T>*>(this->_M_node._M_data->_M_next));}const_iterator end()const{return const_iterator(this->_M_node._M_data);}};

}


namespace std {
struct __false_type{};struct __true_type{};template<class T>struct __type_traits{typedef __false_type has_trivial_destructor;};
template<class T>inline void __destroy_aux(T*p,const __false_type&){p->~T();}template<class T>inline void __destroy_aux(T*,const __true_type&){}
template<class T>inline void _Destroy(T*p){typedef typename __type_traits<T>::has_trivial_destructor tag;__destroy_aux(p,tag());}
template<class It>inline void __destroy_aux(It first,It last,const __false_type&){for(;first!=last;++first)_Destroy(&*first);}template<class It>inline void __destroy_aux(It,It,const __true_type&){}
template<class It,class T>inline void __destroy(It first,It last,T*){typedef typename __type_traits<T>::has_trivial_destructor tag;__destroy_aux(first,last,tag());}
template<class T>inline T*__value_type(T*){return static_cast<T*>(0);}
template<class It>inline void _Destroy(It first,It last){__destroy(first,last,__value_type(first));}

template<class In,class Out>Out uninitialized_copy(In,In,Out);
template<class T,class A>class _Vector_base {public:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;public:_Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,static_cast<T*>(0)){}~_Vector_base(){if(_M_start!=0)_M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start);}};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A> {public:typedef unsigned int size_type;typedef T*iterator;typedef const T*const_iterator;explicit vector(const A&a=A()):_Vector_base<T,A>(a){}~vector(){_Destroy(this->_M_start,this->_M_finish);}iterator begin(){return this->_M_start;}iterator end(){return this->_M_finish;}const_iterator begin()const{return this->_M_start;}const_iterator end()const{return this->_M_finish;}size_type size()const{return size_type(this->_M_finish-this->_M_start);}public:template<class It>T*_M_allocate_and_copy(size_type n,It first,It last){T*p=this->_M_end_of_storage.allocate(n);uninitialized_copy(first,last,p);return p;}public:
__declspec(noinline)void reserve(size_type n){if(size_type(this->_M_end_of_storage._M_data-this->_M_start)<n){size_type old=size();T*tmp=_M_allocate_and_copy(n,this->_M_start,this->_M_finish);_Destroy(this->_M_start,this->_M_finish);if(this->_M_start)this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);this->_M_start=tmp;this->_M_finish=tmp+old;this->_M_end_of_storage._M_data=tmp+n;}}};
}


namespace std{template<class P>struct OutValue;template<class T>struct OutValue<T*>{typedef T type;};template<class In,class Out>Out uninitialized_copy(In first,In last,Out output){for(;first!=last;++first,++output)new(static_cast<void*>(output))typename OutValue<Out>::type(*first);return output;}}
class C3DMeshInfo{public:mutable bool Loaded;mutable int Checks,Loads;bool IsLoaded()const{++Checks;return Loaded;}void BeginLoading(unsigned long priority,bool a,bool b)const{CHECK(priority==static_cast<unsigned long>(-1)&&!a&&!b);++Loads;if(flipOnLoad)Loaded=true;}};
class CMeshDataBank{public:C3DMeshInfo*Entries[8];const C3DMeshInfo*PeekMeshInfo(unsigned long n)const{CHECK(n<8);++lookupCalls;return Entries[n];}};
namespace NGraphicAppearance{
class CMeshInfo{public:long Index;float SizeX;unsigned char PrimaryMesh:1;CMeshInfo(long i,float f,bool b):Index(i),SizeX(f),PrimaryMesh(b){}};
class CGraphicAppearanceDrawInfo;class CTCGraphicAppearance;
class CAGraphicAppearanceComponent{public:virtual~CAGraphicAppearanceComponent(){}virtual void GetMeshesUsed(std::vector<CMeshInfo>&)const=0;};
class CTCGraphicAppearance{public:std::list<CAGraphicAppearanceComponent*>Components;static CMeshDataBank*PMeshBank;bool IsResourcesLoaded(bool)const;};
CMeshDataBank*CTCGraphicAppearance::PMeshBank=0;
class CTestComponent:public CAGraphicAppearanceComponent{public:int N;long Indices[4];virtual void GetMeshesUsed(std::vector<CMeshInfo>&out)const{++componentCalls;for(int i=0;i<N;++i){if(out._M_finish==out._M_end_of_storage._M_data)out.reserve(out.size()+4);new(static_cast<void*>(out._M_finish))CMeshInfo(Indices[i],1.0f,true);++out._M_finish;}}};
bool CTCGraphicAppearance::IsResourcesLoaded(bool beginLoading)const{
 std::vector<CMeshInfo>meshes;meshes.reserve(4);
 for(std::list<CAGraphicAppearanceComponent*>::const_iterator i=reinterpret_cast<const std::list<CAGraphicAppearanceComponent*>&>(Components).begin();i!=reinterpret_cast<const std::list<CAGraphicAppearanceComponent*>&>(Components).end();++i)(*i)->GetMeshesUsed(meshes);
 bool loaded=true;
 for(CMeshInfo* mesh=meshes.begin();mesh!=meshes.end();++mesh){const C3DMeshInfo*info=PMeshBank->PeekMeshInfo(mesh->Index);if(!info->IsLoaded()){if(beginLoading)info->BeginLoading(static_cast<unsigned long>(-1),false,false);loaded=false;}}
 return loaded;
}
}
int main(){
 for(int n=0;n<=4;++n)for(int per=0;per<=3;++per)for(int mask=0;mask<16;++mask)for(int begin=0;begin<2;++begin)for(int flip=0;flip<2;++flip){
  CMeshDataBank bank;C3DMeshInfo meshes[8];bool expectedLoaded[8];int expectedChecks[8],expectedLoads[8];
  for(int j=0;j<8;++j){bank.Entries[j]=&meshes[j];meshes[j].Loaded=(mask&(1<<(j%4)))!=0;meshes[j].Checks=meshes[j].Loads=0;expectedLoaded[j]=meshes[j].Loaded;expectedChecks[j]=expectedLoads[j]=0;}
  NGraphicAppearance::CTCGraphicAppearance appearance;NGraphicAppearance::CTCGraphicAppearance::PMeshBank=&bank;
  NGraphicAppearance::CTestComponent parts[4];std::_List_node<NGraphicAppearance::CAGraphicAppearanceComponent*>nodes[5];
  for(int k=0;k<=n;++k){nodes[k]._M_next=&nodes[k==n?0:k+1];nodes[k]._M_prev=&nodes[k==0?n:k-1];if(k){nodes[k]._M_data=&parts[k-1];parts[k-1].N=per;for(int z=0;z<per;++z)parts[k-1].Indices[z]=(k+z)%4;}}
  appearance.Components._M_node._M_data=nodes;bool expected=true;
  for(int p=0;p<n;++p)for(int x=0;x<per;++x){int id=(p+1+x)%4;++expectedChecks[id];if(!expectedLoaded[id]){expected=false;if(begin){++expectedLoads[id];if(flip)expectedLoaded[id]=true;}}}
  flipOnLoad=flip;lookupCalls=componentCalls=0;CHECK(appearance.IsResourcesLoaded(begin!=0)==expected);CHECK(lookupCalls==n*per&&componentCalls==n);
  for(int q=0;q<8;++q)CHECK(meshes[q].Checks==expectedChecks[q]&&meshes[q].Loads==expectedLoads[q]&&meshes[q].Loaded==expectedLoaded[q]);
  ++cases;
 }
 CHECK(cases==1280);puts("004be3e0_TEST PASS");return 0;
}
