#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
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
// 00ad94f0 CVertexGroupBlockOptimiser::FindBestStartingBlock
// Leaf: ?FindBestStartingBlock@CVertexGroupBlockOptimiser@@AAEKXZ
// Flags:
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
template<class V,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator{_Rb_tree_iterator(){_M_node=0;}_Rb_tree_iterator(_Rb_tree_node<V>*node){_M_node=node;}_Rb_tree_iterator(const _Rb_tree_iterator<V,_Nonconst_traits<V> >&other){_M_node=other._M_node;}typename Traits::pointer operator->()const{return &operator*();}typename Traits::reference operator*()const{return static_cast<_Rb_tree_node<V>*>(_M_node)->_M_value_field;}_Rb_tree_iterator&operator++(){_M_node=_Rb_global_inst::_M_increment(_M_node);return*this;}};
template<class V,class A>struct _Rb_tree_base{protected:_STLP_alloc_proxy<_Rb_tree_node<V>*,_Rb_tree_node<V>,allocator<_Rb_tree_node<V> > >_M_header;public:_Rb_tree_base(const A&a):_M_header(allocator<_Rb_tree_node<V> >(),0){(void)a;_M_header._M_data=_M_header.allocate(1);}~_Rb_tree_base(){_M_header.deallocate(_M_header._M_data,1);}};
template<class K,class V,class Extract,class Compare,class A>class _Rb_tree:public _Rb_tree_base<V,A>{public:typedef _Rb_tree_node<V>*_Link_type;typedef _Rb_tree_iterator<V,_Nonconst_traits<V> >iterator;typedef _Rb_tree_iterator<V,_Const_traits<V> >const_iterator;
protected:unsigned int _M_node_count;Compare _M_key_compare;_Link_type&_M_root()const{return (_Link_type&)this->_M_header._M_data->_M_parent;}_Link_type&_M_leftmost()const{return (_Link_type&)this->_M_header._M_data->_M_left;}_Link_type&_M_rightmost()const{return (_Link_type&)this->_M_header._M_data->_M_right;}
private:void _M_erase(_Link_type node){while(node){_M_erase(static_cast<_Link_type>(node->_M_right));_Link_type left=static_cast<_Link_type>(node->_M_left);node->_M_value_field.~V();this->_M_header.deallocate(node,1);node=left;}}void _M_empty_initialize(){this->_M_header._M_data->_M_color=false;_M_root()=0;_M_leftmost()=this->_M_header._M_data;_M_rightmost()=this->_M_header._M_data;}
public:_Rb_tree(const Compare&compare,const A&a):_Rb_tree_base<V,A>(a),_M_node_count(0),_M_key_compare(compare){_M_empty_initialize();}_Rb_tree(const _Rb_tree&);~_Rb_tree(){clear();}void clear(){if(_M_node_count){_M_erase(_M_root());_M_leftmost()=this->_M_header._M_data;_M_root()=0;_M_rightmost()=this->_M_header._M_data;_M_node_count=0;}}unsigned int size()const{return _M_node_count;}iterator begin(){return iterator(_M_leftmost());}iterator end(){return iterator(this->_M_header._M_data);}const_iterator find(const K&key)const{_Link_type lower=this->_M_header._M_data;_Link_type node=_M_root();while(node){if(!_M_key_compare(Extract()(node->_M_value_field),key)){lower=node;node=static_cast<_Link_type>(node->_M_left);}else node=static_cast<_Link_type>(node->_M_right);}if(lower==this->_M_header._M_data||_M_key_compare(key,Extract()(lower->_M_value_field)))lower=this->_M_header._M_data;return const_iterator(lower);}const_iterator begin()const{return const_iterator(_M_leftmost());}const_iterator end()const{return const_iterator(this->_M_header._M_data);}pair<iterator,bool>insert_unique(const V&);iterator insert_unique(iterator,const V&);
#ifdef PASS9_TEST
friend int ::main();
#endif
};
}
namespace std{template<class P>struct _Select1st{const typename P::first_type&operator()(const P&value)const{return value.first;}};template<class K,class T,class Compare=less<K>,class A=allocator<pair<const K,T> > >class map{public:typedef pair<const K,T>value_type;private:typedef _Rb_tree<K,value_type,_Select1st<value_type>,Compare,A>_Rep_type;_Rep_type _M_t;public:typedef typename _Rep_type::iterator iterator;typedef typename _Rep_type::const_iterator const_iterator;map():_M_t(Compare(),A()){}map(const map&other):_M_t(other._M_t){}map&operator=(const map&);iterator begin(){return _M_t.begin();}iterator end(){return _M_t.end();}const_iterator begin()const{return _M_t.begin();}const_iterator end()const{return _M_t.end();}const_iterator find(const K&key)const{return _M_t.find(key);}pair<iterator,bool>insert(const value_type&v){return _M_t.insert_unique(v);}T&operator[](const K&);bool empty()const{return _M_t.size()==0;}
#ifdef PASS9_TEST
friend int ::main();
#endif
};}
namespace std{template<class T,class A>class _Vector_base{protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;public:_Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}~_Vector_base(){if(_M_start)_M_end_of_storage.deallocate(_M_start,(unsigned int)(_M_end_of_storage._M_data-_M_start));}};template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:vector():_Vector_base<T,A>(A()){}vector(const vector&);~vector(){for(T*p=this->_M_start;p!=this->_M_finish;++p)p->~T();}unsigned int size()const{return this->_M_finish-this->_M_start;}const T&operator[](unsigned int i)const{return this->_M_start[i];}};}
// Layout: true CArray/vector base supported by empty donorCArray and nativeunadjustedvectorcalls.
template<class T>class CArray:public std::vector<T>{};
class CVertexGroupBlock{CArray<unsigned long>Groups;std::map<unsigned long,long>GroupToIndexMap;
#ifdef PASS9_TEST
friend int ::main();
#endif
public:unsigned long GetGroupCount()const{return Groups.size();}unsigned long GetGroup(unsigned long i)const{return Groups[i];}};
class CVertexGroupBlockList{CArray<CVertexGroupBlock>Blocks;
#ifdef PASS9_TEST
friend int ::main();
#endif
public:unsigned long GetBlockCount()const{return Blocks.size();}CVertexGroupBlock GetBlock(unsigned long i)const{return Blocks[i];}};
class CVertexGroupBlockOptimiser{class CGroupInfo{public:unsigned long Count;long BlockIndex;CGroupInfo():Count(0),BlockIndex(-1){}};CVertexGroupBlockList InputList;unsigned char unreadListsAndPointer[12];std::vector<bool>RemainingBlocks;unsigned char unreadTail[56];
#ifdef PASS9_TEST
friend int ::main();
#endif
unsigned long FindBestStartingBlock();};
unsigned long CVertexGroupBlockOptimiser::FindBestStartingBlock(){std::map<unsigned long,CGroupInfo>counts;unsigned long n=InputList.GetBlockCount();for(unsigned long i=0;i<n;++i)if(RemainingBlocks[i]){CVertexGroupBlock block=InputList.GetBlock(i);unsigned long groupCount=block.GetGroupCount();for(unsigned long j=0;j<groupCount;++j){CGroupInfo&info=counts[block.GetGroup(j)];++info.Count;info.BlockIndex=i;}}unsigned long bestBlock=(unsigned long)-1,bestCount=(unsigned long)-1;for(std::map<unsigned long,CGroupInfo>::iterator it=counts.begin();it!=counts.end();++it)if(it->second.Count<bestCount){bestBlock=it->second.BlockIndex;bestCount=it->second.Count;}return bestBlock;}
FABLE_STATIC_ASSERT(sizeof(CVertexGroupBlockOptimiser)==92);FABLE_STATIC_ASSERT(sizeof(CVertexGroupBlock)==24);

