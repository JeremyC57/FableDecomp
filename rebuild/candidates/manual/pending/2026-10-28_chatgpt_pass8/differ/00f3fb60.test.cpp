#include <stdio.h>
#include <stdlib.h>
extern "C" void free(void*);
namespace std {template<class T>class allocator{};template<class V,class Al>class _STLP_alloc_proxy:public Al{public:V _M_data;};template<class T,class Al>class _Vector_base{public:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,Al> _M_end_of_storage;_Vector_base():_M_start(0),_M_finish(0){_M_end_of_storage._M_data=0;}};template<class T,class Al=allocator<T> >class vector:public _Vector_base<T,Al>{public:vector(){} ~vector(){for(T* p=this->_M_start;p!=this->_M_finish;++p)p->~T();if(this->_M_start)free(this->_M_start);}void resize(unsigned int,const T&);void resize(unsigned int n){const T value=T();resize(n,value);}};}
template<class T>class CArray:public std::vector<T>{};
typedef CArray<unsigned short> Element;
template void std::vector<Element>::resize(unsigned int);
static unsigned int seen;static bool zero;namespace std{template<class T,class Al>void vector<T,Al>::resize(unsigned int n,const T& x){seen=n;zero=(x._M_start==0 && x._M_finish==0 && x._M_end_of_storage._M_data==0);}}
int main(){std::vector<Element> v;v.resize(5);if(seen!=5 || !zero)goto fail;v.resize(0);if(seen || !zero)goto fail;puts("00f3fb60_TEST PASS");return 0;fail:puts("00f3fb60_TEST FAIL");return 1;}
