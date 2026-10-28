#include <stdio.h>
// CArray::DeallocateUnusedMemory() -- Release spare capacity while retaining every live element.
// Leaf: ?DeallocateUnusedMemory@?$CArray@U?$pair@KJ@std@@@@QAEXXZ
// Identity: Own CCRCSymbolMap::DeallocateUnusedMemory00a38590 and LoadFromFile00a39010 trim SLONGMap+4; reserve00a39590, rangeinsert00a39610, DeallocateMemory00a39950. PDB original CArray method and real STLport base templates.
// Layout: CArray inherits twelve-byte vector; each type and stride derived independently from own callers.
#include <new>
#include <stdlib.h>
#ifndef _MSC_VER
#define __declspec(x)
#endif
namespace std{
struct forward_iterator_tag{forward_iterator_tag(){}};
template<class A,class B>struct pair{A first;B second;pair(){}pair(const pair& x):first(x.first),second(x.second){}};
template<class T>class allocator{};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;_STLP_alloc_proxy(V p):_M_data(p){}};
template<class T,class A>class _Vector_base{public:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;_Vector_base():_M_start(0),_M_finish(0),_M_end_of_storage(0){}~_Vector_base(){if(_M_start)free(_M_start);}};
template<class T>inline void swap(T& a,T& b){T temp=a;a=b;b=temp;}
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:
unsigned int size()const{return (unsigned int)(this->_M_finish-this->_M_start);}unsigned int capacity()const{return (unsigned int)(this->_M_end_of_storage._M_data-this->_M_start);}
T* begin(){return this->_M_start;}T* end(){return this->_M_finish;}
__declspec(noinline) void reserve(unsigned int n){if(capacity()<n){unsigned int used=size();T* p=(T*)malloc(n*sizeof(T));for(unsigned int i=0;i<used;++i)new(p+i)T(this->_M_start[i]);if(this->_M_start)free(this->_M_start);this->_M_start=p;this->_M_finish=p+used;this->_M_end_of_storage._M_data=p+n;}}
// Stand-in for the range helper: this caller inserts only at end into sufficient reserved space; all input elements are read, tag untouched.
template<class Iter>__declspec(noinline) void _M_range_insert(T* position,Iter first,Iter last,const forward_iterator_tag&){for(;first!=last;++first,++this->_M_finish)new(this->_M_finish)T(*first);}
void insert(T* position,const T* first,const T* last){_M_range_insert(position,first,last,forward_iterator_tag());}
void swap(vector& other){std::swap(this->_M_start,other._M_start);std::swap(this->_M_finish,other._M_finish);std::swap(this->_M_end_of_storage._M_data,other._M_end_of_storage._M_data);}
};}
template<class T>class CArray:public std::vector<T>{public:
__declspec(noinline) void DeallocateMemory(){if(this->capacity()>0){for(T* p=this->begin();p!=this->end();++p)p->~T();this->_M_finish=this->_M_start;CArray<T>().swap(*this);}}
void DeallocateUnusedMemory(){if(this->capacity()>this->size()){CArray<T> temp;temp.reserve(this->size());temp.insert(temp.begin(),this->begin(),this->end());DeallocateMemory();this->swap(temp);}}
};
typedef std::pair<unsigned long,long> Element;
template void CArray<Element>::DeallocateUnusedMemory();

static void init(Element& x,unsigned int n){x.first=n;x.second=n+11;}static bool same(const Element& x,const Element& y){return x.first==y.first&&x.second==y.second;}
static bool run(unsigned int used,unsigned int capacity){CArray<Element> v;v.reserve(capacity);for(unsigned int i=0;i<used;++i){Element value;init(value,i+1);new(v._M_finish)Element(value);++v._M_finish;}Element* old=v.begin();v.DeallocateUnusedMemory();bool ok=v.size()==used&&v.capacity()==used;if(used==capacity)ok=ok&&v.begin()==old;for(unsigned int j=0;j<used;++j){Element value;init(value,j+1);ok=ok&&same(v.begin()[j],value);}v.DeallocateUnusedMemory();return ok&&v.size()==used&&v.capacity()==used;}
int main(){bool ok=run(3,8)&&run(3,3)&&run(0,5)&&run(0,0);puts(ok?"00a39a00_TEST PASS":"00a39a00_TEST FAIL");return ok?0:1;}