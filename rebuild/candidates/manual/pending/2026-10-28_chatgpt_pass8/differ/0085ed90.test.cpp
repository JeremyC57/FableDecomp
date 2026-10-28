#include <stdio.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __declspec(x)
#endif
extern "C" void free(void*);
namespace NThingMap{class CMapwhoNode;class CThingMapCell{public:CMapwhoNode* PFirstNode;};}
namespace std{
template<class T>class allocator{public:allocator(){}void deallocate(T* p,unsigned int){if(p)free(p);}};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;_STLP_alloc_proxy(const A&a,V value):A(a),_M_data(value){}};
template<class T,class A>class _Vector_base{protected:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;_Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,(T*)0){}~_Vector_base(){if(_M_start)_M_end_of_storage.deallocate(_M_start,(unsigned int)(_M_end_of_storage._M_data-_M_start));}};
template<class T,class A=allocator<T> >class vector:protected _Vector_base<T,A>{public:typedef T* iterator;explicit vector(const A&a=A()):_Vector_base<T,A>(a){}~vector(){for(T*p=this->_M_start;p!=this->_M_finish;++p)p->~T();}iterator begin(){return this->_M_start;}const T* begin()const{return this->_M_start;}iterator end(){return this->_M_finish;}const T* end()const{return this->_M_finish;}unsigned int size()const{return (unsigned int)(this->_M_finish-this->_M_start);}iterator erase(iterator,iterator);void insert(iterator p,unsigned int n,const T&x){_M_fill_insert(p,n,x);}__declspec(noinline) void resize(unsigned int n,const T&x){if(n<size())erase(begin()+n,end());else insert(end(),n-size(),x);}void resize(unsigned int n){const T value=T();resize(n,value);}protected:void _M_fill_insert(iterator,unsigned int,const T&);};
}
typedef std::vector<NThingMap::CThingMapCell> Element;
template void std::vector<Element>::resize(unsigned int);
static unsigned int seen;static bool zero;namespace std{template<class T,class A>typename vector<T,A>::iterator vector<T,A>::erase(iterator p,iterator){return p;}template<class T,class A>void vector<T,A>::_M_fill_insert(iterator,unsigned int n,const T&x){seen=n;zero=(x.begin()==0 && x.end()==0);}}
int main(){std::vector<Element> v;v.resize(5);if(seen!=5 || !zero)goto fail;v.resize(0);if(seen || !zero)goto fail;puts("0085ed90_TEST PASS");return 0;fail:puts("0085ed90_TEST FAIL");return 1;}
