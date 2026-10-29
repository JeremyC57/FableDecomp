#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#define __stdcall
#define __declspec(x)
#define __forceinline inline
#define __int64 __attribute__((__mode__(__DI__))) long
#endif
#define PASS9_TEST
#include <new>
int main();
extern "C" void*__cdecl TestMalloc(unsigned int);extern "C" void __cdecl TestFree(void*);
#define malloc TestMalloc
#define free TestFree
// 00ad59d0 std::__set_intersection overconstULtreeiterators andinsert_iterator<set<UL>>
// Leaf: ??$__set_intersection@U?$_Rb_tree_iterator@KU?$_Const_traits@K@std@@@std@@U12@V?$insert_iterator@V?$set@KU?$less@K@std@@V?$allocator@K@2@@std@@@2@U?$less@K@2@@std@@YI?AV?$insert_iterator@V?$set@KU?$less@K@std@@V?$allocator@K@2@@std@@@0@U?$_Rb_tree_iterator@KU?$_Const_traits@K@std@@@0@000V10@U?$less@K@0@@Z
// Flags: /Gr
#include <stdlib.h>
#include "rebuild_abi.h"

namespace std {
template<class T>class allocator{public:allocator(){}allocator(const allocator&){}T*allocate(unsigned int count){return count?(T*)malloc(count*sizeof(T)):0;}void deallocate(T*p,unsigned int){if(p)free(p);}};
template<class P,class T,class A>class _STLP_alloc_proxy:public A{public:P _M_data;_STLP_alloc_proxy(const A&a,P p):A(a),_M_data(p){}};
template<class T>struct less{bool operator()(const T&a,const T&b)const{return a<b;}};
template<class T>struct _Identity{const T&operator()(const T&value)const{return value;}};
template<class T1,class T2>struct pair{typedef T1 first_type;typedef T2 second_type;T1 first;T2 second;pair(const T1&a,const T2&b):first(a),second(b){}};
struct _Rb_tree_node_base{bool _M_color;_Rb_tree_node_base*_M_parent,*_M_left,*_M_right;};
template<class V>struct _Rb_tree_node:public _Rb_tree_node_base{V _M_value_field;explicit _Rb_tree_node(const V&value):_M_value_field(value){}};
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

namespace std{struct input_iterator_tag{};struct output_iterator_tag{};struct forward_iterator_tag:public input_iterator_tag{};struct bidirectional_iterator_tag:public forward_iterator_tag{};template<class Category,class T,class Distance,class Pointer,class Reference>struct iterator{};
template<class Container>class insert_iterator:public iterator<output_iterator_tag,void,void,void,void>{protected:Container*container;typename Container::iterator iter;public:insert_iterator(Container&owner,typename Container::iterator position):container(&owner),iter(position){}insert_iterator&operator=(const typename Container::value_type&value){iter=container->insert(iter,value);++iter;return*this;}insert_iterator&operator*(){return*this;}insert_iterator&operator++(){return*this;}insert_iterator&operator++(int){return*this;}
#ifdef PASS9_TEST
friend int ::main();
#endif
};
template<class Input,class Output,class Distance>inline Output __fastcall __copy(Input first,Input last,Output result,const input_iterator_tag&,Distance*){for(;first!=last;++result,++first)*result=*first;return result;}
template<class Input,class Output>inline Output __fastcall copy(Input first,Input last,Output result){return __copy(first,last,result,bidirectional_iterator_tag(),(int*)0);}
template <class _InputIter1, class _InputIter2, class _OutputIter,
          class _Compare>
_OutputIter __fastcall __set_intersection(_InputIter1 __first1, _InputIter1 __last1,
                               _InputIter2 __first2, _InputIter2 __last2,
                               _OutputIter __result, _Compare __comp) {
  while (__first1 != __last1 && __first2 != __last2)
    if (__comp(*__first1, *__first2))
      ++__first1;
    else if (__comp(*__first2, *__first1))
      ++__first2;
    else {
      *__result = *__first1;
      ++__first1;
      ++__first2;
      ++__result;
    }
  return __result;
}
template insert_iterator<set<unsigned long> > __fastcall __set_intersection<_Rb_tree_iterator<unsigned long,_Const_traits<unsigned long> >,_Rb_tree_iterator<unsigned long,_Const_traits<unsigned long> >,insert_iterator<set<unsigned long> >,less<unsigned long> >(_Rb_tree_iterator<unsigned long,_Const_traits<unsigned long> >,_Rb_tree_iterator<unsigned long,_Const_traits<unsigned long> >,_Rb_tree_iterator<unsigned long,_Const_traits<unsigned long> >,_Rb_tree_iterator<unsigned long,_Const_traits<unsigned long> >,insert_iterator<set<unsigned long> >,less<unsigned long>);
}

#undef malloc
#undef free

static unsigned errors,treeCopies,active,allocationCount;struct Allocation{unsigned char*raw,*data;unsigned size;bool released;};static Allocation records[128];
extern "C" void*__cdecl TestMalloc(unsigned int size){if(allocationCount==128){++errors;return 0;}Allocation&r=records[allocationCount++];r.raw=static_cast<unsigned char*>(malloc(size+32));r.data=r.raw+16;r.size=size;r.released=false;memset(r.raw,0xa5,size+32);++active;return r.data;}
extern "C" void __cdecl TestFree(void*p){Allocation*r=0;for(unsigned i=0;i<allocationCount;++i)if(records[i].data==p&&!records[i].released)r=records+i;if(!r){++errors;return;}for(unsigned i=0;i<16;++i)if(r->raw[i]!=0xa5||r->data[r->size+i]!=0xa5)++errors;r->released=true;--active;free(r->raw);}
namespace std{
template<class K,class V,class Extract,class Compare,class A>pair<typename _Rb_tree<K,V,Extract,Compare,A>::iterator,bool>_Rb_tree<K,V,Extract,Compare,A>::insert_unique(const V&value){_Rb_tree_node_base*parent=this->_M_header._M_data;_Rb_tree_node_base*node=_M_root();bool left=true;while(node){parent=node;const V&current=static_cast<_Rb_tree_node<V>*>(node)->_M_value_field;if(_M_key_compare(value,current)){left=true;node=node->_M_left;}else if(_M_key_compare(current,value)){left=false;node=node->_M_right;}else return pair<iterator,bool>(iterator(static_cast<_Rb_tree_node<V>*>(node)),false);} _Link_type result=this->_M_header.allocate(1);new(result)_Rb_tree_node<V>(value);result->_M_color=true;result->_M_parent=parent;result->_M_left=result->_M_right=0;if(parent==this->_M_header._M_data)_M_root()=result;else if(left)parent->_M_left=result;else parent->_M_right=result;if(_M_node_count==0||_M_key_compare(value,_M_leftmost()->_M_value_field))_M_leftmost()=result;if(_M_node_count==0||_M_key_compare(_M_rightmost()->_M_value_field,value))_M_rightmost()=result;++_M_node_count;return pair<iterator,bool>(iterator(result),true);}
template<class K,class V,class Extract,class Compare,class A>_Rb_tree<K,V,Extract,Compare,A>::_Rb_tree(const _Rb_tree&other):_Rb_tree_base<V,A>(A()),_M_node_count(0),_M_key_compare(other._M_key_compare){_M_empty_initialize();for(const_iterator it=other.begin();it!=other.end();++it)insert_unique(*it);++treeCopies;}
template<class K,class V,class Extract,class Compare,class A>void _Rb_tree<K,V,Extract,Compare,A>::_M_erase(_Link_type node){if(!node)return;_M_erase(static_cast<_Link_type>(node->_M_left));_M_erase(static_cast<_Link_type>(node->_M_right));this->_M_header.deallocate(node,1);}
template<class K,class V,class Extract,class Compare,class A>typename _Rb_tree<K,V,Extract,Compare,A>::iterator _Rb_tree<K,V,Extract,Compare,A>::insert_unique(iterator position,const V&value){(void)position;return insert_unique(value).first;}
}

static const unsigned long keys[8]={0,1,2,0x7fffffffUL,0x80000000UL,0xfffffffdUL,0xfffffffeUL,0xffffffffUL};
static unsigned observe(const std::set<unsigned long>&values){unsigned mask=0;for(std::set<unsigned long>::const_iterator it=values.begin();it!=values.end();++it){bool found=false;for(unsigned i=0;i<8;++i)if(*it==keys[i]){if(mask&(1U<<i))return 512;mask|=1U<<i;found=true;}if(!found)return 512;}return mask;}
int main(){unsigned cases=0;for(unsigned leftMask=0;leftMask<256;++leftMask)for(unsigned rightMask=0;rightMask<256;++rightMask)for(unsigned existingMode=0;existingMode<3;++existingMode){allocationCount=active=errors=treeCopies=0;{std::set<unsigned long>left,right,output;unsigned existingMask=existingMode==0?0:existingMode==1?0x55:0xff;for(unsigned i=0;i<8;++i){if(leftMask&(1U<<i))left.insert(keys[i]);if(rightMask&(1U<<(7-i)))right.insert(keys[7-i]);if(existingMask&(1U<<i))output.insert(keys[i]);}std::_Rb_tree_node<unsigned long>*leftHeader=left._M_t._M_header._M_data;std::_Rb_tree_node<unsigned long>*rightHeader=right._M_t._M_header._M_data;unsigned char leftBefore[20],rightBefore[20];memcpy(leftBefore,leftHeader,20);memcpy(rightBefore,rightHeader,20);std::set<unsigned long>::iterator initial=existingMode==1?output.begin():output.end();std::insert_iterator<std::set<unsigned long> >start(output,initial);std::insert_iterator<std::set<unsigned long> >result=std::__set_intersection(left.begin(),left.end(),right.begin(),right.end(),start,std::less<unsigned long>());unsigned selected=leftMask&rightMask;unsigned expected=existingMask|selected;if(observe(output)!=expected||observe(left)!=leftMask||observe(right)!=rightMask)return 1;if(memcmp(leftBefore,leftHeader,20)||memcmp(rightBefore,rightHeader,20))return 2;if(result.container!=&output||start.container!=&output||start.iter!=initial)return 3;std::set<unsigned long>::iterator expectedPosition=initial;if(selected){unsigned last=7;while(!(selected&(1U<<last)))--last;expectedPosition=output.end();for(std::set<unsigned long>::iterator it=output.begin();it!=output.end();++it)if(*it>keys[last]){expectedPosition=it;break;}}if(result.iter!=expectedPosition)return 4;}if(active||errors)return 5;++cases;}if(cases!=196608)return 6;puts("00ad59d0_TEST PASS");return 0;}