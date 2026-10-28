#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#define NOINLINE __attribute__((noinline))
#else
#define NOINLINE __declspec(noinline)
#endif
enum EHeroMorphType { MORPH_NONE }; enum ECompositeBlendType { BLEND_NONE }; class CTextureMorphEntry { public: EHeroMorphType MorphType; long TextureLayer,ReplacingTextureIndex,BankIndex; ECompositeBlendType Blend; };
namespace std {
struct random_access_iterator_tag {}; template<class T> class allocator {}; template<class V,class Al> class _STLP_alloc_proxy:public Al { public: V _M_data; }; template<class T,class Al> class _Vector_base { public:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,Al> _M_end_of_storage; };
template<class Iter,class Out,class Distance> NOINLINE Out __fastcall __copy(Iter first,Iter last,Out result,const random_access_iterator_tag&,Distance*) { for(Distance n=last-first;n>0;--n,++first,++result)*result=*first;return result; }
template<class T,class Al=allocator<T> > class vector:public _Vector_base<T,Al> { public: typedef T* iterator; typedef unsigned int size_type; iterator begin(){return this->_M_start;}iterator end(){return this->_M_finish;} size_type size()const{return size_type(this->_M_finish-this->_M_start);} void _M_fill_insert(iterator,unsigned int,const T&); iterator erase(iterator first,iterator last){iterator result=__copy(last,end(),first,random_access_iterator_tag(),(int*)0);this->_M_finish=result;return first;}void resize(size_type n,const T& x){if(n<size()){erase(begin()+n,end());}else _M_fill_insert(end(),n-size(),x);} };
}
typedef CTextureMorphEntry Element;
template void std::vector<Element>::resize(unsigned int,const Element&);
static int called=0;namespace std {template<class T,class Al>void vector<T,Al>::_M_fill_insert(T* p,unsigned int n,const T& x){++called;for(unsigned int i=0;i<n;++i)p[i]=x;this->_M_finish=p+n;}}
int main(){Element x;x.MorphType=MORPH_NONE;x.TextureLayer=7;x.ReplacingTextureIndex=11;x.BankIndex=13;x.Blend=BLEND_NONE;Element data[6];for(int i=0;i<6;++i)data[i]=x;std::vector<Element> v;v._M_start=data;v._M_finish=data+4;v._M_end_of_storage._M_data=data+6;v.resize(2,x);if(v._M_finish!=data+2 || called)goto fail;v.resize(5,x);if(v._M_finish!=data+5 || called!=1)goto fail;v.resize(5,x);if(v._M_finish!=data+5 || called!=2)goto fail;v.resize(0,x);if(v._M_finish!=data)goto fail;puts("0081daa0_TEST PASS");return 0;fail:puts("0081daa0_TEST FAIL");return 1;}