#undef malloc
#undef free
static unsigned errors,treeCopies,active,allocationCount;struct Allocation{unsigned char*raw,*data;unsigned size;bool released;};static Allocation records[256];
extern "C" void*__cdecl TestMalloc(unsigned int size){if(allocationCount==256){++errors;return 0;}Allocation&r=records[allocationCount++];r.raw=static_cast<unsigned char*>(malloc(size+32));r.data=r.raw+16;r.size=size;r.released=false;memset(r.raw,0xa5,size+32);++active;return r.data;}
extern "C" void __cdecl TestFree(void*p){Allocation*r=0;for(unsigned i=0;i<allocationCount;++i)if(records[i].data==p&&!records[i].released)r=records+i;if(!r){++errors;return;}for(unsigned i=0;i<16;++i)if(r->raw[i]!=0xa5||r->data[r->size+i]!=0xa5)++errors;r->released=true;--active;free(r->raw);}
namespace std{
template<class K,class V,class Extract,class Compare,class A>pair<typename _Rb_tree<K,V,Extract,Compare,A>::iterator,bool>_Rb_tree<K,V,Extract,Compare,A>::insert_unique(const V&value){_Rb_tree_node_base*parent=this->_M_header._M_data;_Rb_tree_node_base*node=_M_root();bool left=true;while(node){parent=node;const V&current=static_cast<_Rb_tree_node<V>*>(node)->_M_value_field;if(_M_key_compare(Extract()(value),Extract()(current))){left=true;node=node->_M_left;}else if(_M_key_compare(Extract()(current),Extract()(value))){left=false;node=node->_M_right;}else return pair<iterator,bool>(iterator(static_cast<_Rb_tree_node<V>*>(node)),false);} _Link_type result=this->_M_header.allocate(1);new(result)_Rb_tree_node<V>(value);result->_M_color=true;result->_M_parent=parent;result->_M_left=result->_M_right=0;if(parent==this->_M_header._M_data)_M_root()=result;else if(left)parent->_M_left=result;else parent->_M_right=result;if(_M_node_count==0||_M_key_compare(Extract()(value),Extract()(_M_leftmost()->_M_value_field)))_M_leftmost()=result;if(_M_node_count==0||_M_key_compare(Extract()(_M_rightmost()->_M_value_field),Extract()(value)))_M_rightmost()=result;++_M_node_count;return pair<iterator,bool>(iterator(result),true);}
template<class K,class V,class Extract,class Compare,class A>_Rb_tree<K,V,Extract,Compare,A>::_Rb_tree(const _Rb_tree&other):_Rb_tree_base<V,A>(A()),_M_node_count(0),_M_key_compare(other._M_key_compare){_M_empty_initialize();for(const_iterator it=other.begin();it!=other.end();++it)insert_unique(*it);++treeCopies;}

}


