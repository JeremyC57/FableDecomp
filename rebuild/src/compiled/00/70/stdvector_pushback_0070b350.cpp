// std::vector::push_back(value) -- Append an element, growing the buffer when full.
// Leaf: ?push_back@?$vector@VCEngineTextureDecal@@V?$allocator@VCEngineTextureDecal@@@std@@@std@@QAEXABVCEngineTextureDecal@@@Z
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
class CEngineTextureDecal { public: long OriginalTextureIndex; long DecalID; std::vector<CCompositeBlendDesc> OverlaidBlends; };
template void std::vector<CEngineTextureDecal>::push_back(const CEngineTextureDecal&);