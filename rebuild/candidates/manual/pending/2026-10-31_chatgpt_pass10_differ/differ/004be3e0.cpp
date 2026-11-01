// 004be3e0 NGraphicAppearance::CTCGraphicAppearance::IsResourcesLoaded
// Leaf: ?IsResourcesLoaded@CTCGraphicAppearance@NGraphicAppearance@@QBE_N_N@Z
// Flags:
#define CTCGraphicAppearance CTCGraphicAppearanceFields
#include "engine/_quarantine/CTCGraphicAppearance.h"
#undef CTCGraphicAppearance
#define CMeshDataBank CMeshDataBankFields
#include "engine/CMeshDataBank.h"
#undef CMeshDataBank
#define C3DMeshInfo C3DMeshInfoFields
#include "engine/C3DMeshInfo.h"
#undef C3DMeshInfo
extern "C" void __cdecl free(void*);extern "C" void*__cdecl malloc(unsigned int);
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
template<class T,class A> struct _Rb_tree_base {public:_Rb_tree_base(const A&);protected:_STLP_alloc_proxy<_Rb_tree_node<T>*,_Rb_tree_node<T>,allocator<_Rb_tree_node<T> > > _M_header;};
template<class K,class V,class Select,class Compare,class A> class _Rb_tree : public _Rb_tree_base<V,A> {public:_Rb_tree();protected:unsigned int _M_node_count;Compare _M_key_compare;};
template<class K,class V,class Compare=less<K>,class A=allocator<pair<const K,V> > > class map {private:_Rb_tree<K,pair<const K,V>,_Select1st<pair<const K,V> >,Compare,A> _M_t;};
struct _List_node_base {_List_node_base* _M_next;_List_node_base* _M_prev;};
template<class T> struct _List_node : public _List_node_base {T _M_data;};
template<class T,class A> class _List_base {public:_List_base(const A&);_STLP_alloc_proxy<_List_node<T>*,_List_node<T>,allocator<_List_node<T> > > _M_node;};
struct _List_iterator_base {_List_node_base*_M_node;_List_iterator_base(_List_node_base*n):_M_node(n){}_List_iterator_base(){}void _M_incr(){_M_node=_M_node->_M_next;}bool operator!=(const _List_iterator_base&x)const{return _M_node!=x._M_node;}};
template<class T>struct _Nonconst_traits {typedef T value_type;typedef T*pointer;typedef T&reference;};template<class T>struct _Const_traits {typedef T value_type;typedef const T*pointer;typedef const T&reference;};
template<class T,class Traits>struct _List_iterator:public _List_iterator_base {typedef _List_iterator<T,_Nonconst_traits<T> >iterator;_List_iterator(_List_node<T>*n):_List_iterator_base(n){}_List_iterator(){}_List_iterator(const iterator&x):_List_iterator_base(x._M_node){}typename Traits::reference operator*()const{return static_cast<_List_node<T>*>(this->_M_node)->_M_data;}_List_iterator&operator++(){this->_M_incr();return *this;}};
template<class T,class A=allocator<T> > class list : public _List_base<T,A> {public:explicit list(const A& =A());typedef _List_iterator<T,_Nonconst_traits<T> >iterator;typedef _List_iterator<T,_Const_traits<T> >const_iterator;const_iterator begin()const{return const_iterator(static_cast<_List_node<T>*>(this->_M_node._M_data->_M_next));}const_iterator end()const{return const_iterator(this->_M_node._M_data);}};

}
#include <stddef.h>
extern "C" void __cdecl free(void*);
namespace std {
struct __false_type{};struct __true_type{};template<class T>struct __type_traits{typedef __false_type has_trivial_destructor;};
template<class T>inline void __destroy_aux(T*p,const __false_type&){p->~T();}template<class T>inline void __destroy_aux(T*,const __true_type&){}
template<class T>inline void _Destroy(T*p){typedef typename __type_traits<T>::has_trivial_destructor tag;__destroy_aux(p,tag());}
template<class It>inline void __destroy_aux(It first,It last,const __false_type&){for(;first!=last;++first)_Destroy(&*first);}template<class It>inline void __destroy_aux(It,It,const __true_type&){}
template<class It,class T>inline void __destroy(It first,It last,T*){typedef typename __type_traits<T>::has_trivial_destructor tag;__destroy_aux(first,last,tag());}
template<class T>inline T*__value_type(T*){return static_cast<T*>(0);}
template<class It>inline void _Destroy(It first,It last){__destroy(first,last,__value_type(first));}

template<class In,class Out>Out uninitialized_copy(In,In,Out);
template<class T,class A>class _Vector_base {protected:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;public:_Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,static_cast<T*>(0)){}~_Vector_base(){if(_M_start!=0)_M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start);}};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A> {public:typedef unsigned int size_type;typedef T*iterator;typedef const T*const_iterator;explicit vector(const A&a=A()):_Vector_base<T,A>(a){}~vector(){_Destroy(this->_M_start,this->_M_finish);}iterator begin(){return this->_M_start;}iterator end(){return this->_M_finish;}const_iterator begin()const{return this->_M_start;}const_iterator end()const{return this->_M_finish;}size_type size()const{return size_type(this->_M_finish-this->_M_start);}protected:template<class It>T*_M_allocate_and_copy(size_type n,It first,It last){T*p=this->_M_end_of_storage.allocate(n);uninitialized_copy(first,last,p);return p;}public:
__declspec(noinline)void reserve(size_type n){if(size_type(this->_M_end_of_storage._M_data-this->_M_start)<n){size_type old=size();T*tmp=_M_allocate_and_copy(n,this->_M_start,this->_M_finish);_Destroy(this->_M_start,this->_M_finish);if(this->_M_start)this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);this->_M_start=tmp;this->_M_finish=tmp+old;this->_M_end_of_storage._M_data=tmp+n;}}};
}