namespace std{
template<class K,class T,class C,class A>T&map<K,T,C,A>::operator[](const K&key){for(iterator it=begin();it!=end();++it)if(it->first==key)return it->second;return _M_t.insert_unique(value_type(key,T())).first->second;}
template<class T,class A>vector<T,A>::vector(const vector&other):_Vector_base<T,A>(A()){unsigned count=other._M_start?other._M_finish-other._M_start:0;this->_M_start=static_cast<T*>(TestMalloc((count?count:1)*sizeof(T)));this->_M_finish=this->_M_start;this->_M_end_of_storage._M_data=this->_M_start+count;for(unsigned i=0;i<count;++i,++this->_M_finish)new(this->_M_finish)T(other._M_start[i]);}
}
int main(){unsigned cases=0;for(unsigned n=0;n<=4;++n){unsigned limit=1;for(unsigned i=0;i<n;++i)limit*=8;for(unsigned sequence=0;sequence<limit;++sequence)for(unsigned remaining=0;remaining<(1U<<n);++remaining){allocationCount=active=errors=treeCopies=0;{
CVertexGroupBlockOptimiser owner;memset(owner.unreadListsAndPointer,0x35,12);memset(owner.unreadTail,0x63,56);
CVertexGroupBlock*blocks=static_cast<CVertexGroupBlock*>(TestMalloc((n?n:1)*sizeof(CVertexGroupBlock)));CVertexGroupBlock*state[3]={blocks,blocks+n,blocks+n};memcpy(&owner.InputList.Blocks,state,12);
bool*flags=static_cast<bool*>(TestMalloc(n?n:1));bool*flagState[3]={flags,flags+n,flags+n};memcpy(&owner.RemainingBlocks,flagState,12);
unsigned masks[4],counts[3]={0,0,0},last[3]={0,0,0},code=sequence,copies=0;
for(unsigned i=0;i<n;++i){new(blocks+i)CVertexGroupBlock;masks[i]=code&7;code>>=3;flags[i]=(remaining&(1U<<i))!=0;if(flags[i])++copies;unsigned long*groups=static_cast<unsigned long*>(TestMalloc(12));unsigned count=0;for(unsigned k=0;k<3;++k)if(masks[i]&(1U<<k)){groups[count++]=k;blocks[i].GroupToIndexMap.insert(std::pair<const unsigned long,long>(k,(long)k));if(flags[i]){++counts[k];last[k]=i;}}unsigned long*groupState[3]={groups,groups+count,groups+3};memcpy(&blocks[i].Groups,groupState,12);}
unsigned long expected=(unsigned long)-1;unsigned least=5;for(unsigned k=0;k<3;++k)if(counts[k]&&counts[k]<least){least=counts[k];expected=last[k];}
unsigned char before[sizeof(owner)];memcpy(before,&owner,sizeof(owner));unsigned beforeActive=active;
unsigned long actual=owner.FindBestStartingBlock();if(actual!=expected||errors||active!=beforeActive||treeCopies!=copies||memcmp(before,&owner,sizeof(owner)))return 1;
for(unsigned i=0;i<n;++i){if(flags[i]!=((remaining&(1U<<i))!=0))return 2;unsigned observed=0;for(unsigned j=0;j<blocks[i].GetGroupCount();++j)observed|=1U<<blocks[i].GetGroup(j);if(observed!=masks[i])return 3;unsigned count=0;for(std::map<unsigned long,long>::iterator it=blocks[i].GroupToIndexMap.begin();it!=blocks[i].GroupToIndexMap.end();++it){if(!(masks[i]&(1U<<it->first))||it->second!=(long)it->first)return 4;++count;}if(count!=blocks[i].GetGroupCount())return 5;}
}if(errors||active)return 6;++cases;}}
if(cases!=69905)return 7;puts("00ad94f0_TEST PASS");return 0;}