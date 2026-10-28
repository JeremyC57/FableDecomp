#include <stdio.h>
#include <stdlib.h>
extern "C" void free(void*);
namespace std {template<class T>class allocator{};template<class V,class Al>class _STLP_alloc_proxy:public Al{public:V _M_data;};template<class T,class Al>class _Vector_base{public:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,Al> _M_end_of_storage;_Vector_base():_M_start(0),_M_finish(0){_M_end_of_storage._M_data=0;}};template<class T,class Al=allocator<T> >class vector:public _Vector_base<T,Al>{public:vector(){} ~vector(){for(T* p=this->_M_start;p!=this->_M_finish;++p)p->~T();if(this->_M_start)free(this->_M_start);}void resize(unsigned int,const T&);void resize(unsigned int n){const T value=T();resize(n,value);}};}
class CIVCountedPointeeBase{public:virtual ~CIVCountedPointeeBase(){}virtual void Delete()=0;long Count;};template<class T>class CIVCountedPointer{public:CIVCountedPointeeBase* Object;CIVCountedPointer():Object(0){}~CIVCountedPointer(){if(Object && --Object->Count<=0)Object->Delete();}};namespace NParticleEngine{class CParticleDataBank;class CParticleEmitterInfo{public:class CEmitterData;CIVCountedPointer<CEmitterData> Data;CParticleDataBank* Bank;unsigned long Index;CParticleEmitterInfo():Data(),Bank(0),Index(0){} };}
typedef NParticleEngine::CParticleEmitterInfo Element;
template void std::vector<Element>::resize(unsigned int);
static unsigned int seen;static bool zero;namespace std{template<class T,class Al>void vector<T,Al>::resize(unsigned int n,const T& x){seen=n;zero=(x.Data.Object==0 && x.Bank==0 && x.Index==0);}}
int main(){std::vector<Element> v;v.resize(5);if(seen!=5 || !zero)goto fail;v.resize(0);if(seen || !zero)goto fail;puts("00af2200_TEST PASS");return 0;fail:puts("00af2200_TEST FAIL");return 1;}
