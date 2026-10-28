#include <stdio.h>
#include <stdlib.h>
// std::vector::push_back(value) -- Append an element, growing the buffer when full.
// Leaf: ?push_back@?$vector@VCEngineTextureMorph@@V?$allocator@VCEngineTextureMorph@@@std@@@std@@QAEXABVCEngineTextureMorph@@@Z
#include <stddef.h>
inline void* operator new(size_t, void* where) { return where; }
class CRGBColour { public: unsigned char B,G,R,A; };
enum ECompositeBlendType { COMPOSITE_BLEND_NONE };
class CCompositeBlendDesc {
public:
    unsigned long TextureIndex;
    ECompositeBlendType BlendType;
    CRGBColour DiffuseColour, SpecularColour;
    CCompositeBlendDesc();
    CCompositeBlendDesc(const CCompositeBlendDesc&);
};
namespace std {
struct __false_type { __false_type() {} };
template<class A,class B> struct pair {
    A first; B second;
    pair(const pair& x):first(x.first),second(x.second){}
};
template<class T> class allocator { public: allocator() {} };
template<class V,class A> class _STLP_alloc_proxy : public A { public: V _M_data; };
template<class T,class A> class _Vector_base {
public:
    T* _M_start; T* _M_finish;
    _STLP_alloc_proxy<T*,A> _M_end_of_storage;
};
template<class T,class A=allocator<T> > class vector : public _Vector_base<T,A> {
public:
    vector();
    vector(const vector&);
    void _M_insert_overflow(T*,const T&,const __false_type&,unsigned int,bool);
    void push_back(const T&);
};
template<class T,class A> void vector<T,A>::push_back(const T& value)
{
    if(this->_M_finish != this->_M_end_of_storage._M_data) {
        new(this->_M_finish) T(value);
        ++this->_M_finish;
    } else
        _M_insert_overflow(this->_M_finish,value,__false_type(),1,true);
}
}
class CEngineTextureMorph { public: long OriginalTextureIndex; long MorphID; std::vector<CCompositeBlendDesc> Blends; };
template void std::vector<CEngineTextureMorph>::push_back(const CEngineTextureMorph&);

static int copy_calls, grow_calls, grow_ok=1;
CCompositeBlendDesc::CCompositeBlendDesc():TextureIndex(0),BlendType(COMPOSITE_BLEND_NONE) {
    DiffuseColour.B=DiffuseColour.G=DiffuseColour.R=DiffuseColour.A=0;
    SpecularColour=DiffuseColour;
}
CCompositeBlendDesc::CCompositeBlendDesc(const CCompositeBlendDesc& x):TextureIndex(x.TextureIndex),BlendType(x.BlendType),DiffuseColour(x.DiffuseColour),SpecularColour(x.SpecularColour){++copy_calls;}
namespace std {
template<class T,class A> vector<T,A>::vector() {this->_M_start=this->_M_finish=this->_M_end_of_storage._M_data=0;}
template<class T,class A> vector<T,A>::vector(const vector<T,A>& x) {
    ++copy_calls;this->_M_start=x._M_start;this->_M_finish=x._M_finish;this->_M_end_of_storage._M_data=x._M_end_of_storage._M_data;
}
template<class T,class A> void vector<T,A>::_M_insert_overflow(T* position,const T& value,const __false_type&,unsigned int count,bool at_end) {
    ++grow_calls;grow_ok=position==this->_M_finish && count==1 && at_end;
    this->_M_start=(T*)malloc(sizeof(T)*2);
    new(this->_M_start) T(value);this->_M_finish=this->_M_start+1;this->_M_end_of_storage._M_data=this->_M_start+2;
}
}
int main() {
    typedef CEngineTextureMorph Item;
    Item value;
    value.OriginalTextureIndex=7;value.MorphID=11;
    std::vector<Item> v;
    Item* buffer=(Item*)malloc(sizeof(Item)*3);
    v._M_start=v._M_finish=buffer;v._M_end_of_storage._M_data=buffer+3;
    v.push_back(value);
    int ok=v._M_finish==buffer+1 && copy_calls==1 && grow_calls==0 && buffer[0].OriginalTextureIndex==7 && buffer[0].MorphID==11 && buffer[0].Blends._M_start==0;
    v.push_back(value);ok=ok && v._M_finish==buffer+2 && copy_calls==2;
    std::vector<Item> empty;
    empty.push_back(value);ok=ok && grow_calls==1 && grow_ok && empty._M_finish==empty._M_start+1;
    empty._M_start->~Item();free(empty._M_start);
    buffer[0].~Item();buffer[1].~Item();free(buffer);
    puts(ok ? "006cb470_TEST PASS" : "006cb470_TEST FAIL");return ok ? 0 : 1;
}