// Layout: original supplied flattened receiver fields; methods add no storage.
class C3DMeshInfo:public C3DMeshInfoFields{public:bool IsLoaded()const;void BeginLoading(unsigned long,bool,bool)const;};
class CMeshDataBank:public CMeshDataBankFields{public:__declspec(noinline)const C3DMeshInfo*PeekMeshInfo(unsigned long n)const{return reinterpret_cast<C3DMeshInfo*const*>(*reinterpret_cast<void*const*>(MeshesAndAnimations))[n];}};
namespace NGraphicAppearance {
class CMeshInfo {public:long Index;float SizeX;unsigned char PrimaryMesh:1;CMeshInfo(long,float,bool);};
class CGraphicAppearanceDrawInfo;class CTCGraphicAppearance;
class CAGraphicAppearanceComponent{public:~CAGraphicAppearanceComponent();virtual bool OnAppearanceDraw(CTCGraphicAppearance&,const CGraphicAppearanceDrawInfo&)=0;virtual void OnAppearanceClear()=0;virtual void GetMeshesUsed(std::vector<CMeshInfo>&)const;};
class CTCGraphicAppearance:public CTCGraphicAppearanceFields {protected:static CMeshDataBank*PMeshBank;public:bool IsResourcesLoaded(bool)const;};
bool CTCGraphicAppearance::IsResourcesLoaded(bool beginLoading)const{
 std::vector<CMeshInfo>meshes;meshes.reserve(4);
 for(std::list<CAGraphicAppearanceComponent*>::const_iterator i=reinterpret_cast<const std::list<CAGraphicAppearanceComponent*>&>(Components).begin();i!=reinterpret_cast<const std::list<CAGraphicAppearanceComponent*>&>(Components).end();++i)(*i)->GetMeshesUsed(meshes);
 bool loaded=true;
 for(CMeshInfo* mesh=meshes.begin();mesh!=meshes.end();++mesh){const C3DMeshInfo*info=PMeshBank->PeekMeshInfo(mesh->Index);if(!info->IsLoaded()){if(beginLoading)info->BeginLoading(static_cast<unsigned long>(-1),false,false);loaded=false;}}
 return loaded;
}
}
