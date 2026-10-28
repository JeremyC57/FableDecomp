#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __declspec(x) __attribute__((x))
#endif
// std::__uninitialized_copy(first, last, result, tag) -- Copy-construct records into uninitialized storage.
// Leaf: ??$__uninitialized_copy@PAVCEngineTextureMorph@@PAV1@@std@@YIPAVCEngineTextureMorph@@PAV1@00ABU__false_type@0@@Z
// Flags: /Gr
#include <stddef.h>
#include <stdlib.h>
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
template<class T1,class T2> inline void _Construct(T1* p,const T2& value) { new(p) T1(value); }
#pragma auto_inline(off)
template<class InputIter,class ForwardIter>
__declspec(noinline) ForwardIter __fastcall __uninitialized_copy(InputIter first,InputIter last,ForwardIter result,const __false_type&)
{
    ForwardIter cur=result;
    for(;first!=last;++first,++cur) _Construct(&*cur,*first);
    return cur;
}
template CCompositeBlendDesc* __fastcall __uninitialized_copy<const CCompositeBlendDesc*,CCompositeBlendDesc*>(const CCompositeBlendDesc*,const CCompositeBlendDesc*,CCompositeBlendDesc*,const __false_type&);
#pragma auto_inline(on)
template<class T> class allocator {
public:
    T* allocate(unsigned int n) { return n ? (T*)malloc(n*sizeof(T)) : 0; }
    void deallocate(T* p,unsigned int) { if(p) free(p); }
};
template<class V,class T,class A> class _STLP_alloc_proxy : public A { public: V _M_data; };
template<class T,class A> class _Vector_base {
public:
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*,T,A> _M_end_of_storage;
    _Vector_base(unsigned int n):_M_start(0),_M_finish(0) {
        _M_end_of_storage._M_data=0;
        _M_start=_M_end_of_storage.allocate(n);
        _M_finish=_M_start;
        _M_end_of_storage._M_data=_M_start+n;
    }
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start); }
};
template<class T,class A=allocator<T> > class vector : public _Vector_base<T,A> {
public:
    vector():_Vector_base<T,A>(0) {}
    unsigned int size() const { return (unsigned int)(this->_M_finish-this->_M_start); }
    vector(const vector& x):_Vector_base<T,A>(x.size()) {
        this->_M_finish=__uninitialized_copy((const T*)x._M_start,(const T*)x._M_finish,this->_M_start,__false_type());
    }
};
}
class CEngineTextureMorph { public: long OriginalTextureIndex; long MorphID; std::vector<CCompositeBlendDesc> Blends; };
namespace std {
template CEngineTextureMorph* __fastcall __uninitialized_copy<CEngineTextureMorph*,CEngineTextureMorph*>(CEngineTextureMorph*,CEngineTextureMorph*,CEngineTextureMorph*,const __false_type&);
}

static int copy_calls;
CCompositeBlendDesc::CCompositeBlendDesc():TextureIndex(0),BlendType(COMPOSITE_BLEND_NONE) {
    DiffuseColour.B=DiffuseColour.G=DiffuseColour.R=DiffuseColour.A=0;SpecularColour=DiffuseColour;
}
CCompositeBlendDesc::CCompositeBlendDesc(const CCompositeBlendDesc& x):TextureIndex(x.TextureIndex),BlendType(x.BlendType),DiffuseColour(x.DiffuseColour),SpecularColour(x.SpecularColour) {++copy_calls;}
int main() {
    CEngineTextureMorph input[3];
    for(int i=0;i<3;++i){input[i].OriginalTextureIndex=10+i;input[i].MorphID=20+i;}
    CCompositeBlendDesc* blends=(CCompositeBlendDesc*)malloc(2*sizeof(CCompositeBlendDesc));
    new(blends) CCompositeBlendDesc;new(blends+1) CCompositeBlendDesc;
    blends[0].TextureIndex=30;blends[1].TextureIndex=31;
    input[1].Blends._M_start=blends;input[1].Blends._M_finish=blends+2;input[1].Blends._M_end_of_storage._M_data=blends+2;
    CEngineTextureMorph* storage=(CEngineTextureMorph*)malloc(5*sizeof(CEngineTextureMorph));memset(storage,0x5a,5*sizeof(CEngineTextureMorph));CEngineTextureMorph* out=storage+1;
    std::__false_type tag;
    CEngineTextureMorph* first=input;
    CEngineTextureMorph* result=std::__uninitialized_copy(first,first+3,out,tag);
    int ok=result==out+3 && copy_calls==2;
    for(int j=0;j<3;++j)ok=ok && out[j].OriginalTextureIndex==10+j && out[j].MorphID==20+j;
    ok=ok && out[0].Blends._M_start==0 && out[2].Blends._M_start==0;
    ok=ok && out[1].Blends._M_start!=blends && out[1].Blends.size()==2;
    ok=ok && out[1].Blends._M_start[0].TextureIndex==30 && out[1].Blends._M_start[1].TextureIndex==31;
    result=std::__uninitialized_copy(first,first,out,tag);ok=ok && result==out && copy_calls==2;
    unsigned char* raw=(unsigned char*)storage;
    for(unsigned int k=0;k<sizeof(CEngineTextureMorph);++k)ok=ok && raw[k]==0x5a && raw[4*sizeof(CEngineTextureMorph)+k]==0x5a;
    for(int d=0;d<3;++d)out[d].~CEngineTextureMorph();free(storage);
    puts(ok ? "006cb0d0_TEST PASS" : "006cb0d0_TEST FAIL");return ok ? 0 : 1;
}