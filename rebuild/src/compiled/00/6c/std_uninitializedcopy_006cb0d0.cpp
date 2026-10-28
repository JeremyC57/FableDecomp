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