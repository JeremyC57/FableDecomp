// 00ad7c70 std::vector<QuadricOptimiserInternals::CUniqueVertex>::_M_insert_overflow
// Leaf: ?_M_insert_overflow@?$vector@VCUniqueVertex@QuadricOptimiserInternals@@V?$allocator@VCUniqueVertex@QuadricOptimiserInternals@@@std@@@std@@IAEXPAVCUniqueVertex@QuadricOptimiserInternals@@ABV34@ABU__false_type@2@I_N@Z
// Flags: /Gr
#include <stdlib.h>
#include <new>
#include "rebuild_abi.h"

namespace std {
template<class T>class allocator{public:allocator(){}allocator(const allocator&){}T*allocate(unsigned int count){return count?(T*)malloc(count*sizeof(T)):0;}void deallocate(T*p,unsigned int){if(p)free(p);}};
template<class P,class T,class A>class _STLP_alloc_proxy:public A{public:P _M_data;_STLP_alloc_proxy(const A&a,P p):A(a),_M_data(p){}};
template<class T>struct less{bool operator()(const T&a,const T&b)const{return a<b;}};
template<class T>struct _Identity{const T&operator()(const T&value)const{return value;}};
template<class T1,class T2>struct pair{typedef T1 first_type;typedef T2 second_type;T1 first;T2 second;pair(const T1&a,const T2&b):first(a),second(b){}};
struct _Rb_tree_node_base{bool _M_color;_Rb_tree_node_base*_M_parent,*_M_left,*_M_right;};
template<class V>struct _Rb_tree_node:public _Rb_tree_node_base{V _M_value_field;};
template<class D>class _Rb_global{public:static _Rb_tree_node_base*__cdecl _M_increment(_Rb_tree_node_base*node){if(node->_M_right){node=node->_M_right;while(node->_M_left)node=node->_M_left;}else{_Rb_tree_node_base*parent=node->_M_parent;while(node==parent->_M_right){node=parent;parent=parent->_M_parent;}if(node->_M_right!=parent)node=parent;}return node;}};
typedef _Rb_global<bool>_Rb_global_inst;
struct _Rb_tree_base_iterator{_Rb_tree_node_base*_M_node;bool operator==(const _Rb_tree_base_iterator&other)const{return _M_node==other._M_node;}bool operator!=(const _Rb_tree_base_iterator&other)const{return _M_node!=other._M_node;}};
template<class T>struct _Nonconst_traits{typedef T&reference;typedef T*pointer;};template<class T>struct _Const_traits{typedef const T&reference;typedef const T*pointer;};
template<class V,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator{_Rb_tree_iterator(){_M_node=0;}_Rb_tree_iterator(_Rb_tree_node<V>*node){_M_node=node;}_Rb_tree_iterator(const _Rb_tree_iterator<V,_Nonconst_traits<V> >&other){_M_node=other._M_node;}typename Traits::reference operator*()const{return static_cast<_Rb_tree_node<V>*>(_M_node)->_M_value_field;}_Rb_tree_iterator&operator++(){_M_node=_Rb_global_inst::_M_increment(_M_node);return*this;}};
template<class V,class A>struct _Rb_tree_base{protected:_STLP_alloc_proxy<_Rb_tree_node<V>*,_Rb_tree_node<V>,allocator<_Rb_tree_node<V> > >_M_header;public:_Rb_tree_base(const A&a):_M_header(allocator<_Rb_tree_node<V> >(),0){(void)a;_M_header._M_data=_M_header.allocate(1);}~_Rb_tree_base(){_M_header.deallocate(_M_header._M_data,1);}};
template<class K,class V,class Extract,class Compare,class A>class _Rb_tree:public _Rb_tree_base<V,A>{public:typedef _Rb_tree_node<V>*_Link_type;typedef _Rb_tree_iterator<V,_Nonconst_traits<V> >iterator;typedef _Rb_tree_iterator<V,_Const_traits<V> >const_iterator;
protected:unsigned int _M_node_count;Compare _M_key_compare;_Link_type&_M_root()const{return (_Link_type&)this->_M_header._M_data->_M_parent;}_Link_type&_M_leftmost()const{return (_Link_type&)this->_M_header._M_data->_M_left;}_Link_type&_M_rightmost()const{return (_Link_type&)this->_M_header._M_data->_M_right;}
private:void _M_erase(_Link_type);void _M_empty_initialize(){this->_M_header._M_data->_M_color=false;_M_root()=0;_M_leftmost()=this->_M_header._M_data;_M_rightmost()=this->_M_header._M_data;}
public:_Rb_tree(const Compare&compare,const A&a):_Rb_tree_base<V,A>(a),_M_node_count(0),_M_key_compare(compare){_M_empty_initialize();}_Rb_tree(const _Rb_tree&);~_Rb_tree(){clear();}void clear(){if(_M_node_count){_M_erase(_M_root());_M_leftmost()=this->_M_header._M_data;_M_root()=0;_M_rightmost()=this->_M_header._M_data;_M_node_count=0;}}const_iterator begin()const{return const_iterator(_M_leftmost());}const_iterator end()const{return const_iterator(this->_M_header._M_data);}pair<iterator,bool>insert_unique(const V&);iterator insert_unique(iterator,const V&);
#ifdef PASS9_TEST
friend int ::main();
#endif
};
template<class K,class Compare=less<K>,class A=allocator<K> >class set{typedef _Rb_tree<K,K,_Identity<K>,Compare,A>_Rep_type;_Rep_type _M_t;public:typedef K value_type;typedef typename _Rep_type::const_iterator iterator;typedef iterator const_iterator;set():_M_t(Compare(),A()){}set(const set&other):_M_t(other._M_t){}const_iterator begin()const{return _M_t.begin();}const_iterator end()const{return _M_t.end();}pair<iterator,bool>insert(const K&value){pair<typename _Rep_type::iterator,bool>result=_M_t.insert_unique(value);return pair<iterator,bool>(iterator(result.first),result.second);}iterator insert(iterator position,const K&value){typename _Rep_type::iterator mutablePosition;mutablePosition._M_node=position._M_node;return iterator(_M_t.insert_unique(mutablePosition,value));}
#ifdef PASS9_TEST
friend int ::main();
#endif
};
}
namespace QuadricOptimiserInternals{class CIndexList{private:std::set<unsigned long>Indices;};class CUniqueVertex{public:bool Exists;CIndexList VertexIndices,FaceList;CUniqueVertex();~CUniqueVertex();};}
namespace std{struct __false_type{};template<class T>inline const T&max(const T&a,const T&b){return a<b?b:a;}
template<class T1,class T2>inline void _Construct(T1*p,const T2&value){new(p)T1(value);}template<class T>inline void _Destroy(T*p){p->~T();}template<class Forward>inline void _Destroy(Forward first,Forward last){for(;first!=last;++first)_Destroy(&*first);}
template<class Input,class Forward>inline Forward __fastcall __uninitialized_copy(Input first,Input last,Forward result,const __false_type&){Forward current=result;for(;first!=last;++first,++current)_Construct(&*current,*first);return current;}
template<class Forward,class Size,class T>inline Forward __fastcall __uninitialized_fill_n(Forward first,Size count,const T&value,const __false_type&){Forward current=first;for(;count>0;--count,++current)_Construct(&*current,value);return current;}
template<class T,class A>class _Vector_base{protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;public:_Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}~_Vector_base(){if(_M_start)this->_M_end_of_storage.deallocate(_M_start,this->_M_end_of_storage._M_data-_M_start);}};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:typedef unsigned int size_type;typedef T*pointer;explicit vector(const A&a=A()):_Vector_base<T,A>(a){}~vector(){_Destroy(this->_M_start,this->_M_finish);}size_type size()const{return this->_M_finish-this->_M_start;}protected:void _M_clear(){_Destroy(this->_M_start,this->_M_finish);this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);}void _M_set(pointer start,pointer finish,pointer end){this->_M_start=start;this->_M_finish=finish;this->_M_end_of_storage._M_data=end;}void _M_insert_overflow(pointer,const T&,const __false_type&,size_type,bool=false);};
template<class T,class A>void vector<T,A>::_M_insert_overflow(pointer position,const T&value,const __false_type&,size_type fill,bool atEnd){const size_type oldSize=size();const size_type length=oldSize+(max)(oldSize,fill);pointer start=this->_M_end_of_storage.allocate(length);pointer finish=start;finish=__uninitialized_copy(this->_M_start,position,start,__false_type());if(fill==1){_Construct(finish,value);++finish;}else finish=__uninitialized_fill_n(finish,fill,value,__false_type());if(!atEnd)finish=__uninitialized_copy(position,this->_M_finish,finish,__false_type());_M_clear();_M_set(start,finish,start+length);}
template void vector<QuadricOptimiserInternals::CUniqueVertex>::_M_insert_overflow(QuadricOptimiserInternals::CUniqueVertex*,const QuadricOptimiserInternals::CUniqueVertex&,const __false_type&,unsigned int,bool);
}

FABLE_STATIC_ASSERT(sizeof(QuadricOptimiserInternals::CUniqueVertex)==28);